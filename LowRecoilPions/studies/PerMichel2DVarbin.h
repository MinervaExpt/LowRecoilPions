//studies includes
#include "studies/Study.h"
#ifndef __CINT__ //PlotUtils/VariableBase.h uses std::function which is from c++11
#include "util/SafeROOTName.h" //TODO: This is a very bad idea
#endif //__CINT__

//Mehreen's includes
#include "event/MichelEvent.h"
#include "util/Categorized.h"
#include "event/CVUniverse.h"
#include "PlotUtils/VariableBase.h"
//#include "studies/TruthInteractionStudies.h"
#include "util/PionFSCategory.h"

//c++ includes
#include <functional> //for std::function

struct VarConfig2D
{
  std::string name;
  std::string units;
  int nBins;
  std::vector<double> bin;
};

class PerMichel2DVarbin: public Study
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&, const int)>;
    PerMichel2DVarbin(reco_t xreco, reco_t yreco, const VarConfig2D xVar, const VarConfig2D yVar, std::map<std::string, std::vector<CVUniverse*>>& univs): Study(), fxReco(xreco), fyReco(yreco), dataHist(new HIST((yVar.name + "_vs_" + xVar.name + "_DATA").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs)), totalMCHist(new HIST((yVar.name + "_vs_" + xVar.name + "_MC").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs))
    {
       std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                    				 {4, "COH"},
		                                 {3, "DIS"}}; //other category is built in for free. 
//      std::map<int, std::string> GENIELabels = {{0, "All?"}}; //other category is built in for free.  
        std::cout << "Filling 2D Variable bin hists" << std::endl;
 	m_VarToGENIELabel = new util::Categorized<HIST, int>(("GENIE_"+yVar.name + "_vs_" + xVar.name).c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]"), GENIELabels, xVar.bin, yVar.bin, univs); 
        fSignalByPionsInVar = new util::Categorized<HIST, FSCategory*>(pionFSCategories,(yVar.name + "_vs_" + xVar.name).c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs);
        bkgHist = new HIST((yVar.name + "_vs_" + xVar.name + "_bkg").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs);
    }
    
    
    void SaveOrDraw(TDirectory& outDir)
    {
       outDir.cd();
       m_VarToGENIELabel->visit([](HIST& wrapper)
                                {
                                  wrapper.SyncCVHistos();
                                  wrapper.hist->Write();
                                });
       fSignalByPionsInVar->visit([](auto& Hist) {
                                                    Hist.SyncCVHistos();
                                                    //Hist.hist->SetDirectory(&file);
                                                    Hist.hist->Write();
                                                });
       dataHist->SyncCVHistos();
       dataHist->hist->Write();
       totalMCHist->SyncCVHistos();
       totalMCHist->hist->Write();

       //TODO: You could do plotting here
    }

  private:
    using HIST = PlotUtils::Hist2DWrapper<CVUniverse>;
    typedef PlotUtils::Hist2DWrapper<CVUniverse> HW;
    reco_t fxReco, fyReco;

    util::Categorized<HIST, int>* m_VarToGENIELabel;
    util::Categorized<HIST, FSCategory*>* fSignalByPionsInVar;
    HW* totalMCHist;
    HW* dataHist;
    HW* bkgHist;
    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
         (*dataHist).FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
      }
    }


    void fillBackground(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
         (*bkgHist).FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
      }
    }


    //All of your plots happen here so far.
    void fillSelectedSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
        (*totalMCHist).FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
 
        (*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
        const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
        (*fSignalByPionsInVar)[*pionCat].FillUniverse(&univ, fxReco(univ,evt,whichMichel), fyReco(univ, evt, whichMichel), weight);
      }
    }

    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight) {}
};
