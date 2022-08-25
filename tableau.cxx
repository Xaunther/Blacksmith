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
void tableau::Load( const std::string& aFileName )
{
	std::ifstream infile;
	std::string basura;
	infile.open( aFileName.c_str() );
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
tableau::destination tableau::Move( const destination& aOrigin )
{
	int aCountPossible = 0;				  // Track number of possibilities
	destination aDestinations[ maxpos ]; // Save all possible aDestinations
	int dest_number;				  // Random destination number
	if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "Q" )		  // Queen
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible );
		MoveStraight( aOrigin, aDestinations, aCountPossible );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "B" ) // Bishop
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "R" ) // Rook
	{
		MoveStraight( aOrigin, aDestinations, aCountPossible );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "K" ) // Knight
	{
		MoveKnight( aOrigin, aDestinations, aCountPossible );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "1" ) // 1
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible, 1 );
		MoveStraight( aOrigin, aDestinations, aCountPossible, 1 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "2" ) // 2
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible, 2 );
		MoveStraight( aOrigin, aDestinations, aCountPossible, 2 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "3" ) // 3
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible, 3 );
		MoveStraight( aOrigin, aDestinations, aCountPossible, 3 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "4" ) // 4
	{
		MoveDiagonal( aOrigin, aDestinations, aCountPossible, 4 );
		MoveStraight( aOrigin, aDestinations, aCountPossible, 4 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "W" ) // Rum (WILDCARD!)
	{
		MoveWildcard( aOrigin, aDestinations, aCountPossible );
	}
	if ( aCountPossible > 0 ) // If it's possible
	{
		dest_number = rand() % aCountPossible;
	}
	else
	{
		dest_number = 0;
		aDestinations[ dest_number ].first = -1;
		aDestinations[ dest_number ].second = -1;
	}
	mPieces[ aOrigin.first ][ aOrigin.second ] = "E"; // Remove the piece, and make it empty
	return aDestinations[ dest_number ];
}

void tableau::MoveDiagonal( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible, int aSteps )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos ];
	// 4 possible moves
	int i = 0;
	if ( aSteps < 0 )
	{
		test_dest[ i ].first = aOrigin.first - std::min( aOrigin.first - 0, aOrigin.second - 0 );
		test_dest[ i ].second = aOrigin.second - std::min( aOrigin.first - 0, aOrigin.second - 0 );
		i++;
		test_dest[ i ].first = aOrigin.first + std::min( 5 - aOrigin.first, aOrigin.second - 0 );
		test_dest[ i ].second = aOrigin.second - std::min( 5 - aOrigin.first, aOrigin.second - 0 );
		i++;
		test_dest[ i ].first = aOrigin.first + std::min( 5 - aOrigin.first, 5 - aOrigin.second );
		test_dest[ i ].second = aOrigin.second + std::min( 5 - aOrigin.first, 5 - aOrigin.second );
		i++;
		test_dest[ i ].first = aOrigin.first - std::min( aOrigin.first - 0, 5 - aOrigin.second );
		test_dest[ i ].second = aOrigin.second + std::min( aOrigin.first - 0, 5 - aOrigin.second );
		i++;
	}
	else
	{
		test_dest[ i ].first = aOrigin.first - aSteps;
		test_dest[ i ].second = aOrigin.second - aSteps;
		i++;
		test_dest[ i ].first = aOrigin.first + aSteps;
		test_dest[ i ].second = aOrigin.second - aSteps;
		i++;
		test_dest[ i ].first = aOrigin.first + aSteps;
		test_dest[ i ].second = aOrigin.second + aSteps;
		i++;
		test_dest[ i ].first = aOrigin.first - aSteps;
		test_dest[ i ].second = aOrigin.second + aSteps;
		i++;
		// If off-limits, return to initial pos
		for ( i = 0; i < movepos; i++ )
		{
			if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
			{
				test_dest[ i ].first = aOrigin.first;
				test_dest[ i ].second = aOrigin.second;
			}
		}
	}
	for ( i = 0; i < movepos; i++ )
	{
		if ( ( test_dest[ i ].first == aOrigin.first && test_dest[ i ].second == aOrigin.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			aDestinations[ aCountPossible ] = test_dest[ i ];
			aCountPossible++;
		}
	}
	return;
}

