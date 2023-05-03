#include "logger.h"

void InitLog(std::ofstream& log_file, string peer_name) {
  if (!log_file.is_open()) {
    std::cout << "File is not exist. . .";
    return;
  }

  log_file << "New peer is [ " << peer_name << " ]\n";
  log_file << "Id\tSignature\tEachLine\tReference\n";
}

void WriteResultJson(const std::vector<AnalyzeInfo>& info) {
  json log;
  log["Entries"] = json::array();
  log["Entries_count"] = info.size();

  for (const auto& entry : info) log["Entries"].push_back(entry);

  std::ofstream log_file("log.json", std::ios_base::out);

  if (!log_file.is_open()) {
    std::cerr << "Can't write json log\n";
    return;
  }
  log_file << std::setw(2) << log;
}
void WriteResult(std::ofstream& log_file, size_t id, string git_link,
                 int signature_percent, int line_percent, string file1,
                 string file2) {
#ifndef _DEBUG
  if (signature_percent < 30 && line_percent < 30) return;
#endif  //  _DEBUG
  if (!log_file.is_open()) {
    std::cout << "File is not exist. . .";
    return;
  }
  log_file << id << "\t" << signature_percent << "\t\t\t" << line_percent
           << "\t\t\t" << git_link << "\tFile 1: " << file1
           << "\tFile 2:" << file2 << std::endl;
}

void WriteMessage(std::ofstream& log_file, const string& msg) {
  if (!log_file.is_open()) {
    std::cout << "File is not exist. . .";
    return;
  }
  log_file << msg;
}
