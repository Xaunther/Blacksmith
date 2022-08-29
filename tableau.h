// Header file for tableau class
#ifndef tableau_h
#define tableau_h

#include "Constants.h"
#include <array>
#include <vector>
#include <string>
#include <random>
#include <optional>

namespace blacksmith
{

class tableau
{
	class CComboState
	{
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

	public:
		// Updates set and multi states with the piece hit. Returrns score for hitting such piece
		unsigned short Update( const std::string& aPiece );

	private:
		CSetState mSetState;
		CMultiState mMultiState;
	};

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

	const unsigned short& GetScore() const;

	void SetPiece( const destination& aDestination, const std::string& aPiece );

	bool IsInside( const destination& aDestination ) const;

	int CountPieces() const;

private:
	void AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const;

	// Variables
	piece_matrix mPieces; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard

	// State of the current combination
	CComboState mComboState;

	// Accumulated score
	unsigned short mScore;
};

};

#endif
