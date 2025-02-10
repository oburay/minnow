// #include "socket.hh"
#include "tcp_minnow_socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>

using namespace std;

void get_URL( const string& host, const string& path )
{
  /*-----------------------------Pseudocode--------------------------
   1. Create a socket for the connect
   2. Initiate an instance of the Address class and bind it to the socket
   3. Initiate connection to socket
   4. Write HTTP request to socket
   5. Close connection
   6. Loop through reads from the socket until I reach EOF
  */

  string method, protocol, version, close, request, response, host_, end;

  method = "GET ";
  protocol = "http";
  version = " HTTP/1.1";
  close = "Connection: close \r\n";
  host_ = "Host: ";
  end = "\r\n";

  CS144TCPSocket socket;
  Address addr( host, protocol );
  socket.connect( addr );
  request = method + path + version + end;
  socket.write( request );
  request = "";
  request = host_ + host + end;
  socket.write( request );
  socket.write( close );
  socket.write( end );

  // Looping through output chars from the server
  while ( !socket.eof() ) {
    socket.read( response );
    cout << response;
  }

  socket.wait_until_closed();
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}