#include "byte_stream.hh"
#include "debug.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_(), pushed_( 0 ), popped_( 0 ) {}

void Writer::push( string data )
{
  (void)data; // Your code here.

  data.resize( min( data.length(), available_capacity() ) );
  buffer_ += data;
  pushed_ += data.length();
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
  return ( capacity_ - buffer_.length() ); // Your code here
}

uint64_t Writer::bytes_pushed() const
{
  return pushed_; // Your code here.
}

string_view Reader::peek() const
{
  return { buffer_ }; // Your code here.
}

void Reader::pop( uint64_t len )
{
  (void)len; // Your code here.
  buffer_ = buffer_.substr( len );
  popped_ += len;
}

bool Reader::is_finished() const
{
  return ( ( buffer_.empty() ) && ( state_ ) ); //? true : false ; // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return buffer_.size(); // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return popped_; // Your code here.
}
