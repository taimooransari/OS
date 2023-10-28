#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "insert.h"
#include "remove.h"
#include "print.h"

int main()
{

    // Linked List Test
  struct node_ll *head = NULL;

  // Insert nodes
  insert(&head, 1, 0);
  insert(&head, 2, 1);
  insert(&head, 3, 2);
  insert(&head, 5, 0);

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
