
#pragma once
#include <string>
#include <vector>
using std::pair;
using std::string;
using std::vector;
typedef pair<vector<string>, vector<string>> FilePathArrays;

class FileManager {
 private:
  static vector<string> FindFilesByExtension(string path, string ext);

 public:
  static FilePathArrays FindSourcesCPP(string path);
  static FilePathArrays FindSourcesC(string path);
};