#include<stdio.h>

//some of my comments spill over onto the next line in the PDF, Piazza said this was OK

void displayBoard(int[]);
int legalMove(int, int);
int gameWon(int[]);
int readGame(FILE *);
void saveGame(int[], FILE *);
void clearSave(void);
int playAgain(void);
int switchPlayer (int);

int main(void){
	int endgame, location, gametype=4, i;
	  /*-the value of endgame expresses whether the game is still being played, and who (if either) won
		-location is used to store the place a player is attempting to play
		-gametype guides the program to load from the save file and/or to run a 2 player game
		-i is used only as a counter in for loops*/
		
	int gameArray[10]={0,0,0,0,0,0,0,0,0,1};//first nine places hold the board, the tenth place holds whose turn it is
	FILE *gamesave;
	
	while(gametype!=3){
		while(gametype>3){//this loop is here so a new game can be played after the previous game is completed
			
			for (i=0;i<=8;i++)//resets the gameArray at the begining of a new game
				gameArray[i]=0;
			gameArray[9]=1;
			
			endgame=0;
			
			printf("\nWelcome to Tic-Tac-Toe!\nPlease pick an option:\n 0:Load game\n 2:2-player game\n");//main menu
			scanf("%d", &gametype);
		}
		while(!endgame){
			if(gametype==0){//to load a saved game
				gamesave=fopen("TicTacToeSave.txt", "r");
				
				if (gamesave==NULL){
					
					printf("Save file doesn't exist! Starting new game!\n");
				}
				else{
					
					for(i=0;i<=9;i++)
						gameArray[i]=readGame(gamesave);
						fclose(gamesave);
				}
				gametype=2;//begins the game with the board read from the file
			}
			
			if(gametype==2){//for two-player games
				printf("Two-player game! Press '0' at any time to save and exit.\n");
				displayBoard(gameArray);
				
				while(!endgame){
					printf("Player %d, please enter a location (1-9):", gameArray[9]);
					scanf("%d",&location);
					location-=1;//correcting for the array values so typing 1 places a marker in the top left, etc.
					
					if (location==-1){//if a player inputs 0 to save the game
						gamesave=fopen("TicTacToeSave.txt", "w");
						
						saveGame(gameArray, gamesave);
						
						fclose(gamesave);
						endgame=4;//breaks out of game loop and runs 4th possible ending
					}
					
					else if (legalMove(gameArray[location], location)){//checks if the move is legal
						gameArray[location]=gameArray[9];//sets the value in the array to either a 1 or 2, depending on whose turn it is
						
						displayBoard(gameArray);//displays the board
						endgame=gameWon(gameArray);//checks to see if the game is over. If so, it sets endgame to display the correct ending 
						
						gameArray[9]=switchPlayer(gameArray[9]);//switches turn
					}
					else
						printf("Not a valid location! Try again!\n");
				}
			}
		}
	
		if (endgame!=4)//the 4th endgame option is run when the game gets saved, we don't want to clear the save file in this case
			clearSave();//clears the save file
		
		switch (endgame){//possible game results
			case 1://if P1 wins
				printf("\nPlayer 1 WINS!\n\n");
				gametype=playAgain();
				break;
			case 2://if P2 wins
				printf("\nPlayer 2 WINS!\n\n");
				gametype=playAgain();
				break;
			case 3://if draw
				printf("\nIt's a draw!!\n\n");
				gametype=playAgain();
				break;
			case 4://if the game is saved
				printf("\nGame saved!\n\n");
				gametype=3;//Making the assumption that if the player(s) are saving the game, they don't want to play again
				break;
			default:
				gametype=playAgain();
		}
	}
	
	return (0);
}

void displayBoard(int gameArray[]){//displays the board
	int i;
	char displayArray[9];
	for(i=0;i<=8;i++){
		if(gameArray[i]==0)//sets Xs and Os to display
			displayArray[i]=' ';
		else if(gameArray[i]==1)
			displayArray[i]='X';
		else if(gameArray[i]==2)
			displayArray[i]='O';
	}
	
	printf(" %c | %c | %c \n", displayArray[0],displayArray[1],displayArray[2]);
	for (i=1;i<=11;i++)//horizontal line
		printf("-");
	printf("\n %c | %c | %c \n", displayArray[3],displayArray[4],displayArray[5]);
	
	for (i=1;i<=11;i++)
		printf("-");
	printf("\n %c | %c | %c \n", displayArray[6],displayArray[7],displayArray[8]);
}

int legalMove(int arValue, int loc){//checks if move is allowed
	if (arValue==0&&loc>=0&&loc<=8)//making sure that the value in the array is 0 and the location is 1-9
		return(1);
	else
		return(0);
}

int gameWon(int checkArr[9]){//checks if game is completed and who (if either) won
	int i,count=0;
	for(i=0;i<=2;i++){//checks columns
		if (checkArr[i]==checkArr[i+3]&&checkArr[i]==checkArr[i+6])
			return (checkArr[i]);
	}
	for(i=0;i<=6;i+=3){//checks rows
		if (checkArr[i]==checkArr[i+1]&&checkArr[i]==checkArr[i+2])
			return (checkArr[i]);
	}
	if(checkArr[0]==checkArr[4]&&checkArr[0]==checkArr[8])//checks first diagonal
		return(checkArr[0]);
	
	else if(checkArr[2]==checkArr[4]&&checkArr[2]==checkArr[6])//checks second diagonal
		return(checkArr[2]);
		
	for(i=0;i<=8;i++){//checks for draws
		if (checkArr[i]!=0)
			count++;
	}
	if (count==9)//if the game is a draw
		return(3);
	
	else
		return(0);//game isnt done yet
}

int readGame(FILE *readFile){//reads the saved game from the file and establishes gameArray accordingly
	char save;
	
	fscanf(readFile,"%c,",&save);
	if (save=='A')
		return(1);
	else if (save=='B')
		return(2);
	else
		return(0);
}

void saveGame(int saveArray[], FILE *writeFile){//prints the current gameArray to the save file
	char save;
	int i;
	
	for (i=0;i<=9;i++){
		if (saveArray[i]==0)
			save=' ';
		else if (saveArray[i]==1)
			save='A';
		else if (saveArray[i]==2)
			save='B';
		fprintf(writeFile,"%c,",save);
	}
}

void clearSave(void){//clears the contents of a save file after a game is completed (by win or draw)
	FILE *clearing;
	clearing=fopen("TicTacToeSave.txt", "w");
	fprintf(clearing, " , , , , , , , , ,A,");
	fclose(clearing);
}

int playAgain(void){//asks if players want to play again at the end of a game
	int response;
	printf("Play Again?\n 1:Yes\n 2:No\n");
	scanf("%d",&response);
	if (response==1)
		return(4);
	else
		return(3);
}

int switchPlayer (int playernumber){//switches the player whose turn it is
	if (playernumber==1)
		return(2);
	else
		return(1);
}