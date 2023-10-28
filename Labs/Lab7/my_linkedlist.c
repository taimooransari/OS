#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
// Insert new node at index
void insert(struct node_ll **head, int val, int index)
{
  if (!(*head && index))
  {
    index = 0;
  }

  struct node_ll *n = malloc(sizeof(struct node_ll));
  if (n == NULL)
  {
    perror("Error: Memory Allocation Failed\n");
    return;
  }
  n->val = val;

  if (index == 0)
  {
    n->next = *head;
    *head = n;
  }
  else
  {
    struct node_ll *h = *head;
    int i = 0;
    while (i < index - 1 && h)
    {
      h = h->next;
      i++;
    }
    if (h == NULL)
    {
      perror("Index out of bounds");
      return;
    }

    n->next = h->next;
    h->next = n;
  }
}

// Remove node at index
int remove_at(struct node_ll **head, int index)
{

  if (*head == NULL)
  {
    return -1;
  }

  struct node_ll *h = *head;

  if (index == 0)
  {
    *head = h->next;
    int val = h->val;
    free(h);
    return val;
  }

  int i = 0;
  while (i < index - 1 && h)
  {
    h = h->next;
    i++;
  }

  if (h == NULL || h->next == NULL)
  {
    return -1;
  }

  struct node_ll *next = h->next->next;
  int val = h->next->val;
  free(h->next);
  h->next = next;
  return val;
}

void linkedlist_print(struct node_ll *head)
{
  struct node_ll *h = head;
  while (h)
  {
    printf("%d, ", h->val);
    h = h->next;
  }
  printf("\n");
}
