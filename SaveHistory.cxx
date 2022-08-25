// Implementation of the function
#include "destination.h"
#include "tableau.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void SaveHistory( destination history[ 36 ], int steps, tableau* board )
{
	std::ofstream outfile;
	std::string outname = "Best-pattern.txt";
	outfile.open( outname.c_str() );
	outfile << "Step #   (i,j)   Piece" << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < steps; i++ )
	{
		outfile << std::setw( 4 ) << i << "     (" << history[ i ].x << "," << history[ i ].y << ")     " << std::setw( 1 ) << board->pieces[ history[ i ].x ][ history[ i ].y ] << std::endl;
		board->pieces[ history[ i ].x ][ history[ i ].y ] = "E";
	}
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < 6; i++ )
	{
		for ( int j = 0; j < 6; j++ )
		{
			if ( board->pieces[ i ][ j ] != "E" )
			{
				outfile << "(" << i << "," << j << "): " << board->pieces[ i ][ j ] << std::endl;
			}
		}
	}
	outfile.close();
}
