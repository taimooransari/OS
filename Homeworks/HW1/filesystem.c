#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME_MAXLEN 8

// Struct for representing an inode
typedef struct inode
{
  int dir;                    // 1 if it's a directory, 0 if it's a file
  char name[FILENAME_MAXLEN]; // Name of the file or directory
  int size;                   // Size of the file (in bytes)
  int blockptrs[8];           // Pointers to data blocks
  int used;                   // Flag indicating whether the inode is in use
} inode;

// Struct for representing a directory entry
typedef struct dirent
{
  char name[FILENAME_MAXLEN]; // Name of the file or directory
  int namelen;                // Length of the name
  int inode;                  // Index of the associated inode
} dirent;

// Struct for representing the superblock
typedef struct superblock
{
  char freeblocklist[128]; // Free block list
  inode inodes[16];        // Array of inodes
} superblock;

// Struct for representing a data block
typedef struct datablock
{
  char data[1024]; // Data block content
  dirent dr[15];   // Directory entries
} datablock;

// Struct for representing the disk
typedef struct disk
{
  superblock *sb;     // Pointer to the superblock
  datablock *db[127]; // Array of data blocks
} disk;

// Function to initialize the file system
void initialize_file_system(disk *dm);

// Function to create a file
void create_file(disk *my_disk, char *filename, int size);

// Function to create a directory
void create_directory(disk *my_disk, char *path);

// Function to list all files and directories
void list_all(disk *my_disk);

// Function to delete a file
void delete_file(disk *my_disk, char *path);

// Function to delete a directory
void delete_directory(disk *my_disk, char *path);

// Function to copy a file
void copy_file(disk *my_disk, char *src_path, char *dest_path);

// Function to move a file
void move_file(disk *my_disk, char *src_path, char *dest_path);

// Function to load the file system from a file
void load_file(disk *my_disk);

// Function to save the file system to a file
void save_file(disk *my_disk);

// Function to update the size of a file
void update_size(disk *my_disk, char *path, int size);

// Save function implementation
void save_file(disk *dsk)
{
  FILE *file = fopen("./myfs", "wb");
  if (file == NULL)
  {
    perror("Error opening file for writing");
    exit(EXIT_FAILURE);
  }

  fwrite(dsk->sb, sizeof(dsk->sb), 1, file);
  for (size_t i = 0; i < 127; i++)
  {
    fwrite(dsk->db[i], sizeof(dsk->db[i]), 1, file);
  }

  fclose(file);
}

// Load_file function implementation
void load_file(disk *dsk)
{
  FILE *file = fopen("./myfs", "rb");
  if (file == NULL)
  {
    file = fopen("./myfs", "wb");
    fclose(file);
    return;
  }

  fread(dsk->sb, sizeof(dsk->sb), 1, file);
  for (size_t i = 0; i < 127; i++)
  {
    fread(dsk->db[i], sizeof(dsk->db[i]), 1, file);
  }

  fclose(file);
}

