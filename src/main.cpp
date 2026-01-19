
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
#include <CLI/App.hpp>
#include <CLI/CLI.hpp>
#include <filesystem>
#include <print>
#include <fuzzy.hpp>
#include <vector>
using namespace std;


// fuzzy finder
int main() {

  vector<string> paths = get_all_files("/home/user/Downloads/");
  println("{}", paths);

}
