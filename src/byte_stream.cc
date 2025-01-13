#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_(), pushed_( 0 ), popped_( 0 )
{} 

void Writer::push( string data )
{
  (void)data; // Your code here.
  string byte;

  /* -----------------------------Pseudocode--------------------------
   1. Check for available capacity in the byte stream and remaining characters in the data
   2. Loop through each character in the data and push a character per iteration based on conditionalities above
   3. Update variables for available capacity and cumulative number of bytes pushed into the byte stream
  */ 
  
  for ( uint64_t i = 0; capacity_ > 0 && i < data.length(); i++ ) {
    byte = data[i];
    buffer_.push( byte );
    capacity_--;
    pushed_++;
  }
}

void Writer::close()
{
  // Your code here.
  state_ = true;
}

bool Writer::is_closed() const
{
  return state_; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return  capacity_; // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return pushed_ ; // Your code here.
}

string_view Reader::peek() const
{
  return { buffer_.front() }; // Your code here.
}

void Reader::pop( uint64_t len )
{
  (void)len; // Your code here.

  for ( uint64_t i = 0; i < len; i++ ) {
    buffer_.pop();
    capacity_++;
    popped_++;
    
  }
}

bool Reader::is_finished() const
{
  return ( ( buffer_.size() == 0 ) && ( state_ == true ) ) ? true : false ; // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_.size(); // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return  popped_; // Your code here.
}
