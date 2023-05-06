#include "peer_file_data.h"

#include "linecompare.h"
#include "signaturecompare.h"
using Analyze::PeerFileData;
PeerFileData::PeerFileData() {}

PeerFileData::PeerFileData(const FileData& source) : source(source) {
  AnalyzeSource();
}

PeerFileData::PeerFileData(const PeerFileData& data)
    : source(data.source),
      signatureData(data.signatureData),
      lineCompData(data.lineCompData) {}

PeerFileData::PeerFileData(PeerFileData&& data)
    : source(std::move(data.source)),
      signatureData(std::move(data.signatureData)),
      lineCompData(std::move(data.lineCompData)) {}

PeerFileData::~PeerFileData() {}
void PeerFileData::AnalyzeSource() {
  signatureData = source;
  lineCompData = source;
  SignatureCompare::PrepareFileData(signatureData);
  LineCompare::PrepareFileData(lineCompData);
};
void PeerFileData::SetSource(const FileData& source) {
  this->source = source;
  AnalyzeSource();
}
void Analyze::FromFilesData(PeerFilesData& peerData, const FilesData& data) {
  peerData.reserve(data.size());
  if (peerData.size()) peerData.clear();
  for (auto& fileData : data)
    peerData.push_back({fileData.first, fileData.second});
}
void Analyze::ReadPathArrayData(const PathArray& paths,
                                PeerFilesData& files_data) {
  for (auto& path : paths) {
    PeerFileData data = FileManager::ReadFileContent(path);
    if (!data.source.empty()) {
      files_data.push_back({data, path});
    } else {
      std::cerr << "Data is empty in:" << path << "\n";
    }
  }
}
PeerFileData& PeerFileData::operator=(const PeerFileData& data) {
  if (this == &data) return *this;
  source = data.source;
  signatureData = data.signatureData;
  lineCompData = data.lineCompData;
  return *this;
}

PeerFileData& PeerFileData::operator=(PeerFileData&& data) {
  if (this == &data) return *this;
  source = std::move(data.source);
  signatureData = std::move(data.signatureData);
  lineCompData = std::move(data.lineCompData);
  return *this;
}

PeerFileData& Analyze::PeerFileData::operator=(const FileData& data) {
  if (&source == &data) return *this;
  source = data;
  AnalyzeSource();
  return *this;
}

PeerFileData& Analyze::PeerFileData::operator=(FileData&& data) {
  if (&source == &data) return *this;
  source = std::move(data);
  AnalyzeSource();
  return *this;
}
