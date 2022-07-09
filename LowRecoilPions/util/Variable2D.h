#ifndef VARIABLE2D_H
#define VARIABLE2D_H

#include "util/SafeROOTName.h"
#include "PlotUtils/Variable2DBase.h"
#include "util/Categorized.h"
#include "util/TruthInteractionStudies.h"
class Variable2D: public PlotUtils::Variable2DBase<CVUniverse>
{
  private:
    typedef PlotUtils::Hist2DWrapper<CVUniverse> Hist;
  public:
    template <class ...ARGS>
    Variable2D(ARGS... args): PlotUtils::Variable2DBase<CVUniverse>(args...)
    {
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
      fSignalByPionsInVar = new util::Categorized<Hist, FSCategory*>(pionFSCategories,GetName().c_str(),(GetName()+"_npi").c_str() , GetBinVecX(), GetBinVecY(),mc_error_bands);
      
      mcTotalHist = new Hist((GetNameX() + "_" + GetNameY() + "_MC").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);
      selectedSignalReco = new Hist((GetNameX() + "_" + GetNameY() + "_signal_reco").c_str(), GetName().c_str(), GetBinVecX(), GetBinVecY(), mc_error_bands);

      std::vector<double> truediffbins;
      const double diffBinWidth = 25; //MeV
      for(int whichBin = 0; whichBin < 600 + 1; ++whichBin) truediffbins.push_back(-3000+diffBinWidth * whichBin);
      recoMinusTrueTBins = new Hist((GetNameX() + "_" + GetNameY() + "_recoMinusTrueTBins").c_str(), GetName().c_str(), truediffbins, GetBinVecY(), mc_error_bands);
      recoMinusTrueRBins = new Hist((GetNameX() + "_" + GetNameY() + "_recoMinusTrueRBins").c_str(), GetName().c_str(), truediffbins, GetBinVecY(), mc_error_bands);
    }

    //Histograms to be filled
    util::Categorized<Hist, int>* m_backgroundHists;
    util::Categorized<Hist, int>* m_MChists;
    Hist* dataHist;  
    Hist* efficiencyNumerator;
    Hist* efficiencyDenominator;
    util::Categorized<Hist, FSCategory*>* fSignalByPionsInVar; 
    Hist* mcTotalHist;
    Hist* recoMinusTrueTBins;
    Hist* recoMinusTrueRBins;
    Hist* selectedSignalReco;
    void InitializeDATAHists(std::vector<CVUniverse*>& data_error_bands)
    {
        const char* name = GetName().c_str();
  	dataHist = new Hist(name, name, GetBinVecX(), GetBinVecY(), data_error_bands);
 
    }

    void Write(TFile& file)
    {
      SyncCVHistos();
      file.cd();

      m_backgroundHists->visit([&file](Hist& categ)
                                    {
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
      if (dataHist->hist) {
		dataHist->hist->SetDirectory(&file);
		dataHist->hist->Write();
      }

      if (mcTotalHist->hist) {
                mcTotalHist->hist->SetDirectory(&file);
                mcTotalHist->hist->Write();
      }
      if (selectedSignalReco->hist) {
                selectedSignalReco->hist->SetDirectory(&file);
                selectedSignalReco->hist->Write();
      }
      if (recoMinusTrueTBins->hist) {
                recoMinusTrueTBins->hist->SetDirectory(&file);
                recoMinusTrueTBins->hist->Write();
      }
      if (recoMinusTrueRBins->hist) {
                recoMinusTrueRBins->hist->SetDirectory(&file);
                recoMinusTrueRBins->hist->Write();
      } 
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
      m_backgroundHists->visit([](Hist& categ) { categ.SyncCVHistos(); });
      if(mcTotalHist) mcTotalHist->SyncCVHistos();
      if(dataHist) dataHist->SyncCVHistos();
      if(efficiencyNumerator) efficiencyNumerator->SyncCVHistos();
      if(efficiencyDenominator) efficiencyDenominator->SyncCVHistos();
      if(recoMinusTrueTBins) recoMinusTrueTBins->SyncCVHistos();
      if(recoMinusTrueRBins) recoMinusTrueRBins->SyncCVHistos();
    }
    
    void FillCategHistos(const CVUniverse& univ, double varx, double vary, const double weight)
    {
      const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
      (*fSignalByPionsInVar)[*pionCat].FillUniverse(&univ, varx, vary, weight);

    }
};

#endif //VARIABLE2D_H
