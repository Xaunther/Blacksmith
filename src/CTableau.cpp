#include "CTableau.h"

#include <algorithm>
#include <fstream>
#include <ranges>
#include <string>
#include <set>

namespace blacksmith
{

namespace
{

CTableau::pieces LoadPieces( const std::string_view aFileName );
const CTableau::pieces& CheckPieces( const CTableau::pieces& aPieces );
const CTableau::index& CheckRows( const CTableau::index& aRows, const CTableau::index& aPiecesCount );

}

const std::set<CTableau::piece_type>& CTableau::CHESS_PIECES()
{
	static const std::set<piece_type> result{ E_PIECE_TYPE::QUEEN, E_PIECE_TYPE::BISHOP, E_PIECE_TYPE::ROOK, E_PIECE_TYPE::KNIGHT };
	return result;
}

const std::set<CTableau::piece_type>& CTableau::NUMBER_PIECES()
{
	static const std::set<piece_type> result{ E_PIECE_TYPE::ONE, E_PIECE_TYPE::TWO, E_PIECE_TYPE::THREE, E_PIECE_TYPE::FOUR };
	return result;
}
const std::set<CTableau::piece_type>& CTableau::PIECES()
{
	static const std::set<piece_type> result{ E_PIECE_TYPE::ONE, E_PIECE_TYPE::TWO, E_PIECE_TYPE::THREE, E_PIECE_TYPE::FOUR,
			E_PIECE_TYPE::QUEEN, E_PIECE_TYPE::BISHOP, E_PIECE_TYPE::ROOK, E_PIECE_TYPE::KNIGHT,
			E_PIECE_TYPE::EMPTY, E_PIECE_TYPE::WILDCARD };
	return result;
}

CTableau::CTableau( const std::string& aFileName ) :
	mPieces( CheckPieces( LoadPieces( aFileName ) ) ),
	mRows( CheckRows( static_cast< index >( std::sqrt( mPieces.size() ) ), mPieces.size() ) )
{
}

const CTableau::piece_type& CTableau::GetPiece( const index& aRowIndex, const index& aColIndex ) const
{
	return mPieces[ aRowIndex * mRows + aColIndex ];
}

const CTableau::index& CTableau::GetRows() const
{
	return mRows;
}

void CTableau::HitPiece( const index& aRowIndex, const index& aColIndex )
{
	mPieces[ aRowIndex * mRows + aColIndex ] = E_PIECE_TYPE::EMPTY;
}

bool CTableau::IsInside( const index& aRowIndex, const index& aColIndex ) const
{
	return aRowIndex < mRows && aColIndex < mRows;
}

CTableau::index CTableau::CountPieces() const
{
	return mPieces.size() - std::ranges::count( mPieces, E_PIECE_TYPE::EMPTY );
}

CTableau::index CTableau::Size() const
{
	return mPieces.size();
}

std::string CTableau::PieceToString( const E_PIECE_TYPE& aPiece )
{
	switch( aPiece )
	{
	case E_PIECE_TYPE::EMPTY: return "E";
	case E_PIECE_TYPE::QUEEN: return "Q";
	case E_PIECE_TYPE::BISHOP: return "B";
	case E_PIECE_TYPE::ROOK: return "R";
	case E_PIECE_TYPE::KNIGHT: return "K";
	case E_PIECE_TYPE::WILDCARD: return "W";
	case E_PIECE_TYPE::ONE: return "1";
	case E_PIECE_TYPE::TWO: return "2";
	case E_PIECE_TYPE::THREE: return "3";
	default: return "4";
	}
}

CTableau::E_PIECE_TYPE CTableau::StringToPiece( const std::string& aPiece )
{
	if( aPiece == "Q" )
		return E_PIECE_TYPE::QUEEN;
	else if( aPiece == "B" )
		return E_PIECE_TYPE::BISHOP;
	else if( aPiece == "R" )
		return E_PIECE_TYPE::ROOK;
	else if( aPiece == "K" )
		return E_PIECE_TYPE::KNIGHT;
	else if( aPiece == "W" )
		return E_PIECE_TYPE::WILDCARD;
	else if( aPiece == "1" )
		return E_PIECE_TYPE::ONE;
	else if( aPiece == "2" )
		return E_PIECE_TYPE::TWO;
	else if( aPiece == "3" )
		return E_PIECE_TYPE::THREE;
	else if( aPiece == "4" )
		return E_PIECE_TYPE::FOUR;
	else
		return E_PIECE_TYPE::EMPTY;
}

namespace
{

CTableau::pieces LoadPieces( const std::string_view aFileName )
{
	CTableau::pieces result;

	std::ifstream infile;
	infile.open( aFileName.data() );
	if( infile )
		for( const auto& object : std::ranges::istream_view<std::string>( infile ) )
			result.emplace_back( CTableau::StringToPiece( object ) );
	infile.close();

	return result;
}

const CTableau::pieces& CheckPieces( const CTableau::pieces& aPieces )
{
	for( const auto& piece : aPieces )
		if( !CTableau::PIECES().contains( piece ) )
			throw std::runtime_error( "The piece '" + CTableau::PieceToString( piece ) + "' does not exist." );
	return aPieces;
}

const CTableau::index& CheckRows( const CTableau::index& aRows, const CTableau::index& aPiecesCount )
{
	if( aRows * aRows != aPiecesCount )
		throw std::runtime_error( "The tableau is not a square." );
	return aRows;
}

}

};