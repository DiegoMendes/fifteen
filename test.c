#include "a_star.h"
#include "random_board.h"

// board, whereby board[i][j] represents row i and column j
int board_cenas[3][3];

// board's dimension
extern int d;

// empty tile's position
extern int empty_row, empty_col;

void print_solution(lifo * path);
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

char str[40];

int main(int argc, char *argv[])
{
    if (argc != 2)
        return -1;
    
    if (strcmp(argv[1],"1") == 0)
    {
        board_cenas[0][0] = 4;
        board_cenas[0][1] = 2;
        board_cenas[0][2] = 5;
    
        board_cenas[1][0] = 0;
        board_cenas[1][1] = 8;
        board_cenas[1][2] = 6;
    
        board_cenas[2][0] = 1;
        board_cenas[2][1] = 3;
        board_cenas[2][2] = 7;
    }
    else if (strcmp(argv[1],"2") == 0)
    {
        board_cenas[0][0] = 0;
        board_cenas[0][1] = 1;
        board_cenas[0][2] = 3;
    
        board_cenas[1][0] = 4;
        board_cenas[1][1] = 2;
        board_cenas[1][2] = 5;
    
        board_cenas[2][0] = 7;
        board_cenas[2][1] = 8;
        board_cenas[2][2] = 6;
    }
    else if (strcmp(argv[1],"3") == 0)
    {
        board_cenas[0][0] = 1;
        board_cenas[0][1] = 2;
        board_cenas[0][2] = 3;
    
        board_cenas[1][0] = 0;
        board_cenas[1][1] = 7;
        board_cenas[1][2] = 6;
    
        board_cenas[2][0] = 5;
        board_cenas[2][1] = 4;
        board_cenas[2][2] = 8;
    }
    else if (strcmp(argv[1],"4") == 0)
    {
        board_cenas[0][0] = 6;
        board_cenas[0][1] = 3;
        board_cenas[0][2] = 1;
    
        board_cenas[1][0] = 8;
        board_cenas[1][1] = 5;
        board_cenas[1][2] = 7;
    
        board_cenas[2][0] = 4;
        board_cenas[2][1] = 0;
        board_cenas[2][2] = 2;
    }
    else if (strcmp(argv[1],"5") == 0)
    {
        board_cenas[0][0] = 0;
        board_cenas[0][1] = 7;
        board_cenas[0][2] = 5;
    
        board_cenas[1][0] = 6;
        board_cenas[1][1] = 2;
        board_cenas[1][2] = 8;
    
        board_cenas[2][0] = 3;
        board_cenas[2][1] = 4;
        board_cenas[2][2] = 1;
    }
    else if (strcmp(argv[1],"6") == 0)
    {
        board_cenas[0][0] = 7;
        board_cenas[0][1] = 8;
        board_cenas[0][2] = 6;
    
        board_cenas[1][0] = 5;
        board_cenas[1][1] = 3;
        board_cenas[1][2] = 1;
    
        board_cenas[2][0] = 4;
        board_cenas[2][1] = 0;
        board_cenas[2][2] = 2;
    }
    else if (strcmp(argv[1],"7") == 0)
    {
        board_cenas[0][0] = 7;
        board_cenas[0][1] = 4;
        board_cenas[0][2] = 6;
    
        board_cenas[1][0] = 0;
        board_cenas[1][1] = 1;
        board_cenas[1][2] = 3;
    
        board_cenas[2][0] = 2;
        board_cenas[2][1] = 5;
        board_cenas[2][2] = 8;
    }
    else
    {
        return -2;
    }   
    
    d = 3;
    
    empty_row = 0;
    empty_col = 0;
    
    ///
    
    search_node head_node;
    
    head_node.previous = NULL;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            head_node.board[i][j] = board_cenas[i][j];
        }
    }
    gen_node_key(&head_node);
    #if DEBUG
    sprintf(str, "Node has key = %llu\n", head_node.key);
    god_mode_debug(str);
    #endif
    head_node.g = 0;
    //printf("starting a*\n");
    ///
    lifo * solution = NULL;
    int ret = a_star_search(&head_node, &solution);
    if (solution == NULL)
        printf("a_star_search return %d\n", ret);
    draw_node(&head_node);
    print_solution(solution);
    
    return 0;
}

void print_solution(lifo * path)
{
    lifo_entry * entry = path->head;
    lifo_entry * aux;
    while (path->total_entries > 0)
    {
        switch(entry->node->move)
        {
            case up:
                printf("move blank up\n");
                break;
            case down:
                printf("move blank down\n");
                break;
            case left:
                printf("move blank left\n");
                break;
            case right:
                printf("move blank right\n");
                break;
        }
        draw_node(entry->node);
        aux = entry->next;
        free(entry);
        entry = aux;
        path->head = entry;
        path->total_entries--;
    }
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
