#pragma once

#include <algorithm>
#include <iostream>

class Flags {
 public:
  Flags(int argc, char* argv[]) { DetectFlags(argc, argv); }

  bool GetState() { return IsCorrect; }
  std::string GetProjectName() { return project_name; }
  std::string GetPeerName() { return peer_name; }
  std::string GetProjectPath() { return project_path; }

  ~Flags() {}

 private:
  int DetectFlags(int argc, char* argv[]) {
    if (argc == 1) Help();
    for (int i = 1; i < argc; i++) {
      std::string current = argv[i];
      if (current == "-help")
        Help();
      else if (current == "-project") {
        if (i == argc - 1) Help();
        project_name = argv[i++ + 1];
      } else if (current == "-name") {
        if (i == argc - 1) Help();
        peer_name = argv[i++ + 1];
      } else if (current == "-path") {
        if (i == argc - 1) Help();
        project_path = argv[i++ + 1];
      } else {
        Help();
      }
    }
    return 0;
  }

  int Help() {
    std::cout << "-project *project name*\n-name *peer name*\n-path *path to "
                 "the peer project*\n";
    this->IsCorrect = false;
    exit(0);
  }

  bool IsCorrect = true;
  std::string project_name;
  std::string peer_name;
  std::string project_path;
};