int main(int argc, char *argv[])
{

  disk my_disk;
  initialize_file_system(&my_disk);
  load_file(&my_disk);


  FILE *input_fd = fopen(argv[1], "r");

  char tmp_buffer[256];
  char cmd_src[256];
  char cmd_dest[256];
  int cmd_size;
  char *token;

  while (fgets(tmp_buffer, sizeof(tmp_buffer), input_fd) != NULL)
  {

    strtok(tmp_buffer, "\n");
    token = strtok(tmp_buffer, " ");

    if (strcmp(token, "CR") == 0)
    {
      token = strtok(NULL, " ");
      strcpy(cmd_src, token);
      token = strtok(NULL, " ");
      cmd_size = atoi(token);
      create_file(&my_disk, cmd_src, cmd_size);
    }
    else if (strcmp(token, "CD") == 0)
    {

      token = strtok(NULL, " ");
      strcpy(cmd_src, token);

      create_directory(&my_disk, cmd_src);
    }
    else if (strcmp(token, "LL") == 0)
    {
      list_all(&my_disk);
    }
    else if (strcmp(token, "DL") == 0)
    {
      token = strtok(NULL, " ");
      strcpy(cmd_src, token);
      delete_file(&my_disk, cmd_src);
    }
    else if (strcmp(token, "DD") == 0)
    {
      token = strtok(NULL, " ");
      strcpy(cmd_src, token);
      delete_directory(&my_disk, cmd_src);
    }
    else if (strcmp(token, "CP") == 0)
    {
      token = strtok(NULL, " ");
      strcpy(cmd_src, token);
      token = strtok(NULL, " ");
      strcpy(cmd_dest, token);
      copy_file(&my_disk, cmd_src, cmd_dest);
    }
    else if (strcmp(token, "MV") == 0)
    {
      token = strtok(NULL, " ");
      strcpy(cmd_src, token);
      token = strtok(NULL, " ");
      strcpy(cmd_dest, token);
      move_file(&my_disk, cmd_src, cmd_dest);
    }
    // save_file(&my_disk);
  }
  save_file(&my_disk);
  free(my_disk.sb);
  for (size_t i = 0; i < 127; i++)
  {
    free(my_disk.db[i]);
  }

  fclose(input_fd);

  return 0;
}

void update_size(disk *my_disk, char *path, int size)
{

  int isRoot = 0;

  char *dir_name = strrchr(path, '/') + 1;
  *(dir_name - 1) = '\0';

  if (strcmp(path, "") == 0)
  {
    isRoot = 1;
  }

  char *token;
  token = strtok(path, "/");

  my_disk->sb->inodes[0].size += size;

  dirent *root_dirent = my_disk->db[0]->dr;

  while (token != NULL && isRoot != 1)
  {

    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((root_dirent + index)->name, token) == 0 && my_disk->sb->inodes[(root_dirent + index)->inode].dir == 1)
      {

        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        my_disk->sb->inodes[child_inode].size += size;
        root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }

    token = strtok(NULL, "/");
  }
}

void initialize_file_system(disk *my_disk)
{
  my_disk->sb = (superblock *)malloc(sizeof(superblock));

  for (size_t i = 0; i < 127; i++)
  {
    my_disk->db[i] = (datablock *)malloc(sizeof(datablock));
  }

  my_disk->sb->freeblocklist[0] = '1';
  my_disk->sb->freeblocklist[1] = '1';

  for (size_t i = 0; i < 126; i++)
  {
    my_disk->sb->freeblocklist[i + 2] = '0';
  }

  for (size_t i = 0; i < 16; i++)
  {
    my_disk->sb->inodes->used = -1;
  }

  strcpy(my_disk->sb->inodes[0].name, "/");
  my_disk->sb->inodes[0].dir = 1;
  my_disk->sb->inodes[0].size = 0;
  my_disk->sb->inodes[0].used = 1;
  my_disk->sb->inodes[0].blockptrs[0] = 0;
  strcpy(my_disk->db[0]->data, "");

  for (size_t i = 0; i < 15; i++)
  {
    my_disk->db[0]->dr[i].inode = -1;
  }
  return;
}

void list_all(disk *my_disk)
{

  printf("\n------------------------------------------------------------------------\n");
  printf("ALL DIRECTORIES AND FILES: \n\n");
  for (size_t i = 0; i < 16; i++)
  {
    if (my_disk->sb->inodes[i].used == 1)
    {
      inode a = my_disk->sb->inodes[i];
      printf("Name: %s | Size: %d Bytes | Type: %d\n", a.name, a.size, a.dir);
    }
  }

  printf("\n------------------------------------------------------------------------\n");
}

