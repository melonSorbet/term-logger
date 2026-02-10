#include <boost/json/parse.hpp>
#include <fcntl.h>
#include <filesystem>
#include <format>
#include <ripgrep.hpp>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

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
class RipGrepMatch {
  std::string path;
  int line;
  int start_line;
  int end_line;

  std::vector<RipGrepMatch> jsonToStruct() {}
};

std::vector<boost::json::value> run_ripgrep(std::string pattern) {
  if (pattern.empty()) {
    return {};
  }
  boost::process::ipstream output;
  std::string command = std::format("rg -i {} --json", pattern);
  boost::process::child child_process(command,
                                      boost::process::std_out > output);

  std::string line;
  std::vector<boost::json::value> results;

  while (std::getline(output, line)) {
    if (!line.empty()) {
      results.push_back(boost::json::parse(line));
    }
  }
  child_process.wait();

  return results;
}
