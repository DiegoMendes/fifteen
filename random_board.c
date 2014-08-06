#include "fifteen.h"
#include "random_board.h"

// board, whereby board[i][j] represents row i and column j
extern int board[MAX][MAX];

// board's dimension
extern int d;

static int count_inversions(const int * array, int idx);
static int sum_inversions(const int * array);
static bool is_solvable(const int * array);

void board_layout(void)
{
    int total_tiles = d * d;
    int layout[total_tiles];
    int i, j, aux;
    
    for (int i = 0; i < total_tiles; i++)
    {
        layout[i] = i;
    }
    
    shuffle(layout, total_tiles);
    
    if (!is_solvable(layout)) // Odd parity
    {
        if (layout[0] != 0 && layout[1] != 0)
        {
            aux = layout[0];
            layout[0] = layout[1];
            layout[1] = aux;
        }
        else
        {
            aux = layout[total_tiles - 1];
            layout[total_tiles - 1] = layout[total_tiles - 2];
            layout[total_tiles - 2] = aux;
        }
    }
    
    for (i = 0; i < d; i++)
    {
        for (j =0; j < d; j++)
        {
            board[i][j] = layout[i * d + j];
        }
    }
    
    return;
}

/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(int *array, int n)
{    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int usec = tv.tv_usec;
    srand48(usec);


    if (n > 1) {
        int i;
        for (i = n - 1; i > 0; i--) {
            int j = (unsigned int) (drand48()*(i+1));
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
    
    return;
}

/*  Returns n_i (refer to http://mathworld.wolfram.com/15Puzzle.html
 *                  and   http://www.sitepoint.com/randomizing-sliding-puzzle-tiles/)
 *
 */
static int count_inversions(const int * array, int idx)
{
    int n_i = 0;
    
    int value = array[idx];
    
    if (value == 0)
        return 0;
    
    for (int i = idx + 1; i < d * d; i++)
    {
        if (array[i] < value && array[i] != 0)
        {
            n_i++;
        }
    }
    
    return n_i;
}

static int sum_inversions(const int * array)
{
    int N = 0;
    
    for (int i = 0; i < d * d; i++)
    {
        N += count_inversions(array, i);
    }
    
    //printf("%s - %d inversions\n", (N % 2) ? "Odd Parity" : "Even Parity", N);
    return N;
}

static bool is_solvable(const int * array)
{
    int inversions = sum_inversions(array);
    int total_slots = d * d;
    int i, empty_row;
    
    if (d % 2)  // Odd width
    {
        return (inversions % 2) ? false : true;
    }
    else        // Even width
    {
        for (i = 0; i < total_slots; i++)
            if (array[i] == 0)
                break;
                
        empty_row = (i / d) + 1;    
           
        return ((inversions + empty_row) % 2) ? false : true;
    }
    
}