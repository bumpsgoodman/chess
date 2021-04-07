#ifndef NODE_H
#define NODE_H

typedef struct node {
	size_t x;
	size_t y;
	struct node* next;
} node_t;

void insert_front(node_t** phead, const size_t x, const size_t y);
void print_node(const node_t* head);
void destroy_node(node_t* head);

#endif /* NODE_H */