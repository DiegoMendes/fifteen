#include "a_star.h"

// board's dimension
extern int d;

char str[40];

int a_star_search(search_node * root, lifo ** path)
{
    search_node * node = root;
    
    #if DEBUG
        god_mode_debug("Creating new priority queue\n");
    #endif
    lifo closed_set;
    init_lifo_queue(&closed_set);
    
    priority_queue queue;
    init_priority_queue(&queue);
    #if DEBUG
        god_mode_debug("Adding first entry to priority queue\n");
    #endif
    pq_add_entry(&queue, root, h(root));
    
    while (queue.total_entries > 0)
    {
        #if DEBUG
        god_mode_debug("\n----------------\nNew iteration\n----------------\n");
        print_lifo_queue(&closed_set);        
        print_priority_queue(&queue);
        god_mode_debug("Expanding node\n");
        #endif
        node = queue.head->node;
        
        #if DEBUG
        draw_node(node);
        #endif
        if (expand_node(node, &queue, &closed_set) == 1)
        {
            #if DEBUG
            god_mode_debug("Goal node found\n");
            #endif
            *path = recover_path(node);
            if (*path == NULL)
            {
                #if DEBUG
                god_mode_debug("unable to recover path\n");
                #endif
                return -1;
            }
            #if DEBUG
            god_mode_debug("Returning solution\n");  
            #endif 
            return 0;
        }
    }
    // Solution not found
    return -2;
}

lifo * recover_path(search_node * node)
{
    lifo_entry * entry;
    lifo * path = malloc(sizeof (lifo));
    if (path == NULL)
        return NULL;
        
    path->total_entries = 0;
    path->head = NULL;
    
    while (node->previous != NULL)
    {
        entry = malloc(sizeof (lifo_entry));
        if (entry == NULL)
        {
            // TODO: clear entries from lifo
            return NULL;
        }
        //entry->move = node->move;
        entry->node = node;
        entry->next = path->head;
        
        path->head = entry;
        
        path->total_entries++;
        
        node = node->previous;
    }
    
    return path;   
}

void draw_node(search_node * node)
{
    int i, j;
    god_mode_debug("\n");
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3 - 1; j++)
        {
            //printf("%2d ", board[i][j]);
            !node->board[i][j] ? sprintf(str, " _ ") : sprintf(str, "%2d ", node->board[i][j]);
            god_mode_debug(str);
        }
        //printf("%2d\n", board[i][j]);
        !node->board[i][j] ? sprintf(str, " _\n") : sprintf(str, "%2d\n", node->board[i][j]);
        god_mode_debug(str);
    }
    god_mode_debug("\n");
    return;
}

inline int h(search_node * node)
{
    return manhattan_distance(node);
}

inline int manhattan_distance(search_node * node)
{
    int ret = 0;
    int delta_row, delta_col;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (node->board[i][j] == 0)
                continue;
            
            delta_row = i - ((node->board[i][j] - 1) / d - (d - 3));
            delta_col = j - ((node->board[i][j] - 1) % d - (d - 3));
            
            ret += (delta_row < 0) ? (-delta_row) : delta_row;
            ret += (delta_col < 0) ? (-delta_col) : delta_col;
        }
    }   
    
    return ret;
}

int expand_node(search_node * node, priority_queue * open_set, lifo * closed_set)
{
    search_node * successor[4];
    
    if (is_goal(node))
        return 1;
    
    for (int k = 0; k < 4; k++)
    {        
        switch (k)
        {
            case up:
                if (expand_node_up(node, &successor[k]) < 0)
                    continue;
                break;
            case down:
                if (expand_node_down(node, &successor[k]) < 0)
                    continue;
                break;
            case left:
                if (expand_node_left(node, &successor[k]) < 0) 
                    continue;
                break;
            case right:
                if (expand_node_right(node, &successor[k]) < 0)
                    continue;
                break;
        }
        
        #if DEBUG
        sprintf(str, "Checking if node with id %llu is already in history\n", successor[k]->key);
        god_mode_debug(str);
        #endif
        if(node_in_closed_set(closed_set, successor[k]) || node_in_open_set(open_set, successor[k]))
        {
            free(successor[k]);
            #if DEBUG
            god_mode_debug("Node is already in history\n");
            #endif
        }
        else
        {
            #if DEBUG
            god_mode_debug("Node is not in history, it will be added to open_set\n");
            #endif
            pq_add_entry(open_set, successor[k], f(successor[k]));
        }
    }
    
    lifo_add_entry(closed_set, open_set->head->node);
    pq_pop_head(open_set);
    
    return 0;
}

