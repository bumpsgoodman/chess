#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "board.h"
#include "common_defines.h"

void insert_front(node_t** phead, const size_t x, const size_t y)
{
    assert(phead != NULL);

    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    assert(new_node != NULL);

    new_node->x = x;
    new_node->y = y;
    new_node->next = *phead;

    *phead = new_node;
}

void print_node(const node_t* head_or_null)
{
    const node_t* p = head_or_null;
    char coord[COORD_LENGTH];
    while (p != NULL) {
        translate_to_coord(p->x, p->y, coord);
        printf("(%s) ", coord);
        p = p->next;
    }
    printf("\n");
}

void destroy_node(node_t* head_or_null)
{
    node_t* p = head_or_null;
    while (p != NULL) {
        node_t* next = p->next;
        free(p);
        p = next;
    }
}