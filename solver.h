/* 
 * File:   solver.h
 * Author: Diego
 *
 * Created on 5 de Agosto de 2014, 23:52
 */

#ifndef SOLVER_H
#define	SOLVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "fifteen.h"

    typedef enum 
    {
        N,
        NE,
        E,
        SE,
        S,
        SW,
        W,
        NW
    }cardinal_direction;
    
    typedef enum
    {
        up,
        down,
        left,
        right        
    }movement_direction;

    
    void god_mode();
    void look_4_tile(int tile, int * ret_row, int * ret_col);
    void move_blank_2_position(int tile_row, int tile_col);
    void move_blank(movement_direction move);
    void move_tile_2_position(int target_row, int target_col);
    void move_tile(movement_direction move);
    void fix_top_corner(int iteration);
    void fix_bottom_corner(int iteration);
    void rotate_blank(cardinal_direction starting_reference, int number_movements, bool clockwise);
    void god_mode_debug(char * str);
    void fix_top_row(int iteration);
    void fix_left_collumn(int iteration);


#ifdef	__cplusplus
}
#endif

#endif	/* SOLVER_H */

