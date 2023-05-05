// #define _DEBUG

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "analyze.h"
#include "file_manager.h"
#include "flags.h"
#include "logger.h"
#include "repo_manager.h"

using std::string;
int main() {
  FileManager fm;
  FileData data = fm.ReadFileContent(
      "/Users/burnsdov/Eye_of_Verter/src/s21_get_minor.c");
  SignatureCompare::RemoveVariables(data);
  for (const auto& line : data) std::cout << line << "\n";
}
