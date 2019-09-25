#include <UPIDConverter.h>

#include <TROOT.h>
#include <TSystem.h>

#include <fstream>


ClassImp(UPIDConverter);

UPIDConverter* UPIDConverter::fgInstance = NULL;


UPIDConverter::UPIDConverter() :
    TNamed("PIDConverter", "Any-to-PDG particle-ID converter")
{
  if (fgInstance) {
    Warning("UPIDConverter", "object already instantiated");
  } else {
    fgInstance = this;
    gROOT->GetListOfSpecials()->Add(this);
  }
}

Int_t UPIDConverter::GetPDGCode(const Int_t pid, const EConvention pidType)
{
  /* Trivial case first */
  if (pidType == ePDG)
    return pid;

  ConversionTableMap::const_iterator mIt = fConversionTables.find(pidType);

  /* Either pidType has been set to an unknown value or the conversion table
     for specified convention hasn't been loaded yet. Assume the latter and
     try to do so now; if we still cannot find the table afterwards then
     something is indeed wrong. */
  if (mIt == fConversionTables.end()) {
    LoadConversionTable(pidType);
    mIt = fConversionTables.find(pidType);
    if (mIt == fConversionTables.end()) {
      Error("GetPDGCode",
            "PID conversion table not available for convention=%u",
            pidType);
      return 0;
    }
  }

  const std::map<Int_t, Int_t>& conversionTable = mIt->second;
  std::map<Int_t, Int_t>::const_iterator pidIt = conversionTable.find(pid);
  if (pidIt == conversionTable.end()) {
    Warning("GetPDGCode", "PDG code not found for convention=%u, pid=%d",
            pidType, pid);
    return 0;
  }

  return pidIt->second;
}


UPIDConverter* UPIDConverter::Instance()
{
  return (fgInstance) ? fgInstance : new UPIDConverter();
}


void UPIDConverter::LoadConversionTable(const EConvention pidType)
{
  std::string dataFileName = "/input/";
  switch (pidType) {
    case (ePluto):
      dataFileName += "pluto_pdg.dat";
      break;
    case (eUrQMD):
      dataFileName += "urqmd_pdg.dat";
      break;
    case (eWerner):
      // VENUS, NEXUS, EPOS
      dataFileName += "werner_pdg.dat";
      break;
    default:
      Error("LoadConversionTable",
            "I do not know where to find conversion table for convention=%u",
            pidType);
      return;
  }

  const std::string unigenBase = gSystem->Getenv("UNIGEN");
  const std::string fullFileName = unigenBase + dataFileName;
  std::ifstream fin;
  fin.open(fullFileName.c_str());
  if (!fin) {
    Error("LoadConversionTable",
          "Failed to open conversion-table file %s", fullFileName.c_str());
    return;
  }

  std::map<Int_t, Int_t>& conversionTable = fConversionTables[pidType];
  Int_t localPid;
  Int_t pdgPid;
  while (1) {
    // FIXME: we might want to make this more robust against malformed input
    if (fin.eof())
      break;
    fin >> localPid >> pdgPid;
    conversionTable[localPid] = pdgPid;
  }
  fin.close();

  return;
}