inline void node_find_blank(search_node * node, int * row, int * col)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (node->board[i][j] == 0)
            {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

inline int expand_node_up(search_node *source_node, search_node ** new_node)
{
    int aux, blank_row, blank_col;
    
    node_find_blank(source_node, &blank_row, &blank_col);
    
    if (blank_row > 0)
    {
        *new_node = malloc(sizeof (search_node));
        if (*new_node == NULL)
            return -1;
            
        copy_board(source_node, *new_node);
        
        aux = (*new_node)->board[blank_row - 1][blank_col];
        (*new_node)->board[blank_row - 1][blank_col] = 0;
        (*new_node)->board[blank_row][blank_col] = aux;
        (*new_node)->move = up;
        
        gen_node_key(*new_node);
        (*new_node)->g = source_node->g + 1;
        (*new_node)->previous = source_node;
        return 0;
    }
    else
    {
        // is not possible to expand uppward
        *new_node = NULL; 
        return -2;
    }
}

inline int expand_node_down(search_node *source_node, search_node ** new_node)
{
    int aux, blank_row, blank_col;
    
    node_find_blank(source_node, &blank_row, &blank_col);
    
    if (blank_row < 2)
    {
        *new_node = malloc(sizeof (search_node));
        if (*new_node == NULL)
            return -1;
            
        copy_board(source_node, *new_node);
        
        aux = (*new_node)->board[blank_row + 1][blank_col];
        (*new_node)->board[blank_row + 1][blank_col] = 0;
        (*new_node)->board[blank_row][blank_col] = aux;
        (*new_node)->move = down;
        
        gen_node_key(*new_node);
        (*new_node)->g = source_node->g + 1;
        (*new_node)->previous = source_node;
        return 0;
    }
    else
    {
        // is not possible to expand uppward
        *new_node = NULL; 
        return -2;
    }
}

inline int expand_node_left(search_node *source_node, search_node ** new_node)
{
    int aux, blank_row, blank_col;
    
    node_find_blank(source_node, &blank_row, &blank_col);
    
    if (blank_col > 0)
    {
        *new_node = malloc(sizeof (search_node));
        if (*new_node == NULL)
            return -1;
            
        copy_board(source_node, *new_node);
        
        aux = (*new_node)->board[blank_row][blank_col - 1];
        (*new_node)->board[blank_row][blank_col - 1] = 0;
        (*new_node)->board[blank_row][blank_col] = aux;
        (*new_node)->move = left;
        
        gen_node_key(*new_node);
        (*new_node)->g = source_node->g + 1;
        (*new_node)->previous = source_node;
        return 0;
    }
    else
    {
        // is not possible to expand uppward
        *new_node = NULL; 
        return -2;
    }
}

inline int expand_node_right(search_node *source_node, search_node ** new_node)
{
    int aux, blank_row, blank_col;
    
    node_find_blank(source_node, &blank_row, &blank_col);
    
    if (blank_col < 2)
    {
        *new_node = malloc(sizeof (search_node));
        if (*new_node == NULL)
            return -1;
            
        copy_board(source_node, *new_node);
        
        aux = (*new_node)->board[blank_row][blank_col + 1];
        (*new_node)->board[blank_row][blank_col + 1] = 0;
        (*new_node)->board[blank_row][blank_col] = aux;
        (*new_node)->move = right;
        
        gen_node_key(*new_node);
        (*new_node)->g = source_node->g + 1;
        (*new_node)->previous = source_node;
        return 0;
    }
    else
    {
        // is not possible to expand uppward
        *new_node = NULL; 
        return -2;
    }
}

inline bool equal_nodes(search_node * a, search_node * b)
{
    if (a == NULL || b == NULL)
        return false;
        
    return a->key == b->key;
}

inline void copy_board(search_node * a, search_node * b)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            b->board[i][j] = a->board[i][j];
        }
    }
}

inline bool is_goal(search_node * node)
{
    return h(node) == 0;
}

// f = g + h
inline int f(search_node * node)
{
    return node->g + h(node);
}

void gen_node_key(search_node * node)
{
    uint64_t aux = 0;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            aux |= node->board[i][j];
            aux <<= 5;
        }
    }
    aux >>= 5;
    
    node->key = aux;
}

