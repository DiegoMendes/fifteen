#include "a_star.h"

// board, whereby board[i][j] represents row i and column j
extern int board[MAX][MAX];

// board's dimension
extern int d;

// empty tile's position
extern int empty_row, empty_col;

char str[40];

void god_mode()
{
    for (int i = 0; i < d - 3; i++)
    {
        #if DEBUG
        sprintf(str, "Filling %dth top row\n",i);
        god_mode_debug(str);
        #endif
        // Fix top row
        fix_top_row(i);
        
        #if DEBUG
        sprintf(str, "Fix top corner\n");
        god_mode_debug(str);
        #endif
        // Fix top corner
        fix_top_corner(i);
        
        // Fix left collumn
        fix_left_collumn(i);
        
        #if DEBUG
        sprintf(str, "Fix bottom corner\n");
        god_mode_debug(str);
        #endif
        
        // Fix bottom corner
        fix_bottom_corner(i);
    }
    
    #if DEBUG
    // Solve 3 x 3 puzzle
    god_mode_debug("Solving 3 x 3 puzzle\n");
    #endif
    
    search_node head_node;
    
    head_node.previous = NULL;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            head_node.board[i][j] = board[i + d - 3][j + d - 3];
        }
    }
    head_node.g = 0;
    
    lifo * solution = NULL;
    
    #if DEBUG
    god_mode_debug("Calling a_star_search\n");
    #endif
    a_star_search(&head_node, &solution);
    if (solution == NULL)
        printf("solution is NULL\n");
    #if DEBUG
    god_mode_debug("Solution for 3 x 3 puzzle retrieved, begginning tiles movements\n");
    #endif
    lifo_entry * entry = solution->head;
    lifo_entry * aux;
    
    while (solution->total_entries > 0)
    {
        switch(entry->node->move)
        {
            case up:
                move_blank(up);
                break;
            case down:
                move_blank(down);
                break;
            case left:
                move_blank(left);
                break;
            case right:
                move_blank(right);
                break;
        }
        aux = entry->next;
        free(entry);
        entry = aux;
        solution->head = entry;
        solution->total_entries--;
    }
    #if DEBUG
    god_mode_debug("3 x 3 puzzle solved\n");
    #endif
    return;
}

void fix_top_row(int iteration)
{
    int tile_row, tile_col;
    int target_row, target_col;
    
    for (int tile = iteration * d + iteration + 1; (tile - 1) % d < d - 2; tile++)
    {
        #if DEBUG
        sprintf(str, "Moving tile %d to position\n",tile);
        god_mode_debug(str);
        #endif    
        look_4_tile(tile, &tile_row, &tile_col);
        move_blank_2_position(tile_row, tile_col);
            
        target_row = (tile - 1) / d;
        target_col = (tile - 1) % d;
            
        move_tile_2_position(target_row, target_col);
    }
    
    return;
}

void fix_left_collumn(int iteration)
{
    int tile_row, tile_col;
    int target_row, target_col;
    
    for (int tile = (iteration + 1) * (d + 1); (tile - 1)/ d < d - 2; tile += d)
    {
        #if DEBUG
        sprintf(str, "Moving tile %d to position\n",tile);
        god_mode_debug(str);
        #endif
        look_4_tile(tile, &tile_row, &tile_col);
        move_blank_2_position(tile_row, tile_col);
            
        target_row = (tile - 1) / d;
        target_col = (tile - 1) % d;
            
        move_tile_2_position(target_row, target_col);
    }
    return;
}

