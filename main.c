#include <stdio.h>
#include <string.h>
#define BOARD_SIZE  8
int is_move_legal(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination);
int is_pawn_moveable(char board[][BOARD_SIZE], char pawn, int row, int col);
int play_single_turn(char board[][BOARD_SIZE], char pawn,int* quit_flag,int* win_flag);
int play(char board[][BOARD_SIZE]);
void place_pawns(char board[][BOARD_SIZE]);
void print_board(char board[][BOARD_SIZE]);
void update_round(int* round);/*increas round number*/
int end_game(char board[][BOARD_SIZE],char pawn,int type); /*end the game and print relevant message*/
int check_left(char board[][BOARD_SIZE],char pawn,int row,int col);
int check_right(char board[][BOARD_SIZE],char pawn,int row,int col);
int can_eat_left(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination);
int can_eat_right(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination);
void replace_eat(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination);/*replace the eaten pawn with .*/
int is_in_board(char board[][BOARD_SIZE],int row_location,int col_location);/*check if the pawn location is in the board*/
void initial_board(char  board[][BOARD_SIZE],int* X_score, int* O_score,int* win_flag,int* quit_flag,int* round);/*in case of win or quit the function will initial the game*/

int main() {
    char board[BOARD_SIZE][BOARD_SIZE] ;
    printf("Lets play Draughts!\n");
    int stop;
    stop = play(board);
    return 0;
}
/*define functions*/

int is_move_legal(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination) {/* add function that check if eat*/
    int is_legal = 0;
    if (pawn == 'X') {/*case of X */
        if ((row_destination - row_location == 1)&(board[row_destination][col_destination] == '.')&(((col_location-col_destination) == 1)|((col_location-col_destination) == -1))) {/*case of non eat move*/
            return 1;
        }
        if ((((row_destination - row_location) == 2))&(((col_location-col_destination) == 2)|((col_location-col_destination) == -2))) {/*case of eat move*/
            if (col_location == 0) {/*col 0 */
                is_legal = can_eat_right(board, pawn, row_location, col_location, row_destination, col_destination);
            }
            if (col_location == 7) {/*col 7 */
                is_legal = can_eat_left(board, pawn, row_location, col_location, row_destination, col_destination);
            }
            if ((col_location > 0) & (col_location < 7)) {/*between*/
                if (col_location > col_destination) {/*case of eat left*/
                    is_legal = can_eat_left(board, pawn, row_location, col_location, row_destination, col_destination);
                }
                if (col_destination > col_location) {/*case of eat right*/
                    is_legal = can_eat_right(board, pawn, row_location, col_location, row_destination, col_destination);
                }
            }
        }
    }
    if(pawn == 'O') {
        if ((row_location - row_destination == 1) & (board[row_destination][col_destination] == '.')&(((col_location-col_destination) == 1)|((col_location-col_destination) == -1))) {/*case of non eat move*/
            return 1;
        }
        if (((row_location - row_destination) == 2)&(((col_location-col_destination) == 2)|((col_location-col_destination) == -2))){/*case of eat move*/
            if(col_location == 0){/*col 0 */
                is_legal = can_eat_right(board, pawn, row_location, col_location, row_destination, col_destination);
            }
            if(col_location == 7){/*col 7*/
                is_legal = can_eat_left(board, pawn, row_location, col_location, row_destination, col_destination);
            }
            if((col_location>0)&(col_location<7)){/*between*/
                if(col_location<col_destination){/*try to eat left*/
                    is_legal = can_eat_right(board, pawn, row_location, col_location, row_destination, col_destination);
                }
                if(col_location>col_destination){/*try to aet right*/
                    is_legal = can_eat_left(board, pawn, row_location, col_location, row_destination, col_destination);
                }
            }
        }
    }
    return ((is_legal)&(is_in_board(board,row_destination,col_destination)));}

int is_pawn_moveable(char board[][BOARD_SIZE], char pawn, int row, int col){
    /*case of X*/
    int moveable_left = 0 ;
    int moveable_right = 0;
    int  moveable = 0;
    if(pawn=='X'){
        /*case of col 0 */
        if(col==0){
            moveable_right = check_right(board,pawn,row,col);
        }
        /*case of col 7 */
        if(col==7){
            moveable_left = check_left(board,pawn,row,col);
        }
        /*case of middle col*/
        if((col>0)&(col<7)){
            moveable_left = check_left(board,pawn,row,col);
            moveable_right = check_right(board,pawn,row,col);
        }
    }
    if(pawn=='O'){
        /*case of col 0 */
        if(col == 0){
            moveable_right = check_right(board,pawn,row,col);
        }
        /*case of col 7*/
        if (col==7){
            moveable_left= check_left(board,pawn,row,col);
        }
        if((col>0)&(col<7)){
            moveable_left = check_left(board,pawn,row,col);
            moveable_right = check_right(board,pawn,row,col);
        }
    }
    moveable = moveable_left|moveable_right;
    return moveable;
}