void move_file(disk *my_disk, char *src_path, char *dest_path)
{

  char src_update[1024];
  char dst_update[1024];

  strcpy(src_update, src_path);
  strcpy(dst_update, dest_path);

  int isRoot = 0;

  char src_final_path[1024];
  strcpy(src_final_path, src_path);

  char *src_dir_name = strrchr(src_path, '/') + 1;
  *(src_dir_name - 1) = '\0';

  if (strcmp(src_path, "") == 0)
  {
    isRoot = 1;
  }

  char *src_token;
  src_token = strtok(src_path, "/");

  dirent *src_root_dirent = my_disk->db[0]->dr;

  while (src_token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (src_root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((src_root_dirent + index)->name, src_token) == 0 && my_disk->sb->inodes[(src_root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        src_root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", src_token, src_final_path);
      return;
    }

    src_token = strtok(NULL, "/");
  }

  int src_dir_inode = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((src_root_dirent + i)->name, src_dir_name) == 0 && my_disk->sb->inodes[(src_root_dirent + i)->inode].dir == 0)
    {

      src_dir_inode = (src_root_dirent + i)->inode;

      break;
    }
  }
  if (src_dir_inode == -1)
  {
    printf("%s source file doesnt exist in path: %s\n", src_dir_name, src_final_path);
    return;
  }

  char dest_final_path[1024];
  strcpy(dest_final_path, dest_path);

  char *dest_dir_name = strrchr(dest_path, '/') + 1;
  *(dest_dir_name - 1) = '\0';
  isRoot = 0;
  if (strcmp(dest_path, "") == 0)
  {
    isRoot = 1;
  }

  char *dest_token;
  dest_token = strtok(dest_path, "/");

  dirent *dest_root_dirent = my_disk->db[0]->dr;

  while (dest_token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (dest_root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((dest_root_dirent + index)->name, dest_token) == 0 && my_disk->sb->inodes[(dest_root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        dest_root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", dest_token, dest_final_path);
      return;
    }

    dest_token = strtok(NULL, "/");
  }

  int dest_dir_inode = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((dest_root_dirent + i)->name, dest_dir_name) == 0 && my_disk->sb->inodes[(dest_root_dirent + i)->inode].dir == 0)
    {

      dest_dir_inode = (dest_root_dirent + i)->inode;

      break;
    }
  }
  if (dest_dir_inode == -1)
  {
    printf("%s destination file doesnt exist in path: %s | Trying to create it.\n", dest_dir_name, dest_final_path);
    char dst[1024];
    strcpy(dst, dest_final_path);
    int s = my_disk->sb->inodes[src_dir_inode].size;
    delete_file(my_disk, src_final_path);
    create_file(my_disk, dst, s);
  }
  else
  {
    int size_to_update = my_disk->sb->inodes[src_dir_inode].size - my_disk->sb->inodes[dest_dir_inode].size;

    int dest_existing_block = my_disk->sb->inodes[dest_dir_inode].size / 1024;
    if (my_disk->sb->inodes[dest_dir_inode].size % 1024)
    {
      dest_existing_block++;
    }

    int src_existing_block = my_disk->sb->inodes[src_dir_inode].size / 1024;
    if (my_disk->sb->inodes[src_dir_inode].size % 1024)
    {
      src_existing_block++;
    }

    int s = my_disk->sb->inodes[src_dir_inode].size;
    datablock db[src_existing_block];

    for (size_t i = 0; i < src_existing_block; i++)
    {
      int index = my_disk->sb->inodes[src_dir_inode].blockptrs[i];
      db[i] = *(my_disk->db[index]);
    }
    delete_file(my_disk, src_final_path);

    for (size_t i = 0; i < dest_existing_block; i++)
    {
      int index = my_disk->sb->inodes[dest_dir_inode].blockptrs[i];
      strcpy(my_disk->db[index]->data, "");
      my_disk->sb->freeblocklist[index] = '0';
    }

    int blk = 0;
    for (size_t i = 0; i < 128; i++)
    {
      if (blk >= src_existing_block)
      {
        break;
      }
      if (my_disk->sb->freeblocklist[i] == '0')
      {
        my_disk->sb->inodes[dest_dir_inode].blockptrs[blk] = i;
        my_disk->sb->freeblocklist[i] = '1';

        memset(my_disk->db[i]->data, 'A', sizeof(char) * 1024);

        blk++;
      }
    }

    if (blk < src_existing_block)
    {
      printf("Block allocation for destination failed.\n");
      return;
    }
    my_disk->sb->inodes[dest_dir_inode].size = s;

    update_size(my_disk, dst_update, size_to_update);
  }
  printf("Source: %s | Destination: %s | File moved.\n", src_final_path, dest_final_path);
  return;
}

