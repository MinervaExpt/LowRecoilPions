//File: ExtractCrossSection.cpp
//Brief: Given data and MC files from analyses/studies/CrossSection.h, extract a 1D differential cross section.
//       Subtracts backgrounds, performs unfolding, applies efficiency x acceptance correction, and 
//       divides by flux and number of nucleons.  Writes a .root file with the cross section histogram.
//
//Usage: ExtractCrossSection <unfolding iterations> <data.root> <mc.root>
//
//Author: Andrew Olivier aolivier@ur.rochester.edu

//util includes
#include "util/GetIngredient.h"

//UnfoldUtils includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "MinervaUnfold/MnvUnfold.h"

//PlotUtils includes
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"
#include "PlotUtils/MnvPlotter.h"
#include "MinervaUnfold/MnvResponse.h"
#include "MinervaUnfold/MnvUnfold.h"
#pragma GCC diagnostic pop

//ROOT includes
#include "TH1D.h"
#include "TFile.h"
#include "TKey.h"
#include "TParameter.h"
#include "TCanvas.h"
#include "TMatrix.h"
//Cintex is only needed for older ROOT versions like the GPVMs.
////Let CMake decide whether it's needed.
#ifndef NCINTEX
#include "Cintex/Cintex.h"
#endif

//c++ includes
#include <iostream>
#include <exception>
#include <algorithm>
#include <numeric>

//Convince the STL to talk to TIter so I can use std::find_if()
namespace std
{
  template <>
  struct iterator_traits<TIter>
  {
    using value_type = TObject;
    using pointer = TObject*;
    using reference = TObject&;
    using iterator_category = forward_iterator_tag;
  };
}

TMatrixD ZeroDiagonal(const TMatrixD &m){
  std::cout << " TRACE: enter ZeroDiagonal  "   << std::endl;
  TMatrixD newm = TMatrixD(m);
  int n = newm.GetNrows();
  for (int i = 0; i < n; i++){
    newm[i][i] = 0;
  }
  return newm;
}



//Plot a step in cross section extraction.
void Plot(PlotUtils::MnvH2D& hist, const std::string& stepName, const std::string& prefix)
{
  TCanvas can(stepName.c_str());
  hist.GetCVHistoWithError().Clone()->Draw();
  can.Print((prefix + "_" + stepName + ".png").c_str());

  //Uncertainty summary
  PlotUtils::MnvPlotter plotter;
  plotter.ApplyStyle(PlotUtils::kCCQENuStyle);
  plotter.axis_maximum = 0.4;
  
  int nbinsy = hist.GetNbinsY()+2;
  for (int i = 0; i < nbinsy; i++)
  {
	plotter.DrawErrorSummary(hist.ProjectionX("e",i,i));
        can.Print((prefix + "_bin" + std::to_string(i) +"_" + stepName + "_uncertaintySummary.png").c_str());
	plotter.DrawErrorSummary(hist.ProjectionX("e",i,i), "TR", true, true, 1e-5, false, "Other");
        can.Print((prefix + "_bin" + std::to_string(i) + "_" + stepName + "_otherUncertainties.png").c_str());
  }
  //plotter.DrawErrorSummary(&hist);
  //can.Print((prefix + "_" + stepName + "_uncertaintySummary.png").c_str());

  //plotter.DrawErrorSummary(&hist, "TR", true, true, 1e-5, false, "Other");
  //can.Print((prefix + "_" + stepName + "_otherUncertainties.png").c_str());
}

void Plot(PlotUtils::MnvH1D& hist, const std::string& stepName, const std::string& prefix)
{ 
  TCanvas can(stepName.c_str());
  hist.GetCVHistoWithError().Clone()->Draw();
  can.Print((prefix + "_" + stepName + ".png").c_str());

  //Uncertainty summary 
  PlotUtils::MnvPlotter plotter;
  plotter.ApplyStyle(PlotUtils::kCCQENuStyle);
  plotter.axis_maximum = 0.4;
  
  plotter.DrawErrorSummary(&hist);
  can.Print((prefix + "_" + stepName + "_uncertaintySummary.png").c_str());
  
  plotter.DrawErrorSummary(&hist, "TR", true, true, 1e-5, false, "Other");
  can.Print((prefix + "_" + stepName + "_otherUncertainties.png").c_str());
}


