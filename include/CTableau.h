#pragma once

#include "Constants.h"

#include <array>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <vector>

namespace blacksmith
{

class CTableau
{
public:
	using destination = std::pair<int, int>;
	using destinations = std::vector<destination>;
	using piece_matrix = std::array<std::array<std::string, COL>, ROW>;

	CTableau( const std::string& aFileName );

	const piece_matrix& GetPieces() const;
	piece_matrix& Pieces();

	const std::string& GetPiece( const destination& aDestination ) const;

	const unsigned short& GetScore() const;

	void SetPiece( const destination& aDestination, const std::string& aPiece );

	bool IsInside( const destination& aDestination ) const;

	unsigned int CountPieces() const;

	static const std::set<std::string>& CHESS_PIECES();
	static const std::set<std::string>& NUMBER_PIECES();
	static const std::set<std::string>& PIECES();

	// Variables
	piece_matrix mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
};

};
