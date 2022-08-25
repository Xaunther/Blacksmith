// Implementation of tableau

#include "tableau.h"
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <string>

// Initializer
tableau::tableau()
{
	for ( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j++ )
		{
			mPieces[ i ][ j ] = "E";
		}
	}
}

// Load "map" from file, file must follow the codes
void tableau::Load( std::string filename )
{
	std::ifstream infile;
	std::string basura;
	infile.open( filename.c_str() );
	for ( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j++ )
		{
			infile >> mPieces[ i ][ j ];
		}
	}
	infile.close();
}

// Move from current place to another suitable one, randomly. Return next point
tableau::destination tableau::Move( const destination& aDestination )
{
	int N_possible = 0;				  // Track number of possibilities
	destination destinations[ maxpos ]; // Save all possible destinations
	int dest_number;				  // Random destination number
	if ( mPieces[ aDestination.first ][ aDestination.second ] == "Q" )		  // Queen
	{
		MoveDiagonal( aDestination, destinations, N_possible );
		MoveStraight( aDestination, destinations, N_possible );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "B" ) // Bishop
	{
		MoveDiagonal( aDestination, destinations, N_possible );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "R" ) // Rook
	{
		MoveStraight( aDestination, destinations, N_possible );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "K" ) // Knight
	{
		MoveKnight( aDestination, destinations, N_possible );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "1" ) // 1
	{
		MoveDiagonal( aDestination, destinations, N_possible, 1 );
		MoveStraight( aDestination, destinations, N_possible, 1 );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "2" ) // 2
	{
		MoveDiagonal( aDestination, destinations, N_possible, 2 );
		MoveStraight( aDestination, destinations, N_possible, 2 );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "3" ) // 3
	{
		MoveDiagonal( aDestination, destinations, N_possible, 3 );
		MoveStraight( aDestination, destinations, N_possible, 3 );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "4" ) // 4
	{
		MoveDiagonal( aDestination, destinations, N_possible, 4 );
		MoveStraight( aDestination, destinations, N_possible, 4 );
	}
	else if ( mPieces[ aDestination.first ][ aDestination.second ] == "W" ) // Rum (WILDCARD!)
	{
		MoveWildcard( aDestination, destinations, N_possible );
	}
	if ( N_possible > 0 ) // If it's possible
	{
		dest_number = rand() % N_possible;
	}
	else
	{
		dest_number = 0;
		destinations[ dest_number ].first = -1;
		destinations[ dest_number ].second = -1;
	}
	mPieces[ aDestination.first ][ aDestination.second ] = "E"; // Remove the piece, and make it empty
	return destinations[ dest_number ];
}

