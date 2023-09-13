#ifndef VARIABLE2D_H
#define VARIABLE2D_H

#include "util/SafeROOTName.h"
#include "PlotUtils/Variable2DBase.h"
#include "util/Categorized.h"
#include "util/PionFSCategory.h"
#include "util/PionProtonFSCategory.h"
#include "MinervaUnfold/MnvResponse.h"
#include "TArrayD.h"
#include "PlotUtils/AnaBinning.h"

//#include "util/TruthInteractionStudies.h"
class Variable2D: public PlotUtils::Variable2DBase<CVUniverse>
{
  private:
    typedef PlotUtils::Hist2DWrapper<CVUniverse> Hist;
    typedef MinervaUnfold::MnvResponse MResponse;
    typedef PlotUtils::axis_binning axis_binning;
  public:
    template <class ...ARGS>
    Variable2D(ARGS... args): PlotUtils::Variable2DBase<CVUniverse>(args...)
    {
    }
    
    TArrayD GetTArrayFromVec(const std::vector<double>& vec) {
  	double array[vec.size()];
  	std::copy(vec.begin(), vec.end(), array);
  	const int size = sizeof(array) / sizeof(*array);
  	return TArrayD(size, array);
    } 

      

    //TODO: It's really silly to have to make 2 sets of error bands just because they point to different trees.
    //      I'd rather the physics of the error bands remain the same and just change which tree they point to.
    void InitializeMCHists(std::map<std::string, std::vector<CVUniverse*>>& mc_error_bands,
                           std::map<std::string, std::vector<CVUniverse*>>& truth_error_bands)
    {

      std::map<int, std::string> BKGLabels = {{0, "NC_Bkg"},
					       {1, "Bkg_Wrong_Sign"}};
      
      std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                                                 {4, "COH"},
                                                 {3, "DIS"}}; //other category is built in for free. 

