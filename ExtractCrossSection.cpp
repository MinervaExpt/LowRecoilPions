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
#include "util/SafeROOTName.h"
//UnfoldUtils includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "MinervaUnfold/MnvUnfold.h"

//PlotUtils includes
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"
#include "PlotUtils/MnvPlotter.h"
#pragma GCC diagnostic pop

//ROOT includes
#include "TH1D.h"
#include "TFile.h"
#include "TKey.h"
#include "TParameter.h"
#include "TCanvas.h"

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

//Plot a step in cross section extraction.
void Plot(PlotUtils::MnvH1D& hist, const std::string& stepName, const std::string& prefix)
{
  TCanvas can(stepName.c_str());
  hist.GetCVHistoWithError().Clone()->Draw();
  can.Print((prefix + "_" + stepName + ".png").c_str());

  //Uncertainty summary
  PlotUtils::MnvPlotter plotter;
  plotter.ApplyStyle(PlotUtils::kCCNuPionIncStyle);
  plotter.axis_maximum = 0.4;


  plotter.error_summary_group_map.clear();
  plotter.error_summary_group_map["Flux"].push_back("Flux");

  plotter.error_summary_group_map["Muon"].push_back("Muon_Energy_MINOS");
  plotter.error_summary_group_map["Muon"].push_back("Muon_Energy_MINERvA");
  plotter.error_summary_group_map["Muon"].push_back(
      "Muon_Energy_Resolution");
  plotter.error_summary_group_map["Muon"].push_back(
      "MINOS_Reconstruction_Efficiency");
  plotter.error_summary_group_map["Muon"].push_back("MuonAngleXResolution");

  plotter.error_summary_group_map["Muon"].push_back("MuonResolution");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "MichelEfficiency");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "Target_Mass_CH");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "Target_Mass_C");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "Target_Mass_Fe");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "Target_Mass_H2O");
  plotter.error_summary_group_map["PhysicsModel"].push_back(
      "Target_Mass_Pb");
  plotter.error_summary_group_map["Diffractive"].push_back("DiffractiveModelUnc");
  plotter.error_summary_group_map["Diffractive"].push_back("CoherentPiUnc_C");
  plotter.error_summary_group_map["Diffractive"].push_back("CoherentPiUnc_CH");
  plotter.error_summary_group_map["Diffractive"].push_back("CoherentPiUnc_Fe");
  plotter.error_summary_group_map["Diffractive"].push_back("CoherentPiUnc_H2O");
  plotter.error_summary_group_map["Diffractive"].push_back("CoherentPiUnc_Pb");
  plotter.error_summary_group_map["Detector"].push_back("EmuRangeCurve");
  plotter.error_summary_group_map["Detector"].push_back("Birks");
  plotter.error_summary_group_map["Detector"].push_back("BetheBloch");
  plotter.error_summary_group_map["Detector"].push_back("Mass");
  plotter.error_summary_group_map["Detector"].push_back("PartResp");
  plotter.error_summary_group_map["Detector"].push_back("TrackAngle");
  plotter.error_summary_group_map["Detector"].push_back("BeamAngle");
  plotter.error_summary_group_map["Detector"].push_back("NodeCutEff");
  plotter.error_summary_group_map["Detector"].push_back("BeamAngleX");
  plotter.error_summary_group_map["Detector"].push_back("BeamAngleY");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrAbs_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrAbs_pi");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrCEx_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrCEx_pi");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrElas_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrElas_pi");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrInel_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrInel_pi");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrPiProd_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_FrPiProd_pi");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_MFP_N");
  plotter.error_summary_group_map["FSI_Model"].push_back("GENIE_MFP_pi");

  auto genieint = util::SafeROOTName("Genie Others");
  plotter.error_summary_group_map[genieint].push_back("GENIE_AGKYxF1pi");
  plotter.error_summary_group_map[genieint].push_back("GENIE_AhtBY");
  plotter.error_summary_group_map[genieint].push_back("GENIE_BhtBY");
  plotter.error_summary_group_map[genieint].push_back("GENIE_CV1uBY");
  plotter.error_summary_group_map[genieint].push_back("GENIE_CV2uBY");
  plotter.error_summary_group_map[genieint].push_back("GENIE_EtaNCEL");
  plotter.error_summary_group_map[genieint].push_back("GENIE_NormDISCC");
  plotter.error_summary_group_map[genieint].push_back("GENIE_MaNCEL");
  plotter.error_summary_group_map[genieint].push_back("GENIE_RDecBR1gamma");
  auto genieint2 = util::SafeROOTName("Genie CCQE");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_MaCCQEshape");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_MaNCEL");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_NormCCQE");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_NormDISCC");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_VecFFCCQEshape");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_MaCCQE");
  plotter.error_summary_group_map[genieint2].push_back("GENIE_CCQEPauliSupViaKF");
  auto genieint3 = util::SafeROOTName("Genie RES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_MvRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_MaRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_D2_MaRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_D2_NormCCRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_EP_MvRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_NormCCRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_NormNCRES");
  auto genieint4 = util::SafeROOTName("Genie Pion Interaction Model");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Rvn1pi");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Rvn2pi");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Rvn3pi");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Rvp1pi");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Rvp2pi");
  plotter.error_summary_group_map[genieint4].push_back("GENIE_Theta_Delta2Npi");


  plotter.error_summary_group_map["Tune"].push_back("RPA_LowQ2");
  plotter.error_summary_group_map["Tune"].push_back("RPA_HighQ2");
  plotter.error_summary_group_map["Tune"].push_back("NonResPi");
  plotter.error_summary_group_map["Tune"].push_back("2p2h");
  plotter.error_summary_group_map["Tune"].push_back("LowQ2Pi");
  plotter.error_summary_group_map["Tune"].push_back("Low_Recoil_2p2h_Tune");

  plotter.error_summary_group_map["Response"].push_back("response_em");
  plotter.error_summary_group_map["Response"].push_back("response_proton");
  plotter.error_summary_group_map["Response"].push_back("response_pion");
  plotter.error_summary_group_map["Response"].push_back("response_meson");
  plotter.error_summary_group_map["Response"].push_back("response_other");
  plotter.error_summary_group_map["Response"].push_back("response_low_neutron");
  plotter.error_summary_group_map["Response"].push_back("response_mid_neutron");
  plotter.error_summary_group_map["Response"].push_back("response_high_neutron");

  plotter.error_summary_group_map["Geant"].push_back("GEANT_Neutron");
  plotter.error_summary_group_map["Geant"].push_back("GEANT_Proton");
  plotter.error_summary_group_map["Geant"].push_back("GEANT_Pion");

  plotter.error_color_map["Geant"] = kViolet+2;
  plotter.error_color_map["Response"] =  kOrange;
  plotter.error_color_map["Detector"] = kYellow+2;
  plotter.error_color_map["Flux"] = kOrange+2;
  plotter.error_color_map["Muon"] = kRed;
  plotter.error_color_map[genieint] = kCyan+2;
  plotter.error_color_map["FSI Model"] = kPink+2;
  plotter.error_color_map["PhysicsModel"] = kTeal+2;
  plotter.error_color_map["NonResPi"] =  kAzure+2;
  plotter.error_color_map["Tune"] =  kRed+2;
  plotter.error_color_map[genieint2] = kYellow;
  plotter.error_color_map[genieint3] = kViolet-1;
  plotter.error_color_map[genieint4] = kMagenta-1;
  std::vector<std::string> groupnames = {"Tune","Geant", "Response", "Detector", "Flux", "Muon", genieint, genieint2, genieint3, genieint4, "FSI Model", "Diffractive", "PhysicsModel"};

  plotter.DrawErrorSummary(&hist);
  can.Print((prefix + "_" + stepName + "_uncertaintySummary.png").c_str());

  plotter.DrawErrorSummary(&hist, "TR", true, true, 1e-5, false, "Other");
  can.Print((prefix + "_" + stepName + "_otherUncertainties.png").c_str());

  for(int j = 0; j<groupnames.size(); j++){
            auto groupname = util::SafeROOTName(groupnames[j]);
            std::cout << "Name of group is " << groupname << std::endl;
            plotter.SetLegendNColumns(1);
            plotter.DrawErrorSummary(&hist, "TR", true, true, 1e-5, false, groupname, true);
            can.Draw("c");
            can.Print((prefix + "_" + stepName + "_uncertaintysummary_" + groupname + ".png").c_str());
  }

  auto hist2 = hist;
  hist2.Add(&hist, -1);
  plotter.DrawErrorSummary(&hist2,"TR", true, true, 1e-5, false, "", true);
  can.Print((prefix + "_" + stepName + "_uncertaintySummary_JUSTLEGEND.png").c_str());

  /*
  const std::string systname("Cross Section Models");
  plotter.DrawErrorSummary(&hist, "TR", true, true, 0.00001, false, systname);
  can.Print((prefix + "_" + stepName + "_CrossSectionModels.png").c_str());
  
  const std::string fsiname("FSI Models");
  plotter.DrawErrorSummary(&hist, "TR", true, true, 0.00001, false, fsiname);
  can.Print((prefix + "_" + stepName + "_FSIModels.png").c_str());
  */


}

