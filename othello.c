/*			Aaron Kearns 16392391
			Software Engineering Project 1
			COMP10050
			Assignment 3
			Othello/Reversi Game
			Last draft, 30/4/18
			
											*/




#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

struct player {
    char name[20];
    char color[5]; 
    int disks; //Number of disks the player has
}p0, p1;

struct disk {
    int col; //0 - 7
    int row; // 0 - 7
    int color; //White = 1, black = 0
};

struct move {
    int col;
    int row;
    char ogpos;
    int opsl;
    char ops[8]; 
    struct move *next;
};

void addMove(struct move *first, int row, int col, char ogpos){
    struct move *buff;
    struct move *prev;
    prev = first;
    buff = first->next;
    while (buff != NULL){
        if (buff->row == row && buff->col == col){
            //Update  pos
            buff->ops[buff->opsl] = ogpos;
            buff->opsl++;
            buff->ops[buff->opsl] = '\0';
            return; //Done, so exit
        } else {
            prev = buff;
            buff = buff->next;
        }
    }
    //Did not find existing move. 
    struct move *m = malloc(sizeof(struct move));
    m->row = row;
    m->col = col;
    m->ogpos = ogpos;
    m->ops[0] = ogpos;
    m->ops[1] = '\0';
    m->opsl = 1;
    m->next = NULL;
    prev->next = m;
}

void printBoard(char board[][8]){
    printf("\n\n"); //Print Top Column Identifier: 1 2 3 4 5 6 7 8
    printf("  ");
    for (int i = 1; i < 9; i++){
        char x = 96 + i;
        printf("%c ", x);
    }
    printf("\n");

    for (int i = 0; i < 8; i++){ //Print the main board
        printf("%d|", i+1);      //The left row identifier, with a following '|' character
        for (int j = 0; j < 8; j++){
            printf("%c|", board[i][j]); //The piece on the board, x for empty, 0 for black and  for white
        }
        printf("\n");
    }
}

void cleanup(struct move *first){
    struct move *buff, *b2;
    buff = first->next;
    b2 = buff->next;
    while (b2 != NULL){
        free(buff);
        buff = b2;
        b2 = buff->next;
    }
    free(buff);
}

//Function to check adjacent, and accordingly check whole row. 

//Check if terminating is same color, switch the disks.