PlotUtils::MnvH2D* DoResponseUnfolding(std::string basename, PlotUtils::MnvH2D* iresponse,
                                      PlotUtils::MnvH2D* imcsighist, PlotUtils::MnvH2D* iseltruhist, PlotUtils::MnvH2D* bkgsub, PlotUtils::MnvH2D* idatahist,
                                      MinervaUnfold::MnvUnfold unfold, double num_iter){

  PlotUtils::MnvH2D* migration = (PlotUtils::MnvH2D*)iresponse->Clone();
  if (migration == 0) {
  std::cout << " no migration, stop here for " << basename << std::endl;
    return bkgsub;
  }

  migration->PopVertErrorBand("cv");

  std::string unsmearedname = std::string(bkgsub->GetName()) + "_unfolded";

  PlotUtils::MnvH2D* unsmeared = (PlotUtils::MnvH2D*)iseltruhist->Clone(unsmearedname.c_str());
  unsmeared->SetDirectory(0);
  bkgsub->Print();
  iseltruhist->Print();
  imcsighist->Print();

  std::cout << " starting 2D unfolding " << std::endl;

   bool data_unfolded = unfold.UnfoldHisto2D(unsmeared,migration,imcsighist,iseltruhist,bkgsub,num_iter,true,true);
  std::cout << " Done with 2D unfolding " << std::endl;
  std::cout << "unsmeared " << unsmeared->Integral() << " " << unsmeared->Integral() << std::endl;
  bkgsub->Print();
  imcsighist->Print();
  iseltruhist->Print();


  TH2D* hUnfoldedDummy=new TH2D(unsmeared->GetCVHistoWithStatError());
  TH2D* hMigrationDummy=new TH2D(migration->GetCVHistoWithStatError());
  TH2D* hRecoDummy=new TH2D(imcsighist->GetCVHistoWithStatError());
  TH2D* hTruthDummy=new TH2D(iseltruhist->GetCVHistoWithStatError());
  TH2D* hBGSubDataDummy=new TH2D(bkgsub->GetCVHistoWithStatError());
  TMatrixD unfoldingCovMatrixOrig_hist_type;
  std::cout << "HERE for COVARIANCE " << std::endl;

  unfold.UnfoldHisto2D(hUnfoldedDummy, unfoldingCovMatrixOrig_hist_type, hMigrationDummy, hRecoDummy, hTruthDummy, hBGSubDataDummy, num_iter);
  int correctNbins = hUnfoldedDummy->fN;
  int matrixRows = unfoldingCovMatrixOrig_hist_type.GetNrows();

  if(correctNbins!=matrixRows){

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "*  Fixing unfolding matrix size because of RooUnfold bug. From " << matrixRows << " to " << correctNbins << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  // It looks like this DTRT, since the extra last two bins don't have any content
  unfoldingCovMatrixOrig_hist_type.ResizeTo(correctNbins, correctNbins);
  }
  TMatrix unfoldingCov = ZeroDiagonal(unfoldingCovMatrixOrig_hist_type);
  unsmeared->FillSysErrorMatrix("Unfolding",unfoldingCov);
  //SyncBands(unsmeared);
  return unsmeared;

}



