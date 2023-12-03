#pragma once

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
	using pieces = std::vector<std::string>;
	using index = pieces::size_type;
	using indices_set = std::set<index>;
	using indices_sets = std::vector<indices_set>;
	using coordinates = std::pair<index, index>;

	explicit CTableau( const std::string& aFileName );

	const std::string& GetPiece( const index& aRowIndex, const index& aColIndex ) const;
	std::optional<coordinates> GetRandomPieceCoordinates( std::mt19937_64& aRNG ) const;

	const index& GetRows() const;

	std::optional<coordinates> HitPiece( const index& aRowIndex, const index& aColIndex, std::mt19937_64& aRNG );

	bool IsInside( const index& aRowIndex, const index& aColIndex ) const;

	index CountPieces() const;
	index Size() const;

	static const std::set<std::string>& CHESS_PIECES();
	static const std::set<std::string>& NUMBER_PIECES();
	static const std::set<std::string>& PIECES();

private:
	// Variables
	pieces mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
	index mRows;
	indices_sets mTargetIndicesPerPiece;
	indices_sets mOriginIndicesPerPiece;
};

};
