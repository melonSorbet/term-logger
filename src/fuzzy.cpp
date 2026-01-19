#include <filesystem>
#include <string>
#include <vector>
using namespace std;
void fuzzy_find_filenames(){

}

void fuzzy_find_text(){

}

vector<string> get_all_files(filesystem::path path){
  auto iterator = std::filesystem::recursive_directory_iterator(path);
  vector<string> files = {};

  for (const auto& entry : iterator){
    if (entry.exists() && entry.is_regular_file()){
      files.push_back(entry.path().string());
    }
  }
  return files;
}

void fuzzy_find_file(){

}

