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
#include "util/SyncBands.h"
#include "util/SafeROOTName.h"

//UnfoldUtils includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include "MinervaUnfold/MnvUnfold.h"

//PlotUtils includes
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"
#include "PlotUtils/MnvPlotter.h"
#include "PlotUtils/MnvVertErrorBand.h"
#include "PlotUtils/TargetUtils.h"
#include "PlotUtils/FluxReweighter.h"


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
#include "TColor.h"
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

//PlotStyleIncludes
//#include "myPlotStyle.h"

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

  PlotUtils::MnvPlotter plotter;
  //myPlotStyle();
  plotter.draw_normalized_to_bin_width = false;
  TCanvas can1(stepName.c_str());
  hist.GetCVHistoWithError().Clone()->Draw();
  can1.Print((prefix + "_" + stepName + ".png").c_str());

  //Uncertainty summary
  //plotter.ApplyStyle(PlotUtils::kCCNuPionIncStyle);
  plotter.axis_maximum = 0.5;
  plotter.error_color_map.clear(); 
  plotter.error_summary_group_map.clear();
  plotter.error_summary_group_map["Flux"].push_back("Flux");
  //plotter.error_summary_group_map["NonResPi"].push_back("GENIE_Rvn1pi");
  //plotter.error_summary_group_map["NonResPi"].push_back("GENIE_Rvp1pi");
  //plotter.error_summary_group_map["NonResPi"].push_back("GENIE_Rvn2pi");
  //plotter.error_summary_group_map["NonResPi"].push_back("GENIE_Rvp2pi");
  //plotter.error_summary_group_map["2p2h"].push_back("Low_Recoil_2p2h_Tune");
  //plotter.error_summary_group_map["LowQ2Pi"].push_back("LowQ2Pi");
  plotter.error_summary_group_map["Muon"].push_back("Muon_Energy_MINOS");
  plotter.error_summary_group_map["Muon"].push_back("Muon_Energy_MINERvA");
  plotter.error_summary_group_map["Muon"].push_back(
      "Muon_Energy_Resolution");
  plotter.error_summary_group_map["Muon"].push_back(
      "MINOS_Reconstruction_Efficiency");
  plotter.error_summary_group_map["Muon"].push_back("MuonAngleXResolution");
  //plotter.error_summary_group_map["Muon"].push_back("MuonAngleYResolution");
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

  //plotter.error_summary_group_map["RPA"].push_back("RPA_LowQ2");
  //plotter.error_summary_group_map["RPA"].push_back("RPA_HighQ2");
  
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
  //plotter.error_summary_group_map[genieint3].push_back("GENIE_D2_MaRES");
  //plotter.error_summary_group_map[genieint3].push_back("GENIE_D2_NormCCRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_EP_MvRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_NormCCRES");
  plotter.error_summary_group_map[genieint3].push_back("GENIE_NormNCRES");
  auto genieint4 = util::SafeROOTName("Genie Pion Model");  
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
  
  //std::vector<std::string> groupnames = {"Flux", "Recoil Reconstruction", "Cross Section Models", "FSI Models", "Muon Reconstruction", "Others", "Low Recoil Fits"};
  std::vector<std::string> hexcolors = {   "#E5F5F9" , "#1D91C0" , "#CB181D" , "#78C679" , "#F46D43" , "#A6CEE3" , "#FD8D3C", "#D4B9DA" , "#7F0000" , "#964f8e" ,"#F6C700" , "#C7EAE5" , "#0067a7" , "#F16913"  ,  "#FC9272" , "#AE017E" , "#F7F7F7" , "#DF65B0" , "#EF3B2C" , "#74C476"};
				//	{"#004949","#009292","#ff6db6","#ffb6db",
 				//	"#490092","#006ddb","#b66dff","#6db6ff","#b6dbff",
 				//	"#920000","#924900","#db6d00","#EF3B2C","#ffff6d"}; 
  plotter.error_color_map["Geant"] = TColor::GetColor((hexcolors[0]).c_str()); //kViolet+2;
  plotter.error_color_map["Response"] = TColor::GetColor((hexcolors[1]).c_str()); //kOrange;
  plotter.error_color_map["Detector"] = TColor::GetColor((hexcolors[2]).c_str());////kGreen+3;
  plotter.error_color_map["Flux"] = TColor::GetColor((hexcolors[3]).c_str()); //kOrange+2;
  plotter.error_color_map["Muon"] = TColor::GetColor((hexcolors[4]).c_str());// kRed;
  plotter.error_color_map[genieint] = TColor::GetColor((hexcolors[5]).c_str());//kCyan+2;
  plotter.error_color_map["FSI Model"] = TColor::GetColor((hexcolors[6]).c_str());//kPink+2;
  plotter.error_color_map["PhysicsModel"] = TColor::GetColor((hexcolors[7]).c_str());//kTeal+2;
  plotter.error_color_map["NonResPi"] = TColor::GetColor((hexcolors[8]).c_str());// kAzure+2; 
  plotter.error_color_map["Tune"] =  TColor::GetColor((hexcolors[9]).c_str());//kRed+2;
  plotter.error_color_map[genieint2] = TColor::GetColor((hexcolors[10]).c_str());// kYellow;
  plotter.error_color_map[genieint3] = TColor::GetColor((hexcolors[11]).c_str());//kViolet-1; 
  plotter.error_color_map[genieint4] = TColor::GetColor((hexcolors[12]).c_str());//kMagenta-1;
  std::vector<std::string> groupnames = {"Tune","Geant", "Response", "Detector", "Flux", "Muon", genieint, genieint2, genieint3, genieint4, "FSI Model", "Diffractive", "PhysicsModel"};  //#include "util/SafeROOTName.h"
  int nbinsy = 9;//hist.GetNbinsY()+2;
  for (int i = 1; i < nbinsy; i++)
  {
        //auto plothist = hist.ProjectionX("e",i,i);
        //plothist->Draw();
	//plotter.DrawMCWithErrorBand(hist.ProjectionX("e",i,i));
        //can.Draw("c");
	//can.Print((prefix + "_bin" + std::to_string(i) +  "_" + stepName + ".png").c_str());
        //plothist->Write((prefix + "_bin" + std::to_string(i) +  "_" + stepName).c_str());
	//can.Print((prefix + "_bin" + std::to_string(i) +  "_" + stepName + ".root").c_str());  
	std::string bini = "bin"+std::to_string(i);    
        auto can = new TCanvas(bini.c_str(),"error summary",500,500);
	//auto pad = new TPad("pad","",0,0,1,1);
	can->cd();
	auto pad = new TPad("pad","",0,0,1,1);
        pad->Draw();
        if (prefix == "Tpi_pTmubins") {can->SetLogx(1);}
        pad->Update();
        pad->cd();	
        
        plotter.SetLegendNColumns(1);
	plotter.height_nspaces_per_hist = 1.2;
	plotter.width_xspace_per_letter = .4;
	plotter.legend_text_size        = .03;
	plotter.draw_normalized_to_bin_width = true;
	plotter.extra_top_margin = -.035; 
	//plotter.legend_text_size = 0.02;
	//double x1,y1,x2,y2;
	//plotter.DecodeLegendPosition(x1,y1, x2,y2,"TR",groupnames.size(),14,0.02);
	//TLegend *leg = new TLegend(x1, y1, x2, y2);

        //plotter.height_nspaces_per_hist = 1.0;
        //plotter.legend_fill_color = -1;
        //plotter.legend_offset_x= 1.2; 
          
        auto histproj =  hist.ProjectionX("e",i,i);

        plotter.DrawErrorSummary(histproj, "TR", true, true, 1e-5, false, "", true, "", true);
        //can->Draw("c");
        can->Print((prefix + "_bin" + std::to_string(i) +"_" + stepName + "_uncertaintySummary.png").c_str());
	
	plotter.DrawErrorSummary(histproj, "TR", true, true, 1e-5, false, "Others", true, "", true);
        //can->Draw("c");
	can->Print((prefix + "_bin" + std::to_string(i) + "_" + stepName + "_otherUncertainties.png").c_str());
        
        for(int j = 0; j<groupnames.size(); j++){
	    auto groupname = util::SafeROOTName(groupnames[j]);
  	     
	    std::cout << "Name of group is " << groupname << std::endl;
	    plotter.SetLegendNColumns(1);        
            plotter.DrawErrorSummary(histproj, "TR", true, true, 1e-5, false, groupname, true);
            can->Draw("c");
            can->Print((prefix + "_bin" + std::to_string(i) + "_" + stepName + "_uncertaintysummary_" + groupname + ".png").c_str());
        }
        if(i == 9){
	  plotter.axis_maximum = 100.;
          plotter.axis_maximum_group = 100.;
          plotter.headroom = 1.;
	  
          plotter.DrawErrorSummary(histproj,  "TR", true, true, 1e-5, false, "", true, "", true);
          can->Draw("c");
                 
          can->Print((prefix + "_bin" + std::to_string(i) +"_" + stepName + "_JUSTLEGEND.root").c_str());
	}
  }
  //plotter.DrawErrorSummary(&hist);
  //can.Print((prefix + "_" + stepName + "_uncertaintySummary.png").c_str());

  //plotter.DrawErrorSummary(&hist, "TR", true, true, 1e-5, false, "Other");
  //can.Print((prefix + "_" + stepName + "_otherUncertainties.png").c_str());
}

