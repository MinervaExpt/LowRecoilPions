#ifndef VARIABLE_H
#define VARIABLE_H

//Includes from this package
#include "event/CVUniverse.h"
#include "util/SafeROOTName.h"
#include "util/Categorized.h"
//#include "util/TruthInteractionStudies.h"
#include "util/PionFSCategory.h"
//PlotUtils includes
#include "PlotUtils/VariableBase.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"

class Variable: public PlotUtils::VariableBase<CVUniverse>
{
  private:
    typedef PlotUtils::HistWrapper<CVUniverse> Hist;
  public:
    template <class ...ARGS>
    Variable(ARGS... args): PlotUtils::VariableBase<CVUniverse>(args...)
    {
    }
   
    //#include "util/SafeROOTName.h"

    //TODO: It's really silly to have to make 2 sets of error bands just because they point to different trees.
    //      I'd rather the physics of the error bands remain the same and just change which tree they point to.
    void InitializeMCHists(std::map<std::string, std::vector<CVUniverse*>>& mc_error_bands,
                           std::map<std::string, std::vector<CVUniverse*>>& truth_error_bands)
    {

      std::map<int, std::string> BKGLabels = {{0, "NC"},
					       {1, "Wrong_Sign"}};


      std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                                                 {4, "COH"},
						 {3, "DIS"}}; //other category is built in for free.     
      
      m_backgroundHists = new util::Categorized<Hist, int>((GetName() + "_background").c_str(),
							   GetName().c_str(), BKGLabels,
							   GetBinVec(), mc_error_bands);

      m_MChists = new util::Categorized<Hist, int>(("GENIE_"+GetName()).c_str(),
                                                           GetName().c_str(), GENIELabels,
                                                           GetBinVec(), mc_error_bands);

      fSignalByPionsInVar = new util::Categorized<Hist, FSCategory*>(pionFSCategories,(GetName()+"_top").c_str(),(GetName()).c_str() , GetBinVec(),mc_error_bands);
     
      //fSideBandByPionsInVar = new util::Categorized<Hist, FSCategory*>(pionFSCategories,GetName().c_str(),(GetName()).c_str() , GetBinVec(),mc_error_bands);

      efficiencyNumerator = new Hist((GetName() + "_efficiency_numerator").c_str(), GetName().c_str(), GetBinVec(), mc_error_bands);
      efficiencyDenominator = new Hist((GetName() + "_efficiency_denominator").c_str(), GetName().c_str(), GetBinVec(), truth_error_bands);
      selectedSignalReco = new Hist((GetName() + "_selected_signal_reco").c_str(), GetName().c_str(), GetBinVec(), mc_error_bands);
      selectedMCReco = new Hist((GetName() + "_selected_mc_reco").c_str(), GetName().c_str(), GetBinVec(), mc_error_bands);
      std::vector<double> biasbins = {-2.6, -2.4, -2.2, -2. ,-1.8 ,-1.6, -1.4, -1.2 ,-1., -0.8, -0.6, -0.4, -0.2 ,0., 0.2, 0.4, 0.6, 0.8, 1., 1.2, 1.4, 1.6, 1.8, 2., 2.2 ,2.4, 2.6};
      //biasMCReco = new Hist((GetName() + "_selected_bias_reco").c_str(), GetName().c_str(), biasbins, mc_error_bands); 
      //std::vector<double> truediffbins = {-2500.0, -2450.0, -2400.0, -2350.0, -2300.0, -2250.0, -2200.0, -2150.0, -2100.0, -2050.0, -2000.0, -1950.0, -1900.0, -1850.0, -1800.0, -1750.0, -1700.0, -1650.0, -1600.0, -1550.0, -1500.0, -1450.0, -1400.0, -1350.0, -1300.0, -1250.0, -1200.0, -1150.0, -1100.0, -1050.0, -1000.0, -950.0, -900.0, -850.0, -800.0, -750.0, -700.0, -650.0, -600.0, -550.0, -500.0, -450.0, -400.0, -350.0, -300.0, -250.0, -200.0, -150.0, -100.0, -50.0, 0.0, 50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0, 1050.0, 1100.0, 1150.0, 1200.0, 1250.0, 1300.0, 1350.0, 1400.0, 1450.0, 1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0};
      //std::vector<double> truediffbins = {-3000.0, -2950.0, -2900.0, -2850.0, -2800.0, -2750.0, -2700.0, -2650.0, -2600.0, -2550.0, -2500.0, -2450.0, -2400.0, -2350.0, -2300.0, -2250.0, -2200.0, -2150.0, -2100.0, -2050.0, -2000.0, -1950.0, -1900.0, -1850.0, -1800.0, -1750.0, -1700.0, -1650.0, -1600.0, -1550.0, -1500.0, -1450.0, -1400.0, -1350.0, -1300.0, -1250.0, -1200.0, -1150.0, -1100.0, -1050.0, -1000.0, -950.0, -900.0, -850.0, -800.0, -750.0, -700.0, -650.0, -600.0, -550.0, -500.0, -450.0, -400.0, -350.0, -300.0, -250.0, -200.0, -150.0, -100.0, -50.0, 0.0, 50.0, 100.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0, 850.0, 900.0, 950.0, 1000.0, 1050.0, 1100.0, 1150.0, 1200.0, 1250.0, 1300.0, 1350.0, 1400.0, 1450.0, 1500.0, 1550.0, 1600.0, 1650.0, 1700.0, 1750.0, 1800.0, 1850.0, 1900.0, 1950.0, 2000.0, 2050.0, 2100.0, 2150.0, 2200.0, 2250.0, 2300.0, 2350.0, 2400.0, 2450.0, 2500.0, 2550.0, 2600.0, 2650.0, 2700.0, 2750.0, 2800.0, 2850.0, 2900.0, 2950.0, 3000.0}; 
      //const double diffBinWidth = 50; //MeV
      //for(int whichBin = 0; whichBin < 300 + 1; ++whichBin) {
      //	double max = diffBinWidth*whichBin;
	//if (max >= 3000.) break;
	//truediffbins.push_back(-3000.+diffBinWidth * whichBin);
        
