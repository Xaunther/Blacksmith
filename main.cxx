//Main function

#include "tableau.h"
#include "destination.h"
#include "SaveHistory.h"
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

int main()
{
  //Initialize random seed
  srand(time(NULL));

  destination poshistory[36];
  destination best_poshistory[36];  
  int step = 0;
  int best_step = 0;
  //Ask for initial position
  cout << "Initial row? (Number 0-5): ";
  cin >> poshistory[step].x;
  cout << "Initial column? (Number 0-5): ";
  cin >> poshistory[step].y;
  
  tableau board; //Initialize board (tableau)
  tableau initial_board;
  string boarddata = "board.dat"; //File where initial board is saved
  initial_board.Load(boarddata); //Load the board
  
  for(int i=0;i<maxsteps;i++)//Iterate many times
    {
      board = initial_board;
      step = 0;
      while(poshistory[step].x > -1) //When no moves available, it is -1
	{
	  step++;
	  poshistory[step] = board.Move(poshistory[step-1].x, poshistory[step-1].y);
	}
      if(step>best_step)//New record!
	{
	  best_step = step;
	  for(int j=0;j<best_step;j++)
	    {
	      best_poshistory[j] = poshistory[j]; 
	    }
	}
    }
  SaveHistory(best_poshistory, best_step, &initial_board);
  return 0;
}
