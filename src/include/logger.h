#ifndef SRC_INCLUDE_LOGGER_H
#define SRC_INCLUDE_LOGGER_H

#include <fstream>
#include <iostream>

#include "analyze_info.h"
#include "json.hpp"
namespace Logger {
using std::string;
using std::vector;
using json = nlohmann::json;
using Analyze::AnalyzeInfo;
typedef vector<vector<AnalyzeInfo>> ThreadsAnalyzeResults;
void WriteResultJson(const vector<AnalyzeInfo>& info);
bool SortAnalyzeBySignature(const AnalyzeInfo& first,
                            const AnalyzeInfo& second);
void GetResultConfig(ThreadsAnalyzeResults& analyze_results);
}  // namespace Logger
#endif  //  SRC_INCLUDE_LOGGER_H