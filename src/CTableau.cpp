#include "CTableau.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <set>

namespace blacksmith
{

namespace
{

CTableau::piece_matrix LoadPieces( const std::string_view aFileName );
const CTableau::piece_matrix& CheckPieces( const CTableau::piece_matrix& aPieces );

}

const std::set<std::string>& CTableau::CHESS_PIECES()
{
	static const std::set<std::string> result{ "Q", "B", "R", "K" };
	return result;
}

const std::set<std::string>& CTableau::NUMBER_PIECES()
{
	static const std::set<std::string> result{ "1", "2", "3", "4" };
	return result;
}
const std::set<std::string>& CTableau::PIECES()
{
	static const std::set<std::string> result{ "1", "2", "3", "4", "Q", "B", "R", "K", "E", "W" };
	return result;
}

CTableau::CTableau()
{
	for( auto& _row : mPieces )
		for( auto& piece : _row )
			piece = "E";
}

CTableau::CTableau( const std::string& aFileName ) :
	mPieces( CheckPieces( LoadPieces( aFileName ) ) )
{
}

const CTableau::piece_matrix& CTableau::GetPieces() const
{
	return mPieces;
}

CTableau::piece_matrix& CTableau::Pieces()
{
	return mPieces;
}

const std::string& CTableau::GetPiece( const destination& aDestination ) const
{
	return mPieces.at( aDestination.first ).at( aDestination.second );
}

void CTableau::SetPiece( const destination& aOrigin, const std::string& aPiece )
{
	if( IsInside( aOrigin ) )
		mPieces[ aOrigin.first ][ aOrigin.second ] = aPiece;
}

bool CTableau::IsInside( const destination& aDestination ) const
{
	return aDestination.first >= 0 && aDestination.first < ROW && aDestination.second >= 0 && aDestination.second < COL;
}

unsigned int CTableau::CountPieces() const
{
	// Loop around and count non-empty slots
	int count = 0;
	for( const auto& _row : mPieces )
		for( const auto& piece : _row )
			if( piece != "E" )
				count++;
	return count;
}

namespace
{

CTableau::piece_matrix LoadPieces( const std::string_view aFileName )
{
	CTableau::piece_matrix result;

	std::ifstream infile;
	std::string basura;
	infile.open( aFileName.data() );
	for( auto& _row : result )
		for( auto& piece : _row )
			infile >> piece;
	infile.close();

	return result;
}

const CTableau::piece_matrix& CheckPieces( const CTableau::piece_matrix& aPieces )
{
	for( const auto& _row : aPieces )
		for( const auto& piece : _row )
			if( !CTableau::PIECES().contains( piece ) )
				throw std::runtime_error( "No existe la pieza '" + piece + "'." );
	return aPieces;
}

}

};