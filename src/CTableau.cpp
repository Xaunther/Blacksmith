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

CTableau::indices_sets CreateTargetIndicesPerPiece( const CTableau::pieces& aPieces, const CTableau::index& aRows );
CTableau::indices_sets CreateOriginIndicesPerPiece( const CTableau::indices_sets& aTargetIndicesPerPiece );

CTableau::indices_set CreateQueenTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows );
CTableau::indices_set CreateBishopTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows );
CTableau::indices_set CreateRookTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows );
CTableau::indices_set CreateKnightTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows );
CTableau::indices_set CreateWildcardTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces );
CTableau::indices_set CreateNumericTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const CTableau::index& aNumber );

CTableau::indices_set CreateStraightTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const std::optional<CTableau::index>& aDistance = {} );
CTableau::indices_set CreateDiagonalTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const std::optional<CTableau::index>& aDistance = {} );


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
	mRows( CheckRows( static_cast< index >( std::sqrt( mPieces.size() ) ), mPieces.size() ) ),
	mTargetIndicesPerPiece( CreateTargetIndicesPerPiece( mPieces, mRows ) ),
	mOriginIndicesPerPiece( CreateOriginIndicesPerPiece( mTargetIndicesPerPiece ) )
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
	const auto& hitIndex = aRowIndex * mRows + aColIndex;
	mPieces[ hitIndex ] = "E";
	for( const auto& originIndex : mOriginIndicesPerPiece[ hitIndex ] )
		mTargetIndicesPerPiece[ originIndex ].erase( hitIndex );
	mTargetIndicesPerPiece[ hitIndex ].clear();
	mOriginIndicesPerPiece[ hitIndex ].clear();
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

CTableau::indices_sets CreateTargetIndicesPerPiece( const CTableau::pieces& aPieces, const CTableau::index& aRows )
{
	CTableau::indices_sets result;
	result.reserve( aPieces.size() );

	for( auto pieceIt = aPieces.cbegin(); pieceIt != aPieces.cend(); ++pieceIt )
	{
		if( *pieceIt == "E" )
			result.emplace_back( CTableau::indices_set{} );
		else if( *pieceIt == "Q" )
			result.emplace_back( CreateQueenTargets( result.size(), aPieces, aRows ) );
		else if( *pieceIt == "B" )
			result.emplace_back( CreateBishopTargets( result.size(), aPieces, aRows ) );
		else if( *pieceIt == "R" )
			result.emplace_back( CreateRookTargets( result.size(), aPieces, aRows ) );
		else if( *pieceIt == "K" )
			result.emplace_back( CreateKnightTargets( result.size(), aPieces, aRows ) );
		else if( *pieceIt == "W" )
			result.emplace_back( CreateWildcardTargets( result.size(), aPieces ) );
		else
			result.emplace_back( CreateNumericTargets( result.size(), aPieces, aRows, std::stoi( *pieceIt ) ) );
	}

	return result;
}

CTableau::indices_sets CreateOriginIndicesPerPiece( const CTableau::indices_sets& aTargetIndicesPerPiece )
{
	CTableau::indices_sets result( aTargetIndicesPerPiece.size(), CTableau::indices_set{} );

	for( auto targetIndicesIt = aTargetIndicesPerPiece.cbegin(); targetIndicesIt != aTargetIndicesPerPiece.cend(); ++targetIndicesIt )
	{
		const CTableau::index& originIndex = std::distance( aTargetIndicesPerPiece.cbegin(), targetIndicesIt );
		for( const auto& targetIndex : *targetIndicesIt )
			result[ targetIndex ].emplace_hint( result[ targetIndex ].cend(), originIndex );
	}

	return result;
}

CTableau::indices_set CreateQueenTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows )
{
	CTableau::indices_set result;
	result.merge( CreateDiagonalTargets( aOriginIndex, aPieces, aRows ) );
	result.merge( CreateStraightTargets( aOriginIndex, aPieces, aRows ) );
	return result;
}

CTableau::indices_set CreateBishopTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows )
{
	CTableau::indices_set result;
	result.merge( CreateDiagonalTargets( aOriginIndex, aPieces, aRows ) );
	return result;
}

