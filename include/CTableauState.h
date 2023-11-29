#pragma once

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
		std::vector<std::string> mPieceSequence;
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

	using destination = CTableau::destination;
	using destinations = std::vector<destination>;

	explicit CTableauState( const CTableau& aTableau, const destination& aCurrentPosition );

	const CTableau& GetTableau() const;
	const destination& GetCurrentPosition() const;
	const unsigned short& GetScore() const;

	// Updates set and multi states with the piece hit. Returrns score for hitting such piece
	unsigned short Update( const std::string& aPiece );

	destination Randomize();

	destination Move( std::mt19937_64& aRNG );

private:
	void MoveDiagonal( destinations& aDestinations ) const;
	void MoveDiagonal( destinations& aDestinations, int aSteps ) const;
	void MoveStraight( destinations& aDestinations ) const;
	void MoveStraight( destinations& aDestinations, int aSteps ) const;
	void MoveKnight( destinations& aDestinations ) const;
	void MoveWildcard( destinations& aDestinations ) const;

	void AppendDestination( destinations& aDestinations, const destination& aDestination ) const;

	CTableau mTableau;
	destination mCurrentPosition;
	CSetState mSetState;
	CMultiState mMultiState;

	// Accumulated score
	unsigned short mScore = 0;
};

}