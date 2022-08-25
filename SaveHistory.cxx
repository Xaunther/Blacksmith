// Implementation of the function
#include "SaveHistory.h"
#include "tableau.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void SaveHistory( const history& aHistory, const int aSteps, tableau& aTableau )
{
	std::ofstream outfile;
	std::string outname = "Best-pattern.txt";
	outfile.open( outname.c_str() );
	outfile << "Step #   (i,j)   Piece" << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < aSteps; i++ )
	{
		outfile << std::setw( 4 ) << i << "     (" << aHistory[ i ].first << "," << aHistory[ i ].second << ")     " << std::setw( 1 ) << aTableau.GetPieces()[ aHistory[ i ].first ][ aHistory[ i ].second ] << std::endl;
		aTableau.SetPiece( aHistory[ i ], "E" );
	}
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < 6; i++ )
	{
		for ( int j = 0; j < 6; j++ )
		{
			if ( aTableau.GetPieces()[ i ][ j ] != "E" )
			{
				outfile << "(" << i << "," << j << "): " << aTableau.GetPieces()[ i ][ j ] << std::endl;
			}
		}
	}
	outfile.close();
}
