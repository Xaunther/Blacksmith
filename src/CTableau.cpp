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

CTableau::CTableau( const std::string& aFileName ) :
	mPieces( CheckPieces( LoadPieces( aFileName ) ) ),
	mRows( CheckRows( static_cast< index >( std::sqrt( mPieces.size() ) ), mPieces.size() ) )
{
}

const std::string& CTableau::GetPiece( const index& aRowIndex, const index& aColIndex ) const
{
	return mPieces[ aRowIndex * mRows + aColIndex ];
}

const CTableau::index& CTableau::GetRows() const
{
	return mRows;
}

void CTableau::HitPiece( const index& aRowIndex, const index& aColIndex )
{
	mPieces[ aRowIndex * mRows + aColIndex ] = "E";
}

bool CTableau::IsInside( const index& aRowIndex, const index& aColIndex ) const
{
	return aRowIndex < mRows && aColIndex < mRows;
}

CTableau::index CTableau::CountPieces() const
{
	return mPieces.size() - std::ranges::count( mPieces, "E" );
}

CTableau::index CTableau::Size() const
{
	return mPieces.size();
}

namespace
{

CTableau::pieces LoadPieces( const std::string_view aFileName )
{
	CTableau::pieces result;

	std::ifstream infile;
	infile.open( aFileName.data() );
	if( infile )
		for( auto& object : std::ranges::istream_view<std::string>( infile ) )
			result.emplace_back( std::move( object ) );
	infile.close();

	return result;
}

const CTableau::pieces& CheckPieces( const CTableau::pieces& aPieces )
{
	for( const auto& piece : aPieces )
		if( !CTableau::PIECES().contains( piece ) )
			throw std::runtime_error( "The piece '" + piece + "' does not exist." );
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