//#include "include/CCQENuPlotUtils.h"
//#include "include/NukeCCUtilsNSF.h"
#include "/minerva/app/users/mmehmood/cmtuser/Minerva_v22r1p1_MADNew/Ana/NSFNukeCCInclusive/NUKECCSRC/ana_common/include/NukeCCUtilsNSF.h"
#include "TParameter.h"
#include "drawUtils.h"

void makeplot(string histsuffix, TFile* myfile, bool doprelimlabel=false){
  PlotUtils::MnvH2D *num = (PlotUtils::MnvH2D*)myfile->Get("h_mc_pZmu_pTmu");
  PlotUtils::MnvH2D *dem = (PlotUtils::MnvH2D*)myfile->Get("h_truth_pZmu_pTmu");
  num->Divide(num,dem, 1.0, 1.0, "B");
//  num->SetBinContent(4,12,0.0);
  num->GetXaxis()->SetTitle("True p|| (GeV/c)");
  num->GetYaxis()->SetTitle("True pt (GeV/c)");
  num->GetZaxis()->SetTitle("Efficiency");

  applyStyle(num);
  gStyle->SetStripDecimals(0);
  num->GetXaxis()->SetTitleOffset(1.3);
  num->GetZaxis()->SetTitleOffset(1.5);
  num->GetZaxis()->SetRangeUser(0,1);
  TCanvas c2("test","test");
  c2.SetRightMargin(0.1788009);
  num->Draw("COLZ");

//  if (doprelimlabel) {  
//    TLatex* labl=new TLatex(1.7, 2.6,"MINER#nuA Preliminary    POT: 3.34 x10^{20}    #LTE_{#nu}#GT~3.5GeV" );
//    labl->SetTextSize(0.03);
//    labl->SetTextFont(112);//22);                                                                            
//    labl->SetTextColor(4);
//    labl->Draw();
//  }
  
  c2.Print("pZmu_pTmu_Eff.png");
 
}


int main( int argc, char *argv[]){

  if(argc==1){
    std::cout<<"-----------------------------------------------------------------------------------------\
------"<<std::endl;
    std::cout<<"MACROS HELP:\n\n"<<
      "\t-./EffPlots2D Path_to_Output_file Target_number Material_atomic_number doPreminaryLabel\n\n"<<
       "\t-Path_to_Output_file\t =\t Path to the directory where the output ROOT file will be created \n"<\
<
      "\t-Target_number\t \t = \t Number of target you want to run over eg. 1 \n"<<
       "\t-Material_atomic_number\t =\t Atomic number of material, eg. 26 to run iron, 82 to run lead  \n"\
	     <<
      "\t-doPreliminaryLabel= Add MINERvA Preliminary to plot?"<< std::endl;
    std::cout<<"-----------------------------------------------------------------------------------------\
------"<<std::endl;
    return 0;
  }


  PlotUtils::MnvPlotter *plotter = new PlotUtils::MnvPlotter;
  plotter->SetROOT6Palette(87);
  gStyle->SetNumberContours(500);
  gStyle->SetTitleSize(1,"x");
  gStyle->SetTitleSize(1,"y");
  gStyle->SetTitleSize(1,"z");
  gStyle->SetOptStat(0);
  //ROOT::Cintex::Cintex::Enable();
  TH1::AddDirectory(false);


   string location=argv[1];
//  int targetID=atoi(argv[2]);
//  int targetZ=atoi(argv[3]);
//  bool doprelimlabel=argv[4];
  TFile *infile = new TFile("Hists_EfficiencyP1Production_t99_z99_AntiNu.root");
  makeplot("eff", infile, "false"); 
//  makeplot("cc",infile, doprelimlabel);
//  makeplot("qe",infile, doprelimlabel);
//  makeplot("res",infile, doprelimlabel);
//  makeplot("dis",infile, doprelimlabel);
//  makeplot("true_dis",infile, doprelimlabel);
//  makeplot("sis",infile, doprelimlabel);
//  makeplot("2p2h",infile, doprelimlabel);
//  makeplot("oth",infile, doprelimlabel);

  return 0;
}
