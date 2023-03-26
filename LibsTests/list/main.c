#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define NODES_COUNT 1000

list_node_t nodes[10];
list_t      list;

void print_list(list_t* list) {
    for(uint16_t i = 0; i < list_get_count(list); i++){
        char* dat;
        list_get_node_data(list, i, &dat);
        printf("%s\r\n", dat);
    }
}

int main()
{
    list_init(&list, nodes, 10);
    for(uint16_t i = 0; i < 10; i++){
        list_append(&list);
        char* dat = malloc(20);
        sprintf(dat, "Pass_%d", i);
        list_set_head_node_data(&list, dat);
    }

    //list_swap(&list, 9, 8);
    //list_move(&list, 1, 1);
    //list_move(&list, 1, 1);



    //list_swap(&list, 8, 9);
    //list_move(&list, 8, 0);
    //list_remove(&list, 1);
    //list_remove(&list,9);
    //list_remove(&list,0);
    //list_remove(&list,1);
    //list_remove(&list,1);
   // list_remove(&list,1);
    //list_remove(&list,5);
    //#list_remove(&list,9);
    //#list_remove(&list,6);
    //#list_remove(&list,1);
    //#list_remove(&list,7);



    // Main loop for the terminal program
    while(1) {
        printf("\nChoose an operation:\n");
        printf("1. Swap two nodes\n");
        printf("2. Move a node\n");
        printf("3. Remove a node\n");
        printf("4. Exit\n");

        int choice;
        scanf("%d", &choice);

        if(choice == 1) {
            uint16_t node1, node2;
            printf("Enter the indices of the nodes to swap: ");
            scanf("%hu %hu", &node1, &node2);

            list_swap(&list, node1, node2);

            print_list(&list);
        }
        else if(choice == 2) {
            uint16_t node_from, node_to;
            printf("Enter the index of the node to move: ");
            scanf("%hu", &node_from);
            printf("Enter the index to move the node to: ");
            scanf("%hu", &node_to);

            list_move(&list, node_from, node_to);

            print_list(&list);
        }
        else if(choice == 3) {
            uint16_t node_id;
            printf("Enter the index of the node to remove: ");
            scanf("%hu", &node_id);

            list_remove(&list, node_id);

            print_list(&list);
        }
        else if(choice == 4) {
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
