#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct node_ll
{
  int val;
  struct node_ll *next;
};

// Insert new node at given index
void insert(struct node_ll **head, int val, int index);

// Remove node at given index
int remove_at(struct node_ll **head, int index);

void linkedlist_print(struct node_ll *head);

#endif
