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
		using piece_type = CTableau::piece_type;

		CSetState();

		// Updates current state with the piece hit. Returns score for hitting such piece
		unsigned short Update( const piece_type& aPiece );

		void Reset();

	private:
		// Current piece sequence
		std::deque<piece_type> mPieceSequence;
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
	using history = std::vector<coordinates>;

	explicit CTableauState( const std::optional<coordinates>& aCurrentPosition );

	const history& GetHistory() const;
	const std::optional<coordinates>& GetCurrentPosition() const;
	const unsigned short& GetScore() const;

	// Updates set and multi states with the piece hit. Returrns score for hitting such piece
	unsigned short Update( const piece_type& aPiece );

	const coordinates& SetCurrentPositionAtRandom( const CTableau& aTableau, std::mt19937_64& aRNG );

	std::optional<coordinates> Move( const CTableau& aTableau, std::mt19937_64& aRNG );

	void SaveHistory( std::string_view aOutputFileName, const CTableau& aTableau ) const;

private:
	void MoveDiagonal( const CTableau& aTableau, coordinates_vector& aDestinations ) const;
	void MoveDiagonal( const CTableau& aTableau, coordinates_vector& aDestinations, int aSteps ) const;
	void MoveStraight( const CTableau& aTableau, coordinates_vector& aDestinations ) const;
	void MoveStraight( const CTableau& aTableau, coordinates_vector& aDestinations, int aSteps ) const;
	void MoveKnight( const CTableau& aTableau, coordinates_vector& aDestinations ) const;
	void MoveWildcard( const CTableau& aTableau, coordinates_vector& aDestinations ) const;

	void AppendDestination( const CTableau& aTableau, coordinates_vector& aDestinations, const coordinates& aDestination ) const;

	static constexpr unsigned short NUMERIC_DESTINATIONS_COUNT();
	static constexpr unsigned short KQ_DESTINATIONS_COUNT();
	static constexpr unsigned short RB_DESTINATIONS_COUNT();

	history mHistory;
	std::optional<coordinates> mCurrentPosition;
	CSetState mSetState;
	CMultiState mMultiState;

	// Accumulated score
	unsigned short mScore = 0;
};

}