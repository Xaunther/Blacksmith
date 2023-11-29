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

	explicit CTableauState( const CTableau& aTableau );

	const CTableau& GetTableau() const;
	const unsigned short& GetScore() const;

	// Updates set and multi states with the piece hit. Returrns score for hitting such piece
	unsigned short Update( const std::string& aPiece );

	destination Randomize( const destination& aDestination ) const;

	destination Move( const destination& aDestination, std::mt19937_64& aRNG );
	void MoveDiagonal( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const;
	void MoveStraight( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const;
	void MoveKnight( const destination& aOrigin, destinations& aDestinations ) const;
	void MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const;

private:
	void AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const;

	CTableau mTableau;
	CSetState mSetState;
	CMultiState mMultiState;

	// Accumulated score
	unsigned short mScore = 0;
};

}