void copy_file(disk *my_disk, char *src_path, char *dest_path)
{

  char dst_update[1024];

  strcpy(dst_update, dest_path);

  int isRoot = 0;

  char src_final_path[1024];
  strcpy(src_final_path, src_path);

  char *src_dir_name = strrchr(src_path, '/') + 1;
  *(src_dir_name - 1) = '\0';

  if (strcmp(src_path, "") == 0)
  {
    isRoot = 1;
  }

  char *src_token;
  src_token = strtok(src_path, "/");

  dirent *src_root_dirent = my_disk->db[0]->dr;

  while (src_token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (src_root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((src_root_dirent + index)->name, src_token) == 0 && my_disk->sb->inodes[(src_root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        src_root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }

    if (found == 0)
    {
      printf("%s directory not found in %s.\n", src_token, src_final_path);
      return;
    }

    src_token = strtok(NULL, "/");
  }

  int src_dir_inode = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((src_root_dirent + i)->name, src_dir_name) == 0 && my_disk->sb->inodes[(src_root_dirent + i)->inode].dir == 0)
    {

      src_dir_inode = (src_root_dirent + i)->inode;

      break;
    }
  }
  if (src_dir_inode == -1)
  {
    printf("%s source file doesnt exist in path: %s\n", src_dir_name, src_final_path);
    return;
  }

  char dest_final_path[1024];
  strcpy(dest_final_path, dest_path);

  char *dest_dir_name = strrchr(dest_path, '/') + 1;
  *(dest_dir_name - 1) = '\0';
  isRoot = 0;
  if (strcmp(dest_path, "") == 0)
  {
    isRoot = 1;
  }

  char *dest_token;
  dest_token = strtok(dest_path, "/");

  dirent *dest_root_dirent = my_disk->db[0]->dr;

  while (dest_token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (dest_root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((dest_root_dirent + index)->name, dest_token) == 0 && my_disk->sb->inodes[(dest_root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        dest_root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", dest_token, dest_final_path);
      return;
    }

    dest_token = strtok(NULL, "/");
  }

  int dest_dir_inode = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((dest_root_dirent + i)->name, dest_dir_name) == 0 && my_disk->sb->inodes[(dest_root_dirent + i)->inode].dir == 0)
    {

      dest_dir_inode = (dest_root_dirent + i)->inode;

      break;
    }
  }
  if (dest_dir_inode == -1)
  {
    printf("%s destination file doesnt exist in path: %s | Trying to create it.\n", dest_dir_name, dest_final_path);
    char dst[1024];
    strcpy(dst, dest_final_path);
    create_file(my_disk, dst, my_disk->sb->inodes[src_dir_inode].size);

    for (size_t i = 0; i < 15; i++)
    {

      if (strcmp((dest_root_dirent + i)->name, dest_dir_name) == 0 && my_disk->sb->inodes[(dest_root_dirent + i)->inode].dir == 0)
      {

        dest_dir_inode = (dest_root_dirent + i)->inode;

        break;
      }
    }
    if (dest_dir_inode == -1)
    {
      printf("Copying from %s to %s failed.\n", src_final_path, dest_final_path);
      return;
    }
  }
  else
  {
    int updateSize = my_disk->sb->inodes[src_dir_inode].size - my_disk->sb->inodes[dest_dir_inode].size;

    int dest_existing_block = my_disk->sb->inodes[dest_dir_inode].size / 1024;
    if (my_disk->sb->inodes[dest_dir_inode].size % 1024)
    {
      dest_existing_block++;
    }

    int src_existing_block = my_disk->sb->inodes[src_dir_inode].size / 1024;
    if (my_disk->sb->inodes[src_dir_inode].size % 1024)
    {
      src_existing_block++;
    }

    if (dest_existing_block == src_existing_block)
    {

      for (size_t i = 0; i < dest_existing_block; i++)
      {
        int src_ind = my_disk->sb->inodes[src_dir_inode].blockptrs[i];
        int dest_ind = my_disk->sb->inodes[dest_dir_inode].blockptrs[i];

        my_disk->db[dest_ind] = my_disk->db[src_ind];
      }
    }
    else if (dest_existing_block > src_existing_block)
    {
      int remove = dest_existing_block - src_existing_block;
      for (size_t x = 0; x < remove; x++)
      {

        int dest_ind = my_disk->sb->inodes[dest_dir_inode].blockptrs[x + dest_existing_block];
        my_disk->sb->inodes[dest_dir_inode].blockptrs[x + dest_existing_block] = -1;
        strcpy(my_disk->db[dest_ind]->data, "");

        my_disk->sb->freeblocklist[dest_ind] = '0';
      }
    }
    else
    {

      int add = src_existing_block - dest_existing_block;

      int *temp_blocks = (int *)malloc(add * sizeof(int));
      int blk_index = 0;

      for (size_t i = 0; i < add; i++)
      {
        for (size_t x = 0; x < 129; x++)
        {
          if (my_disk->sb->freeblocklist[x] == '0')
          {
            temp_blocks[blk_index] = x;
            break;
          }
          if (x == 128)
          {
            printf("Not enough space to copy %s to %s | Copy Failed.\n", src_final_path, dest_final_path);
            return;
          }
        }
        blk_index++;
      }

      for (size_t i = 0; i < add; i++)
      {
        int blk = temp_blocks[i];

        memset(my_disk->db[blk]->data, 'A', sizeof(char) * 1024);
        my_disk->sb->inodes[src_dir_inode].blockptrs[i + dest_existing_block] = blk;
      }

      free(temp_blocks);
    }
    my_disk->sb->inodes[dest_dir_inode].size = my_disk->sb->inodes[src_dir_inode].size;
    update_size(my_disk, dst_update, updateSize);
  }

  printf("Source: %s | Destination: %s | File copied.\n", src_final_path, dest_final_path);
  return;
}
void delete_directory(disk *my_disk, char *path)
{

  int isRoot = 0;

  char final_path[1024];
  strcpy(final_path, path);

  char *dir_name = strrchr(path, '/') + 1;
  *(dir_name - 1) = '\0';

  if (strcmp(path, "") == 0)
  {
    isRoot = 1;
  }

  char *token;
  token = strtok(path, "/");

  dirent *root_dirent = my_disk->db[0]->dr;

  while (token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((root_dirent + index)->name, token) == 0 && my_disk->sb->inodes[(root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("HERE: %s directory not found in %s.\n", token, final_path);
      return;
    }

    token = strtok(NULL, "/");
  }

  int dir_inode = -1;
  int dir_dirent_from_parent = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((root_dirent + i)->name, dir_name) == 0 && my_disk->sb->inodes[(root_dirent + i)->inode].dir == 1)
    {
      dir_dirent_from_parent = i;
      dir_inode = (root_dirent + i)->inode;

      break;
    }
  }

  if (dir_inode == -1)
  {
    printf("%s directory doesnot exist, cannot delete.\n", final_path);
    return;
  }

  dirent *my_dirent = my_disk->db[my_disk->sb->inodes[dir_inode].blockptrs[0]]->dr;
  for (size_t i = 0; i < 15; i++)
  {
    int child_inode_index = (my_dirent + i)->inode;

    if (child_inode_index == -1)
    {
      continue;
    }

    int type = my_disk->sb->inodes[child_inode_index].dir;

    char sub_dir_path[1024];
    snprintf(sub_dir_path, sizeof(sub_dir_path), "%s/%s", final_path, (my_dirent + i)->name);
    if (type == 0)
    {
      delete_file(my_disk, sub_dir_path);
    }
    else if (type == 1)
    {
      delete_directory(my_disk, sub_dir_path);
    }
  }

  (root_dirent + dir_dirent_from_parent)->inode = -1;
  strcpy((root_dirent + dir_dirent_from_parent)->name, "");

  my_disk->sb->inodes[dir_inode].used = 0;
  my_disk->sb->inodes[dir_inode].size = 0;

  for (size_t i = 0; i < 15; i++)
  {
    (my_dirent + i)->inode = -1;
  }

  printf("Directory Name: %s | Path: %s | Deleted\n", dir_name, final_path);
}

