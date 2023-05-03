#ifndef SRC_INCLUDE_ANALYZE_INFO_H
#define SRC_INCLUDE_ANALYZE_INFO_H
#include "json.hpp"
#include "string"
using std::string;
using json = nlohmann::json;
namespace Analyze {
struct AnalyzeInfo {
  size_t id{};
  string git_link;
  size_t signature_info{};
  size_t line_info{};
  string file_path_ref;
  string file_path_review;
};
void to_json(json& j, const AnalyzeInfo& i);
void from_json(const json& j, AnalyzeInfo& i);
}  // namespace Analyze
#endif  //  SRC_INCLUDE_ANALYZE_INFO_H