#ifndef STACK_H
#define STACK_H

// Node Struct
struct node
{
  int val;
  struct node *next;
};

// push onto stack
void push(struct node **head, int val);
// pop off stack
int pop(struct node **head);
// Print the stack
void stack_print(struct node *head);

#endif
