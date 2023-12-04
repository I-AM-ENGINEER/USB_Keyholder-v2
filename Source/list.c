#include "list.h"

list_return_t list_search_free(list_t* list){
    for(uint16_t i = 0; i < list->max_nodes_count; i++){
		if((list->nodes[i].previous == -1) && (i != list->start)){
			list->free = i;
			return LIST_STATE_OK;
		}
	}
	return LIST_STATE_ERROR;
}

list_return_t list_init(list_t* list, list_node_t* node_buffer, uint16_t nodes_count) {
    if(list == NULL){
        return LIST_STATE_ERROR;
    }
    if(node_buffer == NULL){
        return LIST_STATE_ERROR;
    }
    if(nodes_count == 0){
        return LIST_STATE_ERROR;
    }
	list->head = -1;
	list->free = 0;
	list->start = -1;
	list->max_nodes_count = nodes_count;
	list->nodes_count = 0;
	list->nodes = node_buffer;
	for (uint16_t i = 0; i < list->max_nodes_count; i++) {
		list->nodes[i].next 		= -1;
		list->nodes[i].previous     = -1;
	}
	return LIST_STATE_OK;
}

list_return_t list_append( list_t* list ) {
    if(list == NULL){
        return LIST_STATE_ERROR;
    }
    list_search_free(list);
	if(list->free == -1){
		return LIST_STATE_ERROR;
	}
	if(list->nodes_count == 0){
		list->nodes[list->free].previous = -1;
		list->start = list->free;
	}else{
		list->nodes[list->head].next = list->free;
		list->nodes[list->free].previous = list->head;
	}

	list->head = list->free;

	list->nodes_count++;
	return LIST_STATE_OK;
}

uint16_t list_get_head_node_idx( list_t* list ){
	return list->head;
}

void list_set_head_node_data( list_t* list, void* data ){
	list->nodes[list->head].data = data;
}

list_return_t list_get_head_node_data( list_t* list, void** data ){
	*data = list->nodes[list->head].data;
	return LIST_STATE_OK;
}

uint16_t list_get_count_max( list_t* list ){
	return list->max_nodes_count;
}

uint16_t list_get_count( list_t* list ){
	return list->nodes_count;
}

int16_t list_num_to_id(list_t* list, uint16_t node_number){
	if(node_number >= list->nodes_count) return -1;
	int16_t next_element = list->start;
	for(uint16_t i = 0; i < node_number; i++){
		if(next_element == -1) return -1;
		next_element = list->nodes[next_element].next;
	}
	return next_element;
}

list_return_t list_get_node_data(list_t* list, uint16_t node_number, void** data ) {
	if(node_number >= list->nodes_count){
		return LIST_STATE_ERROR;
	}
	
	int16_t node_id = list_num_to_id(list, node_number);
	if(node_id == -1){
		return LIST_STATE_ERROR;
	}
	
	*data = list->nodes[node_id].data;
	//*data = 1;//list->nodes[node_id].data;
	
	return LIST_STATE_OK;
}



