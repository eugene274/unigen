#if !defined(__CINT__) || defined(__MAKECINT__)

#include <cstdlib>

#include <THtml.h>
#include <TSystem.h>

#endif


void generateHelp(void)
{
  gSystem->AddDynamicPath("../lib");
  gSystem->Load("libUniGen");

  THtml *html = new THtml();
  html->SetInputDir(".:include:src");

  html->SetProductName("UniGen core");
  html->MakeClass("UEvent");
  html->MakeClass("UManager");
  html->MakeClass("UParticle");
  html->MakeClass("URun");
  html->MakeClass("UTask");

  system("cp -a htmldoc ~/web-docs/unigen");
  system("rm -rf ~/web-docs/unigen/base");
  system("mv ~/web-docs/unigen/htmldoc ~/web-docs/unigen/base");

  return;
}