void delete_file(disk *my_disk, char *path)
{

  int isRoot = 0;

  char final_path[1024];
  strcpy(final_path, path);

  char *dir_name = strrchr(path, '/') + 1;
  *(dir_name - 1) = '\0';

  if (strcmp(path, "") == 0)
  {
    isRoot = 1;
  }

  char *token;
  token = strtok(path, "/");
  dirent *root_dirent = my_disk->db[0]->dr;

  while (token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((root_dirent + index)->name, token) == 0 && my_disk->sb->inodes[(root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", token, final_path);
      return;
    }

    token = strtok(NULL, "/");
  }

  int file_inode = -1;
  int file_dirent_from_parent = -1;

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((root_dirent + i)->name, dir_name) == 0 && my_disk->sb->inodes[(root_dirent + i)->inode].dir == 0)
    {
      file_dirent_from_parent = i;
      file_inode = (root_dirent + i)->inode;

      break;
    }
  }

  if (file_inode == -1)
  {
    printf("%s file doesnot exist, cannot delete.\n", final_path);
    return;
  }

  int file_size = my_disk->sb->inodes[file_inode].size;

  (root_dirent + file_dirent_from_parent)->inode = -1;
  strcpy((root_dirent + file_dirent_from_parent)->name, "");

  int block_size = my_disk->sb->inodes[file_inode].size / 1024;
  if (my_disk->sb->inodes[file_inode].size % 1024 != 0)
  {
    block_size++;
  }
  my_disk->sb->inodes[file_inode].used = 0;
  my_disk->sb->inodes[file_inode].size = 0;

  for (size_t i = 0; i < block_size; i++)
  {

    int block_ind = my_disk->sb->inodes->blockptrs[i];
    my_disk->sb->freeblocklist[block_ind] = '0';
    my_disk->sb->inodes->blockptrs[i] = -1;
    strcpy(my_disk->db[block_ind]->data, "");
  }
  char temp[1024];
  strcpy(temp, final_path);
  update_size(my_disk, temp, -1 * file_size);
  printf("File Name: %s | Path: %s | Deleted\n", dir_name, final_path);
}

