/* 
 * File:   fifteen.h
 * Author: Diego
 *
 * Created on 6 de Agosto de 2014, 22:42
 */

#ifndef FIFTEEN_H
#define	FIFTEEN_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>    
    
// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// empty tile's position
int empty_row, empty_col;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);



#ifdef	__cplusplus
}
#endif

#endif	/* FIFTEEN_H */

