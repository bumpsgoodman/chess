#ifndef NODE_H
#define NODE_H

typedef struct node {
	size_t x;
	size_t y;
	struct node* next;
} node_t;

#endif /* NODE_H */