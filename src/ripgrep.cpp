#include <boost/json.hpp>
#include <boost/json/parse.hpp>
#include <cstdio>
#include <fcntl.h>
#include <filesystem>
#include <format>
#include <print>
#include <ripgrep.hpp>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
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

std::vector<RipGrepMatch>
jsonToStruct(std::vector<boost::json::value> matches) {
  //    "type" : "match", "data" : {
  //      "path" : {"text" : "src/tui.cpp"},
  //               "lines"
  //          : {"text" : "  std::vector<std::string> entries = {\"cool\"};\n"},
  //            "line_number" : 19,
  //            "absolute_offset" : 428,
  //            "submatches"
  //          : [ {"match" : {"text" : "coo"}, "start" : 39, "end" : 42} ]
  std::vector<RipGrepMatch> result;
  for (auto json : matches) {
    if (!json.is_object() || json.is_null()) {
      continue;
    }
    auto object = json.as_object();
    if (object.contains("type") && object.at("type").as_string() == "match") {
      const auto &data = object.at("data").as_object();
      std::string path =
          data.at("path").as_object().at("text").as_string().c_str();
      std::string line_text =
          data.at("lines").as_object().at("text").as_string().c_str();
      int line_number = data.at("line_number").as_int64();

      for (auto &submatch : data.at("submatches").as_array()) {
        RipGrepMatch rgm;
        rgm.match_text =
            submatch.at("match").as_object().at("text").as_string().c_str();
        rgm.start = submatch.at("start").as_int64();
        rgm.end = submatch.at("end").as_int64();
        rgm.path = path;
        rgm.line_text = line_text;

        result.push_back(std::move(rgm));
      }
    }
  }
  return result;
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
