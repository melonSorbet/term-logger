#include <boost/json.hpp>
#include <string>
#include <vector>
#include <filesystem>


std::vector<std::string> get_all_files(std::filesystem::path path);

boost::json::value run_ripgrep(std::string);
