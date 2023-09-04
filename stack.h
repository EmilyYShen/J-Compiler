/*
 * Copyright ©2022 Travis McGaha.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Pennsylvania
 * CIS 2400 for use solely during Fall Semester 2022 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#ifndef STACK
#define STACK

#include <stdbool.h>    // for bool type (true, false)

///////////////////////////////////////////////////////////////////////////////
// A Deque is a Double Ended Queue.
//
// Here we are providing the interface to implement a Deque, with an
// internal implementation of a doubly linked dq.
///////////////////////////////////////////////////////////////////////////////

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Internal structures for our Stack implementation.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


// A single node within a stack.
//
// A node contains next and prev pointers
typedef struct s_node_st {
  int num;    // value
  bool check; // else statement or not
  struct s_node_st *next;     // next node in stack, or NULL
  struct s_node_st *prev;     // prev node in stack, or NULL
} StackNode;

// The entire Stack.
// This struct contains metadata about the stack.
typedef struct s_st {
  int         num_elements;  //  # elements in the list
  StackNode   *front;  // beginning of stack, or NULL if empty
  StackNode   *back;  // end of stack, or NULL if empty
} Stack;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// "Methods" for our Stack implementation.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// Allocates and returns a pointer to a new Stack.
// It is the Caller's responsibility to at some point call Stack_Free to free
// the associated memory.
//
// Arguments: none.
//
// Returns:
// - the newly-allocated linked list, or NULL on error.
Stack* Stack_Allocate(void);

// Free a Stack that was previously allocated by Stack_Allocate.
//
// Arguments:
// - s: the stack to free.  It is unsafe to use "s" after this
//   function returns.
void Stack_Free(Stack *s);

// Pushes a new element to the end of the stack.
//
// Arguments:
// - s: the Stack to push onto.
void push(Stack *s, int num, bool check);

// Pops an element from the end of the stack.
//
// Arguments:
// - s: the Stack to remove from
//
// Returns:
// - false: on failure (eg, the stack is empty).
// - true: on success.
bool pop(Stack *s, int num);

void check_change(Stack *s);

// Peeks at the element at the back of the stack.
//
// Arguments:
// - s: the Stack to peek.
//
// Returns:
// - false on failure (eg, the deque is empty).
// - true on success.
int peek(Stack *s);

#endif  // STACK
