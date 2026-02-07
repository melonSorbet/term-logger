#include <string>
#include <vector>
#include <filesystem>


std::vector<std::string> get_all_files(std::filesystem::path path);

std::string run_ripgrep(std::string);
