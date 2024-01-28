
#include <stdio.h>
#include <math.h>



  
struct Move 
{ 
    int row, col; 
    int val;
}; 
  
char player = 'x', opponent = 'o'; 



void showBoard(char board[3][3])
{
    printf("\n\n");

    printf("\t\t\t  %c | %c  | %c  \n", board[0][0],
                             board[0][1], board[0][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n", board[1][0],
                             board[1][1], board[1][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n\n", board[2][0],
                             board[2][1], board[2][2]);

    return;
}


int askUser(char board[3][3])
{
	int val=-1;
	int i,j;
	while(val==-1){
		printf("Your Move ");
		scanf("%d",&val);
		j=val%3;
		i=val/3;
		// Check good move
		if (board[i][j]!='_'){
			printf("%d %d is used \n",i,j);
			val=-1;
		}else{	
			board[i][j]=opponent;
		}
	}
	
	return(val);
}


  
int isMovesLeft(char board[3][3]) 
{ 
    for (int i = 0; i<3; i++) 
        for (int j = 0; j<3; j++) 
            if (board[i][j]=='_') 
                return 1; 
    return 0; 
} 

  
int evaluate(char b[3][3]) 
{ 
   //printf("EVAL \n");
    // Checking for Rows for X or O victory. 
    for (int row = 0; row<3; row++) { 
        if (b[row][0]==b[row][1] && 
            b[row][1]==b[row][2]) { 
            if (b[row][0]==player) 
                return +10; 
            else if (b[row][0]==opponent) 
                return -10; 
        } 
    } 
  
    // Checking for Columns for X or O victory. 
    for (int col = 0; col<3; col++) { 
        if (b[0][col]==b[1][col] && 
            b[1][col]==b[2][col]) { 
            if (b[0][col]==player) 
                return +10; 
  
            else if (b[0][col]==opponent) 
                return -10; 
        } 
    } 
  
    // Checking for Diagonals for X or O victory. 
    if (b[0][0]==b[1][1] && b[1][1]==b[2][2]) { 
        if (b[0][0]==player) 
            return +10; 
        else if (b[0][0]==opponent) 
            return -10; 
    } 
  
    if (b[0][2]==b[1][1] && b[1][1]==b[2][0]) { 
        if (b[0][2]==player) 
            return +10; 
        else if (b[0][2]==opponent) 
            return -10; 
    } 
  
    // Else if none of them have won then return 0 
    return 0; 
} 
  
float reward(char board[3][3], int depth, int isMax) 
{ 
    int score = evaluate(board); 
    if (score == 10) return score; 
    if (score == -10) return score; 
    if (isMovesLeft(board)==0) return 0; 

    float tmp, max;
    float gamma=pow(.5,depth);
  
    if (isMax==1){  // PLAYER
        max=-1000000;
	for(int i=0;i<9;i++){
                if (board[i/3][i%3]=='_'){ 
                    board[i/3][i%3] = player; 
		    tmp=reward(board, depth+1, 0) ;
		    if(tmp>max)max=tmp;
                    board[i/3][i%3] = '_'; //UNDO
            } 
        } 
        return(gamma*max);; 
    }else{ // MIN //  OPONENT
	max=10000000;
	for(int i=0;i<9;i++){
		if (board[i/3][i%3]=='_'){ 
                    // Make the move 
                    board[i/3][i%3] = opponent; 
		    tmp=reward(board, depth+1, 1) ;
		    if(tmp<max)max=tmp;
                    board[i/3][i%3] = '_'; //UNDO
                } 
        } 
       return(gamma*max);
    } 
} 

  
struct Move findBestMove(char board[3][3]) 
{ 
    float bestVal = -100000000; 
    struct Move bestMove; 
    bestMove.row = -1; 
    bestMove.col = -1; 
  
    for (int i = 0; i<3; i++) { 
        for (int j = 0; j<3; j++) { 
            // Check if cell is empty 
            if (board[i][j]=='_') { 
                // Make the move 
                board[i][j] = player; 
  
                float moveVal = reward(board, 0, 0); 
		printf("Move %d %d is %f \n",i,j,moveVal);
  
                // Undo the move 
                board[i][j] = '_'; 
  
                // best/
                if (moveVal > bestVal) { 
                    bestMove.row = i; 
                    bestMove.col = j; 
                    bestVal = moveVal; 
                } 
            } 
        } 
    } 
  
   bestMove.val=bestVal;
  
    return bestMove; 
}


int checkWin(char board[3][3],int player)
{
	int val=0;
	if(isMovesLeft(board)){
		val=evaluate(board);
		if(val==10)
			printf("I WIN \n");
		if(val==-10)
			printf("YOu  WIN \n");
	}else{
		val=-1;
		printf("Tie \n");
	}
	return(val);
}

#define USER_F 
  
int main() 
{ 
 char board[3][3] = 
 { 
     { '_', '_', '_' }, 
     { '_', '_', '_' }, 
     { '_', '_', '_' } 
 };

 
   showBoard(board);	
   while(1){

#ifdef USER_F
    	askUser(board);
	if(checkWin(board,0)!=0)return(0);
	showBoard(board);
#endif

    	struct Move bestMove = findBestMove(board); 
  
    	printf("The Optimal Move is : "); 
    	printf("ROW: %d COL: %d VAlue: %d\n\n", bestMove.row, bestMove.col, bestMove.val ); 
    	board[bestMove.row][bestMove.col]=player;

    	showBoard(board);
	if(checkWin(board,0)!=0)return(0);


#ifndef USER_F
	askUser(board);
        showBoard(board);
	if(checkWin(board,1)!=0)return(0);
#endif




   }
 
    return 0; 
} 