void tableau::MoveStraight( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible, int aSteps )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos ];
	// 4 possible moves
	int i = 0;
	if ( aSteps < 0 )
	{
		test_dest[ i ].first = 0;
		test_dest[ i ].second = aOrigin.second;
		i++;
		test_dest[ i ].first = 5;
		test_dest[ i ].second = aOrigin.second;
		i++;
		test_dest[ i ].first = aOrigin.first;
		test_dest[ i ].second = 0;
		i++;
		test_dest[ i ].first = aOrigin.first;
		test_dest[ i ].second = 5;
		i++;
	}
	else
	{
		test_dest[ i ].first = aOrigin.first + aSteps;
		test_dest[ i ].second = aOrigin.second;
		i++;
		test_dest[ i ].first = aOrigin.first - aSteps;
		test_dest[ i ].second = aOrigin.second;
		i++;
		test_dest[ i ].first = aOrigin.first;
		test_dest[ i ].second = aOrigin.second + aSteps;
		i++;
		test_dest[ i ].first = aOrigin.first;
		test_dest[ i ].second = aOrigin.second - aSteps;
		i++;
		// If off-limits, return to initial pos
		for ( i = 0; i < movepos; i++ )
		{
			if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
			{
				test_dest[ i ].first = aOrigin.first;
				test_dest[ i ].second = aOrigin.second;
			}
		}
	}
	for ( i = 0; i < movepos; i++ )
	{
		if ( ( test_dest[ i ].first == aOrigin.first && test_dest[ i ].second == aOrigin.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			aDestinations[ aCountPossible ] = test_dest[ i ];
			aCountPossible++;
		}
	}
	return;
}

void tableau::MoveKnight( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible )
{
	// Junk destination variable to test before adding
	destination test_dest[ movepos_K ];
	// 4 possible moves
	int i = 0;
	test_dest[ i ].first = aOrigin.first - 1;
	test_dest[ i ].second = aOrigin.second - 2;
	i++;
	test_dest[ i ].first = aOrigin.first - 1;
	test_dest[ i ].second = aOrigin.second + 2;
	i++;
	test_dest[ i ].first = aOrigin.first + 1;
	test_dest[ i ].second = aOrigin.second - 2;
	i++;
	test_dest[ i ].first = aOrigin.first + 1;
	test_dest[ i ].second = aOrigin.second + 2;
	i++;
	test_dest[ i ].first = aOrigin.first - 2;
	test_dest[ i ].second = aOrigin.second - 1;
	i++;
	test_dest[ i ].first = aOrigin.first - 2;
	test_dest[ i ].second = aOrigin.second + 1;
	i++;
	test_dest[ i ].first = aOrigin.first + 2;
	test_dest[ i ].second = aOrigin.second - 1;
	i++;
	test_dest[ i ].first = aOrigin.first + 2;
	test_dest[ i ].second = aOrigin.second + 1;
	i++;

	// If off-limits, return to initial pos
	for ( i = 0; i < movepos_K; i++ )
	{
		if ( test_dest[ i ].first < 0 || test_dest[ i ].first > 5 || test_dest[ i ].second < 0 || test_dest[ i ].second > 5 )
		{
			test_dest[ i ].first = aOrigin.first;
			test_dest[ i ].second = aOrigin.second;
		}
	}
	for ( i = 0; i < movepos_K; i++ )
	{
		if ( ( test_dest[ i ].first == aOrigin.first && test_dest[ i ].second == aOrigin.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			aDestinations[ aCountPossible ] = test_dest[ i ];
			aCountPossible++;
		}
	}
	return;
}

void tableau::MoveWildcard( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible )
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
		if ( ( test_dest[ i ].first == aOrigin.first && test_dest[ i ].second == aOrigin.second ) || mPieces[ test_dest[ i ].first ][ test_dest[ i ].second ] == "E" )
		{
		} // If same place or empty place do nothing
		else
		{
			aDestinations[ aCountPossible ] = test_dest[ i ];
			aCountPossible++;
		}
	}
	return;
}

tableau::destination tableau::Randomize( const destination& aOrigin )
{
	int aCountPossible = 0;				  // Track number of possibilities
	destination aDestinations[ maxpos ]; // Save all possible aDestinations
	int dest_number;				  // Random destination number
	MoveWildcard( aOrigin, aDestinations, aCountPossible );
	if ( aCountPossible > 0 ) // If it's possible
	{
		dest_number = rand() % aCountPossible;
	}
	else
	{
		dest_number = 0;
		aDestinations[ dest_number ].first = -1;
		aDestinations[ dest_number ].second = -1;
	}
	return aDestinations[ dest_number ];
}

const tableau::piece_matrix& tableau::GetPieces() const
{
	return mPieces;
}

void tableau::SetPiece( const destination& aOrigin, const std::string& aPiece )
{
	mPieces[ aOrigin.first ][ aOrigin.second ] = aPiece;
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
