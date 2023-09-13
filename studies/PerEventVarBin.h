//studies includes
#include "studies/Study.h"

//Mehreen's includes
#include "event/MichelEvent.h"
#include "util/Categorized.h"
#include "event/CVUniverse.h"

//c++ includes
#include <functional> //for std::function

class PerEventVarBin: public Study
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&)>;

    PerEventVarBin(reco_t reco, const std::string& varName, const std::string& varUnits, const int nbins, std::vector<double>& binvec, std::map<std::string, std::vector<CVUniverse*>>& univs): Study(), fReco(reco), dataHist(new HIST((varName+"_data").c_str(), (varName + "_" + varUnits + "_").c_str(),nbins, binvec, univs)), totalMCHist(new HIST((varName+"_MC").c_str(), (varName + "_" + varUnits + "_").c_str(), nbins, binvec, univs))

    {
      std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                {8, "2p2h"},
                                                {2, "RES"},
                                                {3, "DIS"}};
      m_VarToGENIELabel = new util::Categorized<HIST, int>(varName, varName + " [" + varUnits + "]", GENIELabels, nbins, binvec, univs);
    }

    void SaveOrDraw(TDirectory& outDir)
    {
       outDir.cd();
       m_VarToGENIELabel->visit([](HIST& wrapper)
                                {
                                  wrapper.SyncCVHistos();
                                  wrapper.hist->Write();
                                });
       std::cout << "Drawing the DATA Histogram" << std::endl;
       dataHist->SyncCVHistos();
       dataHist->hist->Write();

       std::cout << "DRAWING the TOTAL MC histogram" << std::endl;
       totalMCHist->SyncCVHistos();
       totalMCHist->hist->Write();


       //TODO: You could do plotting here
    }

  private:
    using HIST = PlotUtils::HistWrapper<CVUniverse>;

    reco_t fReco;
    HIST* dataHist;
    HIST* totalMCHist;
    util::Categorized<HIST, int>* m_VarToGENIELabel;

    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
	 (*dataHist).FillUniverse(&univ, fReco(univ, evt), 1.0);

	}

    //All of your plots happen here so far.
    void fillSelectedSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
     
        (*totalMCHist).FillUniverse(&univ, fReco(univ, evt), weight);
   
        (*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fReco(univ, evt), weight);
    }

    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight) {}
};
