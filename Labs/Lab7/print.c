#include <stdio.h>
#include <stdlib.h>
#include "print.h"


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
