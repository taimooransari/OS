#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Push Stack Implementation
void push(struct node **head, int val)
{
  if (head == NULL)
  {
    perror("Error: Can't Push to Null Head Pointer.\n");
    return;
  }

  struct node *n = malloc(sizeof(struct node));
  if (n == NULL)
  {
    perror("Error: Memory Allocation Failed\n");
    return;
  }
  n->val = val;
  n->next = *head;
  *head = n;
}

// Pop Stack Implementation
int pop(struct node **head)
{
  if (head == NULL)
  {
    perror("Error: Can't Pop from Null Head Pointer.\n");
    return -1;
  }

  if (*head == NULL)
  {
    printf("Error: Popping From Empty Stack.\n");
    return -1;
  }

  struct node *h = *head;
  int val = h->val;
  *head = (*head)->next;
  free(h);
  return val;
}

// Print Stack Implementation
void stack_print(struct node *head)
{
  struct node *current = head;
  while (current)
  {
    printf("%d, ", current->val);
    current = current->next;
  }
  printf("\n");
}
