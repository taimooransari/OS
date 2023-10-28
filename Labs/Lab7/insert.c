#include <stdio.h>
#include <stdlib.h>
#include "insert.h"

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


