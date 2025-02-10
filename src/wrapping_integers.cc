#include "wrapping_integers.hh"
#include "debug.hh"
#include <cmath>
#include <iostream>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  return zero_point + n;
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{

  int64_t r_seqno = raw_value_ - zero_point.raw_value_;

  // Compute the top 3 likely absolute sequence candidates
  uint64_t lower_candidate = ( ( checkpoint / ( 1ULL << 32 ) ) - 1 ) * ( ( 1ULL << 32 ) ) + r_seqno;
  uint64_t mid_candidate = ( checkpoint / ( 1ULL << 32 ) ) * ( ( 1ULL << 32 ) ) + r_seqno;
  uint64_t upper_candidate = ( ( checkpoint / ( 1ULL << 32 ) ) + 1 ) * ( ( 1ULL << 32 ) ) + r_seqno;

  // Compute the distance of each candidate from the checkpoint but ensure no negative values
  uint64_t diff1
    = ( lower_candidate < checkpoint ? ( checkpoint - lower_candidate ) : ( lower_candidate - checkpoint ) );
  uint64_t diff2 = ( mid_candidate < checkpoint ? ( checkpoint - mid_candidate ) : ( mid_candidate - checkpoint ) );
  uint64_t diff3
    = ( upper_candidate < checkpoint ? ( checkpoint - upper_candidate ) : ( upper_candidate - checkpoint ) );

  // Return candidate with lowest distance from checkpoint
  return ( min( diff1, min( diff2, diff3 ) ) == diff1   ? lower_candidate
           : min( diff1, min( diff2, diff3 ) ) == diff2 ? mid_candidate
                                                        : upper_candidate );
}
