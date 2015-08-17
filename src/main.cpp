
#include "Options.hpp"
#include "Image.hpp"

int main( int argc, char* argv[] ) {

  Options opt;
  opt.parse( argc, argv );
  
  switch( opt.mode() ) {
  case Mode::Training :
    break;
  case Mode::Testing :
    break;
  default:
    assert( false );
  }

}
