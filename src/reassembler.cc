#include "reassembler.hh"
#include <algorithm>
#include <iostream>

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{

  //------------Checking for last_index in the stream ---------------
  if ( is_last_substring ) {
    this->last_substring = is_last_substring;
    last_index = first_index + data.size();
  }

  //-----------Test for overlaps------------------
  if ( ( first_index < next_index ) ) {
    first_index = next_index - first_index; // Calculate the overlaping index within overlapping data stream

    if ( ( first_index
           >= data.size() ) ) { // Address edge cases where overlapping index is out of bound for data stream
      return;
    }

    data = data.substr( first_index ); // Trim data stream to eliminate any overlapping bytes
    first_index = next_index;          // Set actual first byte to start of next_byte required for the byte stream
  }

  //-------------- Trim data to fit available capacity---------------------------
  data.resize(
    min( data.size(),
         ( output_.writer().available_capacity() ) ) ); // Just resize the input data to fit our available capacity

  //---------------- Ensures out-of-bound index access ------------------------
  if ( ( first_index - next_index ) < output_.writer().available_capacity() ) {

    reassembler_buffer.resize( output_.writer().available_capacity(),
                               '0' ); // Resize the reassembler's buffer to the available capacity of byte stream &
                                      // insert 0 for the new spaces
    reassembler_track.resize(
      output_.writer().available_capacity(),
      '0' ); // Resize the tracker as well with 0s to indicate those index haven't been filled

    //-----------------Push bytes to Reassembler -------------------------------
    reassembler_buffer.replace( ( first_index - next_index ), data.size(), data );
    reassembler_track.replace( ( first_index - next_index ), data.size(), data.size(), '1' );

    //----------------------Ensure no out-of-bound inserts into reassembler --------------------
    reassembler_buffer.resize( output_.writer().available_capacity() );
    reassembler_track.resize( output_.writer().available_capacity() );
  }

  //----------------- Keeps count of the longest sequence of indexes in reassembler filled starting from index 0
  //---------------------------------
  uint64_t track_count { 0 };
  for ( uint64_t i = 0; i < reassembler_track.size(); i++ ) {
    if ( reassembler_track[i] == '1' ) {
      track_count++;
    }

    else {
      break;
    }
  }

  //--------------------- Push to bytestream ------------------------------------
  if ( track_count > 0 ) {
    std::string sbytes = reassembler_buffer.substr( 0, track_count );
    output_.writer().push( sbytes ); // push the most continuous ordered sequence of reassembled bytes
    next_index += sbytes.size();     // next_index needs increase
    reassembler_buffer
      = reassembler_buffer.substr( track_count ); // Clear the part going to be pushed to the bystream
    reassembler_track
      = reassembler_track.substr( track_count ); // Clear the part going to be pushed to the bystream
  }

  //--------------------  Check to see if last string has been passed and reassembler is empty
  //-------------------------
  if ( last_substring && ( next_index >= last_index ) ) {

    output_.writer().close();
  }
}

// How many bytes are stored in the Reassembler itself?
// This function is for testing only; don't add extra state to support it.
uint64_t Reassembler::count_bytes_pending() const
{
  uint64_t track_count { 0 }; // Keeps count of the longest sequence of indexes filled starting from index 0
  for ( uint64_t i = 0; i < reassembler_track.size(); i++ ) {
    if ( reassembler_track[i] == '1' ) {
      track_count++;
    }

    else {
      continue;
    }
  }

  return track_count;
}
