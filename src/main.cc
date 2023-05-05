// #define _DEBUG

#include <iostream>
#include <string>
#include <vector>

#include "analyze.h"
#include "flags.h"
using std::string;

string ShowCreators() { return "@DisonGo && @simphoniia"; }

int main() {
  vector<string> argv2 = {
      "main",
      "-project",
      "s21_stringplus",
      "-name",
      "evverenn",
      "-path",
      "/Users/evverenn/Desktop/Projects/C2_s21_stringplus-1"};
  Flags flags(argv2.size(), argv2);
  if (flags.GetState() == false) return 1;
  std::cout << ShowCreators() << "\n";
  Analyze::StartAnalyze(flags.GetProjectName(), flags.GetProjectPath());
  return 0;
}
