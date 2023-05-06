#ifndef SRC_INCLUDE_PEER_FILE_DATA_H
#define SRC_INCLUDE_PEER_FILE_DATA_H
#include "file_manager.h"
namespace Analyze {

// Used for analyze of peer files instead of FileData
// Reason:
// Makes copies of peer source file and prepares them for analyze beforehand
// Reduces N of calculations of peer file data in each thread;
class PeerFileData {
 public:
  FileData source;
  FileData signatureData;
  FileData lineCompData;

  PeerFileData();
  PeerFileData(const FileData& source);
  PeerFileData(const PeerFileData& data);
  PeerFileData(PeerFileData&& data);
  ~PeerFileData();
  PeerFileData& operator=(const PeerFileData& data);
  PeerFileData& operator=(PeerFileData&& data);
  PeerFileData& operator=(const FileData& data);
  PeerFileData& operator=(FileData&& data);
  void SetSource(const FileData& source);

 private:
  void AnalyzeSource();
};

typedef vector<pair<PeerFileData, string>>
    PeerFilesData;  // Replacement for FilesData
void ReadPathArrayData(const PathArray& paths, PeerFilesData& files_data);
void FromFilesData(PeerFilesData& peerData, const FilesData& data);
}  // namespace Analyze
#endif  //  SRC_INCLUDE_NALYZE__H