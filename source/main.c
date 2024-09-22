#include <3ds.h>
#include <stdio.h>

char turn, winner;
char board[3][3];
int turns;

void resetBoard(void) {
    turn = 'X';
    winner = 'w';
    turns = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

int main(int argc, char* argv[]) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);  // Change to GFX_BOTTOM

    resetBoard();
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        printf("\x1b[0;0H");
        if (winner == 'T')
            printf("Tie!\n");
        else if (winner != 'w')
            printf("%c won!\n", winner);
        else
            printf("      \n");

        for (int i = 0; i < 3; i++) {
            printf("\n-------\n");
            for (int j = 0; j < 3; j++)
                printf("|%c", board[i][j]);
            printf("|");
        }
        printf("\n-------\n");

        touchPosition touch;
        if (kDown & KEY_TOUCH && winner == 'w') {
            hidTouchRead(&touch);

            // Convert touch coordinates to Tic-Tac-Toe grid position
            int r = (touch.py - 16) / 16;
            if (r >= 0 && r < 3) {
                int c = (touch.px - 8) / 16;  // Adjust this if the grid size or position changes
                if (c >= 0 && c < 3 && board[r][c] == ' ') {
                    board[r][c] = turn;
                    if ((board[0][c] == turn && board[1][c] == turn && board[2][c] == turn) ||
                        (board[r][0] == turn && board[r][1] == turn && board[r][2] == turn) ||
                        (board[1][1] == turn &&
                         ((board[0][0] == turn && board[2][2] == turn) ||
                          (board[0][2] == turn && board[2][0] == turn))))
                        winner = turn;
                    else if (turns++ == 9)
                        winner = 'T';
                    else
                        turn = (turn == 'X') ? 'O' : 'X';
                }
            }
        }

        if (kDown & KEY_START && winner != 'w') {
            resetBoard();
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}

