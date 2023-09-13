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
//#include "util/PionFSCategory.h"
#include "util/PionProtonFSCategory.h"
//Includes from this package
//#include "event/CVUniverse.h"
//#include "util/Categorized.h"
//
////PlotUtils includes
//c++ includes
#include <functional> //for std::function
//c++ includes
#include <unordered_set>


class PerMichelVarVecFSPart: public Study
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&, const int)>;
    PerMichelVarVecFSPart(reco_t reco, const std::string& varName, const std::string& varUnits,int& nbins,std::vector<double>& binvec, std::map<std::string, std::vector<CVUniverse*>>& univs): Study(), fReco(reco), dataHist(new HIST((varName+"_data").c_str(), (varName + "_" + varUnits).c_str(),nbins, binvec, univs)), totalMCHist(new HIST((varName+"_MC").c_str(), (varName + "_" + varUnits + "_").c_str(), nbins, binvec, univs)), fSignalByPionsInVar(pionFSCategories,(varName+"_top").c_str(),varName,binvec,univs) 
    {
        std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                                                 {4, "COH"},
                                                 {3, "DIS"}}; //other category is built in for free.
      //	m_VarToGENIELabel = new util::Categorized<HIST, int>(("GENIE_"+varName).c_str(), varName + "_" + varUnits, GENIELabels, nbins, binvec ,univs);
           
 	bkgHist = new HIST((varName+"_bkg").c_str(), (varName + "_" + varUnits + "_").c_str(), nbins, binvec, univs);
        sigHist = new HIST((varName+"_signal").c_str(), (varName + "_" + varUnits + "_").c_str(), nbins, binvec, univs);

    }
    

    void SaveOrDraw(TDirectory& outDir)
    {
       outDir.cd();

       std::cout << "Drawing the FSPart categorized histogram" << std::endl;
       	fSignalByPionsInVar.visit([](auto& Hist) { 
						    Hist.SyncCVHistos(); 
						    Hist.hist->Write();
						});
       //m_VarToGENIELabel->visit([](HIST& wrapper)
       //                         {
       //                           wrapper.SyncCVHistos();
       //                           wrapper.hist->Write();
       //                         });
       
       std::cout << "Drawing the DATA Histogram" << std::endl;
         dataHist->SyncCVHistos();
         dataHist->hist->Write();
         std::cout << "DRAWING the TOTAL MC histogram" << std::endl;
       
         totalMCHist->SyncCVHistos();
         totalMCHist->hist->Write();
  
         bkgHist->SyncCVHistos();
         bkgHist->hist->Write();

         sigHist->SyncCVHistos();
         sigHist->hist->Write();     
       //std::cout << "DRAWING THE TRUTH HISTOGRAM" << std::endl;
       
       //truthHist->SyncCVHistos();
       //truthHist->hist->Write();
       //std::cout << "TRUTH HISTOGRAM DRAWN" << std::endl;
       //TODO: You could do plotting here
    }

  private:
    using HIST = PlotUtils::HistWrapper<CVUniverse>;
    typedef PlotUtils::HistWrapper<CVUniverse> HW;
    reco_t fReco;
    HW* bkgHist;
    HW* dataHist;
    HW* totalMCHist;
    HW* sigHist;
    util::Categorized<HIST, FSCategory*> fSignalByPionsInVar;
   // util::Categorized<HIST, int>* m_VarToGENIELabel;

    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
     // for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
     // {
         (*dataHist).FillUniverse(&univ, fReco(univ, evt, 0), weight);
     // }
    }

    //All of your plots happen here so far.
    void fillSelectedSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      //std::cout << "Printing Universe Name: " << univ.ShortName() << std::endl;
      //std::cout << " Pinrting N Michels " << evt.m_nmichels.size() << std::endl;
      
     // for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
     // {
        //std::cout << "Printing Universe Name: " << univ.ShortName() << std::endl;
        
	(*totalMCHist).FillUniverse(&univ, fReco(univ, evt, 0), weight);
     //   (*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fReco(univ, evt, whichMichel), weight);    
        const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univ](auto& category) { return (*category)(univ); });
        fSignalByPionsInVar[*pionCat].FillUniverse(&univ, fReco(univ,evt,0), weight);
     // }
    }

    void fillBackground(const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      //for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
     // {
        (*bkgHist).FillUniverse(&univ, fReco(univ, evt, 0), weight);
     // }
    }
    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
       (*sigHist).FillUniverse(&univ, fReco(univ, evt, 0), weight);	
       /*for(size_t whichMichel = 0; whichMichel < evt.m_nmichels.size(); ++whichMichel)
        {
          (*truthHist).FillUniverse(&univ, fReco(univ, evt, whichMichel), weight);
	}*/

    }
};
