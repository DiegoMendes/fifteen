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
    void move_blank_2_position(int tile);
    inline cardinal_direction get_direction_2_tile(int tile_row, int tile_col);
    inline void move_blank(movement_direction move);
    void new_move_blank_2_position(int tile);

#ifdef	__cplusplus
}
#endif

#endif	/* SOLVER_H */