//Unfolding function from Aaron Bercelle
//TODO: Trim it down a little?  Remove that static?
PlotUtils::MnvH1D* UnfoldHist( PlotUtils::MnvH1D* h_folded, PlotUtils::MnvH2D* h_migration, int num_iter )
{
  static MinervaUnfold::MnvUnfold unfold;
  unfold.setUseBetterStatErrorCalc(true);

  PlotUtils::MnvH1D* h_unfolded = nullptr;

  //bool bUnfolded = false;

  TMatrixD dummyCovMatrix;
  if(!unfold.UnfoldHisto( h_unfolded, dummyCovMatrix, h_migration, h_folded, RooUnfold::kBayes, num_iter, true, true ))
    return nullptr;

  //unfold.Unfold(h_unfolded, h_migration, h_folded, RooUnfold::kBayes, num_iter);
  /////////////////////////////////////////////////////////////////////////////////////////  
  //No idea if this is still needed
  //Probably.  This gets your stat unfolding covariance matrix
  TMatrixD unfoldingCovMatrixOrig; 
  int correctNbins;
  int matrixRows;  
  TH1D* hUnfoldedDummy  = new TH1D(h_unfolded->GetCVHistoWithStatError());
  TH1D* hRecoDummy      = new TH1D(h_migration->ProjectionX()->GetCVHistoWithStatError());
  TH1D* hTruthDummy     = new TH1D(h_migration->ProjectionY()->GetCVHistoWithStatError());
  TH1D* hBGSubDataDummy = new TH1D(h_folded->GetCVHistoWithStatError());
  TH2D* hMigrationDummy = new TH2D(h_migration->GetCVHistoWithStatError());
   
  //hUnfoldedDummy->ClearUnderflowAndOverflow(); 
  //hRecoDummy->ClearUnderflowAndOverflow();
  //hTruthDummy->ClearUnderflowAndOverflow();
  //hBGSubDataDummy->ClearUnderflowAndOverflow();
  //hMigrationDummy->ClearUnderflowAndOverflow(); // Adding this line to see if maybe unfolding doesn't go crazy in overflow bins. 
  
  unfold.UnfoldHisto(hUnfoldedDummy, unfoldingCovMatrixOrig, hMigrationDummy, hRecoDummy, hTruthDummy, hBGSubDataDummy,RooUnfold::kBayes, num_iter);//Stupid RooUnfold.  This is dummy, we don't need iterations
  
  
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

//The final step of cross section extraction: normalize by flux, bin width, POT, and number of targets
PlotUtils::MnvH1D* normalize(PlotUtils::MnvH1D* efficiencyCorrected, PlotUtils::MnvH1D* fluxIntegral, const double nNucleons, const double POT)
{
  efficiencyCorrected->Divide(efficiencyCorrected, fluxIntegral);

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
  /*
  if(argc != 4)
  {
    std::cerr << "Expected 3 arguments, but I got " << argc-1 << ".\n"
              << "USAGE: ExtractCrossSection <unfolding iterations> <data.root> <mc.root>\n";
    return 1;
  }
  */
  const int nIterations = std::stoi(argv[1]);
  double totmcPOT = 0.0;
  double totalPOT = 0.0;
  auto dataFile = TFile::Open(argv[2], "READ");
  auto mcFile = TFile::Open(argv[3], "READ");
  auto potFile = TFile::Open(argv[4], "READ");
  if (argc >= 4){
   //const int nIterations = std::stoi(argv[1]);
   //auto dataFile = TFile::Open(argv[2], "READ");
   if(!dataFile)
   {
    std::cerr << "Failed to open data file " << argv[2] << ".\n";
    return 2;
   }

   //auto mcFile = TFile::Open(argv[3], "READ");
   if(!mcFile)
   {
    std::cerr << "Failed to open MC file " << argv[3] << ".\n";
    return 3;
   }

   if (argc == 5)
   {
      //auto potFile = TFile::Open(argv[4], "READ");
      if(!potFile)
      {
        std::cerr << "Failed to open total POT file " << argv[3] << ".\n";
        return 3;
      }
      totalPOT =  util::GetIngredient<TParameter<double>>(*potFile, "dataPOTUsed")->GetVal();
      totmcPOT =  util::GetIngredient<TParameter<double>>(*potFile, "mcPOTUsed")->GetVal();
    }   

  }

  std::vector<std::string> crossSectionPrefixes;
  for(auto key: *dataFile->GetListOfKeys())
  {
    const std::string keyName = key->GetName();
    const size_t endOfPrefix = keyName.find("_data");
    if(endOfPrefix != std::string::npos) crossSectionPrefixes.push_back(keyName.substr(0, endOfPrefix));
  }

  const double mcPOT = util::GetIngredient<TParameter<double>>(*mcFile, "POTUsed")->GetVal(),
               dataPOT = util::GetIngredient<TParameter<double>>(*dataFile, "POTUsed")->GetVal();

  for(const auto& prefix: crossSectionPrefixes)
  {
    try
    {
      auto flux = util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "reweightedflux_integrated", prefix);
      auto folded = util::GetIngredient<PlotUtils::MnvH1D>(*dataFile, "data", prefix);
      Plot(*folded, "data", prefix);
      auto migration = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "migration", prefix);
      auto effNum = util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "efficiency_numerator", prefix);
      auto effDenom = util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "efficiency_denominator", prefix);
      auto simEventRate = effDenom->Clone(); //Make a copy for later
      auto recosignal = util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "selected_signal_reco", prefix);
      auto truesignal = util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "efficiency_numerator", prefix);
      auto purdenom = util::GetIngredient<PlotUtils::MnvH1D>(*dataFile, "data", prefix); 

      //folded->GetXaxis()->SetRangeUser(0.0, 1400.);
      //migration->GetXaxis()->SetRangeUser(0.0,1400.);
      //migration->GetYaxis()->


      auto purnum = recosignal->Clone(); 
      //Plot(*folded, "data", prefix);
      auto purden =  util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, "data", prefix); //purdenom->Clone();
      const auto fiducialFound = std::find_if(mcFile->GetListOfKeys()->begin(), mcFile->GetListOfKeys()->end(),
                                              [&prefix](const auto key)
                                              {
                                                const std::string keyName = key->GetName();
                                                const size_t fiducialEnd = keyName.find("_fiducial_nucleons");
                                                return (fiducialEnd != std::string::npos) && (prefix.find(keyName.substr(0, fiducialEnd)) != std::string::npos);
                                              });
      if(fiducialFound == mcFile->GetListOfKeys()->end()) throw std::runtime_error("Failed to find a number of nucleons that matches prefix " + prefix);

      auto nNucleons = util::GetIngredient<TParameter<double>>(*mcFile, (*fiducialFound)->GetName()); //Dan: Use the same truth fiducial volume for all extractions.  The acceptance correction corrects data back to this fiducial even if the reco fiducial cut is different.

      //Look for backgrounds with <prefix>_<analysis>_Background_<name>
      std::vector<PlotUtils::MnvH1D*> backgrounds;
      for(auto key: *mcFile->GetListOfKeys())
      {
        if(std::string(key->GetName()).find(prefix + "_background_") != std::string::npos)
        {
          backgrounds.push_back(util::GetIngredient<PlotUtils::MnvH1D>(*mcFile, key->GetName()));
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
      auto bkgtoSubtract = toSubtract->GetBinNormalizedCopy(); //GetCVHistoWithError().Clone();
      //bkgtoSubtract.GetXaxis()->SetTitle(prefix);
      bkgtoSubtract.Scale(dataPOT/mcPOT);
      //bkgtoSubtract.GetYaxis()->SetRangeUser(0.0, 3500.);
      bkgtoSubtract.GetYaxis()->SetTitle("N Background Events");
      Plot(bkgtoSubtract, "BackgroundSumNorm", prefix);
      auto bkgSubtracted = std::accumulate(backgrounds.begin(), backgrounds.end(), folded->Clone(),
                                           [mcPOT, dataPOT](auto sum, const auto hist)
                                           {
                                             std::cout << "Subtracting " << hist->GetName() << " scaled by " << -dataPOT/mcPOT << " from " << sum->GetName() << "\n";
                                             sum->Add(hist, -dataPOT/mcPOT);
					     //sum->Add(hist, -1); // To Scale to POT: -dataPOT/mcPOT);
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
      auto unfolded = UnfoldHist(bkgSubtracted, migration, nIterations);
      if(!unfolded) throw std::runtime_error(std::string("Failed to unfold ") + folded->GetName() + " using " + migration->GetName());
      Plot(*unfolded, "unfolded", prefix);
      unfolded->Clone()->Write("unfolded"); //TODO: Seg fault first appears when I uncomment this line
      std::cout << "Survived writing the unfolded histogram.\n" << std::flush; //This is evidence that the problem is on the final file Write() and not unfolded->Clone()->Write().
      //purnum->Scale(dataPOT/mcPOT);
      purnum->Divide(purnum, purden);
      recosignal->Scale(dataPOT/mcPOT);
      Plot(*recosignal, "Signalreco", prefix);   
      recosignal->Clone()->Write("Signalreco");
      truesignal->Scale(dataPOT/mcPOT);
      Plot(*truesignal,"Signaltrue", prefix);
      truesignal->Clone()->Write("trueSignal");
      Plot(*purnum, "Purity", prefix);
      purnum->Clone()->Write("Purity");
      

      effNum->Divide(effNum, effDenom,1,1, "B"); //Only the 2 parameter version of MnvH1D::Divide()
                                        //handles systematics correctly.
      Plot(*effNum, "efficiency", prefix);
      effNum->Clone()->Write("Efficiency");
      unfolded->Divide(unfolded, effNum);
      Plot(*unfolded, "efficiencyCorrected", prefix);
      unfolded->Clone()->Write("efficiencyCorrected");
      auto crossSection = normalize(unfolded, flux, nNucleons->GetVal(), dataPOT); //totalPOT); //dataPOT);
      Plot(*crossSection, "crossSection", prefix);
      crossSection->Clone()->Write("crossSection");
      auto effnume = simEventRate->Clone();
      effnume->Scale(dataPOT/mcPOT);
      effnume->Clone()->Write("SimEventRate");
      //Write a "simulated cross section" to compare to the data I just extracted.
      //If this analysis passed its closure test, this should be the same cross section as
      //what GENIEXSecExtract would produce.
      normalize(simEventRate, flux, nNucleons->GetVal(), mcPOT);//totalPOT); //mcPOT
      
      Plot(*simEventRate, "simulatedCrossSection", prefix);
      simEventRate->Write("simulatedCrossSection");
    }
    catch(const std::runtime_error& e)
    {
      std::cerr << "Failed to extract cross section for prefix " << prefix << ": " << e.what() << "\n";
      return 4;
    }
  }

  return 0;
}
