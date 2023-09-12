//studies includes
#include "studies/Study.h"
#include "studies/TruthInteractionStudies.h"
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

class PerMichelVarByGENIELabel: public Study
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&, const int)>;
    PerMichelVarByGENIELabel(reco_t reco, const std::string& varName, const std::string& varUnits, const int nBins, const double minBin, const double maxBin, std::map<std::string, std::vector<CVUniverse*>>& univs): Study(), fReco(reco), dataHist(new HIST((varName+"_data").c_str(), (varName + "[" + varUnits + "]").c_str(), nBins, minBin, maxBin, univs)), totalMCHist(new HIST((varName+"_MC").c_str(), (varName + " [" + varUnits + "]").c_str(), nBins, minBin, maxBin, univs))
   {
       std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                                                 {4, "COH"},
						 {3, "DIS"}}; //other category is built in for free. 
//      std::map<int, std::string> GENIELabels = {{0, "All?"}}; //other category is built in for free.  
     
     m_VarToGENIELabel = new util::Categorized<HIST, int>(varName, varName + " [" + varUnits + "]", GENIELabels, nBins, minBin, maxBin, univs);
     std::map<std::string, std::vector<CVUniverse*>> truthunivs;
     truthHist = new HIST((varName+"_truth").c_str(), (varName + " [" + varUnits + "]").c_str(), nBins, minBin, maxBin, truthunivs);
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

       truthHist->SyncCVHistos();
       truthHist->hist->Write();

       //TODO: You could do plotting here
    }

  private:
    using HIST = PlotUtils::HistWrapper<CVUniverse>;
    typedef PlotUtils::HistWrapper<CVUniverse> HW;
    reco_t fReco;

    util::Categorized<HIST, int>* m_VarToGENIELabel;
    HW* dataHist;
    HW* totalMCHist;
    HW* truthHist;

    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
         (*dataHist).FillUniverse(&univ, fReco(univ, evt, whichMichel), 1.0);
      }
    }

    //All of your plots happen here so far.
    void fillSelectedSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
      {
        (*totalMCHist).FillUniverse(&univ, fReco(univ, evt, whichMichel), weight);
    
        (*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fReco(univ, evt, whichMichel), weight);
      }
    }

    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
 	/*for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
        {
          (*truthHist).FillUniverse(&univ, fReco(univ, evt, whichMichel), weight);
	}*/

     }
};
