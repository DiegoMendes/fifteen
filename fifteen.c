/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
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

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);
void board_layout(void);
bool solvable(void);
void shuffle(int * array, int n);
bool is_permutation_even(int * array, int n);
static int count_inversions(const int * array, int idx);
static int sum_inversions(const int * array);
static bool is_solvable(const int * array);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    
    
    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }
    
    
    
    // initialize the board
    init();
    
    board_layout();
    
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }


        int tile;
        // prompt for move
        printf("Tile to move: "); 
        while (true)
        {
            string line = GetString();
            if (strncmp(line,"GOD", 3) == 0)
            {
                printf("Enter GOD mode\n");
                free(line);
                return 0;
            }
            else
            {
                char c;
                puts(line);
                if (sscanf(line, " %d %c", &tile, &c) == 1)
                {
                    free(line);
                    break;
                }
                else
                {
                    free(line);
                    printf("Retry: ");
                }
            }
        }

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    int i;
    
    for (i = d * d - 1; i > 2; i--)
    {
        board[d - i / d - 1][d -  i % d - 1] = i;
    }
    
    if (d % 2)
    {
        board[d - 1][d - 3] = 2;
        board[d - 1][d - 2] = 1;
        board[d - 1][d - 1] = 0;
    }
    else
    {
        board[d - 1][d - 3] = 1;
        board[d - 1][d - 2] = 2;
        board[d - 1][d - 1] = 0;
    }
    
    return;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    int i, j;
    
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d - 1; j++)
        {
            //printf("%2d ", board[i][j]);
            !board[i][j] ? printf(" _ ") : printf("%2d ", board[i][j]);
        }
        //printf("%2d\n", board[i][j]);
        !board[i][j] ? printf(" _\n") : printf("%2d\n", board[i][j]);
    }
    
    return;
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int i, j;
    
    // ensure input is valid
    if (tile < 1 || tile > d * d - 1)
        return false;
    
    // look for the tile's position
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (board[i][j] == tile)
                goto position_found;
        } 
    }
    
    position_found: ;// nop for legibility
    
    // look for the empty slot next to the tile
    // check above for empty slot
    if (i < d - 1 && board[i + 1][j] == 0)
    {
        board[i + 1][j] = tile;
        board[i][j] = 0 ;
        return true;
    }
    // check below for empty slot
    if (i > 0 && board[i - 1][j] == 0)
    {
        board[i - 1][j] = tile;
        board[i][j] = 0 ;
        return true;
    }
    // check right for empty slot
    if (j < d - 1 && board[i][j + 1] == 0)
    {
        board[i][j + 1] = tile;
        board[i][j] = 0 ;
        return true;
    }
    // check left for empty slot
    if (j > 0 && board[i][j - 1] == 0)
    {
        board[i][j - 1] = tile;
        board[i][j] = 0 ;
        return true;
    }
    
    // empty slot isn't adjecent to the tile
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
   
    for (int i = 0; i < d * d - 1; i++)
    {
        if (board[i / d][i % d] != i + 1)
            return false;
    }
    return true;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
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
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}

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
