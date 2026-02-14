#include <boost/json.hpp>
#include <filesystem>
#include <string>
#include <vector>

std::vector<std::string> get_all_files(std::filesystem::path path);

std::vector<boost::json::value> run_ripgrep(std::string);

struct RipGrepMatch {
  std::string path;
  std::string line_text;
  int line_number;
  int start;
  int end;
  std::string match_text;
  std::string to_string() const {
    return "RipGrepMatch { "
           "path: \"" +
           path +
           "\", "
           "line_text: \"" +
           line_text +
           "\", "
           "line_number: " +
           std::to_string(line_number) +
           ", "
           "start: " +
           std::to_string(start) +
           ", "
           "end: " +
           std::to_string(end) +
           ", "
           "match_text: \"" +
           match_text + "\" }";
  }
};

std::vector<RipGrepMatch> jsonToStruct(std::vector<boost::json::value> matches);
