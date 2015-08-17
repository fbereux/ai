#include <cassert>
#include <string>
#include <vector>

enum class Mode { Training, Testing };

class Options {

private :
  Mode mode_;
  std::string train_file_;
  std::string label_file_;
  std::string test_file_;
  
public :

  Options( ) {
    mode_       = Mode::Training;
    train_file_ = "";
    label_file_ = "";
    test_file_  = "";
  }

  Mode mode() { return mode_; }
  std::string train_file() { return train_file_; }
  std::string label_file() { return label_file_; }
  std::string test_file() { return test_file_; }

  void parse( int argc, char* argv[] ) {
  
    // convert argv to std::vector<std::string>
    std::vector<std::string> args;
    for(int i=0; i<argc; i++) {
      args.push_back( argv[i] );
    }
    
    // parse arguments
    std::string buf;
    std::string state;
    for(auto it = args.begin(); it != args.end(); it++) {
      buf = *it;
      if ( buf == "--mode" ) {
        state = buf;
      } else if ( state == "--mode" ) {
        if ( buf == "training" ) {
          mode_ = Mode::Training;
        } else if ( buf == "testing" ) {
          mode_ = Mode::Testing;
        } else {
          assert( false );
        }
        state      = "";
      } else if ( buf == "--train_file" ) {
        state = buf;
      } else if ( state == "--train_file" ) {
        train_file_ = buf;
        state       = "";
      } else if ( buf == "--label_file" ) {
        state = buf;
      } else if ( state == "--label_file" ) {
        label_file_ = buf;
        state       = "";
      } else if ( buf == "--test_file" ) {
        state = buf;
      } else if ( state == "--test_file" ) {
        test_file_ = buf;
        state      = "";
      } else {
        assert( false );
      }
    }
  }

}; // class Options
