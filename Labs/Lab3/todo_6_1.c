#include <stdio.h>

size_t my_strlen(const char *str)
{
    size_t length = 0;
    while (str[length] != '\0')
    {
        length++;
    }
    return length;
}

void my_strcpy(char *dest, const char *src)
{
    while ((*dest++ = *src++) != '\0')
        ;
}

int my_strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str1 == *str2)
    {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

int main(int argc, char *argv[])
{

    // Length
    const char *str = "Hello World";
    size_t length = my_strlen(str);
    printf("Length of string: %zu\n", length);

    // Copy
    char src[] = "Hello, World!";
    char dst[20];
    my_strcpy(dst, src);
    printf("Copied string: %s\n", dst);

    // Compare
    const char *string1 = "Hello";
    const char *string2 = "World";
    int result = my_strcmp(string1, string2);
    if (result == 0)
    {
        printf("Strings are equal.\n");
    }
    else if (result < 0)
    {
        printf("String1 is less than String2.\n");
    }
    else
    {
        printf("String1 is greater than String2.\n");
    }

    return 0;
}

// int main()
// {
//     const char *string1 = "Hello";
//     const char *string2 = "World";
//     int result = my_strcmp(string1, string2);
//     if (result == 0)
//     {
//         printf("Strings are equal.\n");
//     }
//     else if (result < 0)
//     {
//         printf("String1 is less than String2.\n");
//     }
//     else
//     {
//         printf("String1 is greater than String2.\n");
//     }
//     return 0;
// }
