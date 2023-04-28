
#pragma once
#include <fstream>
#include <string>
#include <vector>
using std::ifstream;
using std::pair;
using std::string;
using std::vector;

typedef vector<string> PathArray;
typedef vector<string> FileData;
typedef vector<pair<FileData, string>> FilesData;
class FilePathArrays {
 public:
  PathArray headers;
  PathArray sources;
  FilePathArrays(){};
  ~FilePathArrays(){};
  FilePathArrays(const FilePathArrays& other) noexcept {
    this->headers = other.headers;
    this->sources = other.sources;
  }
  // headers(std::move(headers)), sources(std::move(sources))
  FilePathArrays(PathArray& headers, PathArray& sources) noexcept {
    this->headers = std::move(headers);
    this->sources = std::move(sources);
  }
  FilePathArrays& operator=(const FilePathArrays& other) noexcept {
    if (this == &other) return *this;
    this->headers = other.headers;
    this->sources = other.sources;
    return *this;
  }
  FilePathArrays& operator=(FilePathArrays&& other) noexcept {
    this->headers = std::move(other.headers);
    this->sources = std::move(other.sources);
    return *this;
  }
};

class FileManager {
 private:
  static vector<string> FindFilesByExtension(const string& path,
                                             const string& ext);

 public:
  static FilePathArrays FindSourcesCPP(const string& path);
  static FilePathArrays FindSourcesC(const string& path);
  static vector<string> FindSubDirs(const string& path);
  static FileData ReadFileContent(const string& path);
  static FileData ReadFileContent(ifstream& file);
  static FileData ReadFileContent(const string& path,
                                  string (*line_transform)(const string&));
  static FileData ReadFileContent(ifstream& file,
                                  string (*line_transform)(const string&));
  static void ReadPathArrayData(const PathArray& paths, FilesData& files_data);
  static FilesData ReadPathArrayData(const PathArray& paths);
  static FileData TransformFileData(const FileData& data,
                                    string (*line_transform)(const string&));
};
