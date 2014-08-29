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
 
#include "fifteen.h"
#include "random_board.h"
#include "a_star.h"

// board, whereby board[i][j] represents row i and column j
extern int board[MAX][MAX];

// board's dimension
extern int d;

// empty tile's position
extern int empty_row, empty_col;


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
                god_mode();
                printf("GOD mode finnished\n");
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
    board_layout();
    
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == 0)
            {
                empty_row = i;
                empty_col = j;
                return;
            }
        }
    }
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
            if (i == ((board[i][j] - 1) / d) && (j == ((board[i][j] - 1) % d)))
                printf("\x1B[32m""%2d ""\033[0m", board[i][j]);
            else
                !board[i][j] ? printf(" _ ") : printf("%2d ", board[i][j]);
        }
        //printf("%2d\n", board[i][j]);
        if (i == ((board[i][j] - 1) / d) && (j == ((board[i][j] - 1) % d)))
            printf("\x1B[32m""%2d\n""\033[0m", board[i][j]);
        else
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
    
    // ensure input is valid
    if (tile < 1 || tile > d * d - 1)
        return false;
    
    if (empty_row < d - 1 && board[empty_row + 1][empty_col] == tile) // is tile below blank?
    {
        board[empty_row][empty_col] = tile;
        empty_row++;
        board[empty_row][empty_col] = 0;
        return true;
    }
    if (empty_row > 0 && board[empty_row - 1][empty_col] == tile) // is tile above blank?
    {
        board[empty_row][empty_col] = tile;
        empty_row--;
        board[empty_row][empty_col] = 0;
        return true;
    }
    if (empty_col < d - 1 && board[empty_row][empty_col + 1] == tile) // is tile right of blank?
    {
        board[empty_row][empty_col] = tile;
        empty_col++;
        board[empty_row][empty_col] = 0;
        return true;
    }
    if (empty_col > 0 && board[empty_row][empty_col - 1] == tile) // is tile left of blank?
    {
        board[empty_row][empty_col] = tile;
        empty_col--;
        board[empty_row][empty_col] = 0;
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

