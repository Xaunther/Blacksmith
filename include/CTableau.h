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
	enum class E_PIECE_TYPE
	{
		EMPTY,
		ONE = 1,
		TWO = 2,
		THREE = 3,
		FOUR = 4,
		QUEEN,
		BISHOP,
		ROOK,
		KNIGHT,
		WILDCARD
	};

	using piece_type = E_PIECE_TYPE;
	using pieces = std::vector<piece_type>;
	using index = pieces::size_type;

	explicit CTableau( const std::string& aFileName );

	const piece_type& GetPiece( const index& aRowIndex, const index& aColIndex ) const;

	const index& GetRows() const;

	bool IsInside( const index& aRowIndex, const index& aColIndex ) const;

	index CountPieces() const;
	index Size() const;

	static std::string PieceToString( const E_PIECE_TYPE& aPiece );
	static E_PIECE_TYPE StringToPiece( const std::string& aPiece );

	static const std::set<piece_type>& CHESS_PIECES();
	static const std::set<piece_type>& NUMBER_PIECES();
	static const std::set<piece_type>& PIECES();

private:
	// Variables
	pieces mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
	index mRows;

};

};
