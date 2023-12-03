#pragma once

#include <deque>

#include "CTableau.h"

namespace blacksmith
{

class CTableauState
{
public:
	class CSetState
	{
	public:
		CSetState();

		// Updates current state with the piece hit. Returns score for hitting such piece
		unsigned short Update( const std::string& aPiece );

		void Reset();

	private:
		// Current piece sequence
		std::deque<std::string> mPieceSequence;
		// True if current set is chess, false if it is a number set. Undefined if any.
		std::optional<bool> mChessSet;
		// Sets completed
		unsigned short mCount;
	};

	class CMultiState
	{
	public:
		CMultiState();

		// Updates current state with the piece hit. Returns score for hitting such piece
		unsigned short Update( const std::string& aPiece );

		void Reset();

	private:
		// Current piece being repeated
		std::string mPiece;
		// Number of repetitions
		unsigned short mCount;
	};

	using coordinates = CTableau::coordinates;

	explicit CTableauState( const CTableau& aTableau, const std::optional<coordinates>& aCurrentPosition );

	const CTableau& GetTableau() const;
	const std::optional<coordinates>& GetCurrentPosition() const;
	const unsigned short& GetScore() const;

	// Updates set and multi states with the piece hit. Returrns score for hitting such piece
	unsigned short Update( const std::string& aPiece );

	const coordinates& SetCurrentPositionAtRandom( std::mt19937_64& aRNG );

	std::optional<coordinates> Move( std::mt19937_64& aRNG );

private:
	CTableau mTableau;
	std::optional<coordinates> mCurrentPosition;
	CSetState mSetState;
	CMultiState mMultiState;

	// Accumulated score
	unsigned short mScore = 0;
};

}