//Unfolding function from Aaron Bercelle
//TODO: Trim it down a little?  Remove that static?
/*
PlotUtils::MnvH2D* UnfoldHist( PlotUtils::MnvH2D* h_folded, PlotUtils::MnvH2D* h_migration, int num_iter )
{
  static MinervaUnfold::MnvUnfold unfold;
  PlotUtils::MnvH2D* h_unfolded = nullptr;

  //bool bUnfolded = false;

  TMatrixD dummyCovMatrix;
  //if(!unfold.UnfoldHisto2D( h_unfolded, dummyCovMatrix, h_migration, h_folded, RooUnfold::kBayes, num_iter, true, false ))
  //  return nullptr;

  /////////////////////////////////////////////////////////////////////////////////////////  
  //No idea if this is still needed
  //Probably.  This gets your stat unfolding covariance matrix
  TMatrixD unfoldingCovMatrixOrig;
  auto recodum = h_migration->ProjectionX();
  auto truthdum = h_migration->ProjectionY();
 
  int correctNbins;
  int matrixRows;  
  TH2D* hUnfoldedDummy  = new TH2D(h_unfolded->GetCVHistoWithStatError());
  TH2D* hRecoDummy      = new TH2D(recodum);//->GetCVHistoWithStatError());
  TH2D* hTruthDummy     = new TH2D(truthdum);//->GetCVHistoWithStatError());
  TH2D* hBGSubDataDummy = new TH2D(h_folded->GetCVHistoWithStatError());
  TH2D* hMigrationDummy = new TH2D(h_migration->GetCVHistoWithStatError());
  unfold.UnfoldHisto2D(hUnfoldedDummy, unfoldingCovMatrixOrig, hMigrationDummy, hRecoDummy, hTruthDummy, hBGSubDataDummy,RooUnfold::kBayes, num_iter);//Stupid RooUnfold.  This is dummy, we don't need iterations

  correctNbins=hUnfoldedDummy->fN;
  matrixRows=unfoldingCovMatrixOrig.GetNrows();
  if(correctNbins!=matrixRows){
    std::cout << "****************************************************************************" << std::endl;
    std::cout << "*  Fixing unfolding matrix size because of RooUnfold bug. From " << matrixRows << " to " << correctNbins << std::endl;
    std::cout << "****************************************************************************" << std::endl;
    // It looks like this, since the extra last two bins don't have any content
    unfoldingCovMatrixOrig.ResizeTo(correctNbins, correctNbins);
  }

  for(int i=0; i<unfoldingCovMatrixOrig.GetNrows(); ++i) unfoldingCovMatrixOrig(i,i)=0;
  delete hUnfoldedDummy;
  delete hMigrationDummy;
  delete hRecoDummy;
  delete hTruthDummy;
  delete hBGSubDataDummy;
  h_unfolded->PushCovMatrix("unfoldingCov",unfoldingCovMatrixOrig);

  /////////////////////////////////////////////////////////////////////////////////////////  
  return h_unfolded;
}
*/
//The final step of cross section extraction: normalize by flux, bin width, POT, and number of targets
PlotUtils::MnvH2D* normalize(PlotUtils::MnvH2D* efficiencyCorrected, PlotUtils::MnvH2D* fluxIntegral, const double nNucleons, const double POT)
{
  //double integral = (fluxIntegral->GetIntegral());
  efficiencyCorrected->Divide(efficiencyCorrected, fluxIntegral);
  //efficiencyCorrected->Scale(1./integral);//fluxIntegral->GetIntegral());
  efficiencyCorrected->Scale(1./nNucleons/POT);
  efficiencyCorrected->Scale(1.e4); //Flux histogram is in m^-2, but convention is to report cm^2
  efficiencyCorrected->Scale(1., "width");

  return efficiencyCorrected;
}

