#include "SaveHistory.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

namespace blacksmith
{

void SaveHistory( const history& aHistory, const CTableau& aTableau, const CTableau& aInitialTableau )
{
	std::ofstream outfile;
	std::string outname = "Best-pattern.txt";
	outfile.open( outname.c_str() );
	outfile << "Step #   (i,j)   Piece" << std::endl;
	outfile << "----------------------" << std::endl;

	unsigned int index = 0;
	for( const auto& epoch : aHistory )
		outfile << std::setw( 4 ) << index++ << "     (" << epoch.first << "," << epoch.second << ")     " << std::setw( 1 ) << aInitialTableau.GetPieces()[ epoch.first ][ epoch.second ] << std::endl;
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for( int i = 0; i < row; i++ )
		for( int j = 0; j < col; j++ )
			if( aTableau.GetPieces()[ i ][ j ] != "E" )
				outfile << "(" << i << "," << j << "): " << aTableau.GetPieces()[ i ][ j ] << std::endl;
	outfile.close();
}

};