void getPlays(char board[][8], char myCol, struct move *last){ //Get possible moves for a player
    int oppInt; //Opponent color
    char oppCol;
    struct move *buff = NULL;
    if (myCol == '0'){
        oppInt = 1;
        oppCol = '1';
    } else {
        oppInt = 0;
        oppCol = '0';
    }
    
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (board[i][j] == oppCol){
                //Time to check adjacent squares
                int a = 0, b = 0;

                //Check move on RIGHT
                while (board[i][j+a] == oppCol && j + a < 8){ //Keep going while there are more of same color
                    a++;
                }
                if (j + a < 8 && board[i][j+a] == 'x'){ // Empty square on the right. 
                    //If j + a == 8, ,that means last square also opponent color and hence no place to make move
                    if (board[i][j-1] == myCol){ //We are on the left of (the sequence) of white(s)
                        // So we can make the move on right.
                        addMove(last, i, j+a, 'l'); 
                    }
                }
                a = 0, b = 0;

                //Time to check move on LEFT
                while (board[i][j-a] == oppCol && j - a >= 0){
                    a++;
                }
                if (j - a >= 0 && board[i][j-a] == 'x'){
                    //Empty square on left. Check if we are on right.
                    if (j + b < 8 && board[i][j+1] == myCol){
                        //We are on the right, so our move on left is possible.
                        addMove(last, i, j-a, 'r');
                    }
                }

                //Check move ABOVE
                a = 1, b = 0;
                if (i > 0){
                    if (board[i-1][j] == 'x'){
                        //Empty square on top. Time to check if we are on bottom. 
                        while (board[i+b][j] == oppCol && i + b < 8){
                            b++;
                        }
                        if (i + b < 8 && board[i+b][j] == myCol){
                            addMove(last, i-a, j, 'd');
                        }
                    }
                }
                a = 0, b = 1;
                
                //Check move BELOW
                while (board[i+a][j] == oppCol && i + a < 8){
                    a++;
                }
                if (i + a < 8 && board[i+a][j] == 'x'){
                    if (i + b >= 0 && board[i-b][j] == myCol){
                        //We are on top. So can move below
                        addMove(last, i+a, j, 'u');
                    }
                }

                //Check move top left
                int ht = 0, vt = 0, dt = 0; //dt = diagonal translate
                while (board[i-dt][j-dt] == oppCol && i-dt >= 0 && j-dt >= 0){
                    dt++;
                }
                if (i - dt >= 0 && j - dt >= 0 && board[i-dt][j-dt] == 'x'){
                    //Check we are bottom right
                    if (i+1 < 8 && j + 1 < 8 && board[i+1][j+1] == myCol){
                        addMove(last, i-dt, j-dt, 'm'); //m = bottom right, based on qwerty keyboard corners
                    }
                }

                //Check move bottom right
                dt = 0;
                while (board[i+dt][j+dt] == oppCol && i + dt < 8 && j + dt < 8){
                    dt++;
                }
                if (i + dt < 8 && j + dt < 8 && board[i+dt][j+dt] == 'x'){
                    //Check if we are top left of the possible spot
                    if (i-1 >= 0 && j - 1 >= 0 && board[i-1][j-1] == myCol){
                        addMove(last, i+dt, j+dt, 'q'); //q = top right our position
                    }
                }

                //Check top right
                dt = 0;
                while (board[i-dt][j+dt] == oppCol && i-dt >= 0 && j+dt < 8){
                    dt++;
                }
                if (i-dt >= 0 && j + dt < 8 && board[i-dt][j+dt] == 'x'){
                    ///Check if we are to the bottom left
                    if (i+1 < 8 && j - 1 >= 0 && board[i+1][j-1] == myCol){
                        addMove(last, i-dt, j+dt, 'z'); //z = we are bottom left
                    }
                }

                //Check bottom left
                dt = 0;
                while (board[i+dt][j-dt] == oppCol && i + dt < 8 && j - dt >= 0){
                    dt++;
                }
                if (i + dt < 8 && j - dt >= 0 && board[i+dt][j-dt] == 'x'){
                    //Check if we top right
                    if (i-1 >= 0 && j+1 < 8 && board[i-1][j+1] == myCol){
                        addMove(last, i+dt, j-dt, 'p'); //p = we are top right
                    }
                }
            }
        }
    }
}