int main(const int argc, const char** argv)
{
  #ifndef NCINTEX
  ROOT::Cintex::Cintex::Enable(); //Needed to look up dictionaries for PlotUtils classes like MnvH1D
  #endif

  TH1::AddDirectory(kFALSE); //Needed so that MnvH1D gets to clean up its own MnvLatErrorBands (which are TH1Ds).

  if(argc != 4)
  {
    std::cerr << "Expected 3 arguments, but I got " << argc-1 << ".\n"
              << "USAGE: ExtractCrossSection <unfolding iterations> <data.root> <mc.root>\n";
    return 1;
  }

  const int nIterations = std::stoi(argv[1]);
  auto dataFile = TFile::Open(argv[2], "READ");
  if(!dataFile)
  {
    std::cerr << "Failed to open data file " << argv[2] << ".\n";
    return 2;
  }

  auto mcFile = TFile::Open(argv[3], "READ");
  if(!mcFile)
  {
    std::cerr << "Failed to open MC file " << argv[3] << ".\n";
    return 3;
  }

  std::vector<std::string> crossSectionPrefixes;
  std::vector<std::string> prefix1D;
  for(auto key: *dataFile->GetListOfKeys())
  {
    const std::string keyName = key->GetName();
    const size_t endOfPrefix = keyName.find("_data");
    const size_t check2d = keyName.find("_pTmubins_");
    const size_t endof1DPrefix = keyName.find("_data");
    if (keyName.substr(0, endOfPrefix) == check2d) continue;
    if(endOfPrefix == std::string::npos and check2d == std::string::npos) continue; 
    if(endOfPrefix != std::string::npos and check2d != std::string::npos) crossSectionPrefixes.push_back(keyName.substr(0, endOfPrefix));// Checks to make sure I am only picking up the 2D variables
    //if (keyName.substr(0, endOfPrefix) == check2D) continue;
    //std::cout << "Printing Prefix " << keyName.substr(0, endOfPrefix) << std::endl;
    if(endOfPrefix != std::string::npos) prefix1D.push_back(keyName.substr(0, endOfPrefix)); 
  }

  const double mcPOT = util::GetIngredient<TParameter<double>>(*mcFile, "POTUsed")->GetVal(),
               dataPOT = util::GetIngredient<TParameter<double>>(*dataFile, "POTUsed")->GetVal();

  for(const auto& prefix: crossSectionPrefixes)
  {
    try
    {
      std::cout << "Printing Prefix " << prefix << std::endl;
      std::string xfluxname; // = nullptr;
      std::string yfluxname = "pTmubins";
      for (const auto name: prefix1D)
      {
	if (prefix.find(name) != std::string::npos and prefix.find("pTmubins") != std::string::npos){
	   xfluxname = name;
	   break;
	}
      }
      auto flux = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "reweightedflux_integrated", prefix);
      //PlotUtils::MnvH2D* flux = new PlotUtils::MnvH2D(xflux, yflux); 
      auto folded = util::GetIngredient<PlotUtils::MnvH2D>(*dataFile, "data", prefix);
      Plot(*folded, "data", prefix);
      //auto mnvresp = util::GetIngredient<MinervaUnfold::MnvResponse>(*mcFile, "migration", prefix);
      
      PlotUtils::MnvH1D* hTruth_dummy     = nullptr;//new PlotUtils::MnvH2D("MnvRespTruth");
      PlotUtils::MnvH1D* hReco_dummy     = nullptr;//new PlotUtils::MnvH2D("MnvRespReco");
      PlotUtils::MnvH2D* hMigration_dummy     = nullptr;//new PlotUtils::MnvH1D("MnvRespMigration");    
      //mnvresp->GetMigrationObjects(hMigration_dummy, hReco_dummy, hTruth_dummy);
      auto migration = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "migration", prefix); //hMigration_dummy->Clone();
      hReco_dummy = migration->ProjectionX();
      hTruth_dummy = migration->ProjectionY();
      Plot(*hReco_dummy, "migration_reco", prefix);
      std::cout << "Printing Migration object" << migration << std::endl;
      auto effNum = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "efficiency_numerator", prefix);
      auto effDenom = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "efficiency_denominator", prefix);
      auto simEventRate = effDenom->Clone(); //Make a copy for later
      auto recosignal = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "signal_reco", prefix);
      auto purdenom = util::GetIngredient<PlotUtils::MnvH2D>(*dataFile, "data", prefix); 

      //folded->GetXaxis()->SetRangeUser(0.0, 800.);
      //migration->GetXaxis()->SetRangeUser(0.0, 800.);
      //migration->GetYaxis()->SetRangeUser(0.0, 800.);
      //effNum->GetXaxis()->SetRangeUser(0.0, 800.);
      //effDenom->GetXaxis()->SetRangeUser(0.0, 800.);
      //simEventRate->GetXaxis()->SetRangeUser(0.0, 800.);
      //recosignal->GetXaxis()->SetRangeUser(0.0, 800.);

      
      const auto fiducialFound = std::find_if(mcFile->GetListOfKeys()->begin(), mcFile->GetListOfKeys()->end(),
                                              [&prefix](const auto key)
                                              {
                                                const std::string keyName = key->GetName();
                                                const size_t fiducialEnd = keyName.find("_fiducial_nucleons");
                                                return (fiducialEnd != std::string::npos) && (prefix.find(keyName.substr(0, fiducialEnd)) != std::string::npos);
                                              });
      if(fiducialFound == mcFile->GetListOfKeys()->end()) throw std::runtime_error("Failed to find a number of nucleons that matches prefix " + prefix);

      auto nNucleons = util::GetIngredient<TParameter<double>>(*mcFile, (*fiducialFound)->GetName()); 

      //Look for backgrounds with <prefix>_<analysis>_Background_<name>
      std::vector<PlotUtils::MnvH2D*> backgrounds;
      for(auto key: *mcFile->GetListOfKeys())
      {
        if(std::string(key->GetName()).find(prefix + "_by_BKG") != std::string::npos)
        {
          backgrounds.push_back(util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, key->GetName()));
        }
      }

      //There are no error bands in the data, but I need somewhere to put error bands on the results I derive from it.
      folded->AddMissingErrorBandsAndFillWithCV(*migration);

      //Basing my unfolding procedure for a differential cross section on Alex's MINERvA 101 talk at https://minerva-docdb.fnal.gov/cgi-bin/private/RetrieveFile?docid=27438&filename=whatsACrossSection.pdf&version=1

      //TODO: Remove these debugging plots when done
      auto toSubtract = std::accumulate(std::next(backgrounds.begin()), backgrounds.end(), (*backgrounds.begin())->Clone(),
                                        [mcPOT, dataPOT](auto sum, const auto hist)
                                        {
					  hist->Scale(dataPOT/mcPOT);
                                          sum->Add(hist);
                                          return sum;
                                        });
      Plot(*toSubtract, "BackgroundSum", prefix);
      Plot(*flux, "Flux",prefix);
      //toSubtract->Write();
      auto bkgtoSubtract = toSubtract->GetBinNormalizedCopy();//.GetCVHistoWithError().Clone();
      bkgtoSubtract.GetXaxis()->SetTitle("Available Energy (MeV)");
      bkgtoSubtract.GetYaxis()->SetRangeUser(0.0, 17000.);
      bkgtoSubtract.GetYaxis()->SetTitle("N Background Events");
      Plot(bkgtoSubtract, "BackgroundSumNorm", prefix);
      auto bkgSubtracted = std::accumulate(backgrounds.begin(), backgrounds.end(), folded->Clone(),
                                           [mcPOT, dataPOT](auto sum, const auto hist)
                                           {
                                             std::cout << "Subtracting " << hist->GetName() << " scaled by " << -dataPOT/mcPOT << " from " << sum->GetName() << "\n";
                                             sum->Add(hist, -dataPOT/mcPOT);
                                             return sum;
                                           });
      Plot(*bkgSubtracted, "backgroundSubtracted", prefix);

      auto outFile = TFile::Open((prefix + "_crossSection.root").c_str(), "CREATE");
      if(!outFile)
      {
        std::cerr << "Could not create a file called " << prefix + "_crossSection.root" << ".  Does it already exist?\n";
        return 5;
      }

      bkgSubtracted->Write("backgroundSubtracted");

      //d'Aogstini unfolding
      MinervaUnfold::MnvUnfold unfold;
      auto unfolded = DoResponseUnfolding(prefix, migration, recosignal , effNum, bkgSubtracted, toSubtract, unfold, nIterations); 
      //auto unfolded = UnfoldHist(bkgSubtracted, migration, nIterations);
      if(!unfolded) throw std::runtime_error(std::string("Failed to unfold ") + folded->GetName() + " using " + migration->GetName());
      Plot(*unfolded, "unfolded", prefix);
      unfolded->Clone()->Write("unfolded"); //TODO: Seg fault first appears when I uncomment this line
      std::cout << "Survived writing the unfolded histogram.\n" << std::flush; //This is evidence that the problem is on the final file Write() and not unfolded->Clone()->Write().
      recosignal->Divide(recosignal, purdenom);
      Plot(*recosignal, "Purity", prefix);   
      recosignal->Clone()->Write("Purity");
      effNum->Divide(effNum, effDenom); //Only the 2 parameter version of MnvH1D::Divide()
                                        //handles systematics correctly.
      Plot(*effNum, "efficiency", prefix);

      unfolded->Divide(unfolded, effNum);
      Plot(*unfolded, "efficiencyCorrected", prefix);

      auto crossSection = normalize(unfolded, flux, nNucleons->GetVal(), dataPOT);
      Plot(*crossSection, "crossSection", prefix);
      crossSection->Clone()->Write("crossSection");

      //Write a "simulated cross section" to compare to the data I just extracted.
      //If this analysis passed its closure test, this should be the same cross section as
      //what GENIEXSecExtract would produce.
      normalize(simEventRate, flux, nNucleons->GetVal(), mcPOT);
      
      Plot(*simEventRate, "simulatedCrossSection", prefix);
      simEventRate->Write("simulatedCrossSection");
    }
    catch(const std::runtime_error& e)
    {
      std::cerr << "Failed to extra a cross section for prefix " << prefix << ": " << e.what() << "\n";
      return 4;
    }
  }

  return 0;
}
