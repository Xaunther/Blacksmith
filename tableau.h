// Header file for tableau class
#ifndef tableau_h
#define tableau_h

#include "Constants.h"
#include <utility>
#include <array>
#include <string>

class tableau
{
public:
	using destination = std::pair<int, int>;
	using piece_matrix = std::array<std::array<std::string, col>, row>;

	tableau();

	void Load( const std::string& aFileName );

	destination Move( const destination& aDestination );

	destination Randomize( const destination& aDestination );

	void MoveDiagonal( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible, int aSteps = -1 );
	void MoveStraight( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible, int aSteps = -1 );
	void MoveKnight( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible );
	void MoveWildcard( const destination& aOrigin, destination aDestinations[ maxpos ], int& aCountPossible );

	const piece_matrix& GetPieces() const;

	void SetPiece( const destination& aDestination, const std::string& aPiece );

	int CountPieces() const;

	// Variables
	piece_matrix mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
};

#endif