void tableau::MoveDiagonal( const destination& aDestination, destination destinations[ maxpos ], int& N_possible, int steps )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos ];
	// 4 possible moves
	int i = 0;
	if ( steps < 0 )
	{
		test_dest[ i ].first = aDestination.first - std::min( aDestination.first - 0, aDestination.second - 0 );
		test_dest[ i ].second = aDestination.second - std::min( aDestination.first - 0, aDestination.second - 0 );
		i++;
		test_dest[ i ].first = aDestination.first + std::min( 5 - aDestination.first, aDestination.second - 0 );
		test_dest[ i ].second = aDestination.second - std::min( 5 - aDestination.first, aDestination.second - 0 );
		i++;
		test_dest[ i ].first = aDestination.first + std::min( 5 - aDestination.first, 5 - aDestination.second );
		test_dest[ i ].second = aDestination.second + std::min( 5 - aDestination.first, 5 - aDestination.second );
		i++;
		test_dest[ i ].first = aDestination.first - std::min( aDestination.first - 0, 5 - aDestination.second );
		test_dest[ i ].second = aDestination.second + std::min( aDestination.first - 0, 5 - aDestination.second );
		i++;
	}
	else
	{
		test_dest[ i ].first = aDestination.first - steps;
		test_dest[ i ].second = aDestination.second - steps;
		i++;
		test_dest[ i ].first = aDestination.first + steps;
		test_dest[ i ].second = aDestination.second - steps;
		i++;
		test_dest[ i ].first = aDestination.first + steps;
		test_dest[ i ].second = aDestination.second + steps;
		i++;
		test_dest[ i ].first = aDestination.first - steps;
		test_dest[ i ].second = aDestination.second + steps;
		i++;
		// If off-limits, return to initial pos
		for ( i = 0; i < movepos; i++ )
		{
			if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
			{
				test_dest[ i ].first = aDestination.first;
				test_dest[ i ].second = aDestination.second;
			}
		}
	}
	for ( i = 0; i < movepos; i++ )
	{
		if ( ( test_dest[ i ].first == aDestination.first && test_dest[ i ].second == aDestination.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[ N_possible ] = test_dest[ i ];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveStraight( const destination& aDestination, destination destinations[ maxpos ], int& N_possible, int steps )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos ];
	// 4 possible moves
	int i = 0;
	if ( steps < 0 )
	{
		test_dest[ i ].first = 0;
		test_dest[ i ].second = aDestination.second;
		i++;
		test_dest[ i ].first = 5;
		test_dest[ i ].second = aDestination.second;
		i++;
		test_dest[ i ].first = aDestination.first;
		test_dest[ i ].second = 0;
		i++;
		test_dest[ i ].first = aDestination.first;
		test_dest[ i ].second = 5;
		i++;
	}
	else
	{
		test_dest[ i ].first = aDestination.first + steps;
		test_dest[ i ].second = aDestination.second;
		i++;
		test_dest[ i ].first = aDestination.first - steps;
		test_dest[ i ].second = aDestination.second;
		i++;
		test_dest[ i ].first = aDestination.first;
		test_dest[ i ].second = aDestination.second + steps;
		i++;
		test_dest[ i ].first = aDestination.first;
		test_dest[ i ].second = aDestination.second - steps;
		i++;
		// If off-limits, return to initial pos
		for ( i = 0; i < movepos; i++ )
		{
			if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
			{
				test_dest[ i ].first = aDestination.first;
				test_dest[ i ].second = aDestination.second;
			}
		}
	}
	for ( i = 0; i < movepos; i++ )
	{
		if ( ( test_dest[ i ].first == aDestination.first && test_dest[ i ].second == aDestination.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[ N_possible ] = test_dest[ i ];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveKnight( const destination& aDestination, destination destinations[ maxpos ], int& N_possible )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos_K ];
	// 4 possible moves
	int i = 0;
	test_dest[ i ].first = aDestination.first - 1;
	test_dest[ i ].second = aDestination.second - 2;
	i++;
	test_dest[ i ].first = aDestination.first - 1;
	test_dest[ i ].second = aDestination.second + 2;
	i++;
	test_dest[ i ].first = aDestination.first + 1;
	test_dest[ i ].second = aDestination.second - 2;
	i++;
	test_dest[ i ].first = aDestination.first + 1;
	test_dest[ i ].second = aDestination.second + 2;
	i++;
	test_dest[ i ].first = aDestination.first - 2;
	test_dest[ i ].second = aDestination.second - 1;
	i++;
	test_dest[ i ].first = aDestination.first - 2;
	test_dest[ i ].second = aDestination.second + 1;
	i++;
	test_dest[ i ].first = aDestination.first + 2;
	test_dest[ i ].second = aDestination.second - 1;
	i++;
	test_dest[ i ].first = aDestination.first + 2;
	test_dest[ i ].second = aDestination.second + 1;
	i++;

	// If off-limits, return to initial pos
	for ( i = 0; i < movepos_K; i++ )
	{
		if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
		{
			test_dest[ i ].first = aDestination.first;
			test_dest[ i ].second = aDestination.second;
		}
	}
	for ( i = 0; i < movepos_K; i++ )
	{
		if ( ( test_dest[ i ].first == aDestination.first && test_dest[ i ].second == aDestination.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[ N_possible ] = test_dest[ i ];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveWildcard( const destination& aDestination, destination destinations[ maxpos ], int& N_possible )
{
	// Junk destination variable to test before adding
	destination test_dest[ maxpos ];
	// Wildcard, all are possible
	for ( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j++ )
		{
			test_dest[ col * i + j ].first = i;
			test_dest[ col * i + j ].second = j;
		}
	}
	for ( int i = 0; i < maxpos; i++ )
	{
		if ( ( test_dest[ i ].first == aDestination.first && test_dest[ i ].second == aDestination.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[ N_possible ] = test_dest[ i ];
			N_possible++;
		}
	}
	return;
}

tableau::destination tableau::Randomize( const destination& aDestination )
{
	int N_possible = 0;				  // Track number of possibilities
	destination destinations[ maxpos ]; // Save all possible destinations
	int dest_number;				  // Random destination number
	MoveWildcard( aDestination, destinations, N_possible );
	if ( N_possible > 0 ) // If it's possible
	{
		dest_number = rand() % N_possible;
	}
	else
	{
		dest_number = 0;
		destinations[ dest_number ].first = -1;
		destinations[ dest_number ].second = -1;
	}
	return destinations[ dest_number ];
}

const tableau::piece_matrix& tableau::GetPieces() const
{
	return mPieces;
}

void tableau::SetPiece( const destination& aDestination, const std::string& aPiece )
{
	mPieces[ aDestination.first ][ aDestination.second ] = aPiece;
}

int tableau::CountPieces() const
{
	// Loop around and count non-empty slots
	int count = 0;
	for ( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j++ )
		{
			if ( mPieces[ i ][ j ] != "E" )
			{
				count++;
			}
		}
	}
	return count;
}
