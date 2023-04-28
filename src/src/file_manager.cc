#include "file_manager.h"

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;
vector<string> FileManager::FindFilesByExtension(const string& path,
                                                 const string& ext) {
  vector<string> files;
  if (!fs::is_directory(path)) {
    std::cerr << "'" << path << "' is not a dir"
              << "\n";
    return files;
  }
  for (auto& p : fs::recursive_directory_iterator(path))
    if (p.path().extension() == ext)
      files.push_back(fs::absolute(p.path()).string());
  return files;
}

FilePathArrays FileManager::FindSourcesCPP(const string& path) {
  vector<string> headers = std::move(FindFilesByExtension(path, ".h"));
  vector<string> sources = std::move(FindFilesByExtension(path, ".cc"));
  return FilePathArrays(headers, sources);
}

FilePathArrays FileManager::FindSourcesC(const string& path) {
  vector<string> headers = std::move(FindFilesByExtension(path, ".h"));
  vector<string> sources = std::move(FindFilesByExtension(path, ".c"));
  return FilePathArrays(headers, sources);
}

vector<string> FileManager::FindSubDirs(const string& path) {
  vector<string> dirs;
  if (!fs::exists(path)) {
    fs::create_directory(path);
    return dirs;
  }
  for (auto& p : fs::directory_iterator(path))
    if (p.is_directory()) dirs.push_back(p.path().filename());
  return dirs;
}

FileData FileManager::ReadFileContent(const string& path) {
  FileData data;
  ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Can't open file:'" << path << "'\n";
    return data;
  }
  string buffer;
  while (std::getline(file, buffer))
    if (buffer.size() > 0) data.push_back(buffer);
  file.close();
  return data;
}

FileData FileManager::ReadFileContent(ifstream& file) {
  FileData data;
  if (!file.is_open()) {
    std::cerr << "File is not open.\n";
    return data;
  }
  string buffer;
  while (std::getline(file, buffer))
    if (buffer.size() > 0) data.push_back(buffer);
  return data;
}

FileData FileManager::ReadFileContent(const string& path,
                                      string (*line_transform)(const string&)) {
  FileData data;
  ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Can't open file:'" << path << "'\n";
    return data;
  }
  string buffer;
  while (std::getline(file, buffer)) {
    buffer = std::move(line_transform(buffer));
    if (buffer.size() > 0) data.push_back(buffer);
  }
  file.close();
  return data;
}

FileData FileManager::ReadFileContent(ifstream& file,
                                      string (*line_transform)(const string&)) {
  FileData data;
  if (!file.is_open()) {
    std::cerr << "File is not open.\n";
    return data;
  }
  string buffer;
  while (std::getline(file, buffer)) {
    buffer = std::move(line_transform(buffer));
    if (buffer.size() > 0) data.push_back(buffer);
  }
  return data;
}
void FileManager::ReadPathArrayData(const PathArray& paths,
                                    FilesData& files_data) {
  for (auto& path : paths) {
    FileData data = std::move(FileManager::ReadFileContent(path));
    if (!data.empty()) files_data.push_back({data, path});
  }
}

FilesData FileManager::ReadPathArrayData(const PathArray& paths) {
  FilesData files_data;
  FileManager::ReadPathArrayData(paths, files_data);
  return files_data;
}

FileData FileManager::TransformFileData(
    const FileData& data, string (*line_transform)(const string&)) {
  string buffer;
  FileData new_data;
  for (auto& line : data) {
    if (line.size() > 0) {
      buffer = std::move(line_transform(line));
      if (buffer.size() > 0) new_data.push_back(buffer);
    }
  }
  return new_data;
}
