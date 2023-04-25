#ifndef SRC_INCLUDE_LOGGER_H
#define SRC_INCLUDE_LOGGER_H

#include <fstream>
#include <iostream>

void InitLog(std::ofstream& log_file, std::string peer_name);
void WriteResult(std::ofstream& log_file, size_t id, std::string git_link,
                 int signature_percent, int line_percent);

#endif  //  SRC_INCLUDE_LOGGER_H