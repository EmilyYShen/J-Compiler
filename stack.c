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

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


///////////////////////////////////////////////////////////////////////////////
// Stack implementation.

Stack* Stack_Allocate() {
  // TODO: The rest of this function
  Stack *s = malloc(sizeof(Stack));
  s->num_elements = 0;
  s->front = NULL;
  s->back = NULL;
  return s;
}

// TODO: The rest of the Deque functions described in Deque.h
void Stack_Free(Stack *s) {
  while (s->front) {
    StackNode* next_front = s->front->next;
    free(s->front);
    s->front = next_front;
  }
  free(s);
}

void push(Stack *s, int num, bool check) {
  StackNode *node = malloc(sizeof(StackNode));

  if (node == NULL) {
    printf("Couldn't allocate space for a new Deque Node\n");
    free(node);
    exit(EXIT_FAILURE);
  }

  node->next = NULL;
  node->prev = NULL;
  node->num = num;
  node->check = false;

  if (s->back) { 
    s->back->next = node;
    node->prev = s->back;
    s->back = node;
  } else {
    s->front = node;
    s->back = node;
  }

  s->num_elements++;

}

bool pop(Stack *s, int num) {
  if (!s->back) { 
    return false;
  }


  StackNode* prev_back = s->back->prev;

  free(s->back);
  s->back = prev_back;

  if (s->num_elements == 1) {
    s->back = NULL;
    s->front = NULL;
    //prev_back = NULL;
  } else {
    prev_back->next = NULL;
  }

  s->num_elements--;

  return true;
}

int peek(Stack *s) {
  if (!s->back) { 
    return false;
  }

  return s->back->num;
}

void check_change(Stack *s) {
  s->back->check = true;
}

