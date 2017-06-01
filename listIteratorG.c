// listIteratorG.c ... Generic List Iterator implementation
// Written by jung

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"

void * search(IteratorG it, void * start, void *vp, int right);

typedef struct Node {
	void * vp; // value of the node
	struct Node * next; // pointer to next node
	struct Node * prev; // pointer to previous node
} Node;

typedef struct IteratorGRep {
	int size; // size of the list
	Node * after; // node after cursor
	Node * before; // node before cursor
	Node * first; // first node
	Node * last; // last node
	Node * recent; // node called by most recent next, prev etc

	ElmCompareFp cmp;
	ElmCopyFp copy;
	ElmFreeFp free; 
} IteratorGRep;

Node * newNode(IteratorG it, void *vp) {
	Node * new;
	new = malloc(sizeof(Node));
	assert(new != NULL);
	new->vp = it->copy(vp);
	new->prev = new->next = NULL;
	return new;
}

IteratorG IteratorGNew(ElmCompareFp cmp, ElmCopyFp copy, ElmFreeFp free) {
	struct IteratorGRep * it;

	it = malloc(sizeof(struct IteratorGRep));
	assert(it != NULL);
	it->size = 0;
	it->after = NULL;
	it->before = NULL;
	it->first = NULL;
	it->last = NULL;
	it->recent = NULL;

	it->cmp = cmp;
	it->copy = copy;
	it->free = free;
	return it;
}

int  add(IteratorG it, void *vp) {
	assert(it != NULL);
	Node * n;

	n = malloc(sizeof(Node));
	assert(n != NULL);
	n->vp = it->copy(vp);
	n->prev = n->next = NULL;

	if(it->first == NULL) {
		it->after = NULL;
		it->before = it->first = it->last = n;
	} else if(it->after == NULL) {
		it->before->next = n;
		n->prev = it->before;
		it->before = it->last = n;
	} else if(it->before == NULL) {
		it->after->prev = n;
		n->next = it->after;
		it->before = it->first = n;
	} else {
		n->next = it->after;
		it->after->prev = it->before->next = n;
		n->prev = it->before;
		it->before = n;
	}
	it->size++;
	it->recent = NULL;
	return 1;
}

int hasNext(IteratorG it) {
	return it->after != NULL;
}

int hasPrevious(IteratorG it) {
	return it->before != NULL;
}

void *next(IteratorG it) {
	if(!hasNext(it))
		return NULL;
	assert(it != NULL);
	it->before = it->after;
	it->after = it->after->next;
	it->recent = it->before;
	return it->recent->vp;
}

void *previous(IteratorG it){
	if(!hasPrevious(it))
		return NULL;
	assert(it != NULL);
	it->after = it->before;
	it->before = it->before->prev;
	it->recent = it->after;
	return it->recent->vp;
}

int  delete(IteratorG it) {
	if(it->recent == NULL || it->size == 0)
		return 0;

	assert(it != NULL);
	if(it->size == 1)
		it->after = it->before = it->first = it->last = NULL;
	else if(it->after == NULL) {
		it->last = it->before->prev;
		it->before->prev->next = NULL;
		it->before = it->last;
	} else if(it->before == NULL) {
		it->after->next->prev = NULL;
		it->first = it->after->next;
		it->after = it->first;
	} else {
		if(it->recent == it->after) {
			if(it->recent == it->last) {
				it->before->next = NULL;
				it->last = it->before;
				it->after = NULL;
			} else {
				it->after->next->prev = it->before;
				it->before->next = it->after->next;
				it->after = it->after->next;
			}
		} else if(it->recent == it->before) {
			if(it->recent == it->first) {
				it->after->prev = NULL;
				it->first = it->after;
				it->before = NULL;
			} else {
				it->before->prev->next = it->after;
				it->after->prev = it->recent->prev;
				it->before = it->before->prev;
			}
		}
	}
	it->free(it->recent->vp);
	it->size--;
	it->recent = NULL;
	return 1;
}

int set(IteratorG it, void *vp) {
  	if(it->recent == NULL)
		return 0;

	assert(it != NULL);
	if(it->recent == it->before)
		it->before->vp = it->copy(vp);
	else if(it->recent == it->after)
		it->after->vp = it->copy(vp);
	it->recent = NULL;
	return 1;
}

void *findNext(IteratorG it, void *vp) {
	return search(it, next(it), vp, 1);
}

void *findPrevious(IteratorG it, void *vp) {
	return search(it, previous(it), vp, 0);
}

void * search(IteratorG it, void * start, void *vp, int right) {
	assert(it != NULL);
	if(start == NULL)
		return NULL;
	if(it->cmp(start, vp) == 0)
		return start;
	if(right)
		return search(it, next(it), vp, 1);
	return search(it, previous(it), vp, 0);
}

void reset(IteratorG it) {
	if(it->size == 0)
		return;
	it->after = it->first;
	it->before = NULL;
	it->recent = NULL;
}