#include <stdio.h>
#include "cprocessing.h"

#define GOL_GRID_COLS 30
#define GOL_GRID_ROWS 30
#define GOL_GRID_BUFFERS 2

#define GOL_ALIVE 1
#define GOL_DEAD 0

#define TRUE 1
#define FALSE 0

int gIsPaused;
int gGrids[GOL_GRID_BUFFERS][GOL_GRID_ROWS][GOL_GRID_COLS];

/* Feel free to declare your own variables here */
float CellWidth, CellHeight;
int RefBuffer, DrawBuffer;
CP_Color AliveCol, DeadCol;


void game_init(void)
{
    /* Set every grids' cells as 'dead' */
    for (int row = 0; row < GOL_GRID_ROWS; ++row) {
        for (int col = 0; col < GOL_GRID_COLS; ++col) {
            for (int i = 0; i < GOL_GRID_BUFFERS; ++i) {
                gGrids[i][row][col] = GOL_DEAD;
            }
        }
    }

    /*********************************************************
    *  Let's start with gGrids[0] as the 'reference grid'
    *  and gGrids[1] as the 'displaying grid'.
    *
    *  We initialize gGrids[0] with a simple 'glider'.
    *********************************************************/

    gGrids[0][1][2] = GOL_ALIVE;
    gGrids[0][2][3] = GOL_ALIVE;
    gGrids[0][3][1] = GOL_ALIVE;
    gGrids[0][3][2] = GOL_ALIVE;
    gGrids[0][3][3] = GOL_ALIVE;

    /* We start unpaused */
    gIsPaused = FALSE;

    /* Initialization of your other variables here */
    CP_System_SetFrameRate(10.f);
    CP_System_SetWindowSize(800, 800);
    CellWidth  = (float) CP_System_GetWindowWidth()  / GOL_GRID_ROWS;
    CellHeight = (float) CP_System_GetWindowHeight() / GOL_GRID_COLS;
    RefBuffer = 0;
    DrawBuffer = 1;
    AliveCol = CP_Color_Create(200, 0, 0, 255);
    DeadCol = CP_Color_Create(100, 100, 100, 255);
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(100, 100, 100, 255));

    if (CP_Input_KeyTriggered(KEY_ANY))
        gIsPaused = !gIsPaused;
    // If paused, edit.
    if (gIsPaused) {
        if (CP_Input_MouseClicked()) {
            CP_Vector click; click.x = CP_Input_GetMouseX(); click.y = CP_Input_GetMouseY();
            int row = click.x / CellWidth;
            int col = click.y / CellHeight;
            gGrids[DrawBuffer][row][col] = !gGrids[DrawBuffer][row][col]; // Flip alive/dead state
        }
    }

    // Playing, update current buffer.
    else {
        for (int row = 0; row < GOL_GRID_ROWS; row++) {
            for (int col = 0; col < GOL_GRID_COLS; col++) {
                // Using refbuffer to count neightbours,
                int neighbours = 0;
                for (int i = row - 1; i <= row + 1; i++) {
                    for (int j = col - 1; j <= col + 1; j++) {
                        if ((i == row && j == col) || // if is current cell
                            (i < 0 || j < 0) || // Out of range
                            (i >= GOL_GRID_ROWS || j >= GOL_GRID_COLS)) // Out of range
                            continue;
                        if (gGrids[RefBuffer][i][j] == GOL_ALIVE)
                            neighbours++;
                    }
                }

                // Updating drawbuffer with reference to refbuf, neighbours
                if (gGrids[DrawBuffer][row][col] == GOL_ALIVE) { // If current cell alive
                    if (neighbours < 2 || neighbours > 3)
                        gGrids[DrawBuffer][row][col] = GOL_DEAD;
                    else if (neighbours == 2 || neighbours == 3)
                        gGrids[DrawBuffer][row][col] = GOL_ALIVE;
                }
                else {
                    if (neighbours == 3)
                        gGrids[DrawBuffer][row][col] = GOL_ALIVE;
                }
            }
        }
    }

    // Finally, draw the drawbuffer
    for (int row = 0; row < GOL_GRID_ROWS; row++) {
        for (int col = 0; col < GOL_GRID_COLS; col++) {
            CP_Settings_Fill(DeadCol);
            if (gGrids[DrawBuffer][row][col])
                CP_Settings_Fill(AliveCol);
            CP_Graphics_DrawRect(row * CellWidth, col * CellHeight, CellWidth, CellHeight);

            // current drawbuffer becomes next frame's refbuffer
            gGrids[RefBuffer][row][col] = gGrids[DrawBuffer][row][col];
        }
    }

    // swap draw and ref buf.
    DrawBuffer = RefBuffer;
    RefBuffer= !RefBuffer;
}

void game_exit(void)
{

}