int play_single_turn(char board[][BOARD_SIZE], char pawn,int* quit_flag,int* win_flag) {
    int row_location;int col_location;
    int row_destination;int col_destination;
    int can_move = 0;int is_legal=0;
    printf("Player %c - Please enter pawn's location (row number followed by column number):\n", pawn);
    do { /*checking if pawn can move*/
        scanf("%d %d", &row_location, &col_location);
        if((row_location == 999)|(col_location == 999)){
            *quit_flag = 1;
            return 0;
        }
        if((board[row_location][col_location]==pawn)&(is_in_board(board,row_location,col_location))) {
            can_move = is_pawn_moveable(board, pawn, row_location, col_location);
            if (can_move == 0) {
                printf("This pawn has nowhere to move!\n");
                printf("Player %c - Bad pawn's selection please enter pawn's location again\n",pawn);
            }
        }
        else{printf("Player %c - Bad pawn's selection please enter pawn's location again\n",pawn);}
    }
    while(can_move == 0);
/*now pawn can move, we check if the move is legal*/
    printf("Player %c - Please enter pawn's destination (row number followed by column number):\n",pawn);
    do {
        scanf("%d %d", &row_destination, &col_destination);
        is_legal= is_move_legal(board,pawn,row_location,col_location,row_destination,col_destination)&(is_in_board(board,row_destination,col_destination));
        if(is_legal==0){printf("Player %c - Bad pawn's destination please enter pawn's destination again:\n",pawn);}
    }
    while (is_legal==0);
/*check if game is over*/
    if(((row_destination==7)&(pawn=='X'))|((row_destination==0)&(pawn=='O'))){*win_flag =1;}
/*the pawn can move and the move is legal, update locations*/
    board[row_location][col_location]= '.';board[row_destination][col_destination] = pawn;
/*check if pawns has been eaten and return*/
    if((row_location-row_destination==2)|(row_location-row_destination==-2)){
        printf("Ho nice move! you got him...\n");
        replace_eat(board, pawn, row_location, col_location, row_destination, col_destination);
        return 1;
    }
    else{return 0;}
}

int play(char board[][BOARD_SIZE]){
    char pawnX = 'X'; char pawnO = 'O';int win_flag = 0; char current_pawn;int quit_flag=0; int play_again = 0;int X_round_score;int O_round_score;int X_score = 0;int O_score = 0;int round = 1; /*odd X turn and even O turn*/
    place_pawns(board);
    while ((win_flag == 0)&(quit_flag == 0)){
        printf("----------Play number:%d----------\n",round);
        print_board(board);
        if(round%2!=0) {
            X_round_score = play_single_turn(board, pawnX,&quit_flag,&win_flag);
            X_score = X_score + X_round_score; current_pawn ='X';
            if(X_score == 12) {win_flag = 1;}
            else{update_round(&round);}
        }
        else{
            O_round_score = play_single_turn(board, pawnO,&quit_flag,&win_flag);
            O_score = O_score + O_round_score; current_pawn='O';
            if(O_score == 12){win_flag = 1;}
            else{update_round(&round);}
        }
        if(quit_flag == 1){
            if(end_game(board,current_pawn,0)){
                initial_board(board,&X_score,&O_score,&win_flag,&quit_flag,&round);
                continue;
            }
            else{
                break;
            }
        }
        if(win_flag == 1){
            if(end_game(board,current_pawn,1)){
                initial_board(board,&X_score,&O_score,&win_flag,&quit_flag,&round);
                continue;
            }
            else{ break;}
        }
        printf("X score:%d       O score:%d\n",X_score,O_score);
    }
    return 0;
}

void place_pawns(char board[][BOARD_SIZE]){
    int i ;
    int j ;
    for(i=0;i<BOARD_SIZE;i++){
        for(j=0;j<BOARD_SIZE;j++){
            if((i%2 == 0 && j%2==0)|(i%2!=0 && j%2!=0))
                board[i][j]=' ';
            if((i<3 && i%2 ==0 && j%2!=0)|(i<3 && i%2 !=0 && j%2==0))
                board[i][j] ='X';
            if((i==3 & j%2==0)|(i==4 & j%2!=0))
                board[i][j]='.';
            if((i>4 && i%2 !=0 && j%2==0)|(i>4 && i%2 ==0 && j%2!=0))
                board[i][j] ='O';
        }
    }
}

