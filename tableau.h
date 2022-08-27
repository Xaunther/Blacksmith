// Header file for tableau class
#ifndef tableau_h
#define tableau_h

#include "Constants.h"
#include <array>
#include <vector>
#include <string>
#include <random>

namespace blacksmith
{

class tableau
{
public:
	using destination = std::pair<int, int>;
	using destinations = std::vector<destination>;
	using piece_matrix = std::array<std::array<std::string, col>, row>;

	tableau();

	tableau( const std::string& aFileName );

	void Load( const std::string& aFileName );

	destination Move( const destination& aDestination, std::mt19937_64& aRNG );

	destination Randomize( const destination& aDestination ) const;

	void MoveDiagonal( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const;
	void MoveStraight( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const;
	void MoveKnight( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const;

	const piece_matrix& GetPieces() const;

	const std::string& GetPiece( const destination& aDestination ) const;

	void SetPiece( const destination& aDestination, const std::string& aPiece );

	bool IsInside( const destination& aDestination ) const;

	int CountPieces() const;

private:
	void AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const;

	// Variables
	piece_matrix mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
};

};

#endif
