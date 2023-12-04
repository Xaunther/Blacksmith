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
		using piece_type = CTableau::piece_type;

		CSetState();

		// Updates current state with the piece hit. Returns score for hitting such piece
		unsigned short Update( const piece_type& aPiece );

		void Reset();

	private:
		// Current piece sequence
		std::vector<piece_type> mPieceSequence;
		// True if current set is chess, false if it is a number set. Undefined if any.
		std::optional<bool> mChessSet;
		// Sets completed
		unsigned short mCount;
	};

	class CMultiState
	{
	public:
		using piece_type = CTableau::piece_type;

		CMultiState();

		// Updates current state with the piece hit. Returns score for hitting such piece
		unsigned short Update( const piece_type& aPiece );

		void Reset();

	private:
		// Current piece being repeated
		piece_type mPiece;
		// Number of repetitions
		unsigned short mCount;
	};

	using coordinates = std::pair<CTableau::index, CTableau::index>;
	using coordinates_vector = std::vector<coordinates>;
	using piece_type = CTableau::piece_type;

	explicit CTableauState( const CTableau& aTableau, const std::optional<coordinates>& aCurrentPosition );

	const CTableau& GetTableau() const;
	const std::optional<coordinates>& GetCurrentPosition() const;
	const unsigned short& GetScore() const;

	// Updates set and multi states with the piece hit. Returrns score for hitting such piece
	unsigned short Update( const piece_type& aPiece );

	const coordinates& SetCurrentPositionAtRandom( std::mt19937_64& aRNG );

	std::optional<coordinates> Move( std::mt19937_64& aRNG );

private:
	void MoveDiagonal( coordinates_vector& aDestinations ) const;
	void MoveDiagonal( coordinates_vector& aDestinations, int aSteps ) const;
	void MoveStraight( coordinates_vector& aDestinations ) const;
	void MoveStraight( coordinates_vector& aDestinations, int aSteps ) const;
	void MoveKnight( coordinates_vector& aDestinations ) const;
	void MoveWildcard( coordinates_vector& aDestinations ) const;

	void AppendDestination( coordinates_vector& aDestinations, const coordinates& aDestination ) const;

	static constexpr unsigned short NUMERIC_DESTINATIONS_COUNT();
	static constexpr unsigned short KQ_DESTINATIONS_COUNT();
	static constexpr unsigned short RB_DESTINATIONS_COUNT();

	CTableau mTableau;
	std::optional<coordinates> mCurrentPosition;
	CSetState mSetState;
	CMultiState mMultiState;

	// Accumulated score
	unsigned short mScore = 0;
};

}