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
  for (auto& fileData : data) {
    peerData.push_back(
        std::make_pair((PeerFileData){fileData.first}, fileData.second));
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
