/* 
 * File:   random_board.h
 * Author: Diego
 *
 * Created on 6 de Agosto de 2014, 22:51
 */

#ifndef RANDOM_BOARD_H
#define	RANDOM_BOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

void board_layout(void);
void shuffle(int * array, int n);
static int count_inversions(const int * array, int idx);
static int sum_inversions(const int * array);
static bool is_solvable(const int * array);


#ifdef	__cplusplus
}
#endif

#endif	/* RANDOM_BOARD_H */