void Plot(PlotUtils::MnvH1D& hist, const std::string& stepName, const std::string& prefix)
{
  //myPlotStyle(); 
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
 
  util::SyncBands(migration); 
  //migration->PopVertErrorBand("cv");

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
  util::SyncBands(unsmeared);
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
PlotUtils::MnvH2D* normalize(PlotUtils::MnvH2D* efficiencyCorrected, PlotUtils::MnvH2D* fluxIntegral, const double  nNucleons, const double POT)
{
  //fluxIntegral->Scale(1./POT);
  efficiencyCorrected->Divide(efficiencyCorrected, fluxIntegral);
  //efficiencyCorrected->Divide(efficiencyCorrected, nNucleons);
  //fluxIntegral->Scale(1./POT);
  efficiencyCorrected->Scale(1./nNucleons/POT);///POT);
  //efficiencyCorrected->Scale(1.e4); //Flux histogram is in m^-2, but convention is to report cm^2
  efficiencyCorrected->Scale(1., "width");

  return efficiencyCorrected;
}

PlotUtils::MnvH2D* getFluxNormalized(const PlotUtils::MnvH2D* effCorrData)
{
   PlotUtils::MnvH2D* effcorrData = effCorrData->Clone();
   PlotUtils::MnvH2D* h_flux_normalization = 
          (PlotUtils::MnvH2D*)effcorrData->Clone("flux_normalization");
   h_flux_normalization->ClearAllErrorBands();
   h_flux_normalization->Reset();
   const bool use_hundred_universes = true;
   static PlotUtils::FluxReweighter* frw = 
   new PlotUtils::FluxReweighter( 14, true, "minervame1d1m1nweightedave",
                                  PlotUtils::FluxReweighter::gen2thin, 
                                  PlotUtils::FluxReweighter::g4numiv6,
                                  100);

   h_flux_normalization = 
          frw->GetIntegratedFluxReweighted(14, effcorrData, 0., 100.);
    // remove redundant error bands
   h_flux_normalization->PopVertErrorBand("Flux_BeamFocus");
   h_flux_normalization->PopVertErrorBand("ppfx1_Total"); 
    // Convert flux units from nu/m^2/POT to nu/cm^2/POT
   h_flux_normalization->Scale( 1.0e-4 );
   return h_flux_normalization;
}

PlotUtils::MnvH2D* expandBinning(const PlotUtils::MnvH2D* toExpand, const PlotUtils::MnvH2D* toMatch)
{
  assert(toExpand->GetXaxis()->GetNbins() == 1 && "expandBinning() only works with histograms that have exactly 1 bin");
  auto result = toMatch->Clone((std::string(toExpand->GetName()) + "_expanded").c_str());
  result->Clear("ICEM"); //Keep only the binning
   //Put the same content of toExpand's single bin in each bin of the CV...
  const int nBinsx = result->GetNbinsX();
  const int nBinsy = result->GetNbinsY();
  for(int whichBin = 0; whichBin <= nBinsx; ++whichBin) 
     {
       for(int whichBiny = 0; whichBiny <= nBinsy; ++whichBiny) result->SetBinContent(whichBin, whichBiny, toExpand->GetBinContent(1));
    
     }

  //...and in each bin of each universe
  const auto bandNames = result->GetVertErrorBandNames();
  for(const auto& bandName: bandNames)
  {
    auto band = result->GetVertErrorBand(bandName);
    for(size_t whichUniv = 0; whichUniv < band->GetNHists(); ++whichUniv)
    {
      auto hist = band->GetHist(whichUniv);
      const double expandedContent = toExpand->GetVertErrorBand(bandName)->GetHist(whichUniv)->GetBinContent(1);
      for(int whichBin = 0; whichBin <= nBinsx; ++whichBin) 
      {
        for(int whichBiny = 0; whichBiny <= nBinsy; ++whichBiny)
          { 
             hist->SetBinContent(whichBin, whichBiny, expandedContent);
	  }
      }
    }
  }

  return result;
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
  
  auto potFile = TFile::Open(argv[4], "READ");
  if(!potFile)
  {
    std::cerr << "Failed to open MC file " << argv[4] << ".\n";
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
               dataPOT = util::GetIngredient<TParameter<double>>(*dataFile, "POTUsed")->GetVal(),
	       totmcPOT = util::GetIngredient<TParameter<double>>(*potFile, "mcPOTUsed")->GetVal(),
	       totalPOT = util::GetIngredient<TParameter<double>>(*potFile, "dataPOTUsed")->GetVal();

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
      //auto flux = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "reweightedflux_integrated", prefix);
      //PlotUtils::MnvH2D* flux = new PlotUtils::MnvH2D(xflux, yflux); 
      auto folded = util::GetIngredient<PlotUtils::MnvH2D>(*dataFile, "data", prefix);
      //Plot(*folded, "data", prefix);
      //auto mnvresp = util::GetIngredient<MinervaUnfold::MnvResponse>(*mcFile, "migration", prefix);
      auto mcfolded = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "MC", prefix); 
      Plot(*mcfolded, "MCselection", prefix);
      
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
      auto purdenom = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "MC", prefix); 

      auto npip = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_Npip_Only", prefix);
      auto onepi = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_1pip_wo_p_or_n", prefix);
      auto onepin = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_1pip_w_LNeutron", prefix);
      auto onepip = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_1pip_w_LProton", prefix); 	
      auto npires = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_NPi_and_Mesons_RES", prefix); 
      auto npidis = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_NPi_and_Mesons_DIS", prefix);
      auto cohpi = util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, "denom_COH_Pions", prefix);
      std::cout << "Check folded: " << folded->Integral() << std::endl;
      std::cout << "Check selSig: " << recosignal->Integral() << std::endl;
      std::cout << "Check migration: " << migration->Integral() << std::endl;
      std::cout << "Check effNum: " << effNum->Integral() << std::endl;
      std::cout << "Check effDenom: " << effDenom->Integral() << std::endl;
      std::cout << "Check simEventRate: " << simEventRate->Integral() << std::endl;

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
                                                const size_t fiducialEnd = keyName.find("_pTmubins_fiducial_nucleons");
                                                return (fiducialEnd != std::string::npos) && (prefix.find(keyName.substr(0, fiducialEnd)) != std::string::npos);
                                              });
      if(fiducialFound == mcFile->GetListOfKeys()->end()) throw std::runtime_error("Failed to find a number of nucleons that matches prefix " + prefix);

      //const auto nNucleons = expandBinning(util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, (*fiducialFound)->GetName()), effNum); //Dan: Use the same truth fiducial volume for all extractions.  The acceptance correction corrects data back to this fiducial even if the reco fiducial cut is different.
      
      
      //auto nNucleons = util::GetIngredient<TParameter<double>>(*potFile, (*fiducialFound)->GetName());
      auto nNucleons = util::GetIngredient<TParameter<double>>(*mcFile,  (*fiducialFound)->GetName() );

      //Look for backgrounds with <prefix>_<analysis>_Background_<name>
      std::vector<PlotUtils::MnvH2D*> backgrounds;
      for(auto key: *mcFile->GetListOfKeys())
      {

        
        if(std::string(key->GetName()).find(prefix + "_by_BKG") != std::string::npos)
        {
	  std::string name = key->GetName();
 	  if (prefix.at(0) ==  name.at(0)){
          	std::cout << "bkg found " << key->GetName() << std::endl;
	  	backgrounds.push_back(util::GetIngredient<PlotUtils::MnvH2D>(*mcFile, key->GetName()));
          }
	}
      }

      //There are no error bands in the data, but I need somewhere to put error bands on the results I derive from it.
      folded->AddMissingErrorBandsAndFillWithCV(*migration);

      //Basing my unfolding procedure for a differential cross section on Alex's MINERvA 101 talk at https://minerva-docdb.fnal.gov/cgi-bin/private/RetrieveFile?docid=27438&filename=whatsACrossSection.pdf&version=1

      //TODO: Remove these debugging plots when done
      auto toSubtract = std::accumulate(std::next(backgrounds.begin()), backgrounds.end(), (*backgrounds.begin())->Clone(),
                                        [mcPOT, dataPOT](auto sum, const auto hist)
                                        {
				
					  //hist->Scale(dataPOT/mcPOT);
                                          sum->Add(hist);
                                          return sum;
                                        });
      Plot(*toSubtract, "BackgroundSum", prefix);
      mcfolded->Add(toSubtract, -1);
      Plot(*mcfolded, "mcFolded_bkgsubtracted", prefix);  
      //Plot(*flux, "Flux",prefix);
      //toSubtract->Write();
      auto bkgtoSubtract = toSubtract->GetBinNormalizedCopy();//.GetCVHistoWithError().Clone();
      bkgtoSubtract.GetXaxis()->SetTitle("Available Energy (MeV)");
      bkgtoSubtract.GetYaxis()->SetRangeUser(0.0, toSubtract->GetMaximum()*1.5);
      bkgtoSubtract.GetYaxis()->SetTitle("N Background Events");
      Plot(bkgtoSubtract, "BackgroundSumNorm", prefix);
      auto bkgSubtracted = std::accumulate(backgrounds.begin(), backgrounds.end(), folded->Clone(),
                                           [mcPOT, dataPOT](auto sum, const auto hist)
                                           {
                                             std::cout << "Subtracting " << hist->GetName() << " scaled by " << -dataPOT/mcPOT << " from " << sum->GetName() << "\n";
                                             sum->Add(hist, -1); //-dataPOT/mcPOT);
                                             return sum;
                                           });
      Plot(*bkgSubtracted, "backgroundSubtracted", prefix);

      auto outFile = TFile::Open((prefix + "_crossSection.root").c_str(), "CREATE");
      if(!outFile)
      {
        std::cerr << "Could not create a file called " << prefix + "_crossSection.root" << ".  Does it already exist?\n";
        return 5;
      }
      toSubtract->Clone()->Write("background");
      bkgSubtracted->Write("backgroundSubtracted");

      //d'Aogstini unfolding
      MinervaUnfold::MnvUnfold unfold;
      auto unfolded = DoResponseUnfolding(prefix, migration, recosignal , effNum, bkgSubtracted, toSubtract, unfold, nIterations); 
      //auto unfolded = UnfoldHist(bkgSubtracted, migration, nIterations);
      if(!unfolded) throw std::runtime_error(std::string("Failed to unfold ") + folded->GetName() + " using " + migration->GetName());
      Plot(*unfolded, "unfolded", prefix);
      unfolded->Clone()->Write("unfolded"); //TODO: Seg fault first appears when I uncomment this line
      std::cout << "Survived writing the unfolded histogram.\n" << std::flush; //This is evidence that the problem is on the final file Write() and not unfolded->Clone()->Write().
      Plot(*recosignal, "recoSignal", prefix);
      auto recosig = recosignal->Clone();
      //recosig->Scale(dataPOT/mcPOT);
      recosig->Write("recoSignal_potnorm"); 
      recosignal->Divide(recosignal, purdenom);
      Plot(*recosignal, "Purity", prefix);   
      recosignal->Clone()->Write("Purity");
      effNum->Divide(effNum, effDenom); //Only the 2 parameter version of MnvH1D::Divide()
                                        //handles systematics correctly.
      Plot(*effNum, "efficiency", prefix);
      effNum->Clone()->Write("Efficiency");
      unfolded->Divide(unfolded, effNum);
      Plot(*unfolded, "efficiencyCorrected", prefix);
      unfolded->Clone()->Write("EfficiencyCorrected");
       // STart of the code that gets xsec for ALL playlists. Normalization happens differently in this stage
 
      auto h_flux_normalization = getFluxNormalized(unfolded);
   
      auto h_cross_section = 
          (PlotUtils::MnvH2D*)unfolded->Clone();
      h_cross_section->AddMissingErrorBandsAndFillWithCV(*h_flux_normalization);
      
      h_cross_section->Divide( h_cross_section, h_flux_normalization );

      
       
      static const double apothem    = 865.;
      static const double upstream   = 5900.; // ~module 25 plane 1
      static const double downstream = 8430.; // ~module 81 plane 1

      double n_target_nucleons = 
          PlotUtils::TargetUtils::Get().GetTrackerNNucleons(upstream, downstream,
                                                            false, // isMC
                                                            apothem);
      
      double data_scale = 1.0 / ( n_target_nucleons*totalPOT);
     
      h_cross_section->Scale( data_scale, "width" ); 
   
      //auto crossSection = normalize(unfolded, h_flux_normalization, n_target_nucleons, dataPOT);
      Plot(*h_cross_section, "crossSection", prefix);
      h_cross_section->Write("crossSection");
      
      // ===== End of All statistics 

      //==== Start of xsec for 1 playlist. Normalization happens here...using standard tutorial way
      //
      /* 
      auto crossSection = normalize(unfolded, flux, nNucleons->GetVal(), dataPOT);
      Plot(*crossSection, "crossSection", prefix);
      crossSection->Clone()->Write("crossSection");
      */
      


      //auto crossSection = normalize(unfolded, flux, n_target_nucleons, totalPOT);//totalPOT); //dataPOT if you are going playlist by playlist
      //Plot(*crossSection, "crossSection", prefix);
      //crossSection->Write("crossSection");

      /*  // Start of Simulated xsec extraction for all playlists
      auto h_mc_cross_section = simEventRate->Clone();
   
      h_mc_cross_section->Divide( h_mc_cross_section, h_flux_normalization );

      static const double mc_scale = 1.0 / ( n_target_nucleons *totalPOT /* * util.m_mc_pot*/ // );
      /* h_mc_cross_section->Scale(mc_scale, "width");

      //double fake_data_integral = h_cross_section->Integral();
      //double mc_integral = h_mc_cross_section->Integral();

      //Write a "simulated cross section" to compare to the data I just extracted.
      //If this analysis passed its closure test, this should be the same cross section as
      //what GENIEXSecExtract would produce.
      //normalize(simEventRate, flux, nNucleons->GetVal(),1.0);// totalPOT); //mcPOT if you go playlist by playlist
      auto mcxsec = h_mc_cross_section->Clone();
      Plot(*mcxsec,"simulatedCrossSection", prefix);
      mcxsec->Write("simulatedCrossSection");       

      */ // ==== End of Simulated xsec extraction for all playlists
      //simEventRate->Write("simulatedCrossSection");
      //

      double mcPOT = totalPOT;    

      auto h_mc_cross_section = simEventRate->Clone();
   
      h_mc_cross_section->Divide( h_mc_cross_section, h_flux_normalization ); 

      h_mc_cross_section->Scale( data_scale, "width" );

      auto mcxsec = h_mc_cross_section; //normalize(simEventRate, h_flux_normalization, n_target_nucleons, totalPOT);
      Plot(*mcxsec,"simulatedCrossSection", prefix);
      mcxsec->Write("simulatedCrossSection");   

      //normalize(simEventRate, flux, n_target_nucleons, mcPOT);
      
      //Plot(*simEventRate, "simulatedCrossSection", prefix);
      //simEventRate->Write("simulatedCrossSection");


      auto h_npip = npip->Clone();
      h_npip->Divide(h_npip,  h_flux_normalization );
      h_npip->Scale(data_scale, "width");
      h_npip->Write("NpiplusOnlyxsec");

      auto h_onepi = onepi->Clone();
      h_onepi->Divide(h_onepi,  h_flux_normalization );
      h_onepi->Scale(data_scale, "width");
      h_onepi->Write("OnepiNoPNxsec");

      
      auto h_onepin = onepin->Clone();
      h_onepin->Divide(h_onepin,  h_flux_normalization );
      h_onepin->Scale(data_scale, "width");
      h_onepin->Write("OnepiplusNeutronxsec");

      auto h_onepip = onepip->Clone();
      h_onepip->Divide(h_onepip,  h_flux_normalization );
      h_onepip->Scale(data_scale, "width");
      h_onepip->Write("OnepiplusProtonxsec");

      auto h_cohpi = cohpi->Clone();
      h_cohpi->Divide(h_cohpi,  h_flux_normalization );
      h_cohpi->Scale(data_scale, "width");
      h_cohpi->Write("COHxsec");

      auto h_npidis = npidis->Clone();
      h_npidis->Divide(npidis,  h_flux_normalization );
      h_npidis->Scale(data_scale, "width");
      h_npidis->Write("NpiplusDISxsec");

      auto h_npires = npires->Clone();
      h_npires->Divide(h_npires,  h_flux_normalization );
      h_npires->Scale(data_scale, "width");
      h_npires->Write("NpiplusRESxsec"); 

      //normalize(npip, flux, n_target_nucleons, mcPOT);
      //Plot(*npip, "NpiplusOnlyxsec", prefix);
      //npip->Write("NpiplusOnlyxsec");
 
      //normalize(onepi, flux, n_target_nucleons, mcPOT);
      //Plot(*onepi, "OnepiNoPNxsec", prefix);
      //onepi->Write("OnepiNoPNxsec");

     /*
      normalize(onepin, flux,n_target_nucleons, mcPOT);
      Plot(*onepin, "OnepiplusNeutronxsec", prefix);
      onepin->Write("OnepiplusNeutronxsec");


      normalize(onepip, flux,n_target_nucleons, mcPOT);
      Plot(*onepip, "OnepiplusProtonxsec", prefix);
      onepip->Write("OnepiplusProtonxsec");
      
     
      normalize(npires, flux, n_target_nucleons, mcPOT);
      Plot(*npires, "NpiplusRESxsec", prefix);
      npires->Write("NpiplusRESxsec");

      normalize(npidis, flux, n_target_nucleons, mcPOT);
      Plot(*npidis, "NpiplusDISxsec", prefix);
      npidis->Write("NpiplusDISxsec");

      normalize(cohpi, flux,n_target_nucleons, mcPOT);
      Plot(*cohpi, "COHxsec", prefix);
      cohpi->Write("COHxsec");*/
    }
    catch(const std::runtime_error& e)
    {
      std::cerr << "Failed to extra a cross section for prefix " << prefix << ": " << e.what() << "\n";
      return 4;
    }
  }

  return 0;
}
