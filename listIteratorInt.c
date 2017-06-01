
 // listIteratorInt.c ... list Iterator ADT implementation
 // Written by jung
 // Date: 1-04-2016


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorInt.h"


typedef struct Node {
	int v; // value of the node
	struct Node * next; // pointer to next node
	struct Node * prev; // pointer to previous node
} Node;

typedef struct IteratorIntRep {

	int size; // size of the list
	Node * after; // node after cursor
	Node * before; // node before cursor
	Node * first; // first node
	Node * last; // last node
	Node * recent; // node called by most recent next, prev etc
  
} IteratorIntRep;

IteratorInt IteratorIntNew() {
	struct IteratorIntRep * it;

	it = malloc(sizeof(struct IteratorIntRep));
	assert (it != NULL);
	it->size = 0;
	it->after = NULL;
	it->before = NULL;
	it->first = NULL;
	it->last = NULL;
	it->recent = NULL;
	return it;
}

Node * newNode(int v) {
	Node * new;
	new = malloc(sizeof(Node));
	assert (new != NULL);
	new->v = v;
	new->prev = new->next = NULL;
	return new;
}

int  add(IteratorG it, void *vp);
int  hasNext(IteratorG it);
int  hasPrevious(IteratorG it);
void *next(IteratorG it);
void *previous(IteratorG it);
int  delete(IteratorG it);
int  set(IteratorG it, void *vp);
void *findNext(IteratorG it, void *vp);
void *findPrevious(IteratorG it, void *vp);

void reset(IteratorG it) {
	if (it->size == 0)
		return;
	it->after = it->first;
	it->before = NULL;
	it->recent = NULL;
}
