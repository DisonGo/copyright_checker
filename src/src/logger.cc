#include "logger.h"

void Logger::WriteResultJson(const vector<AnalyzeInfo>& info) {
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
bool Logger::SortAnalyzeBySignature(const AnalyzeInfo& first,
                                    const AnalyzeInfo& second) {
  return first.signature_info > second.signature_info;
}
void Logger::GetResultConfig(ThreadsAnalyzeResults& analyze_results) {
  vector<AnalyzeInfo> results;
  for (auto& result_array : analyze_results) {
    results.insert(results.end(), result_array.begin(), result_array.end());
  }
  std::sort(results.begin(), results.end(), SortAnalyzeBySignature);
  WriteResultJson(results);
}
