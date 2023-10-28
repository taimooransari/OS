#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "linkedlist.h"

int main()
{
  // Stack Test

  // Create empty stack
  struct node *stck = NULL;

  // Push values
  push(&stck, 5);
  push(&stck, 4);
  push(&stck, 3);
  push(&stck, 2);
  push(&stck, 1);

  // Print stack
  printf("Stack: ");
  stack_print(stck);

  // Pop values
  printf("Popped %d\n", pop(&stck));
  printf("Popped %d\n", pop(&stck));
  printf("Popped %d\n", pop(&stck));
  printf("Popped %d\n", pop(&stck));
  printf("Popped %d\n", pop(&stck));

  // Pop from empty stack
  if (pop(&stck) == -1)
  {
    printf("Empty stack.\n");
  }

  printf("\n-------------------------------------\n");

    // Linked List Test
  struct node_ll *head = NULL;

  // Insert nodes
  insert(&head, 1, 0);
  insert(&head, 2, 1);
  insert(&head, 3, 2);

  // Check index validation
  insert(&head, 3, 5);

  // Print Linked List
  linkedlist_print(head);

  // Remove node
  int val = remove_at(&head, 1);
  printf("Removed %d from linked list.\n", val);
  linkedlist_print(head);

  // main function end
  return 0;
}