void fix_bottom_corner(int iteration)
{
    int larger_tile = d * (d  - 1) + iteration + 1;
    int smaller_tile = larger_tile - d;
    
    int tile_row, tile_col;
    int target_row, target_col;
    
    look_4_tile(larger_tile, &tile_row, &tile_col);
    move_blank_2_position(tile_row, tile_col);
    
    // Move larger tile to position [d - 2][iteration + 1]
    target_row = d - 2;
    target_col = iteration + 1;
    #if DEBUG
    sprintf(str, "Moving tile %d to position [d - 2][iteration + 1]\n",larger_tile);
    god_mode_debug(str);
    #endif
    move_tile_2_position(target_row, target_col);
    
    // If smaller_tile is in position [d - 2][iteration], reorder tiles
    if (board[d - 2][iteration] == smaller_tile)
    {
        #if DEBUG
        sprintf(str, "Tile %d in position [d - 2][iteration]\n",smaller_tile);
        god_mode_debug(str);
        
        sprintf(str, "Reordering tiles\n");
        god_mode_debug(str);
        #endif
        
        // Reorder tiles
        move_blank(left);
        rotate_blank(NE, 3, true);
        move_blank(right);
        move_blank(right);
        rotate_blank(NE, 2, true);
        move_blank(left);
        rotate_blank(SW, 3, true);
        rotate_blank(SW, 2, false);
        #if DEBUG
        sprintf(str, "Reordering complete\n");
        god_mode_debug(str);
        #endif
    }
    else    // Else move larger tile to position [d - 2][iteration]
    {
        #if DEBUG
        sprintf(str, "Moving tile %d to position [d - 2][iteration]\n",larger_tile);
        god_mode_debug(str);
        #endif
    
        // Move larger tile to position [d - 2][iteration]
        move_tile(left);
        #if DEBUG
        sprintf(str, "Moving tile %d to position [d - 2][iteration + 1]\n",smaller_tile);
        god_mode_debug(str);
        #endif
        
        // Move smaller tile to position [d - 2][iteration + 1]
        look_4_tile(smaller_tile, &tile_row, &tile_col);
        move_blank_2_position(tile_row, tile_col);
    
        target_col = iteration + 1;
    
        move_tile_2_position(target_row, target_col);
    }
    #if DEBUG
    sprintf(str, "Rotating corner tiles\n");
    god_mode_debug(str);
    #endif
    
    // Rotate tiles to fix corner
    rotate_blank(NE, 2, true);
    rotate_blank(SE, 3, true);
}

void fix_top_corner(int iteration)
{
    int larger_tile = (iteration + 1) * d;
    int smaller_tile = larger_tile - 1;
    
    int tile_row, tile_col;
    int target_row, target_col;
    
    look_4_tile(larger_tile, &tile_row, &tile_col);
    move_blank_2_position(tile_row, tile_col);
        
    // If smaller_tile is in position [iteration][d - 2], reorder tiles 
    if (board[iteration][d - 2] == smaller_tile)
    {
        #if DEBUG
        sprintf(str, "Tile %d in position [iteration][d - 2]\n",smaller_tile);
        god_mode_debug(str);
        #endif
        
        // Move larger tile to position [iteration + 2][d - 2]
        target_row = iteration + 2;
        target_col = d - 2;
        
        #if DEBUG
        sprintf(str, "Moving tile %d to position [iteration + 2][d - 2]\n",larger_tile);
        god_mode_debug(str);
        #endif
        
        move_tile_2_position(target_row, target_col);
        
        #if DEBUG
        sprintf(str, "Reordering tiles\n");
        god_mode_debug(str);
        #endif
        
        // Reorder tiles
        move_blank_2_position(iteration, d - 2);
        
        rotate_blank(NE, 3, false);
        
        move_blank_2_position(target_row, target_col);
        move_tile(up);
        
        rotate_blank(SE, 4, false);
        
        #if DEBUG
        sprintf(str, "Reordering complete\n");
        god_mode_debug(str);
        #endif
    }
    else    // Else move larger tile to position [iteration][d - 2]
    {
        #if DEBUG
        sprintf(str, "Moving tile %d to position [iteration][d - 2]\n",larger_tile);
        god_mode_debug(str);
        #endif
        
        // Move larger tile to position [iteration][d - 2]
        target_row = iteration;
        target_col = d - 2;
    
        move_tile_2_position(target_row, target_col);
        
        #if DEBUG
        sprintf(str, "Moving tile %d to position [iteration + 1][d - 2]\n",smaller_tile);
        god_mode_debug(str);
        #endif
        
        // Move smaller tile to position [iteration + 1][d - 2]
        look_4_tile(smaller_tile, &tile_row, &tile_col);
        move_blank_2_position(tile_row, tile_col);
    
        target_row = iteration + 1;
    
        move_tile_2_position(target_row, target_col);
    }
    #if DEBUG
    sprintf(str, "Rotating corner tiles\n");
    god_mode_debug(str);
    #endif
    
    // Rotate tiles to fix corner
    rotate_blank(SE, 3, false);
}

/*  NW | NE
 *  -------
 *  SW | SE
 *
 *
 */
