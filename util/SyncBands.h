#ifndef SYNC
#define SYNC
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"

namespace util
{

  void SyncBands(PlotUtils::MnvH1D* hist) {
    TH1D* theCVHisto = new TH1D(*hist);
    theCVHisto->SetDirectory(0);
    std::vector<std::string> bandnames = hist->GetErrorBandNames();
    for (std::vector<std::string>::const_iterator bandname = bandnames.begin();
       bandname != bandnames.end(); ++bandname) {
       PlotUtils::MnvVertErrorBand& band =
        *(hist->GetVertErrorBand((*bandname).c_str()));

       band.TH1D::operator=(*theCVHisto);
    }
    delete theCVHisto;
  }

  void SyncBands(PlotUtils::MnvH2D* hist) {
    TH2D* theCVHisto = new TH2D(*hist);
    theCVHisto->SetDirectory(0);
    std::vector<std::string> bandnames = hist->GetErrorBandNames();
    for (std::vector<std::string>::const_iterator bandname = bandnames.begin();
       bandname != bandnames.end(); ++bandname) {
       PlotUtils::MnvVertErrorBand2D& band =
        *(hist->GetVertErrorBand((*bandname).c_str()));

       band.TH2D::operator=(*theCVHisto);
    }
 }

}

#endif