void print_board(char board[][BOARD_SIZE]) {

    int i = 0;
    int j = 0;
    printf("  |0 1 2 3 4 5 6 7\n");
    printf("  *----------------\n");
    for(i=0;i<BOARD_SIZE;i++){
        printf(" %d|", i);
        for(j=0;j<BOARD_SIZE;j++){
            if (j==7)
                printf("%c\n",board[i][j]);
            else
                printf("%c ",board[i][j]);
        }
    }
}

void update_round(int *round){
    *round = *round +1;
}

int end_game(char board[][BOARD_SIZE],char pawn,int type) {/*add the finish command*/
    /*type 0 = quit, type 1 = win*/
    /*return 0 = stop play, return 1 play new game*/
    int flag = 0;char answer; char trash;
    if(type == 0){/*quit*/
        printf("player %c decided to quit!\n",pawn);
        print_board(board);
        do{
            printf("Would you like to play another game?(y\\n)\n");
            scanf("%c",&trash);
            scanf("%c",&answer);
            if((answer=='y')|(answer=='n')){
                flag = 1;
            }
        }
        while(flag == 0);
    }
    if(type == 1){/*win*/
        printf("Wooohooo! player %c Won\n",pawn);
        print_board(board);
        do{
            printf("Would you like to play another game?(y\\n)\n");
            scanf("%c",&trash);
            scanf("%c",&answer);

            if((answer=='y')|(answer=='n')){
                flag = 1;
            }
        }
        while(flag == 0);
    }
    if(answer == 'y'){
        return 1;
    }
    else{
        return 0;
    }
}

int check_left(char board[][BOARD_SIZE],char pawn,int row,int col){
    /*case of X */
    if(pawn=='X'){
        if((board[row+1][col-1]=='.')|((board[row+1][col-1]=='O')&(board[row+2][col-2]=='.'))){
            return 1;
        }
    }
    /*case of O */
    if(pawn=='O'){
        if((board[row-1][col-1]=='.')|((board[row-1][col-1]=='X')&(board[row-2][col-2]=='.'))){
            return  1;
        }
    }
    return  0;
}

int check_right(char board[][BOARD_SIZE],char pawn,int row,int col){
    /*case of X */
    if(pawn=='X'){
        if((board[row+1][col+1]=='.')|((board[row+1][col+1]=='O')&(board[row+2][col+2]=='.'))){
            return 1;
        }
    }
    /*case of O */
    if(pawn=='O'){
        if((board[row-1][col+1]=='.')|((board[row-1][col+1]=='X')&(board[row-2][col+2]=='.'))){
            return  1;
        }
    }
    return  0;
}

int can_eat_left(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination){
    if(pawn=='X'){
        if((board[row_location+1][col_location-1]=='O')&(board[row_destination][col_destination]=='.')){
            return 1;
        }
        else{
            return 0;
        }
        if(pawn=='O'){
            if((board[row_location-1][col_location-1]=='X')&(board[row_destination][col_destination]=='.')){
                return 1;
            }
            else{
                return 0;
            }
        }
    }
}

int can_eat_right(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination){
    if(pawn=='X'){
        if((board[row_location+1][col_location+1]=='O')&(board[row_destination][col_destination]=='.')){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(pawn=='O'){
        if((board[row_location-1][col_location+1]=='X')&(board[row_destination][col_destination]=='.')){
            return 1;
        }
        else{
            return 0;
        }
    }
}

void replace_eat(char board[][BOARD_SIZE],char pawn,int row_location,int col_location,int row_destination,int col_destination){
    if(pawn=='X'){
        /*eat left*/
        if(col_destination<col_location){
            board[row_location+1][col_location-1]='.';
        }
            /*eat right*/
        else{
            board[row_location+1][col_location+1]='.';
        }
    }
    if(pawn=='O'){
        /*eat left*/
        if(col_destination<col_location){
            board[row_location-1][col_location-1]='.';
        }
            /*eat right*/
        else{
            board[row_location-1][col_location+1]='.';
        }
    }
}

int is_in_board(char board[][BOARD_SIZE],int row_location,int col_location){
    if((row_location<8)&(row_location>-1)&(col_location>-1)&(col_location<8)){
        return 1;
    }
    else{
        return 0;
    }
}

void initial_board(char  board[][BOARD_SIZE],int* X_score, int* O_score,int* win_flag,int* quit_flag,int* round){
    *round=1;
    *X_score=0;
    *O_score=0;
    *win_flag=0;
    *quit_flag=0;
    place_pawns(board);
    printf("Lets play Draughts!\n");
}

