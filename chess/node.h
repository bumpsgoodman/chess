#ifndef NODE_H
#define NODE_H

typedef struct node {
	size_t x;
	size_t y;
	struct node* next;
} node_t;

void insert_front(node_t** phead, const size_t x, const size_t y);
int delete_node(node_t** phead, node_t* deleted_node);
void print_list(const node_t* head_or_null);
void destroy_list(node_t* head_or_null);

#endif /* NODE_H */