#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>

typedef struct {
	void 	*data;
	int16_t previous;
	int16_t next;
} list_node_t;

typedef struct {
	list_node_t *nodes;
	uint16_t nodes_count;
	int16_t start;
	int16_t head;
	int16_t free;
	int16_t max_nodes_count;
} list_t;

typedef enum {
	LIST_STATE_OK,
	LIST_STATE_ERROR,
} list_return_t;

uint16_t list_get_count( list_t* list );
uint16_t list_get_head_node_idx( list_t* list );
void list_set_head_node_data( list_t* list, void* data );
list_return_t list_get_head_node_data( list_t* list, void** data );
list_return_t list_get_node_data(list_t* list, uint16_t node_number, void** data );
list_return_t list_init( list_t* list, list_node_t* node_buffer, uint16_t nodes_count);
list_return_t list_append( list_t* list );
list_return_t list_remove( list_t* list, uint16_t node_number);
list_return_t list_move( list_t* list, uint16_t node_from, uint16_t node_to);
list_return_t list_swap( list_t* list, uint16_t node_a, uint16_t node_b);

#endif // __LIST_H__
