#include "file_manager.h"

#include <filesystem>
#include <fstream>
#include <iostream>
namespace fs = std::filesystem;
vector<string> FileManager::FindFilesByExtension(string path, string ext) {
  vector<string> files;
  for (auto &p : fs::recursive_directory_iterator(path))
    if (p.path().extension() == ext)
      files.push_back(fs::absolute(p.path()).string());
  return files;
}

FilePathArrays FileManager::FindSourcesCPP(string path) {
  vector<string> headers = FindFilesByExtension(path, ".h");
  vector<string> sources = FindFilesByExtension(path, ".cc");
  return FilePathArrays(headers, sources);
}

FilePathArrays FileManager::FindSourcesC(string path) {
  vector<string> headers = FindFilesByExtension(path, ".h");
  vector<string> sources = FindFilesByExtension(path, ".c");
  return FilePathArrays(headers, sources);
}

vector<string> FileManager::FindSubDirs(string path) {
  vector<string> dirs;
  for (auto &p : fs::directory_iterator(path))
    if (p.is_directory()) dirs.push_back(p.path().filename());
  return dirs;
}