      //}
      //recoMinusTrue = new Hist((GetName() + "_recoMinusTrue").c_str(), GetName().c_str(), truediffbins, mc_error_bands);

      migration = new PlotUtils::Hist2DWrapper<CVUniverse>((GetName() + "_migration").c_str(), GetName().c_str(), GetBinVec(), GetBinVec(), mc_error_bands);
    }

    //Histograms to be filled
    util::Categorized<Hist, int>* m_backgroundHists;
    util::Categorized<Hist, int>* m_sidebandHists;
    util::Categorized<Hist, int>* m_MChists;
    util::Categorized<Hist, FSCategory*>* fSignalByPionsInVar;
    //util::Categorized<Hist, FSCategory*>* fSideBandByPionsInVar;
    Hist* dataHist;
    Hist* efficiencyNumerator;
    Hist* efficiencyDenominator;
    Hist* selectedSignalReco; //Effectively "true background subtracted" distribution for warping studies.
                              //Also useful for a bakground breakdown plot that you'd use to start background subtraction studies.
    Hist* selectedMCReco; //Treat the MC CV just like data for the closure test
    //Hist* recoMinusTrue; // Plotting Abs Residuals (reco - true)
    //Hist* biasMCReco; // Plotting bias;

    PlotUtils::Hist2DWrapper<CVUniverse>* migration;

    void InitializeDATAHists(std::vector<CVUniverse*>& data_error_bands)
    {
      dataHist = new Hist((GetName() + "_data").c_str(), GetName().c_str(), GetBinVec(), data_error_bands);
    }

    void WriteData(TFile& file)
    {
      if (dataHist->hist) {
                dataHist->hist->SetDirectory(&file);
                dataHist->hist->Write();
      }
    }

    void WriteMC(TFile& file)
    {
      SyncCVHistos();
      file.cd();

      m_MChists->visit([&file](Hist& categ)
                                    {
                                      categ.SyncCVHistos();
 				      categ.hist->SetDirectory(&file);
                                      categ.hist->Write(); //TODO: Or let the TFile destructor do this the "normal" way?                                                                                           
                                    });

      m_backgroundHists->visit([&file](Hist& categ)
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

      if(migration)
      {
        migration->hist->SetDirectory(&file); 
        migration->hist->Write();
      }

      if(selectedSignalReco)
      {
        selectedSignalReco->hist->SetDirectory(&file);
        selectedSignalReco->hist->Write();
      }
      
      /*
      if(recoMinusTrue)
      {
        recoMinusTrue->hist->SetDirectory(&file);
        recoMinusTrue->hist->Write();
      }
      if(biasMCReco)
      {
	biasMCReco->hist->SetDirectory(&file);
	biasMCReco->hist->Write();

      } */

      if(selectedMCReco)
      {
        selectedMCReco->hist->SetDirectory(&file);
        selectedMCReco->hist->Write((GetName() + "_data").c_str()); //Make this histogram look just like the data for closure tests
      }
    }

    //Only call this manually if you Draw(), Add(), or Divide() plots in this
    //program.
    //Makes sure that all error bands know about the CV.  In the Old Systematics
    //Framework, this was implicitly done by the event loop.
    void SyncCVHistos()
    {
      m_backgroundHists->visit([](Hist& categ) { categ.SyncCVHistos(); });
      m_MChists->visit([](Hist& categ) { categ.SyncCVHistos(); });
      fSignalByPionsInVar->visit([](auto& Hist) {Hist.SyncCVHistos();});
      //fSideBandByPionsInVar->visit([](auto& Hist) {Hist.SyncCVHistos();});
      if(dataHist) dataHist->SyncCVHistos();
      if(efficiencyNumerator) efficiencyNumerator->SyncCVHistos();
      if(efficiencyDenominator) efficiencyDenominator->SyncCVHistos();
      if(selectedSignalReco) selectedSignalReco->SyncCVHistos();
      if(selectedMCReco) selectedMCReco->SyncCVHistos();
      if(migration) migration->SyncCVHistos();
      //if(biasMCReco) biasMCReco->SyncCVHistos();
      //if(recoMinusTrue) recoMinusTrue->SyncCVHistos();
    }

    void FillCategHistos(const CVUniverse& univ, double var,  const double weight)
    {
      const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
      (*fSignalByPionsInVar)[*pionCat].FillUniverse(&univ, var, weight);
      //(*fSideBandByPionsInVar)[*pionCat].FillUniverse(&univ, var, weight);
    }
};

#endif //VARIABLE_H
