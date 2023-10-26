//studies includes
#include "studies/Study.h"
#ifndef __CINT__ //PlotUtils/VariableBase.h uses std::function which is from c++11
#include "util/SafeROOTName.h" //TODO: This is a very bad idea
#endif //__CINT__

//Mehreen's includes
#include "PlotUtils/LowRecoilPionReco.h"
typedef LowRecoilPion::Michel<CVUniverse> Michel;
typedef LowRecoilPion::MichelEvent<CVUniverse> MichelEvent;

#include "util/Categorized.h"
#include "event/CVUniverse.h"
#include "PlotUtils/VariableBase.h"
//c++ includes
#include <functional> //for std::function

struct VarConfig
{
  std::string name;
  std::string units;
  int nBins;
  double min;
  double max;
};

class PerMichel2DVar: public Study
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&, const int)>;
    PerMichel2DVar(reco_t xreco, reco_t yreco, const VarConfig xVar, const VarConfig yVar, std::map<std::string, std::vector<CVUniverse*>>& univs): Study(), fxReco(xreco), fyReco(yreco), dataHist(new HIST((yVar.name + "_vs_" + xVar.name + "_DATA").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.nBins, xVar.min, xVar.max, yVar.nBins, yVar.min, yVar.max, univs)), totalMCHist(new HIST((yVar.name + "_vs_" + xVar.name + "_MC").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.nBins, xVar.min, xVar.max, yVar.nBins, yVar.min, yVar.max, univs))
	{
       std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
						 {4, "COH"},
                                                 {3, "DIS"}}; //other category is built in for free. 
//      std::map<int, std::string> GENIELabels = {{0, "All?"}}; //other category is built in for free.  
     m_VarToGENIELabel = new util::Categorized<HIST, int>(yVar.name + "_vs_" + xVar.name, (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]"), GENIELabels, xVar.nBins, xVar.min, xVar.max, yVar.nBins, yVar.min, yVar.max, univs); 
    }
    
    
    void SaveOrDraw(TDirectory& outDir)
    {
       outDir.cd();
       m_VarToGENIELabel->visit([](HIST& wrapper)
                                {
                                  wrapper.SyncCVHistos();
                                  wrapper.hist->Write();
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
    HW* totalMCHist;
    HW* dataHist;
    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
         (*dataHist).FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
      }
    }

    //All of your plots happen here so far.
    void fillSelectedSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
        (*totalMCHist).FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
 
        (*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fxReco(univ, evt, whichMichel), fyReco(univ, evt, whichMichel), weight);
      }
    }

    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight) {}
};