CTableau::indices_set CreateRookTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows )
{
	CTableau::indices_set result;
	result.merge( CreateStraightTargets( aOriginIndex, aPieces, aRows ) );
	return result;
}

CTableau::indices_set CreateKnightTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows )
{
	CTableau::indices_set result;

	const auto& maxUp = aOriginIndex / aRows;
	const auto& maxDown = ( aRows - 1 ) - ( aOriginIndex / aRows );
	const auto& maxLeft = aOriginIndex % aRows;
	const auto& maxRight = ( aRows - 1 ) - ( aOriginIndex % aRows );

	if( maxLeft >= 2 && maxUp >= 1 )
	{
		const auto& targetIndex = aOriginIndex - ( aRows + 2 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxLeft >= 1 && maxUp >= 2 )
	{
		const auto& targetIndex = aOriginIndex - ( 2 * aRows + 1 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxRight >= 1 && maxUp >= 2 )
	{
		const auto& targetIndex = aOriginIndex - ( 2 * aRows - 1 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxRight >= 2 && maxUp >= 1 )
	{
		const auto& targetIndex = aOriginIndex - ( aRows - 2 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxRight >= 2 && maxDown >= 1 )
	{
		const auto& targetIndex = aOriginIndex + ( aRows + 2 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxRight >= 1 && maxDown >= 2 )
	{
		const auto& targetIndex = aOriginIndex + ( 2 * aRows + 1 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxLeft >= 1 && maxDown >= 2 )
	{
		const auto& targetIndex = aOriginIndex + ( 2 * aRows - 1 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}
	if( maxLeft >= 2 && maxDown >= 1 )
	{
		const auto& targetIndex = aOriginIndex + ( aRows - 2 );
		if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}

	return result;
}

CTableau::indices_set CreateWildcardTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces )
{
	CTableau::indices_set result;

	for( auto pieceIt = aPieces.cbegin(); pieceIt != aPieces.cend(); ++pieceIt )
	{
		const CTableau::index& targetIndex = std::distance( aPieces.cbegin(), pieceIt );
		if( targetIndex != aOriginIndex && ( *pieceIt ) != "E" )
			result.emplace_hint( result.cend(), targetIndex );
	}

	return result;
}

CTableau::indices_set CreateNumericTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const CTableau::index& aNumber )
{
	CTableau::indices_set result;
	result.merge( CreateDiagonalTargets( aOriginIndex, aPieces, aRows, aNumber ) );
	result.merge( CreateStraightTargets( aOriginIndex, aPieces, aRows, aNumber ) );
	return result;
}

CTableau::indices_set CreateStraightTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const std::optional<CTableau::index>& aDistance )
{
	CTableau::indices_set result;

	{
		const auto& maxDistance = aOriginIndex % aRows;
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex - aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = aOriginIndex / aRows;
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex - aRows * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = ( aRows - 1 ) - ( aOriginIndex % aRows );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex + aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = ( aRows - 1 ) - ( aOriginIndex / aRows );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex + aRows * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}

	return result;
}

CTableau::indices_set CreateDiagonalTargets( const CTableau::index& aOriginIndex, const CTableau::pieces& aPieces, const CTableau::index& aRows, const std::optional<CTableau::index>& aDistance )
{
	CTableau::indices_set result;

	{
		const auto& maxDistance = std::min( aOriginIndex % aRows, aOriginIndex / aRows );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex - ( aRows + 1 ) * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = std::min( ( aRows - 1 ) - ( aOriginIndex % aRows ), aOriginIndex / aRows );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex - ( aRows - 1 ) * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = std::min( aOriginIndex % aRows, ( aRows - 1 ) - ( aOriginIndex / aRows ) );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex + ( aRows - 1 ) * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}
	{
		const auto& maxDistance = std::min( ( aRows - 1 ) - ( aOriginIndex % aRows ), ( aRows - 1 ) - ( aOriginIndex / aRows ) );
		if( !aDistance || maxDistance >= *aDistance )
		{
			const auto& targetIndex = aOriginIndex + ( aRows + 1 ) * aDistance.value_or( maxDistance );
			if( aOriginIndex != targetIndex && aPieces[ targetIndex ] != "E" )
				result.emplace_hint( result.cend(), targetIndex );
		}
	}

	return result;
}

}

};