void rotate_blank(cardinal_direction starting_reference, int number_movements, bool clockwise)
{
    if (number_movements == 0)
        return;
        
    switch (starting_reference)
    {
        case NE:
            move_blank(clockwise ? down : left);
            rotate_blank(clockwise ? SE : NW, number_movements - 1, clockwise);
            return;
        case SE:
            move_blank(clockwise ? left : up);
            rotate_blank(clockwise ? SW : NE, number_movements - 1, clockwise);
            return;
        case SW:
            move_blank(clockwise ? up : right);
            rotate_blank(clockwise ? NW : SE, number_movements - 1, clockwise);
            return;
        case NW:
            move_blank(clockwise ? right : down);
            rotate_blank(clockwise ? NE : SW, number_movements - 1, clockwise);
            return;
        default:
            return;      
    }
}

void look_4_tile(int tile, int * ret_row, int * ret_col)
{
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                *ret_row = i;
                *ret_col = j;
                return;
            }
        }
    }
}

void move_blank(movement_direction movement)
{    
    switch (movement)
    {
        case up:
            move(board[empty_row - 1][empty_col]);
            break;
        case down:
            move(board[empty_row + 1][empty_col]);
            break;
        case left:
            move(board[empty_row][empty_col - 1]);
            break;
        case right:
            move(board[empty_row][empty_col + 1]);
            break;
    }
    
    // clear the screen
    clear();

    // draw the current state of the board
    draw();

    // saves the current state of the board (for testing)
    save();
    
    // sleep for animation's sake
    usleep(250000);
}

/* TODO Attempt to reduce this function's complexity
 *
 * Move blank tile to the right of the target tile
 *
 * tile_row - target tile's row
 * tile_col - target tile's collumn
 *
 */
void move_blank_2_position(int tile_row, int tile_col)
{
    if (tile_col == empty_col)
    {
        if (tile_col < d - 1)
        {
            move_blank(right);
            
            while (empty_row < tile_row)
                move_blank(down);
                
            while (empty_row > tile_row)
                move_blank(up);
        }
        else
        {
            move_blank(down); // Verify this condition
            
            move_blank(left);
            
            while (empty_row < tile_row)
                move_blank(down);
                
            while (empty_row > tile_row)
                move_blank(up);
                
            move_blank(right);    
        }
        
        return;
    }
    else
    {
        if (tile_col < d - 1)
        {
            if (empty_row == tile_row)
            {
                while (empty_col < tile_col)
                    move_blank(right);
            }
            else
            {
                while (empty_col < tile_col + 1)
                    move_blank(right);
            }
            
            
            while (empty_row < tile_row)
                move_blank(down);   
                
            while (empty_row > tile_row)
                move_blank(up);
            
            while (empty_col > tile_col + 1)
                move_blank(left);
        }
        else
        {
            while (empty_row < tile_row)
                move_blank(down);
                
            while (empty_col < tile_col - 1)
                move_blank(right);
                
            while (empty_row > tile_row)
                move_blank(up);
                   
            move_blank(right);
        }
    }
    
    
    return;
}

void move_tile_2_position(int target_row, int target_col)
{
    while (target_col + 1 > empty_col)
        move_tile(right);
    
    while (target_row < empty_row)
        move_tile(up);
        
    while (target_row > empty_row)
        move_tile(down);
    
    while (target_col + 1 < empty_col)
        move_tile(left);
}

// TODO: Implement diagonal movements (use cardinal_direction structure?)
void move_tile(movement_direction move)
{
    bool aux_cond; // auxiliary condition
    
    switch (move)
    {
        case up:
            rotate_blank(SE, 5, false);
            break;
        case down:
            rotate_blank(NE, 5, true);
            break;
        case left:
            aux_cond = (empty_row == d - 1);
            rotate_blank(aux_cond ? SE : NE, 2, !aux_cond);
            rotate_blank(aux_cond ? NE : SE, 3, !aux_cond);
            break;
        case right:
            aux_cond = (empty_row == d - 1);
            rotate_blank(aux_cond ? SE : NE, 3, aux_cond);
            rotate_blank(aux_cond ? NW : SW, 2, aux_cond);            
            break;
    }
}


void god_mode_debug(char * str)
{
    // log
    const string log = "log.txt";
    
    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }
    
    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "%s", str);

    // close log
    fclose(p);
}