      m_backgroundHists = new util::Categorized<Hist, int>((GetName() + "_by_BKG_Label").c_str(),
							   GetName().c_str(), BKGLabels,
							   GetBinVecX(), GetBinVecY(), mc_error_bands);
      m_MChists = new util::Categorized<Hist, int>(("GENIE_"+GetName()).c_str(),
                                                           GetName().c_str(), GENIELabels,
                                                           GetBinVecX(),GetBinVecY(), mc_error_bands);
      efficiencyNumerator = new Hist((GetNameX() + "_" + GetNameY() + "_efficiency_numerator").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);
      efficiencyDenominator = new Hist((GetNameX() + "_" + GetNameY() + "_efficiency_denominator").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), truth_error_bands);
      fSignalByPionsInVar = new util::Categorized<Hist, FSCategory*>(pionFSCategories,(GetName()+"_top").c_str(),(GetName()).c_str() , GetBinVecX(), GetBinVecY(),mc_error_bands);
      fSignalByPionNucleonInVar = new util::Categorized<Hist, FSpCategory*>(pionprotonFSCategories,(GetName()+"_picat").c_str(),(GetName()).c_str() , GetBinVecX(), GetBinVecY(),mc_error_bands);

      
      mcTotalHist = new Hist((GetNameX() + "_" + GetNameY() + "_MC").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);
      selectedSignalReco = new Hist((GetNameX() + "_" + GetNameY() + "_signal_reco").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);

      mc_trueHist = new Hist((GetNameX() + "_" + GetNameY() + "_signal_true").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);
   
      //xaxis = new axis(GetBinVecX(), GetNBinsX(), GetBinVecX()[0], GetBinVecX()[GetNBinsX()-1], false);
      

     // m_response = new MResponse((GetNameX() + "_" + GetNameY() + "_MnvResponse").c_str(), GetName().c_str(), GetNBinsX(), GetTArrayFromVec(GetBinVecX()), GetNBinsY(), GetTArrayFromVec(GetBinVecY()), GetNBinsX(), GetTArrayFromVec(GetBinVecX()), GetNBinsY(), GetTArrayFromVec(GetBinVecY()));
    }
  
    void SetupResponse(std::map<const std::string,  int> systematics){
	const char* name = GetName().c_str();
        const char* newname = (GetNameX() + "_vs_" + GetNameY() + "_MnvResponse").c_str();//GetName().c_str();
        axis_binning bin_x, bin_y;
        bin_x.uniform=false;

        std::vector<double> vx;

        for(int i=0; i<=GetNBinsX(); i++){vx.push_back(GetBinVecX().data()[i]);}

        std::vector<double> vy;
        for(int j=0; j<=GetNBinsY(); j++){vy.push_back(GetBinVecY().data()[j]);}
        bin_x.bin_edges = vx;
        bin_x.nbins      = GetNBinsX();
        bin_x.min        = GetBinVecX().data()[0];
        bin_x.max       = GetBinVecX().data()[GetNBinsX()];
        bin_y.bin_edges = vy;
        bin_y.nbins      = GetNBinsY();
        bin_y.min        = GetBinVecY().data()[0];
        bin_y.max       = GetBinVecY().data()[GetNBinsY()];
	Response.insert(std::pair<const std::string, MinervaUnfold::MnvResponse*>(newname, new MinervaUnfold::MnvResponse(Form("response2d_%s",newname), newname, bin_x, bin_y, bin_x, bin_y, systematics)));
    }

    void FillResponse(double x_reco, double y_reco, double x_true, double y_true, const std::string name, double w, int unv){
        for(mnv_itr = Response.begin(); mnv_itr != Response.end(); ++mnv_itr){
                (mnv_itr->second)->Fill(x_reco,y_reco,x_true,y_true,name,unv, w);
        }

    }

    template <typename T>
    void getResponseObjects(T univs)
    {
	for(mnv_itr2 = Response.begin(); mnv_itr2 != Response.end(); ++mnv_itr2){
                (mnv_itr2->second)->GetMigrationObjects( migrationH, h_reco, h_truth );;
        }

        const bool clear_bands = false;
  	//migrationH->PopVertErrorBand("cv");
	mresp = new Hist(migrationH, univs, clear_bands);
        mreco = new Hist(h_reco, univs, clear_bands);
        mtrue = new Hist(h_truth, univs, clear_bands);   


    } 
    


    //Histograms to be filled
    util::Categorized<Hist, int>* m_backgroundHists;
    util::Categorized<Hist, int>* m_MChists;
    Hist* dataHist;  
    Hist* efficiencyNumerator;
    Hist* efficiencyDenominator;
    util::Categorized<Hist, FSCategory*>* fSignalByPionsInVar; 
    util::Categorized<Hist, FSpCategory*>* fSignalByPionNucleonInVar;
    Hist* mcTotalHist;
    Hist* selectedSignalReco;
    Hist* mc_trueHist;

    //Adding Response Matrix for 2D Unfolding needs
    MResponse m_response;
    std::map<std::string,MinervaUnfold::MnvResponse*> Response;
    std::map<std::string,MinervaUnfold::MnvResponse*>::iterator mnv_itr;
    std::map<std::string,MinervaUnfold::MnvResponse*>::iterator mnv_itr2;
    Hist* mresp;
    Hist* mreco;
    Hist* mtrue;
    MnvH2D *migrationH = NULL;
    MnvH2D *h_reco = NULL;
    MnvH2D *h_truth = NULL;
    void InitializeDATAHists(std::vector<CVUniverse*>& data_error_bands)
    {
        const char* name = (GetName()).c_str();
  	dataHist = new Hist((GetNameX() + "_" + GetNameY() + "_data").c_str(), name, GetBinVecX(), GetBinVecY(), data_error_bands);
 
    }



    void Write(TFile& file)
    {
      SyncCVHistos();
      file.cd();

      m_backgroundHists->visit([&file](Hist& categ)
                                    {
      				      categ.SyncCVHistos();
                                      categ.hist->SetDirectory(&file);
                                      categ.hist->Write(); //TODO: Or let the TFile destructor do this the "normal" way?                                                                                           
                                    });
      m_MChists->visit([&file](Hist& categ)
                                    {
				      categ.SyncCVHistos(); 
                                      categ.hist->SetDirectory(&file);
                                      categ.hist->Write(); //TODO: Or let the TFile destructor do this the "normal" way?                                                                                           
                                    });
      fSignalByPionsInVar->visit([&file](auto& Hist) {
						    Hist.SyncCVHistos();
                                                    Hist.hist->SetDirectory(&file);
						    Hist.hist->Write();
                                                });
      fSignalByPionNucleonInVar->visit([&file](auto& Hist) {
                                                    Hist.SyncCVHistos();
                                                    Hist.hist->SetDirectory(&file);
                                                    Hist.hist->Write();
                                                });


      if (dataHist->hist) {
		dataHist->SyncCVHistos();
                dataHist->hist->SetDirectory(&file);
                dataHist->hist->Write();
      }

      if (mc_trueHist->hist){
     		mc_trueHist->SyncCVHistos();         
		mc_trueHist->hist->SetDirectory(&file);
 		mc_trueHist->hist->Write();
      }

      if (mcTotalHist->hist) {
                mcTotalHist->SyncCVHistos();
                mcTotalHist->hist->SetDirectory(&file);
                mcTotalHist->hist->Write(); //(GetNameX() + "_" + GetNameY() + "_mcreco").c_str());
      }
      if (selectedSignalReco) {
       		selectedSignalReco->SyncCVHistos();
                selectedSignalReco->hist->SetDirectory(&file);
                selectedSignalReco->hist->Write();
      }
      /*
      if (recoMinusTrueTBins) {
                recoMinusTrueTBins->hist->SetDirectory(&file);
                recoMinusTrueTBins->hist->Write();
      }
      if (recoMinusTrueRBins) {
                recoMinusTrueRBins->hist->SetDirectory(&file);
                recoMinusTrueRBins->hist->Write();
      }

     if (biasMCReco){
	biasMCReco->hist->SetDirectory(&file);
	biasMCReco->hist->Write();

     }*/
 
     if(efficiencyNumerator)
      {
        efficiencyNumerator->hist->SetDirectory(&file); //TODO: Can I get around having to call SetDirectory() this many times somehow?
        efficiencyNumerator->hist->Write();
      }

      if(efficiencyDenominator)
      {
        efficiencyDenominator->hist->SetDirectory(&file);
        efficiencyDenominator->hist->Write();
      }

      if(mresp)
      {
        //mresp->hist->SetDirectory(&file);
	//mresp->hist->Write((GetNameX() + "_" + GetNameY() + "_migration").c_str());

        migrationH->SetDirectory(&file);
	migrationH->Write((GetNameX() + "_" + GetNameY() + "_migration").c_str());
	
      }


    }


    //Only call this manually if you Draw(), Add(), or Divide() plots in this
    //program.
    //Makes sure that all error bands know about the CV.  In the Old Systematics
    //Framework, this was implicitly done by the event loop.
    void SyncCVHistos()
    {
      m_MChists->visit([](Hist& categ) { categ.SyncCVHistos(); });
      if(selectedSignalReco) selectedSignalReco->SyncCVHistos();
      fSignalByPionsInVar->visit([](auto& Hist) {Hist.SyncCVHistos();});
      fSignalByPionNucleonInVar->visit([](auto& Hist) {Hist.SyncCVHistos();});
      m_backgroundHists->visit([](Hist& categ) { categ.SyncCVHistos(); });
      if(mcTotalHist) mcTotalHist->SyncCVHistos();
      if(dataHist) dataHist->SyncCVHistos();
      if(efficiencyNumerator) efficiencyNumerator->SyncCVHistos();
      if(efficiencyDenominator) efficiencyDenominator->SyncCVHistos();
      //if(mresp) mresp->SyncCVHistos();
      if(mc_trueHist) mc_trueHist->SyncCVHistos();
     
       //if(recoMinusTrueTBins) recoMinusTrueTBins->SyncCVHistos();
      //if(recoMinusTrueRBins) recoMinusTrueRBins->SyncCVHistos();
      //if(biasMCReco) biasMCReco->SyncCVHistos();
    }
    
    void FillCategHistos(const CVUniverse& univ, double varx, double vary, const double weight)
    {
      const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
      (*fSignalByPionsInVar)[*pionCat].FillUniverse(&univ, varx, vary, weight);
      const auto pionprotonCat = std::find_if(pionprotonFSCategories.begin(), pionprotonFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
      (*fSignalByPionNucleonInVar)[*pionprotonCat].FillUniverse(&univ, varx, vary, weight);

    }
};

#endif //VARIABLE2D_H
