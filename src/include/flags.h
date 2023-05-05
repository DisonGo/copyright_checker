#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
using std::string;
using std::vector;
class Flags {
 public:
  Flags(int argc, char* argv[]) { DetectFlags(argc, argv); }
  Flags(int argc, vector<string> argv) { DetectFlags(argc, argv); }

  inline bool GetState() { return IsCorrect; }
  inline string GetProjectName() { return project_name; }
  inline string GetPeerName() { return peer_name; }
  inline string GetProjectPath() { return project_path; }
  bool IsConfirmProject();
  ~Flags() {}

 private:
  int DetectFlags(int argc, char* argv[]);
  int DetectFlags(int argc, vector<string> argv);

  int Help();

  bool IsCorrect = true;
  string project_name;
  string peer_name;
  string project_path;
};