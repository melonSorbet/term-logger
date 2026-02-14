
// first setup check for configuration, setup in bash script.
// create config file
// add it to path
// then add startup in your preferred shell,

// everytime you have a terminal session,
// it saves your terminal output into a file
// the file will have the session name + date + 1-999

// either you save after each clear and shutdown
// or you do clear + shutdown + auto saves

// then when you want to look through the logs
// either you open the files per .local/state/term-logger
// or you say term-logger find which opens a screen like
#include <boost/json/src.hpp>
#include <cstdlib>
#include <ripgrep.hpp>
#include <tui.hpp>
#include <unistd.h>

int main(int argc, char *argv[]) {

  // if (argc <= 1) {
  //   return EXIT_FAILURE;
  // }

  // std::string str(argv[1]);
  // std::vector<boost::json::value> ripgrep_result_json = run_ripgrep(str);

  // auto results = jsonToStruct(ripgrep_result_json);

  setupTUI();

  return EXIT_SUCCESS;
}
