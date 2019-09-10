//Implementation of the function
#include "tableau.h"
#include "destination.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

void SaveHistory(destination history[36], int steps, tableau* board)
{
  ofstream outfile;
  string outname = "Best-pattern.txt";
  outfile.open(outname.c_str());
  outfile << "Step #   (i,j)   Piece" << endl;
  outfile << "----------------------" << endl;
  for(int i=0;i<steps;i++)
    {
      outfile << setw(4) << i << "     (" << history[i].x << "," << history[i].y << ")     " << setw(1) << board->pieces[history[i].x][history[i].y] << endl;
      board->pieces[history[i].x][history[i].y] = "E";
    }
  //Write also remaining pieces
  outfile << "----------------------" << endl;
  outfile << "   Remaining Pieces   " << endl;
  outfile << "----------------------" << endl;
  for(int i=0;i<6;i++)
    {
      for(int j=0;j<6;j++)
	{
	  if(board->pieces[i][j] != "E")
	    {
	      outfile << "(" << i << "," << j << "): " << board->pieces[i][j] << endl;
	    }
	}
    }
  outfile.close();
}
