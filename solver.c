#include "solver.h"
#include "fifteen.h"

// board, whereby board[i][j] represents row i and column j
extern int board[MAX][MAX];

// board's dimension
extern int d;

void god_mode()
{
    move_blank_2_position(1);
}

void look_4_tile(int tile, int * ret_row, int * ret_col)
{
    for (int i = 1; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
            {
                ret_row = i;
                ret_col = j;
                return;
            }
        }
    }
}


void move_blank_2_position(int tile)
{
    int tile_row, tile_col;
    
    look_4_tile(tile, &tile_row, &tile_col);
    
    cardinal_direction direction_2_tile = get_direction_2_tile(tile_row, tile_col);
    
    switch (direction_2_tile)
    {
        case N:
            if (empty_col == d - 1)
            {
                while (empty_row != tile_row + 1)
                    move_blank(up);
                
                move_blank(left);
                move_blank(up);
                move_blank(right);
            }
            else
            {
                move_blank(right);
                
                while (empty_row != tile_row)
                    move_blank(up);
            }
            break;
        case NE:
            
            while (empty_row < tile_row)
                move_blank(up);
            
            while (empty_col < tile_col)
                move_blank(right);
            
            break;
        case E:
            while (empty_col != tile_col)
                move_blank(right);
            
            break;
        case SE:
            while (empty_row != tile_row)
                move_blank(down);
            
            while (empty_col != tile_col)
                move_blank(right);
            
            break;
        case S:
            if (empty_col == d - 1)
            {
                while (empty_row != tile_row - 1)
                    move_blank(down);

                move_blank(left);
                move_blank(down);
                move_blank(right);
            }
            else
            {
                move_blank(right);
                
                while (empty_row != tile_row)
                    move_blank(down);
            }
            break;
        case SW:
            while (empty_row != tile_row)
                move_blank(down);
            
            while (empty_col != tile_col + 1)
                move_blank(left);
            
            break;
        case W:
            while (empty_col != tile_col + 1)
                move_blank(left);
            
            break;
        case NW:
            while (empty_row != tile_row)
                move_blank(up);
            
            while (empty_col != tile_col + 1)
                move_blank(left);
            
            break;
    }
}

// Get the direction from blank to target's tile adjacent position
inline cardinal_direction get_direction_2_tile(int tile_row, int tile_col)
{
    if (tile_row == empty_row)
    {
        if (tile_col < empty_col)
            return W;
        else
            return E;
    }
    
    if (tile_col == empty_col)
    {
        if (tile_row < empty_row)
            return N;
        else
            return S;
    }
    
    if (tile_row < empty_row)
    {
        if (tile_col < empty_col)
            return NW;
        else
            return NE;
    }
    else
    {
        if (tile_col < empty_col)
            return SW;
        else
            return SE;
    }
}

inline void move_blank(movement_direction move)
{    
    switch (move)
    {
        case up:
            board[empty_row][empty_col] = board[empty_row - 1][empty_col];
            board[empty_row - 1][empty_col] = 0;
            empty_row--;
            break;
        case down:
            board[empty_row][empty_col] = board[empty_row + 1][empty_col];
            board[empty_row + 1][empty_col] = 0;
            empty_row++;
            break;
        case left:
            board[empty_row][empty_col] = board[empty_row][empty_col - 1];
            board[empty_row][empty_col - 1] = 0;
            empty_col--;
            break;
        case right:
            board[empty_row][empty_col] = board[empty_row][empty_col + 1];
            board[empty_row][empty_col + 1] = 0;
            empty_col++;
            break;
    }
    
    // clear the screen
    clear();

    // draw the current state of the board
    draw();

    // saves the current state of the board (for testing)
    save();
    
    // sleep for animation's sake
    usleep(500000);
}

void new_move_blank_2_position(int tile)
{
    int tile_row, tile_col;
    
    look_4_tile(tile, &tile_row, &tile_col);
    
    if (tile == 1)
    {
        while (empty_col > tile_col + 1)
            move_blank(left);
    }
    
    if (tile_col != empty_col)
    {
        while (empty_row != tile_row)
            move_blank(down);
        
        while (empty_col > tile_col + 1)
            move_blank(left);
        
        while (empty_col < tile_col)
            move_blank(right);
    }
    else
    {
        
    }
}