// Header file for tableau class
#ifndef tableau_h
#define tableau_h

#include "Constants.h"
#include <utility>
#include <array>
#include <vector>
#include <string>

class tableau
{
public:
	using destination = std::pair<int, int>;
	using destinations = std::vector<destination>;
	using piece_matrix = std::array<std::array<std::string, col>, row>;

	tableau();

	tableau( const std::string& aFileName );

	void Load( const std::string& aFileName );

	destination Move( const destination& aDestination );

	destination Randomize( const destination& aDestination ) const;

	void MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps = -1 ) const;
	void MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps = -1 ) const;
	void MoveKnight( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const;

	const piece_matrix& GetPieces() const;

	void SetPiece( const destination& aDestination, const std::string& aPiece );

	int CountPieces() const;

	// Variables
	piece_matrix mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
};

#endif