// Priority Queue functions

void init_priority_queue(priority_queue * queue)
{
    queue->head = NULL;
    queue->total_entries = 0;
}

int pq_add_entry(priority_queue * queue, search_node * node, int priority)
{
    pq_entry * aux;
    
    pq_entry * entry = malloc(sizeof(pq_entry));
    if (entry == NULL)
        return -1;
    
    entry->node = node;
    entry->priority = priority;
    
    if (queue->total_entries > 0)
    {
        aux = queue->head;
        while (aux->next != NULL)
        {
            if (equal_nodes(aux->node, node) && (aux->node->g < node->g))
            {
                free(entry);
                return -1;
            }
            aux = aux->next;
        }    
        
        aux = queue->head;
        while (aux->next != NULL && aux->next->priority < priority)
            aux = aux->next;
         
        entry->next = aux->next;
        aux->next = entry;
    }
    else
    {
        queue->head = entry;
    }
    queue->total_entries++;
    
    return 0;
}

void pq_pop_head(priority_queue * queue)
{
    if (queue->total_entries > 1)
    {
        pq_entry * new_head = queue->head->next;
        free(queue->head);
        queue->head = new_head;
    }
    else
    {
        free(queue->head);
        queue->head = NULL;
    }
    queue->total_entries--;
}

bool node_in_open_set(priority_queue * open_set, search_node * node)
{
    pq_entry * aux;
    aux = open_set->head;
    for (aux = open_set->head; aux != NULL; aux = aux->next)
    {        
        if(equal_nodes(aux->node, node))
            return true;
    }
    
    return false;
}

void print_priority_queue(priority_queue * queue)
{
    pq_entry * entry = queue->head;
    god_mode_debug("_____________________________________________\n");
    sprintf(str, "Total entries in PQ is %d\n\n",queue->total_entries);
    god_mode_debug(str);
    for (int i = 0; i < queue->total_entries; i++, entry = entry->next)
    {
        sprintf(str, "priority = %d\n", entry->priority);
        god_mode_debug(str);
        sprintf(str, "moves = %d\n", entry->node->g);
        god_mode_debug(str);
        sprintf(str, "manhattan = %d\n",h(entry->node));
        god_mode_debug(str);
        draw_node(entry->node);
        
    }
    god_mode_debug("_____________________________________________\n");
}

// LIFO Queue functions

int lifo_add_entry(lifo * queue, search_node * node)
{
    if (queue == NULL || node == NULL)
        return -1;
        
    lifo_entry * entry = malloc(sizeof (lifo_entry));
    if (entry == NULL)
        return -2;
    
    entry->node = node;
    entry->next = queue->head;
    
    queue->head = entry;
    queue->total_entries++;
    
    return 0;
}

int lifo_rem_entry(lifo * queue, search_node * node)
{
    if (queue == NULL)
        return -1;
    
    lifo_entry * entry = queue->head;
    lifo_entry * aux = NULL;
    
    while (entry->next != NULL)
    {
        if(equal_nodes(entry->next->node, node))
        {
            aux = entry->next;
            entry->next = aux->next;
            free(aux);
            return 0;
        }
        entry = entry->next;
    }
    
    return -2;
}

void init_lifo_queue(lifo * queue)
{
    queue->total_entries = 0;
    queue->head = NULL;
}

bool node_in_closed_set(lifo * closed_set, search_node * node)
{
    if (closed_set->total_entries == 0)
        return false;
        
    lifo_entry * aux;
    
    for (aux = closed_set->head; aux != NULL; aux = aux->next)
    {
        #if DEBUG
        sprintf(str, "[%s] Comparing nodes with id %llu and %llu\n", __FUNCTION__, node->key, aux->node->key);
        god_mode_debug(str);
        #endif
        
        if(equal_nodes(aux->node, node))
            return true;
    }
        
    return false;
}

void print_lifo_queue(lifo * queue)
{
    lifo_entry * entry = queue->head;
    god_mode_debug("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    sprintf(str, "Total entries in LIFO is %d\n\n",queue->total_entries);
    god_mode_debug(str);
    for (int i = 0; i < queue->total_entries; i++, entry = entry->next)
    {
        sprintf(str, "id = %llu\n", entry->node->key);
        god_mode_debug(str);
        sprintf(str, "moves = %d\n", entry->node->g);
        god_mode_debug(str);
        draw_node(entry->node);
        
    }
    god_mode_debug("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
}

