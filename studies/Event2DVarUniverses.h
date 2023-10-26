//studies includes
#include "studies/Study2D.h"
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
#include "PlotUtils/Model.h"
//#include "studies/TruthInteractionStudies.h"
#include "util/PionFSCategory.h"

//c++ includes
#include <functional> //for std::function

struct eVarConfig2D
{
  std::string name;
  std::string units;
  int nBins;
  std::vector<double> bin;
};

class Event2DVarUniverses: public Study2D
{
  public:
    //PerMichelVarByGENIELabel fills a histogram with 1 entry per Michel with some variable calculated from that Michel.  Your function will get to see the CVUniverse, the MichelEvent (= reconstructed Michels), and which Michel it's looping over.
    using reco_t = std::function<double(const CVUniverse&, const MichelEvent&)>;
    Event2DVarUniverses(reco_t xreco, reco_t yreco, const eVarConfig2D xVar, const eVarConfig2D yVar, std::map<std::string, std::vector<CVUniverse*>>& univs): Study2D(), fxReco(xreco), fyReco(yreco), dataHist(new HIST((yVar.name + "_vs_" + xVar.name + "_DATA").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs)), totalMCHist(new HIST((yVar.name + "_vs_" + xVar.name + "_MC").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs))//,fSignalByPionsInVar(pionFSCategories,("npi_"+yVar.name + "_vs_" + xVar.name).c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs) 
     {
	std::map<int, std::string> GENIELabels = {{1, "QE"},
                                                 {8, "2p2h"},
                                                 {2, "RES"},
                    				 {4, "COH"},
		                                 {3, "DIS"}}; //other category is built in for free. 
//      std::map<int, std::string> GENIELabels = {{0, "All?"}}; //other category is built in for free.  
        std::cout << "Filling 2D Variable bin hists" << std::endl;
 	m_VarToGENIELabel = new util::Categorized<HIST, int>(("GENIE_"+yVar.name + "_vs_" + xVar.name).c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]"), GENIELabels, xVar.bin, yVar.bin, univs); 
        fSignalByPionsInVar = new util::Categorized<HIST, FSCategory*>(pionFSCategories,("top_"+yVar.name + "_vs_" + xVar.name).c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs);
        bkgHist = new HIST((yVar.name + "_vs_" + xVar.name + "_bkg").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs);
        truthHist = new HIST((yVar.name + "_vs_" + xVar.name + "_efficiencynumerator").c_str(), (xVar.name + " [" + xVar.units + "];" + yVar.name + " [" + yVar.units + "]").c_str(), xVar.bin, yVar.bin, univs);
    }
    /*   
    void fillHistForAllUniverses(const PlotUtils::Hist2DWrapper<CVUniverse>& histw, const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt)
    {
         const int whichBin = histw.univHist(univs.front())->FindBin(fxReco(*univs.front(), evt), fyReco(*univs.front(), evt));
         for (const auto univ: univs)
         {
             auto uniHist = histw.univHist(univ); //Getting the histogram for this universse from the histwrapper 
             const double weight = model.GetWeight(*univ, evt);
             uniHist->AddBinContent(whichBin, weight);
             uniHist->SetEntries(uniHist->GetEntries()+1);

             const double err = uniHist->GetBinError(whichBin);
             const double newErr = err*err + weight*weight;
             uniHist->SetBinError(whichBin,(0 < newErr)?std::sqrt(newErr):0);

         } // end of loop over universe
    } // end of fillHistForallUniverses 
    */
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

       bkgHist->SyncCVHistos();
       bkgHist->hist->Write();

       truthHist->SyncCVHistos();
       truthHist->hist->Write();
       //TODO: You could do plotting here
    }

  private:
    using HIST = PlotUtils::Hist2DWrapper<CVUniverse>;
    reco_t fxReco, fyReco;

    util::Categorized<HIST, int>* m_VarToGENIELabel;
    util::Categorized<HIST, FSCategory*>* fSignalByPionsInVar;
    HIST* totalMCHist;
    HIST* dataHist;
    HIST* bkgHist;
    HIST* truthHist;
    //Overriding base class functions
    //Do nothing for now...  Good place for data comparisons in the future. 
    void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) {
         (*dataHist).FillUniverse(&univ, fxReco(univ, evt), fyReco(univ, evt), weight);
    }

    //All of your plots happen here so far.
    void fillSelectedSignal(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt)
    {

        fillHistForAllUniverses((*totalMCHist), univs,model, evt);
        //(*totalMCHist).FillUniverse(&univ, fxReco(univ, evt), fyReco(univ, evt), weight);
        
        fillHistForAllUniverses((*m_VarToGENIELabel)[univs.front()->GetInteractionType()], univs,model, evt);

        //(*m_VarToGENIELabel)[univ.GetInteractionType()].FillUniverse(&univ, fxReco(univ, evt), fyReco(univ, evt), weight);
        const auto pionCat = std::find_if(pionFSCategories.begin(), pionFSCategories.end(), [&univs](auto& category) { return (*category)(*univs.front()); });
        fillHistForAllUniverses((*fSignalByPionsInVar)[*pionCat],  univs,model, evt);
        //(*fSignalByPionsInVar)[*pionCat].FillUniverse(&univ, fxReco(univ,evt), fyReco(univ, evt), weight);
    }
    
    void fillBackground(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt)
    {
        fillHistForAllUniverses(*bkgHist, univs,model, evt);
        //(*bkgHist).FillUniverse(&univ, fxReco(univ, evt), fyReco(univ, evt), weight);
    }
    
    void fillHistForAllUniverses(const HIST& histw, const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt)
    {
         //std::cout << "Filling HISTS for universes " << univs.size() << std::endl;
         //std::cout << "Printing name of front universe " << univs.front()->ShortName() << std::endl;
         //CVUniverse* uni = 
         //std::cout << "Print hist name " << histw.hist->GetName() << std::endl;
         const int whichBin = histw.univHist(univs.front())->FindBin(fxReco(*univs.front(), evt), fyReco(*univs.front(), evt));
         
         //histw.AddUniverses(missinguniverse);
         
         //std::cout << "Found which bin " << whichBin << std::endl;
         for (const auto univ: univs)
         {
            // std::cout << "Filling Hist for univ: " << univ->ShortName() << std::endl;
             auto uniHist = histw.univHist(univ); //Getting the histogram for this universse from the histwrapper 
             const double weight = model.GetWeight(*univ, evt);
             uniHist->AddBinContent(whichBin, weight);
             uniHist->SetEntries(uniHist->GetEntries()+1);

             const double err = uniHist->GetBinError(whichBin);
             const double newErr = err*err + weight*weight;
             uniHist->SetBinError(whichBin,(0 < newErr)?std::sqrt(newErr):0);
 
         } // end of loop over universe
    } // end of fillHistForallUniverses
    
    //Do nothing for now...  Good place for efficiency denominators in the future.
    void fillTruthSignal(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) {
         int blah = 1;
	 //fillHistForAllUniverses((*truthHist), univs,model,evt);
         //Do Nothing for now
    }
};