void applyMove(char board[][8], char myCol, int row, int col, char ogpos){ 
    int flipped = 0;
    char oppCol;
    board[row][col] = myCol;

    if (myCol == '0'){
        oppCol = '1';
    } else {
        oppCol = '0';
    }
    //If found space on left, ogpos = r (right), 
    //so keep going right till get my color, and flip all in between
    if (ogpos == 'r'){
        for (int i = col + 1; i < 8; i++){
            if (board[row][i] == oppCol){
                board[row][i] = myCol;
                flipped++;
            } else {
                break; //Reached our disk, stop.
            }
        }
    } else if (ogpos == 'l'){
        for (int i = col - 1; i >= 0; i--){
            if (board[row][i] == oppCol){
                board[row][i] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos == 'u'){     //We found space below, original position was above
        for (int i = row - 1; i >= 0; i--){
            if (board[i][col] == oppCol){
                board[i][col] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos == 'd'){
        for (int i = row + 1; i < 8; i++){
            if (board[i][col] == oppCol){
                board[i][col] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos == 'm'){
        for (int dt = 1; dt < 8; dt++){
            if (board[row + dt][col + dt] == oppCol){
                board[row+dt][col+dt] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos == 'p'){
        for (int dt = 1; dt < 8; dt++){
            if (board[row-dt][col+dt] == oppCol){
                board[row-dt][col+dt] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos == 'q'){
        for (int dt =1; dt < 8; dt++){
            if (board[row-dt][col-dt] == oppCol){
                board[row-dt][col-dt] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    } else if (ogpos  == 'z'){
        for (int dt = 1; dt < 8; dt++){
            if (board[row+dt][col-dt] == oppCol){
                board[row+dt][col-dt] = myCol;
                flipped++;
            } else {
                break;
            }
        }
    }

    if (myCol == '0'){
        p0.disks += flipped;
        p1.disks -= flipped;
    } else {
        p1.disks += flipped;
        p0.disks -= flipped;
    }
}

void printScores(){
    FILE *fp;
    fp = fopen("winner.txt", "w");

    printf("Player1 %s, points: %d", p0.name, p0.disks);
    fprintf(fp, "Player1 %s, points: %d", p0.name, p0.disks);
    printf("\nPlayer2 %s, points: %d", p1.name, p1.disks);
    fprintf(fp, "\nPlayer2 %s, points: %d", p1.name, p1.disks);
    printf("\n\nThe winner is ");
    fprintf(fp, "\n\nThe winner is ");
    if (p0.disks > p1.disks){
        printf("%s", p0.name);
        fprintf(fp, "%s", p0.name);
    } else {
        printf("%s", p1.name);
        fprintf(fp, "%s", p1.name);
    }
    fclose(fp);
}

void main(){
    //struct player p0, p1; //Declare our 2 players
    printf("Player 1 (black) enter your name: ");
    scanf("%s", p0.name);
    //printf("Enter your disk type (black or white): ");
    // scanf("%s", p1.color);
    printf("Player 2 (white) enter your name: ");
    scanf("%s", p1.name);
    // printf("Enter your disk type (black or white): ");
    // scanf("%s", p2.color);
    // strcpy(p0.name, "Kek");
    strcpy(p0.color, "Black");
    // strcpy(p1.name, "Man");
    strcpy(p1.color, "White");
    char board[8][8]; //Declare board array

    for (int i = 0; i < 8; i++){ //Initialize with 'x's
        for (int j = 0; j < 8; j++){
            board[i][j] = 'x';
        }
    }

    board[3][3] = '1'; //Center four squares with 1s and 0s
    board[4][4] = '1';
    board[3][4] = '0';
    board[4][3] = '0';
    board[3][5] = '1';

    p0.disks = 2; //Initially both have 2 disks each (the center disks)
    p1.disks = 2; 

    struct move *first = NULL;
    struct move *anEl = malloc(sizeof(struct move));
    anEl->next = NULL;
    anEl->row = -1;
    anEl->col = -1;
    int i = 0;

    while(1){
        system("cls");
        printBoard(board);
        char pl;

        if (i % 2 == 0){
            //i = 0;
            pl = '0';
        } else {
            pl = '1';
        }

        anEl->next = NULL;
        getPlays(board, pl, anEl);
        first = anEl->next;
        int lv = 0;

        if (first == NULL){
            printScores();
            break;
        } else if (p0.disks + p1.disks == 64){
            printScores();
            break;
        } else {
            if (pl == '0'){
                printf("Player %s choose your next move:\n", p0.name);
                p0.disks += 1;
            } else {
                printf("Player %s choose your next move:\n", p1.name);
                p1.disks += 1;
            }
            while (first != NULL){
                if (first->opsl > 1){
                    
                }
                printf("%d. (%d, %c)\t", lv + 1, first->row + 1, first->col + 97);
                first = first->next;
                lv++;
            }

            printf("\n\nEnter move number to apply ");
            int choice;
            scanf("%d", &choice);

            lv = 0;
            first = anEl->next;

            while (lv + 1 != choice){
                first = first->next;
                lv++;
            }

            //first is the move object
            for (int k = 0; k < first->opsl; k++){
                applyMove(board, pl, first->row, first->col, first->ops[k]);
            }
            //printf("You chose option %d, which is the position %d, %c", choice, first->row + 1, first->col + 97);
            applyMove(board, pl, first->row, first->col, first->ogpos);
            
            cleanup(anEl);
            i++;
        }
    }
}