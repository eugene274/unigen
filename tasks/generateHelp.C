#if !defined(__CINT__) || defined(__MAKECINT__)

#include <cstdlib>

#include <THtml.h>
#include <TSystem.h>

#endif


void generateHelp(void)
{
  gSystem->AddDynamicPath("../lib");
  gSystem->Load("libUniGen");
  gSystem->Load("libTasks");

  THtml *html = new THtml();
  html->SetInputDir(".:include:src");

  html->SetProductName("UniGen tasks");
  html->MakeClass("UTaskCm2Lab");
  html->MakeClass("UTaskFilter");
  html->MakeClass("UTaskHSDGetInel");
  html->MakeClass("UTaskKin");
  html->MakeClass("UTaskLab2Cm");
  html->MakeClass("UTaskRead");
  html->MakeClass("UTaskUrQMDGetInel");
  html->MakeClass("UTaskWrite");

  system("cp -a htmldoc ~/web-docs/unigen");
  system("rm -rf ~/web-docs/unigen/tasks");
  system("mv ~/web-docs/unigen/htmldoc ~/web-docs/unigen/tasks");

  return;
}
