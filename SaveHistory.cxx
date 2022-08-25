// Implementation of the function
#include "SaveHistory.h"
#include "tableau.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void SaveHistory( const history& aHistory, const int aSteps, const tableau& aTableau, const tableau& aInitialTableau )
{
	std::ofstream outfile;
	std::string outname = "Best-pattern.txt";
	outfile.open( outname.c_str() );
	outfile << "Step #   (i,j)   Piece" << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < aSteps; i++ )
		outfile << std::setw( 4 ) << i << "     (" << aHistory[ i ].first << "," << aHistory[ i ].second << ")     " << std::setw( 1 ) << aInitialTableau.GetPieces()[ aHistory[ i ].first ][ aHistory[ i ].second ] << std::endl;
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			if ( aTableau.GetPieces()[ i ][ j ] != "E" )
				outfile << "(" << i << "," << j << "): " << aTableau.GetPieces()[ i ][ j ] << std::endl;
	outfile.close();
}