void create_file(disk *my_disk, char *path, int size)
{

  if (size > 8192)
  {
    printf("File cannot exceed 8192 Bytes.\n");
    return;
  }

  int block_size = size / 1024;
  if (size % 1024 != 0)
  {
    block_size++;
  }

  int isRoot = 0;

  char final_path[1024];
  strcpy(final_path, path);

  char *dir_name = strrchr(path, '/') + 1;
  *(dir_name - 1) = '\0';

  if (strcmp(path, "") == 0)
  {
    isRoot = 1;
  }

  char *token;
  token = strtok(path, "/");

  dirent *root_dirent = my_disk->db[0]->dr;

  while (token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((root_dirent + index)->name, token) == 0 && my_disk->sb->inodes[(root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", token, final_path);
      return;
    }

    token = strtok(NULL, "/");
  }

  for (size_t i = 0; i < 15; i++)
  {

    if (strcmp((root_dirent + i)->name, dir_name) == 0 && my_disk->sb->inodes[(root_dirent + i)->inode].dir == 0)
    {

      printf("%s file already exists, cannot create again.\n", final_path);
      return;
    }
  }

  int free_inode = -1;
  for (size_t i = 0; i < 16; i++)
  {
    if (my_disk->sb->inodes[i].used == 0)
    {
      free_inode = i;
      break;
    }
  }

  int temp_blocks[block_size];
  int block_ind = 0;
  for (int x = 0; x < block_size; x++)
  {
    while (my_disk->sb->freeblocklist[block_ind] != '0')
    {
      block_ind++;
      if (block_ind >= 128)
      {
        printf("No more data blocks left. This file cannot be created as its size is larger than free space.\n");
        return;
      }
    }
    temp_blocks[x] = block_ind;
    block_ind++;
  }

  if (free_inode == -1)
  {
    printf("No inodes/datablock left for new directory. \n");
    return;
  }

  for (size_t i = 0; i < 15; i++)
  {
    if ((root_dirent + i)->inode == -1)
    {

      strcpy((root_dirent + i)->name, dir_name);
      (root_dirent + i)->inode = free_inode;
      break;
    }
  }

  my_disk->sb->inodes[free_inode].used = 1;
  my_disk->sb->inodes[free_inode].dir = 0;
  my_disk->sb->inodes[free_inode].size = size;
  strcpy(my_disk->sb->inodes[free_inode].name, dir_name);

  for (size_t i = 0; i < 8; i++)
  {
    if (i < block_size)
    {
      my_disk->sb->inodes[free_inode].blockptrs[i] = temp_blocks[i];

      memset(my_disk->db[temp_blocks[i]]->data, 'A', sizeof(char) * 1024);
    }
    else
    {
      my_disk->sb->inodes[free_inode].blockptrs[i] = -1;
    }
  }

  char temp[1024];
  strcpy(temp, final_path);
  update_size(my_disk, temp, size);

  printf("File Name: %s | Path: %s | Created\n", dir_name, final_path);
}

void create_directory(disk *my_disk, char *path)
{
  int isRoot = 0;

  char final_path[1024];
  strcpy(final_path, path);

  char *dir_name = strrchr(path, '/') + 1;
  *(dir_name - 1) = '\0';

  if (strcmp(path, "") == 0)
  {
    isRoot = 1;
  }

  char *token;
  token = strtok(path, "/");

  dirent *root_dirent = my_disk->db[0]->dr;

  while (token != NULL && isRoot != 1)
  {

    int found = 0;
    int index = 0;
    for (size_t i = 0; i < 15; i++)
    {
      index = i;
      int child_inode = (root_dirent + i)->inode;
      if (child_inode == -1)
      {
        continue;
      }

      if (strcmp((root_dirent + index)->name, token) == 0 && my_disk->sb->inodes[(root_dirent + index)->inode].dir == 1)
      {

        found = 1;
        int tmp = my_disk->sb->inodes[child_inode].blockptrs[0];
        root_dirent = my_disk->db[tmp]->dr;
        break;
      }
    }
    if (found == 0)
    {
      printf("%s directory not found in %s.\n", token, final_path);
      return;
    }

    token = strtok(NULL, "/");
  }

  for (size_t i = 0; i < 15; i++)
  {
    if (strcmp((root_dirent + i)->name, dir_name) == 0 && my_disk->sb->inodes[(root_dirent + i)->inode].dir == 1)
    {
      printf("%s directory already exists, cannot create again.\n", final_path);
      return;
    }
  }

  int free_inode = -1;
  for (size_t i = 0; i < 16; i++)
  {
    if (my_disk->sb->inodes[i].used == 0)
    {
      free_inode = i;
      break;
    }
  }

  int free_block = -1;
  for (size_t i = 0; i < 16; i++)
  {
    if (my_disk->sb->freeblocklist[i] == '0')
    {
      free_block = i;
      break;
    }
  }

  if (free_block == -1 || free_inode == -1)
  {
    printf("No inodes/datablock left for new directory. \n");
    return;
  }

  for (size_t i = 0; i < 15; i++)
  {
    if ((root_dirent + i)->inode == -1)
    {

      strcpy((root_dirent + i)->name, dir_name);
      (root_dirent + i)->inode = free_inode;
      break;
    }
  }

  my_disk->sb->inodes[free_inode].used = 1;
  my_disk->sb->inodes[free_inode].dir = 1;
  my_disk->sb->inodes[free_inode].size = 0;
  strcpy(my_disk->sb->inodes[free_inode].name, dir_name);
  my_disk->sb->inodes[free_inode].blockptrs[0] = free_block;

  my_disk->sb->freeblocklist[free_block] = '1';

  for (size_t i = 0; i < 15; i++)
  {
    my_disk->db[free_block]->dr[i].inode = -1;
  }
  printf("Directory Name: %s | Path: %s | Created\n", dir_name, final_path);
}