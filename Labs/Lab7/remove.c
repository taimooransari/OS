#include <stdio.h>
#include <stdlib.h>
#include "remove.h"

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

