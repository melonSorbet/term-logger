#include <boost/json.hpp>
#include <filesystem>
#include <string>
#include <vector>

std::vector<std::string> get_all_files(std::filesystem::path path);

std::vector<boost::json::value> run_ripgrep(std::string);
