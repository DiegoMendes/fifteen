#include "solver.h"

#include <inttypes.h>

#include <stdint.h>

// Search nodes structure
typedef struct node
{
    uint64_t key;
    int board[3][3];
    int g;
    struct node * previous;
    movement_direction move;
}search_node;

// Priority Queue structures
typedef struct queue_entry{
    search_node * node;
    int priority;
    struct queue_entry * next;
    struct queue_entry * previous;
}pq_entry;

typedef struct{
    pq_entry * head;
    int total_entries;
}priority_queue;

// LIFO Queue structures
typedef struct lifo_entry{
    struct lifo_entry * next;
    //struct lifo_entry * previous;
    search_node * node;
}lifo_entry;

typedef struct{
    lifo_entry * head;
    int total_entries;
}lifo;

// Search nodes functions

int a_star_search(search_node * root, lifo ** path);
extern inline int manhattan_distance(search_node * node);
extern inline int h(search_node * node);
extern inline void node_find_blank(search_node * node, int * row, int * col);
int expand_node(priority_queue * open_set, lifo * closed_set);
extern inline int expand_node_up(search_node *source_node, search_node ** new_node);
extern inline int expand_node_down(search_node *source_node, search_node ** new_node);
extern inline int expand_node_left(search_node *source_node, search_node ** new_node);
extern inline int expand_node_right(search_node *source_node, search_node ** new_node);
extern inline bool equal_nodes(search_node * a, search_node * b);
extern inline void copy_board(search_node * a, search_node * b);
extern inline bool is_goal(search_node * node);
extern inline int f(search_node * node);
void draw_node(search_node * node);
lifo * recover_path(search_node * node);
void gen_node_key(search_node * node);

// Priority Queue functions

int pq_add_entry(priority_queue * queue, search_node * node, int priority);
void pq_pop_head(priority_queue * queue);
void init_priority_queue(priority_queue * queue);
bool node_in_open_set(priority_queue * open_set, search_node * node);
void print_priority_queue(priority_queue * queue);

// LIFO functions
int lifo_add_entry(lifo * queue, search_node * node);
int lifo_rem_entry(lifo * queue, search_node * node);
void init_lifo_queue(lifo * queue);
bool node_in_closed_set(lifo * closed_set, search_node * node);
void print_lifo_queue(lifo * queue);