list_return_t list_swap(list_t* list, uint16_t node_a, uint16_t node_b) {
	if(list == NULL){
		return LIST_STATE_ERROR;
	}
	if(node_b >= list->nodes_count){
		return LIST_STATE_ERROR;
	}
	if(node_a >= list->nodes_count){
		return LIST_STATE_ERROR;
	}
	if(node_a == node_b){
		return LIST_STATE_OK;
	}

    uint16_t node_id_1 = list_num_to_id(list, node_a);
    uint16_t node_id_2 = list_num_to_id(list, node_b);

    // Get the nodes to be swapped
    list_node_t* node_1 = &list->nodes[node_id_1];
    list_node_t* node_2 = &list->nodes[node_id_2];

    // Get the neighbors of the nodes to be swapped
    list_node_t* node_1_prev = (node_1->previous >= 0) ? &list->nodes[node_1->previous] : NULL;
    list_node_t* node_1_next = (node_1->next >= 0) ? &list->nodes[node_1->next] : NULL;
    list_node_t* node_2_prev = (node_2->previous >= 0) ? &list->nodes[node_2->previous] : NULL;
    list_node_t* node_2_next = (node_2->next >= 0) ? &list->nodes[node_2->next] : NULL;

    // Update the previous pointers of the neighbors
    if (node_1_prev != NULL) {
        node_1_prev->next = node_id_2;
    }
    if (node_2_prev != NULL) {
        node_2_prev->next = node_id_1;
    }

    // Update the next pointers of the neighbors
    if (node_1_next != NULL) {
        node_1_next->previous = node_id_2;
    }
    if (node_2_next != NULL) {
        node_2_next->previous = node_id_1;
    }

    // Swap the previous pointers of the nodes
    int16_t temp_prev = node_1->previous;
    node_1->previous = node_2->previous;
    node_2->previous = temp_prev;

    // Swap the next pointers of the nodes
    int16_t temp_next = node_1->next;
    node_1->next = node_2->next;
    node_2->next = temp_next;

    // Update the start and head pointers if necessary
    if (list->start == node_id_1) {
        list->start = node_id_2;
    } else if (list->start == node_id_2) {
        list->start = node_id_1;
    }

    if (list->head == node_id_1) {
        list->head = node_id_2;
    } else if (list->head == node_id_2) {
        list->head = node_id_1;
    }
    return LIST_STATE_OK;
}

list_return_t list_move( list_t* list, uint16_t node_from, uint16_t node_to){
	if(list == NULL){
		return LIST_STATE_ERROR;
	}
	if(node_from >= list->nodes_count){
		return LIST_STATE_ERROR;
	}
	if(node_to > list->nodes_count){
		return LIST_STATE_ERROR;
	}
	if(node_from == node_to){
		return LIST_STATE_OK;
	}

    uint16_t node_to_id;
    uint16_t node_from_id = list_num_to_id(list, node_from);
    if(node_to == list->nodes_count){
        node_to_id = list_num_to_id(list, node_to-1);
    }else{
        node_to_id = list_num_to_id(list, node_to);
    }

    // Get the previous node of the node to be moved from
    int16_t node_from_id_previous = list->nodes[node_from_id].previous;

    // Remove the node to be moved from its current position
    if (node_from_id_previous < 0) {
        // Node to be moved is the first node in the list
        list->start = list->nodes[node_from_id].next;
        if (list->start >= 0) {
            list->nodes[list->start].previous = -1;
        }
    } else {
        list->nodes[node_from_id_previous].next = list->nodes[node_from_id].next;
    }
    if (list->nodes[node_from_id].next >= 0) {
        list->nodes[list->nodes[node_from_id].next].previous = node_from_id_previous;
    } else {
        // Node to be moved is the last node in the list
        list->head = node_from_id_previous;
    }

    // Insert the node to be moved to its new position
    if (node_to_id == list->start) {
        // Node to be moved is inserted at the beginning of the list
        list->start = node_from_id;
        list->nodes[node_from_id].previous = -1;
    } else {
        list->nodes[list->nodes[node_to_id].previous].next = node_from_id;
        list->nodes[node_from_id].previous = list->nodes[node_to_id].previous;
    }

    list->nodes[node_from_id].next = node_to_id;
    list->nodes[node_to_id].previous = node_from_id;

    if(node_to == list->nodes_count){
        list_swap(list, node_to-2, node_to-1);
    }

	return LIST_STATE_OK;
}


list_return_t list_remove(list_t* list, uint16_t node_number) {
	if(list == NULL){
		return LIST_STATE_ERROR;
	}
	if(node_number >= list->nodes_count){
		return LIST_STATE_ERROR;
	}

	uint16_t node_id = list_num_to_id(list, node_number);

    if(node_id == list->start){
        list->start = list->nodes[list->start].next;
    }else{
        list->nodes[list->nodes[node_id].previous].next = list->nodes[node_id].next;
    }

    if(node_id == list->head){
        list->head = list->nodes[list->head].previous;
    }else{
        list->nodes[list->nodes[node_id].next].previous = list->nodes[node_id].previous;
    }

	list->nodes[node_id].data = NULL;
	list->nodes[node_id].next = -1;
	list->nodes[node_id].previous = -1;

	list->nodes_count--;

	return LIST_STATE_OK;
}
