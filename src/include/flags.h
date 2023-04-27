#pragma once

#include <algorithm>
#include <iostream>

class Flags {
 public:
  Flags(int argc, char* argv[]) {
    int result = DetectFlags(argc, argv);
    if (result == -1) {
        std::cout << "No flags or incorrect flags\n";
    }
  }


  bool GetState() { return IsCorrect; }
  std::string GetProjectName() { return project_name; }
  std::string GetPeerName() { return peer_name; }
  std::string GetProjectPath() { return project_path; }

  ~Flags() {}

 private:
  int DetectFlags(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
      std::string current_argument = argv[i];
      if (current_argument == "-project") this->project_name = argv[i + 1];
      if (current_argument == "-name") this->peer_name = argv[i + 1];
      if (current_argument == "-path") this->project_path = argv[i + 1];
      if (current_argument == "-help") {
          std::cout << "-project *project name*\n-name *peer name*\n-path *path to the peer project*\n";
          this->IsCorrect = false;
      }
    }
    return 0;
  }
  bool IsCorrect = true;
  std::string project_name;
  std::string peer_name;
  std::string project_path;
};