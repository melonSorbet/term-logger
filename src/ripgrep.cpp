#include <boost/json.hpp>
#include <filesystem>
#include <format>
#include <string>

#include <ripgrep.hpp>
#include <boost/process.hpp>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;
void get_text_preview() {}
vector<string> get_all_files(filesystem::path path) {
  auto iterator = std::filesystem::recursive_directory_iterator(path);
  vector<string> files = {};

  for (const auto &entry : iterator) {
    if (entry.exists() && entry.is_regular_file()) {
      files.push_back(entry.path().string());
    }
  }
  return files;
}

// run ripgrep and build data structure for easy data access
// run the command then build it on teh json return
boost::json::value run_ripgrep(std::string pattern) {
  boost::process::ipstream output;
  std::string command = std::format("rg -i {} --json", pattern);
  boost::process::child child_process(command,
                                      boost::process::std_out > output);
  std::string output_string((std::istreambuf_iterator<char>(output)),
                            std::istreambuf_iterator<char>());

  child_process.wait(); // Wait for the child to finish

  // println("{}", output_string);

  return boost::json::parse(output_string);
}
