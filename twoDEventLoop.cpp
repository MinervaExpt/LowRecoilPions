#define MC_OUT_FILE_NAME "runEventLoopMC.root"
#define DATA_OUT_FILE_NAME "runEventLoopData.root"
#define MC_SIDE_FILE_NAME "runEventLoopMC_Aug312022_Sideband_nopmu_noreweight.root"
#define DATA_SIDE_FILE_NAME "runEventLoopDATA_Aug312022_Sideband_nopmu_noreweight.root"


#define USAGE \
"\n*** USAGE ***\n"\
"runEventLoop <dataPlaylist.txt> <mcPlaylist.txt>\n\n"\
"*** Explanation ***\n"\
"Reduce MasterAnaDev AnaTuples to event selection histograms to extract a\n"\
"single-differential inclusive cross section for the 2021 MINERvA 101 tutorial.\n\n"\
"*** The Input Files ***\n"\
"Playlist files are plaintext files with 1 file name per line.  Filenames may be\n"\
"xrootd URLs or refer to the local filesystem.  The first playlist file's\n"\
"entries will be treated like data, and the second playlist's entries must\n"\
"have the \"Truth\" tree to use for calculating the efficiency denominator.\n\n"\
"*** Output ***\n"\
"Produces a two files, " MC_OUT_FILE_NAME " and " DATA_OUT_FILE_NAME ", with\n"\
"all histograms needed for the ExtractCrossSection program also built by this\n"\
"package.  You'll need a .rootlogon.C that loads ROOT object definitions from\n"\
"PlotUtils to access systematics information from these files.\n\n"\
"*** Environment Variables ***\n"\
"Setting up this package appends to PATH and LD_LIBRARY_PATH.  PLOTUTILSROOT,\n"\
"MPARAMFILESROOT, and MPARAMFILES must be set according to the setup scripts in\n"\
"those packages for systematics and flux reweighters to function.\n"\
"If MNV101_SKIP_SYST is defined at all, output histograms will have no error bands.\n"\
"This is useful for debugging the CV and running warping studies.\n\n"\
"*** Return Codes ***\n"\
"0 indicates success.  All histograms are valid only in this case.  Any other\n"\
"return code indicates that histograms should not be used.  Error messages\n"\
"about what went wrong will be printed to stderr.  So, they'll end up in your\n"\
"terminal, but you can separate them from everything else with something like:\n"\
"\"runEventLoop data.txt mc.txt 2> errors.txt\"\n"

enum ErrorCodes
{
  success = 0,
  badCmdLine = 1,
  badInputFile = 2,
  badFileRead = 3,
  badOutputFile = 4
};

//PlotUtils includes
//No junk from PlotUtils please!  I already
//know that MnvH1D does horrible horrible things.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverloaded-virtual"

//Includes from this package
#include "event/CVUniverse.h"
#include "PlotUtils/LowRecoilPionReco.h"
typedef LowRecoilPion::Michel<CVUniverse> Michel;
typedef LowRecoilPion::MichelEvent<CVUniverse> MichelEvent;
#include "systematics/Systematics.h"
#include "cuts/MaxPzMu.h"
#include "util/Variable.h"
#include "util/Variable2DResp.h"
//#include "util/Variable2D.h"
#include "util/GetFluxIntegral.h"
#include "util/GetPlaylist.h"
//#include "cuts/SignalDefinition.h"
//#include "util/TruthInteractionStudies.h"
#include "util/PionFSCategory.h"
#include "cuts/q3RecoCut.h"
#include "studies/Study.h"
//#include "studies/PerMichelVarByGENIELabel.h"
#include "studies/PerMichelEventVarByGENIELabel.h"  
#include "studies/PerMichel2DVar.h"
#include "studies/PerMichel2DVarbin.h"
#include "studies/PerMichelVarVec.h"
#include "studies/PerMichelEvent2DVarBin.h"
#include "studies/PerEventVarBin.h"
#include "studies/PerMichelVarVecFSPart.h"
#include "PlotUtils/LowRecoilPionCuts.h"
#include "cuts/BestMichelDistance2D.h"
#include "cuts/VtxMatchFirst.h"
//#include "cuts/hasTruePion.h"
#include "cuts/PTRangeReco.h"
#include "cuts/GetClosestMichel.h"
#include "cuts/Distance2DSideband.h"
#include "cuts/RecoilERange.h"
#include "cuts/PmuCut.h"
#include "cuts/PzmuCut.h"
#include "cuts/WexpRecoCut.h"
#include "event/SetDistanceMichelSideband.h"
#include "event/SetDistanceMichelSelection.h"
#include "event/GetClosestMichel.h"
//#include "Binning.h" //TODO: Fix me

//Sideband Cuts
//
//#include "cuts/RemoveSignalEvents.h"
//#include "cuts/GetClosestMichelSide.h"
//#include "cuts/Distance2DSideband.h"


//PlotUtils includes
#include "PlotUtils/makeChainWrapper.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"
#include "PlotUtils/MacroUtil.h"
#include "PlotUtils/MnvPlotter.h"
#include "MinervaUnfold/MnvResponse.h"
#include "PlotUtils/CCInclusiveCuts.h"
#include "PlotUtils/CCInclusiveSignal.h"
#include "PlotUtils/CrashOnROOTMessage.h" //Sets up ROOT's debug callbacks by itself
#include "PlotUtils/Cutter.h"
#include "PlotUtils/Model.h"
#include "PlotUtils/FluxAndCVReweighter.h"
#include "PlotUtils/GENIEReweighter.h"
#include "PlotUtils/LowRecoil2p2hReweighter.h"
#include "PlotUtils/RPAReweighter.h"
#include "PlotUtils/MINOSEfficiencyReweighter.h"
#include "PlotUtils/TargetUtils.h"
#include "PlotUtils/WeightFunctions.h"
#include "PlotUtils/MKReweighter.h" 
#include "util/PionReweighter.h"
#include "util/DiffractiveReweighter.h"
#include "cuts/LowRecPionSignal.h"
#include "PlotUtils/LowQ2PiReweighter.h"
#include "PlotUtils/GeantNeutronCVReweighter.h"
#include "PlotUtils/FSIReweighter.h"
#include "util/COHPionReweighter.h"
#include "util/TargetMassReweighter.h"
#include "util/MnvTunev431Reweighter.h"
//#include "util/BkgSigReweighter.h"
#include "util/BkgSideReweighter.h"
#pragma GCC diagnostic pop

//ROOT includes
#include "TParameter.h"

//c++ includes
#include <iostream>
#include <cstdlib> //getenv()
#include <fstream>
//==============================================================================
// Loop and Fill
//==============================================================================
void LoopAndFillEventSelection(
    PlotUtils::ChainWrapper* chain,
    std::map<std::string, std::vector<CVUniverse*> > error_bands,
    std::vector<Variable*> vars,
    std::vector<Variable2D*> vars2D,
    std::vector<Study*> studies,
    std::vector<Variable*> sidevars,
    std::vector<Variable2D*> sidevars2D,
    //std::vector<Study*> sideband_studies,
    PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
    PlotUtils::Model<CVUniverse, MichelEvent>& model)
{
  assert(!error_bands["cv"].empty() && "\"cv\" error band is empty!  Can't set Model weight.");
  auto& cvUniv = error_bands["cv"].front();

  //std::cout << " Opening csv for resolution " << std::endl;

  

  std::cout << "Starting MC reco loop...\n";
  const int nEntries = chain->GetEntries(); // TODO: July 10 CHANGE BACK TO GETENTRIES
  for (auto i=0; i < nEntries; ++i)
  {
      
    if(i%10000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
    //std::cout << "Now Printing for Event " << i << std::endl;
    //MichelEvent cvEvent;
    cvUniv->SetEntry(i);
    MichelEvent cvEvent;
    model.SetEntry(*cvUniv, cvEvent); 
    const double cvWeight = model.GetWeight(*cvUniv, cvEvent);// TODO: Put this model weight back. model.GetWeight(*cvUniv, cvEvent); 
    //auto cvcuts = michelcuts;
    const auto cvCutResults =  michelcuts.isMCSelected(*cvUniv, cvEvent, cvWeight);
    //if (!cvCutResults.all()) continue;
 
    //std::cout << "Printing Muon Momentum At Beginning of Entry Loop: " << cvUniv->GetMuonPTTrue() << std::endl;
    //std::cout << " ============================================================= " << std::endl;
    //=========================================
    // Systematics loop(s)
    //=========================================
    for (auto band : error_bands)
    {
      int unv_count = -1;
      std::vector<CVUniverse*> error_band_universes = band.second;
      for (auto universe : error_band_universes)
      {
        //int unv_count = 0;
        //if (universe->ShortName() != "cv") continue;
        //MichelEvent myevent; // make sure your event is inside the error band loop. 
        //if (universe->ShortName() != "cv") continue;
        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);
        MichelEvent myevent;// = cvEvent; //Use MichelEvent from CV so you dont have to redo michel matches
        if (universe->ShortName() != "cv") myevent = cvEvent;
        //std::cout << " Printing universe name " << universe->ShortName() << std::endl;
        const auto cutResults = michelcuts.isMCSelected(*universe, myevent, cvWeight);
	//const auto cutResults = michelcuts.isDataSelected(*universe, myevent);       
        //if (universe->ShortName() != "cv") continue;
        ++unv_count;
        if (!cutResults.all()) continue;
        
        const double weight2 = model.GetWeight(*universe, myevent);
	if (cutResults.all()){
		for(auto& study: studies) study->SelectedSignal(*universe, myevent, weight2);
		//std::cout << "THIS IS ENTRY NUMBER " << i << " AND UNIVERSE NAME : " << universe->ShortName() << std::endl;

	        double dist = myevent.m_nmichels[0].Best3Ddist;
   	        double recotpi = universe->GetTpiFromRange(dist)/1000.; // in GeV
		TVector3 p_pi(myevent.m_nmichels[0].reco_ppix, myevent.m_nmichels[0].reco_ppiy, myevent.m_nmichels[0].reco_ppiz); // MeV
                int idxpi = universe->GetTrueLowestTpiIdx(); // integer         
                double t_truth = universe->GetTruet(idxpi);
                double t_reco = universe->Gettreco(recotpi,p_pi);
		double eavreco = universe->NewEavailGEV();
	        double EavMinusTpi = eavreco - recotpi;
	        if (recotpi > eavreco) {EavMinusTpi = 0.0; }
		double trueEavMinTpi = universe->GetTrueEavMinusTpiGEV(); //GetTrueEavMinusTpi()/1000.; // -9999.;
		double trueTpi = universe->GetTrueLowestTpiEventGEV(); //-9999.;
         	//std::cout << " Printing True E Avail - Tpi  (GeV) " << trueEavMinTpi << std::endl;
                //std::cout << "reco |t| = " << t_reco << std::endl;
		//std::cout << "truth |t| = " << t_truth << std::endl;	
		//std::cout << "Printing relevant vars: recotpi " << recotpi << std::endl;
   		for(auto& var: vars) {
			if (var->GetName() == "PionRange"){
			   var->selectedMCReco->FillUniverse(universe, dist, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, dist, weight2);
                           var->FillCategHistos(*universe,dist, weight2);
			}
        		else if (var->GetName() == "Tpi"){
			   var->selectedMCReco->FillUniverse(universe, recotpi, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, recotpi, weight2);
                           var->FillCategHistos(*universe,recotpi, weight2);
			   //trueTpi = var->GetTrueValue(*universe);
			}
			else if (var->GetName() == "EavMinusTpi"){
			   //double eavtpidiff = EavMinusTpi;
			   //std::cout << "Printing relevant vars: EavMinusTpi " << eavtpidiff << std::endl;
			   var->selectedMCReco->FillUniverse(universe, EavMinusTpi, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, EavMinusTpi, weight2);
                           var->FillCategHistos(*universe,EavMinusTpi, weight2);
		        }
			else if (var->GetName() == "texp"){
			   var->selectedMCReco->FillUniverse(universe, t_reco, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, weight2);
                           var->FillCategHistos(*universe,t_reco, weight2);
			}
			else if (var->GetName() == "Tpibins"){
			   var->selectedMCReco->FillUniverse(universe, recotpi, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, recotpi, weight2);
                           var->FillCategHistos(*universe,recotpi, weight2);
			}
			else if (var->GetName() == "EavMinTpibins"){
			   //double eav = var->GetRecoValue(*universe);
                           //double eavtpidiff = eav - recotpi;
                           //if (recotpi > eav) eavtpidiff = 0.0;
			   var->selectedMCReco->FillUniverse(universe, EavMinusTpi, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, EavMinusTpi, weight2);
                           var->FillCategHistos(*universe,EavMinusTpi, weight2);
			}
			else{
				if (var->GetName() == "Abin") continue;
				if (var->GetName() == "Bbin") continue;
				if (var->GetName() == "Cbin") continue;
				if (var->GetName() == "Dbin") continue; // Skip these 1D hists. Only matter for figuring out COH reweight stuff for now. Oct 4 2023
				var->selectedMCReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //"Fake data" for closure
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
                                var->FillCategHistos(*universe,var->GetRecoValue(*universe), weight2);
			
			}
			
  		}
        	for(auto& var: vars2D) {
  			const std::string keyName = var->GetName();
    			const size_t endOfPrefix1 = keyName.find("EavMinusTpi_pTmub");
    			const size_t endOfPrefix2 = keyName.find("Tpi_pTmu");
			const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");

			const size_t endOfPrefix6 = keyName.find("Abin_Tpibins");
			const size_t endOfPrefix7 = keyName.find("Bbin_Tpibins");
			const size_t endOfPrefix8 = keyName.find("Cbin_Tpibins");
			const size_t endOfPrefix9 = keyName.find("Dbin_Tpibins");
		        //std::cout << "looping over 2D vars" << std::endl;
		        //double   trueeavminustpi = trueEavMinTpi;
			if(endOfPrefix1 != std::string::npos){
			  //double eavtpidiff = EavMinusTpi;
			  var->mcTotalHist->FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);
                          (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);
                          var->FillCategHistos(*universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2); 
			}
			else if(endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos && endOfPrefix4 == std::string::npos && endOfPrefix5 == std::string::npos){
			   var->mcTotalHist->FillUniverse(universe, recotpi, var->GetRecoValueY(*universe), weight2);
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, recotpi, var->GetRecoValueY(*universe), weight2);
                           var->FillCategHistos(*universe,recotpi, var->GetRecoValueY(*universe), weight2);
		
			}
			else if(endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
			   var->mcTotalHist->FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);
                           var->FillCategHistos(*universe,t_reco, var->GetRecoValueY(*universe), weight2);
			}
			else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
			   //if (abs(EavMinusTpi) > 0.025 and  abs(EavMinusTpi) < 0.050){
			      //std::cout << "EavMinusTpi < 25 MeV. Interaction Type: " << universe->GetInteractionType() << std::endl;  
			      var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                              (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                              var->FillCategHistos(*universe,t_reco, recotpi, weight2);
			   //}
			}
			else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
		 	   //double eav = var->GetRecoValueY(*universe);
                           //double eavtpidiff = eav - recotpi;
                           //if (recotpi > eav) eavtpidiff = 0.0;
			   var->mcTotalHist->FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                           var->FillCategHistos(*universe,t_reco, EavMinusTpi, weight2);

			}
			else if (endOfPrefix6 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
			   if (EavMinusTpi >= 0.0 && EavMinusTpi < 0.025){
				
				/*if (t_reco < 0.09) {
					std::cout << " ==================================== " << std::endl;
					universe->PrintTrueArachneLink();
					std::cout << "Reco EavMinustpi is " << EavMinusTpi << " Tpi is: " << recotpi << std::endl;
					std::cout << "True EavMinustpi is " << trueEavMinTpi << " Tpi is: " << trueTpi << std::endl;
					std::cout << "|t| reco is " << t_reco << std::endl;
					std::cout << " ==================================== " << std::endl; 
			 	} */
				var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                                var->FillCategHistos(*universe,t_reco, recotpi, weight2);
			   }
			}
			else if (endOfPrefix7 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                           if (EavMinusTpi >= 0.025 && EavMinusTpi < 0.050){
                                 /* 
				 if (t_reco > 0.04 && t_reco < 0.1) {
                                        std::cout << " ==================================== " << std::endl;
                                        universe->PrintTrueArachneLink();
                                        std::cout << "Reco EavMinustpi is " << EavMinusTpi << " Tpi is: " << recotpi << std::endl;
                                        std::cout << "True EavMinustpi is " << trueEavMinTpi << " Tpi is: " << trueTpi << std::endl;
                                        std::cout << "|t| reco is " << t_reco << std::endl;
                                        std::cout << " ==================================== " << std::endl;
                                }*/

				var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                                var->FillCategHistos(*universe,t_reco, recotpi, weight2);
                           }     
                        }
			else if (endOfPrefix8 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                           if (EavMinusTpi >= 0.05 && EavMinusTpi < 0.075){
                                var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                                var->FillCategHistos(*universe,t_reco, recotpi, weight2);
                           }
                        }
			else if (endOfPrefix9 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                           if (EavMinusTpi >= 0.075 && EavMinusTpi < 0.150){
                                var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                                var->FillCategHistos(*universe,t_reco, recotpi, weight2);
                           }
                        }	
			else {
                 	   var->mcTotalHist->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                	   (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                	   var->FillCategHistos(*universe,var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
			}
         	}

        	//for(auto& study: studies) study->SelectedSignal(*universe, myevent, weight2);

        	const bool isSignal = michelcuts.isSignal(*universe, weight2);
		const bool isPhaseSpace = michelcuts.isPhaseSpace(*universe, weight2);
		if(isSignal){
			for(auto& study: studies) study->TruthSignal(*universe, myevent, weight2);
			for(auto& var: vars)
          		{
				double diff = var->GetTrueValue(*universe) - var->GetRecoValue(*universe);
				if (var->GetName() == "PionRange"){	
					var->selectedSignalReco->FillUniverse(universe, dist, weight2);
				}
				if (var->GetName() == "Tpi"){
					var->migration->FillUniverse(universe, recotpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, recotpi, weight2); //Ef
				
					//double resolution = (recotpi - var->GetTrueValue(*universe)); //var->GetTrueValue(*universe);
					//var->biasMCReco->FillUniverse(universe, resolution, weight2);
				}
				else if (var->GetName() == "EavMinusTpi"){ // Get the Variable that is the difference between Available Energy and Tpi. Non pion available energy. Not sure yet what to do about case with multiple pions. 
                           		//double eav = var->GetRecoValue(*universe);
                           		//double eavtpidiff = eav - recotpi;
					//if (recotpi > eav) eavtpidiff = 0.0;
					//std::cout << "Printing Reco Eavail - Tpi inside 1D signal loop " << EavMinusTpi << std::endl;
					//std::cout << "Printing True Eavail - Tpi inside 1D signal loop " << trueEavMinTpi << std::endl;
					var->migration->FillUniverse(universe, EavMinusTpi,  trueEavMinTpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe,  trueEavMinTpi, weight2);
                                        var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, weight2);
					//double resolution = (EavMinusTpi - var->GetTrueValue(*universe)); ///var->GetTrueValue(*universe);
					//var->biasMCReco->FillUniverse(universe, resolution, weight2);
				}
				else if (var->GetName() == "texp"){
					continue;
					var->migration->FillUniverse(universe, t_reco, t_truth, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, weight2);
                                        var->selectedSignalReco->FillUniverse(universe, t_reco, weight2);
				        //double resolution = (t_reco - t_truth);// /t_truth;
					//var->biasMCReco->FillUniverse(universe, resolution, weight2);
					
				}
				else if (var->GetName() == "Tpibins"){

					continue;
					var->migration->FillUniverse(universe, recotpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, recotpi, weight2);
				 	//double resolution = (recotpi - var->GetTrueValue(*universe));///var->GetTrueValue(*universe);
                                        //var->biasMCReco->FillUniverse(universe, resolution, weight2);
			
				}
				else if (var->GetName() == "EavMinTpibins"){
					//double eav = var->GetRecoValue(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
                               		continue;
				        var->migration->FillUniverse(universe, EavMinusTpi,  trueEavMinTpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe,  trueEavMinTpi, weight2);
                                        var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, weight2);
					//double resolution = (EavMinusTpi - var->GetTrueValue(*universe));///var->GetTrueValue(*universe);
                                        //var->biasMCReco->FillUniverse(universe, resolution, weight2);

				}
				else {
					if (var->GetName() == "Abin") continue;
                                	if (var->GetName() == "Bbin") continue;
                                	if (var->GetName() == "Cbin") continue;
                                	if (var->GetName() == "Dbin") continue; // Skip these 1D hists. Only matter for figuring out COH reweight stuff for now. Oct 4 2023
					var->migration->FillUniverse(universe, var->GetRecoValue(*universe), var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //Efficiency numerator in reco variables.  Useful for warping studies 
					//double resolution = (var->GetRecoValue(*universe) - var->GetTrueValue(*universe));///var->GetTrueValue(*universe);
					//var->biasMCReco->FillUniverse(universe, resolution, weight2);
	
				}
				//if (var->GetName() == "AvailableE" and var->GetTrueValue(*universe) > 2000.) universe->PrintTrueArachneLink();			
			}
			for(auto& var: vars2D)
          		{
                 		const std::string keyName = var->GetName();
                        	const size_t endOfPrefix1 = keyName.find("EavMinusTpi_pTmu");
                        	const size_t endOfPrefix2 = keyName.find("Tpi_pTmu");
				const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        	const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        	const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");
				const size_t endOfPrefixAv = keyName.find("AvailableE_pTmubi");
				const size_t endOfPrefix6 = keyName.find("Abin_Tpibins");
                        	const size_t endOfPrefix7 = keyName.find("Bbin_Tpibins");
                        	const size_t endOfPrefix8 = keyName.find("Cbin_Tpibins");
                        	const size_t endOfPrefix9 = keyName.find("Dbin_Tpibins");


				if(endOfPrefix1 != std::string::npos){
				        //std::cout << "this should be prefix1 " <<  keyName << std::endl;
					//std::cout << "Printing Reco Eavail - Tpi inside 2D signal loop " << EavMinusTpi << std::endl;
                                        //std::cout << "Printing True Eavail - Tpi inside 2D signal loop " << trueEavMinTpi << std::endl;
					//std::cout << " ====================== " << std::endl;	
					
					/*if (trueEavMinTpi < 0.00005){
						std::cout << " ====================== " << std::endl;   
						universe->PrintTrueArachneLink();
						std::cout << "True Tpi for the event is " << trueTpi << std::endl;
					 	
						std::cout << "Name of histo " << var->GetName() << " (EavMinTpi) True Value X " << trueEavMinTpi << " and (should be pTmu) " << var->GetTrueValueY(*universe) << std::endl;
						std::cout << "Reco Eav - Tpi " << EavMinusTpi << " with Reco Eav " << eavreco << " and Reco Tpi = " << recotpi << std::endl; 
						//double ratio = EavMinusTpi/trueEavMinTpi;
						std::cout << " ====================== " << std::endl;   
						//std::cout << "Ratio between Reco / True EAvMinusTpi " << ratio << std::endl;
					}*/

					//std::cout << " ========================= " << std::endl;
					
					var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe,  trueEavMinTpi, var->GetTrueValueY(*universe), weight2);
					var->FillResponse(EavMinusTpi, var->GetRecoValueY(*universe), trueEavMinTpi, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
					var->FillCategNumHistos((*universe), trueEavMinTpi, var->GetTrueValueY(*universe), weight2);
					//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);
				}
				else if (endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos && endOfPrefix4 == std::string::npos && endOfPrefix5 == std::string::npos){
				        //std::cout << "this should be prefix2 " <<  keyName << std::endl;
					//std::cout << " ====================== " << std::endl;
				        //std::cout << "Name of histo " << var->GetName() << " (Tpi) True Value X " << var->GetTrueValueX(*universe) << " and (should be pTmu) " << var->GetTrueValueY(*universe) << std::endl; 
					//std::cout << " =======================" << std::endl;
					var->FillCategNumHistos((*universe), var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
					var->selectedSignalReco->FillUniverse(universe, recotpi, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(recotpi, var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
			                //var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);

				}
				else if (endOfPrefixAv != std::string::npos && endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos && endOfPrefix4 == std::string::npos && endOfPrefix5 == std::string::npos){
					var->selectedSignalReco->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
					var->FillCategNumHistos((*universe), var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);

				}
				else if(endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				 	continue;	
					var->selectedSignalReco->FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                        //var->FillResponse(t_reco, var->GetRecoValueY(*universe),t_truth, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
					//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);				

				}
				else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				       continue; 
				    //if ( abs(EavMinusTpi) > 0.025 and abs(EavMinusTpi) < 0.050) {		
					var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                        //var->FillResponse(t_reco, recotpi, t_truth, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
					//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);
				     //}
				}
				else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					continue;
					//double eav = var->GetRecoValueY(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
					var->selectedSignalReco->FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, trueEavMinTpi, weight2);
                                        //var->FillResponse(t_reco, EavMinusTpi, t_truth, trueEavMinTpi,universe->ShortName(),weight2,unv_count);
					//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);				

				}
				else if(endOfPrefix6 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				     continue;
				     if (abs(EavMinusTpi) < 0.025){


					var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);

				     }		
				}
				else if(endOfPrefix6 == std::string::npos && endOfPrefix7 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                     continue;
				     if (abs(EavMinusTpi) > 0.025 && abs(EavMinusTpi) < 0.050){
					continue;
					var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);

                                     }   
                                }
				else if(endOfPrefix6 == std::string::npos && endOfPrefix8 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                    
				     continue;

				     if (abs(EavMinusTpi) > 0.05 && abs(EavMinusTpi) < 0.075){
                                
                                        var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                     
                                     }
                                }
				else if(endOfPrefix6 == std::string::npos && endOfPrefix9 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                    


				     continue;
				     if (abs(EavMinusTpi) > 0.075 && abs(EavMinusTpi) < 0.150){
                                
                                        var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                     
                                     }
                                }
				
				else{

					continue;
					var->selectedSignalReco->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
					//var->FillResponse(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
					//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);
				}
	
				//var->m_response->Fill(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),unv_count,weight2);			 		
	
	  		}
		} // end of if Signal()
                else if (!isSignal){
		 /*	
			std::cout << " -------------- " << std::endl;	
			std::cout << "This is a background Event " << std::endl;
			universe->PrintTrueArachneLink();
			if(universe->GetTrueNPionsinEvent() == 0) std::cout << "Event has no FS pions " << std::endl;
	                if(universe->GetCurrent()==1) std::cout << "Wrong Sign of muon " << std::endl;
        	        if(universe->GetCurrent()==2) std::cout << "Neutral Current Event " << std::endl;
			std::cout << " -------------- " << std::endl;
          	 */
			for(auto& study: studies) study->BackgroundSelected(*universe, myevent, weight2);	
			int bkgd_ID = -1;
          		if (universe->GetCurrent()==2)bkgd_ID=0;
          		else if( universe->GetCurrent()==1) bkgd_ID=1;

		
          		for(auto& var: vars){
				if (var->GetName() == "PionRange"){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, dist, weight2);
				}
          			if (var->GetName() == "Tpi"){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, recotpi, weight2);
				}
				else if (var->GetName() == "EavMinusTpi"){
                                        //double eav = var->GetRecoValue(*universe);
                                        //double eavtpidiff = eav - recotpi;
					//if (recotpi > eav) eavtpidiff = 0.0;
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, EavMinusTpi, weight2);
				}
				else if (var->GetName() == "texp"){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, weight2);
				}
				else if (var->GetName() == "Tpibins"){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, recotpi, weight2);
				}
				else if (var->GetName() == "EavMinTpibins"){
					//double eav = var->GetRecoValue(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
                                        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, EavMinusTpi, weight2);
				}
				else {
				        if (var->GetName() == "Abin") continue;
                                	if (var->GetName() == "Bbin") continue;
                                	if (var->GetName() == "Cbin") continue;
                                	if (var->GetName() == "Dbin") continue; // Skip these 1D hists. Only matter for figuring out COH reweight stuff for now. Oct 4 2023
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
				}	
			}
          		for(auto& var: vars2D){
				const std::string keyName = var->GetName();
                                const size_t endOfPrefix1 = keyName.find("EavMinusTpi_");
                                const size_t endOfPrefix2 = keyName.find("Tpi_");
				const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        	const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        	const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");
			  	const size_t endOfPrefixAv = keyName.find("AvailableE_pTmubi");	
				const size_t endOfPrefix6 = keyName.find("Abin_Tpibins");
                                const size_t endOfPrefix7 = keyName.find("Bbin_Tpibins");
                                const size_t endOfPrefix8 = keyName.find("Cbin_Tpibins");
                                const size_t endOfPrefix9 = keyName.find("Dbin_Tpibins");
				
				if(endOfPrefix1 != std::string::npos){
                                        //double eav = var->GetRecoValueX(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
				        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);

				}
				else if (endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos && endOfPrefix4 == std::string::npos && endOfPrefix5 == std::string::npos){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe,recotpi, var->GetRecoValueY(*universe), weight2);
				}
				else if(endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){

				        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);

				}
				else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				     //if( abs(EavMinusTpi) > 0.025 and  abs(EavMinusTpi) < 0.050 ){
					 (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
				     //}
				}
				else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					 //double eav = var->GetRecoValueY(*universe);
                                         //double eavtpidiff = eav - recotpi;
                                         //if (recotpi > eav) eavtpidiff = 0.0;
					 (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, EavMinusTpi, weight2);

				}
				else if(endOfPrefix6 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                     if (EavMinusTpi >= 0.0 && EavMinusTpi < 0.025){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
				     }
				}
				else if(endOfPrefix6 == std::string::npos && endOfPrefix7 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                     if (EavMinusTpi >= 0.025 && EavMinusTpi < 0.050){
                                        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
                                     }
                                }
				else if(endOfPrefix6 == std::string::npos && endOfPrefix8 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                     if (EavMinusTpi >= 0.050 && EavMinusTpi < 0.075){
                                        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
                                     }
                                }
				else if(endOfPrefix6 == std::string::npos && endOfPrefix9 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                                     if (EavMinusTpi >= 0.075 && EavMinusTpi < 0.150){
                                        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
                                     }
                                }					
				else{
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
				}
			}
        	} // End of else statement for if Signal 
       } // If event passes PreCuts
       //++unv_count;
       //delete universe;
      } // End band's universe loop
    } // End Band loop
  //std::cout << "Printing Muon Momentum At End of Entry Loop: " << cvUniv->GetMuonPTTrue() << std::endl;
  //std::cout << "====================================================== ==========" << std::endl;
  } //End entries loop
  std::cout << " Now Getting Response Objects" << std::endl; 
  for (auto v : vars2D){ 
	if (v->GetName() == "AvailableE_pTmubins" || v->GetName() == "Tpi_pTmubins" || v->GetName() == "EavMinusTpi_pTmubins") v->getResponseObjects(error_bands);
  }
  std::cout << "Finished MC reco loop.\n";
}


void LoopAndFillData( PlotUtils::ChainWrapper* data,
			        std::vector<CVUniverse*> data_band,
				std::vector<Variable*> vars,
                                std::vector<Variable2D*> vars2D,
                                std::vector<Study*> studies,
				std::vector<Variable*> sidevars,
                                std::vector<Variable2D*> sidevars2D,
                                //std::vector<Study*> data_sidebands,
				PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts)

{
  std::cout << "Starting data loop...\n";
  const int nEntries = data->GetEntries(); // TODO: July 10 CHANGE BACK TO GEtENTRIES
  for (int i=0; i <nEntries; ++i) {
   // std::cout << "Now Printing for Event " << i << std::endl;
    //for (auto universe : data_band) {
      const auto universe = data_band.front();
      universe->SetEntry(i);
      if(i%10000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
      //std::cout << "Creating Michel Event" << std::endl;
      //if (universe->ShortName() != "cv") continue;
      MichelEvent myevent; 
      //std::cout << "Applying Cuts to Data Event " << std::endl;
      const auto cutResults = michelcuts.isDataSelected(*universe, myevent);
      if (!cutResults.all()) continue;
      if (cutResults.all()){
		for(auto& study: studies) study->Selected(*universe, myevent, 1);
		double dist = myevent.m_nmichels[0].Best3Ddist; // mm
                double recotpi = universe->GetTpiFromRange(dist)/1000.; //GeV
		TVector3 p_pi(myevent.m_nmichels[0].reco_ppix, myevent.m_nmichels[0].reco_ppiy, myevent.m_nmichels[0].reco_ppiz); // MeV
		double t_reco;
	       	t_reco = universe->Gettreco(recotpi,p_pi);
		double availE = universe->NewEavailGEV();
	        double EavMinusTpi = availE - recotpi;
		if (recotpi > availE) EavMinusTpi = 0.0; 		
		for(auto& var: vars)
      		{

			if (var->GetName() == "PionRange"){
			   var->dataHist->FillUniverse(universe, dist, 1);
			}
      			if (var->GetName() == "Tpi"){
			   var->dataHist->FillUniverse(universe, recotpi, 1);
			}
			else if (var->GetName() == "EavMinusTpi"){
			   var->dataHist->FillUniverse(universe, EavMinusTpi, 1);
			}
			else if (var->GetName() == "texp"){
			   var->dataHist->FillUniverse(universe, t_reco, 1);
			}
			else if (var->GetName() == "Tpibins"){
			   var->dataHist->FillUniverse(universe, recotpi, 1);
			}
			else if (var->GetName() == "EavMinTpibins"){
			   var->dataHist->FillUniverse(universe, EavMinusTpi, 1);
			}
			else{
			   if (var->GetName() == "Abin") continue;
                           if (var->GetName() == "Bbin") continue;
                           if (var->GetName() == "Cbin") continue;
                           if (var->GetName() == "Dbin") continue; // Skip these 1D hists. Only matter for figuring out COH reweight stuff for now. Oct 4 2023
			   var->dataHist->FillUniverse(universe, var->GetRecoValue(*universe), 1);
			}
		}

      		for(auto& var: vars2D)
      		{
        		const std::string keyName = var->GetName();
                        const size_t endOfPrefix1 = keyName.find("EavMinusTpi_pT");
                        const size_t endOfPrefix2 = keyName.find("Tpi_pT");
                	const size_t endOfPrefix3 = keyName.find("texp_eavbins");
			const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");

			const size_t endOfPrefix6 = keyName.find("Abin_Tpibins");
                        const size_t endOfPrefix7 = keyName.find("Bbin_Tpibins");
                        const size_t endOfPrefix8 = keyName.find("Cbin_Tpibins");
                        const size_t endOfPrefix9 = keyName.find("Dbin_Tpibins");


		        if(endOfPrefix1 != std::string::npos){
				//double eav = var->GetRecoValueX(*universe);
                                //double eavtpidiff = eav - recotpi;
                                //if (recotpi > eav) eavtpidiff = 0.0;
				var->dataHist->FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), 1);
			}
			else if(endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos && endOfPrefix4 == std::string::npos && endOfPrefix5 == std::string::npos){
				var->dataHist->FillUniverse(universe, recotpi, var->GetRecoValueY(*universe), 1);
			}
			else if (endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos && endOfPrefix2 == std::string::npos &&  endOfPrefix1 == std::string::npos){
				var->dataHist->FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), 1);
			}
			else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				 var->dataHist->FillUniverse(universe, t_reco, recotpi, 1);
			}
                        else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
				 var->dataHist->FillUniverse(universe, t_reco, EavMinusTpi, 1);
			}
			else if(endOfPrefix6 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                             if ( EavMinusTpi  >= 0.0 && EavMinusTpi < 0.025){      
                                 var->dataHist->FillUniverse(universe, t_reco, recotpi, 1);
   				 /*
				 universe->PrintDataArachneLink();
                        	 
				 if (t_reco < 0.1 && recotpi > 0 && recotpi < .030){
			         //print Make a selection at low pTmu 
			         // recot, eavail, reco tpi, true tpi, and low pT and low Eavail - Tpi.  
				 std::cout << "Reco EavMinustpi is " << EavMinusTpi << " Tpi is: " << recotpi << std::endl;
                                 std::cout << "|t| reco is " << t_reco << std::endl;
                                 std::cout << "reco Tpi is " << recotpi << std::endl; 
				std::cout << "Michel time : " << myevent.m_nmichels[0].time << std::endl;
                                 std::cout << " Michel end point 1 position (X Y Z) : " << myevent.m_nmichels[0].m_x1 << "," << myevent.m_nmichels[0].m_y1 << "," << myevent.m_nmichels[0].m_z1 << std::endl;
                                 std::cout << " Michel end point 2 position (X Y Z) : " << myevent.m_nmichels[0].m_x2 << "," << myevent.m_nmichels[0].m_y2 << "," << myevent.m_nmichels[0].m_z2 << std::endl; 
     				}
				*/

                        }
                        }
			else if(endOfPrefix6 == std::string::npos && endOfPrefix7 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                             if ( EavMinusTpi >= 0.025 && EavMinusTpi < 0.050){
                                 var->dataHist->FillUniverse(universe, t_reco, recotpi, 1);
                            	 /*if (t_reco > 0.04 && t_reco < 0.1) {
                                        std::cout << " ==================================== " << std::endl;
                                        universe->PrintDataArachneLink();
                                        std::cout << "Reco EavMinustpi is " << EavMinusTpi << " Tpi is: " << recotpi << std::endl;
                                        std::cout << "|t| reco is " << t_reco << std::endl;
					std::cout << "Michel time : " << myevent.m_nmichels[0].time << std::endl;
                                        std::cout << " Michel end point 1 position (X Y Z) : " << myevent.m_nmichels[0].m_x1 << "," << myevent.m_nmichels[0].m_y1 << "," << myevent.m_nmichels[0].m_z1 << std::endl;
					std::cout << " Michel end point 2 position (X Y Z) : " << myevent.m_nmichels[0].m_x2 << "," << myevent.m_nmichels[0].m_y2 << "," << myevent.m_nmichels[0].m_z2 << std::endl; 
				
					if (!myevent.m_nmichels[0].cluster_to_up_match.empty()){
						std::cout << " Cluster 1 Match Endpoint 1 View / Position / Z : " << myevent.m_nmichels[0].cluster_to_up_match[0].view << " / " << myevent.m_nmichels[0].cluster_to_up_match[0].pos << " / " << myevent.m_nmichels[0].cluster_to_up_match[0].zpos << " And energy " << myevent.m_nmichels[0].cluster_to_up_match[0].energy <<  std::endl; 
						std::cout << " Cluster 2 Match Endpoint 1 View / Position / Z : " << myevent.m_nmichels[0].cluster_to_up_match[1].view << " / " << myevent.m_nmichels[0].cluster_to_up_match[1].pos << " / " << myevent.m_nmichels[0].cluster_to_up_match[1].zpos << " And energy " << myevent.m_nmichels[0].cluster_to_up_match[1].energy <<  std::endl;
						std::cout << " Cluster 2 Match Endpoint 1 View / Position / Z : " << myevent.m_nmichels[0].cluster_to_up_match[2].view << " / " << myevent.m_nmichels[0].cluster_to_up_match[2].pos << " / " << myevent.m_nmichels[0].cluster_to_up_match[2].zpos << " And energy " << myevent.m_nmichels[0].cluster_to_up_match[2].energy <<  std::endl;

					}

					if (!myevent.m_nmichels[0].cluster_to_down_match.empty()){
					  std::cout << " Cluster 1 Match Endpoint 2 View / Position / Z " << myevent.m_nmichels[0].cluster_to_down_match[0].view << " / " << myevent.m_nmichels[0].cluster_to_down_match[0].pos << " / " << myevent.m_nmichels[0].cluster_to_down_match[0].zpos << " And energy " << myevent.m_nmichels[0].cluster_to_down_match[0].energy <<  std::endl;
                                          std::cout << " Cluster 2 Match Endpoint 2 View / Position / Z " << myevent.m_nmichels[0].cluster_to_down_match[1].view << " / " << myevent.m_nmichels[0].cluster_to_down_match[1].pos << " / " << myevent.m_nmichels[0].cluster_to_down_match[1].zpos << " And energy " << myevent.m_nmichels[0].cluster_to_down_match[1].energy <<  std::endl;

					}
					std::cout << " ==================================== " << std::endl;
                                }*/	
				 

			     }
                        }
			else if(endOfPrefix6 == std::string::npos &&endOfPrefix8 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                             if (EavMinusTpi >= 0.050 && EavMinusTpi < 0.075){
                                 var->dataHist->FillUniverse(universe, t_reco, recotpi, 1);
                             }
                        }
			else if(endOfPrefix6 == std::string::npos && endOfPrefix9 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
                             if ( EavMinusTpi >= 0.075 && EavMinusTpi < 0.150){
                                 var->dataHist->FillUniverse(universe, t_reco, recotpi, 1);
                             }
                        }
			else { var->dataHist->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), 1);}
      		}
      }// End of PreCuts Pass
    //} // End of Data Band 
   } // End of Entries
  std::cout << "Finished data loop.\n";
}

void LoopAndFillEffDenom( PlotUtils::ChainWrapper* truth,
    				std::map<std::string, std::vector<CVUniverse*> > truth_bands,
    				std::vector<Variable*> vars,
                                std::vector<Variable2D*> vars2D,
 			        //std::vector<Study*> studies,
    				PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
                                PlotUtils::Model<CVUniverse, MichelEvent>& model)
{
  assert(!truth_bands["cv"].empty() && "\"cv\" error band is empty!  Could not set Model entry.");
  auto& cvUniv = truth_bands["cv"].front();

  std::cout << "Starting efficiency denominator loop...\n";
  const int nEntries = truth->GetEntries();
  std::cout << "Getting N Entries for Eff Deno Loop " << nEntries << std::endl;
  for (int i=0; i<nEntries; ++i)
  {

    //std::cout << " Starting loop over Entries " << std::endl;
    if(i%10000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
    //std::cout << " Creating A Michel Event " << std::endl;
    MichelEvent cvEvent;
    //std::cout << "Michel Event Created" << std::endl;
    cvUniv->SetEntry(i);
    //std::cout << " Setting CV univ Entry " << i << std::endl;
    model.SetEntry(*cvUniv, cvEvent);
    //std::cout << "Getting CV Weight " << std::endl;
  
    const double cvWeight = model.GetWeight(*cvUniv, cvEvent);

    //const auto cutResults = michelcuts.isEfficiencyDenom(*cvUniv, cvEvent, cvWeight);
 

    //std::cout << "CV Weight is " << cvWeight << std::endl;
    //=========================================
    // Systematics loop(s)
    //=========================================
    for (auto band : truth_bands)
    {
      //std::cout << "Starting loop over truth bands " << std::endl;
      std::vector<CVUniverse*> truth_band_universes = band.second;
      for (auto universe : truth_band_universes)
      {
       // std::cout << "Starting Loop over band universe " << std::endl;
        MichelEvent myevent;// = cvEVent; //Only used to keep the Model happy
        //std::cout << "Set Michel Event " << std::endl;
        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);
	//std::cout << "checking to see if event is in signal and phase space" << std::endl;
        //if (!michelcuts.isEfficiencyDenom(*universe, cvWeight)) continue; //Weight is ignored for isEfficiencyDenom() in all but the CV universe 
        //const double weight = model.GetWeight(*universe, myevent); //Only calculate the weight for events that will use it
        auto isEffDenom = michelcuts.isEfficiencyDenom(*universe, cvWeight);
        //const bool isSignal = michelcuts.isSignal(*universe, weight2);
        //const bool isPhaseSpace = michelcuts.isPhaseSpace(*universe, weight2);
	if (!isEffDenom) continue;
        //if (!isSignal || !isPhaseSpace) continue;
        const double weight = model.GetWeight(*universe, myevent);
        //const bool isSignal = michelcuts.isSignal(*universe, weight);
        //const bool isPhaseSpace = michelcuts.isPhaseSpace(*universe, weight);
        
        //Fill efficiency denominator now: 
        //double dist = myevent.m_nmichels[0].Best3Ddist;
        //double recotpi = universe->GetTpiFromRange(dist);
        //double truetpi = myevent.m_nmichels[0].pionKE;
        //
        int idxpi = universe->GetTrueLowestTpiIdx(); 
	double t_truth;
        if (idxpi == 9999){ // Put events that pass my cuts but don't have a real pion in underflow bin
            t_truth = -9999.;
        }
        else {t_truth = universe->GetTruet(idxpi);}	

        double trueTpi = universe->GetTrueLowestTpiEventGEV();//-9999.;
	double trueEavminusTpi = universe->GetTrueEavMinusTpiGEV(); //-9999.;
        for(auto var: vars)
        {
	  if (var->GetName() == "Tpi"){
		var->efficiencyDenominator->FillUniverse(universe, trueTpi, weight);
	  }

          else if (var->GetName() == "texp"){
		var->efficiencyDenominator->FillUniverse(universe, t_truth, weight);
	  }
          else if (var->GetName() == "EavMinusTpi"){
	      //double trueEav = var->GetTrueValue(*universe);
	      //trueEavminusTpi = var->GetTrueValue(*universe); //trueEav - trueTpi;
	      //if (trueEavminusTpi < 0.0) trueEavminusTpi = 0.0;
	      var->efficiencyDenominator->FillUniverse(universe, trueEavminusTpi, weight);
	  }
	  else {
	      if (var->GetName() == "Abin") continue;
              if (var->GetName() == "Bbin") continue;
              if (var->GetName() == "Cbin") continue;
              if (var->GetName() == "Dbin") continue; // Skip these 1D hists. Only matter for figuring out COH reweight stuff for now. Oct 4 2023
	      var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValue(*universe), weight);
       	  }
	  //if (var->GetName() == "Tpi"){
  	  //	var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValue(*universe), weight);		
	  //}
	}
        // Fill Studies denominator:
//        for(auto& study: studies) study->SelectedSignal(*universe, cvEvent, weight);

        for(auto var: vars2D)
        {
	  const std::string keyName = var->GetName();
	  //const size_t endOfPrefix3 = keyName.find("texp_");
	  //const size_t endOfPrefix3 = keyName.find("texp_eavbins");
          //const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
          const size_t endOfPrefix2 = keyName.find("EavMinusTpi_pTmub");
          const size_t endOfPrefix3 = keyName.find("texp_EavMinTpi");
	  const size_t endOfPrefixtpi = keyName.find("Tpi_pTmubi");
	  const size_t endOfPrefixtAE = keyName.find("AvailableE_pTmu");
	  const size_t endOfPrefixA = keyName.find("Abin");
	  const size_t endOfPrefixB = keyName.find("Bbin");
	  const size_t endOfPrefixC = keyName.find("Cbin");
	  const size_t endOfPrefixD = keyName.find("Dbin");

	

	  if(endOfPrefix2 != std::string::npos){
		var->efficiencyDenominator->FillUniverse(universe, trueEavminusTpi, var->GetTrueValueY(*universe), weight);
	 	var->FillCategDenomHistos((*universe), trueEavminusTpi, var->GetTrueValueY(*universe), weight);
       		//std::cout << "Name of histo " << var->GetName() << " (EavMinTpi) True Value X " << trueEavminusTpi << " and (should be pTmu) " << var->GetTrueValueY(*universe) << std::endl;
          }
  	  else if(endOfPrefix3 != std::string::npos && endOfPrefix2 == std::string::npos){
		var->efficiencyDenominator->FillUniverse(universe, t_truth, trueEavminusTpi, weight);
	  }
	  else if (endOfPrefixtpi != std::string::npos &&  endOfPrefixtAE == std::string::npos && endOfPrefix2 == std::string::npos){
		var->FillCategDenomHistos((*universe),trueTpi, var->GetTrueValueY(*universe), weight);
                var->efficiencyDenominator->FillUniverse(universe, trueTpi, var->GetTrueValueY(*universe), weight);

	  } 
	  else if ( endOfPrefixtAE != std::string::npos ){
		var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight);
		var->FillCategDenomHistos((*universe), var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight);
	  }
	  else if(endOfPrefixA != std::string::npos || endOfPrefixB != std::string::npos || endOfPrefixC != std::string::npos || endOfPrefixD != std::string::npos ){continue;}
          else{
		continue;
		var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight);
	  }
        }
      }
    }
  }
  std::cout << "Finished efficiency denominator loop.\n";
}

//Returns false if recoTreeName could not be inferred
bool inferRecoTreeNameAndCheckTreeNames(const std::string& mcPlaylistName, const std::string& dataPlaylistName, std::string& recoTreeName)
{
  const std::vector<std::string> knownTreeNames = {"Truth", "Meta", "Header"};
  bool areFilesOK = false;

  std::ifstream playlist(mcPlaylistName);
  std::string firstFile = "";
  playlist >> firstFile;
  auto testFile = TFile::Open(firstFile.c_str());
  if(!testFile)
  {
    std::cerr << "Failed to open the first MC file at " << firstFile << "\n";
    return false;
  }

  //Does the MC playlist have the Truth tree?  This is needed for the efficiency denominator.
  const auto truthTree = testFile->Get("Truth");
  if(truthTree == nullptr || !truthTree->IsA()->InheritsFrom(TClass::GetClass("TTree")))
  {
    std::cerr << "Could not find the \"Truth\" tree in MC file named " << firstFile << "\n";
    return false;
  }

  //Figure out what the reco tree name is
  for(auto key: *testFile->GetListOfKeys())
  {
    if(static_cast<TKey*>(key)->ReadObj()->IsA()->InheritsFrom(TClass::GetClass("TTree"))
       && std::find(knownTreeNames.begin(), knownTreeNames.end(), key->GetName()) == knownTreeNames.end())
    {
      recoTreeName = key->GetName();
      areFilesOK = true;
    }
  }
  delete testFile;
  testFile = nullptr;

  //Make sure the data playlist's first file has the same reco tree
  playlist.open(dataPlaylistName);
  playlist >> firstFile;
  testFile = TFile::Open(firstFile.c_str());
  if(!testFile)
  {
    std::cerr << "Failed to open the first data file at " << firstFile << "\n";
    return false;
  }

  const auto recoTree = testFile->Get(recoTreeName.c_str());
  if(recoTree == nullptr || !recoTree->IsA()->InheritsFrom(TClass::GetClass("TTree")))
  {
    std::cerr << "Could not find the \"" << recoTreeName << "\" tree in data file named " << firstFile << "\n";
    return false;
  }

  return areFilesOK;
}

//==============================================================================
// Main
//==============================================================================
int main(const int argc, const char** argv)
{
  TH1::AddDirectory(false);
  ROOT::EnableImplicitMT();
  //Validate input.
  //I expect a data playlist file name and an MC playlist file name which is exactly 2 arguments.
  const int nArgsExpected = 2;
  if(argc != nArgsExpected + 1) //argc is the size of argv.  I check for number of arguments + 1 because
                                //argv[0] is always the path to the executable.
  {
    std::cerr << "Expected " << nArgsExpected << " arguments, but got " << argc - 1 << "\n" << USAGE << "\n";
    return badCmdLine;
  }

  //One playlist must contain only MC files, and the other must contain only data files.
  //Only checking the first file in each playlist because opening each file an extra time
  //remotely (e.g. through xrootd) can get expensive.
  //TODO: Look in INSTALL_DIR if files not found?
  const std::string mc_file_list = argv[2],
                    data_file_list = argv[1];
  //Check that necessary TTrees exist in the first file of mc_file_list and data_file_list
  std::string reco_tree_name;
  if(!inferRecoTreeNameAndCheckTreeNames(mc_file_list, data_file_list, reco_tree_name))
  {
    std::cerr << "Failed to find required trees in MC playlist " << mc_file_list << " and/or data playlist " << data_file_list << ".\n" << USAGE << "\n";
    return badInputFile;
  }

  const bool doCCQENuValidation = (reco_tree_name == "CCQENu"); //Enables extra histograms and might influence which systematics I use.

  const bool is_grid = false;
  PlotUtils::MacroUtil options(reco_tree_name, mc_file_list, data_file_list, "minervame1A", true);
  std::cout << options.m_mc->GetChain()->GetName() << std::endl;
  options.m_plist_string = util::GetPlaylist(*options.m_mc, true); //TODO: Put GetPlaylist into PlotUtils::MacroUtil

  // You're required to make some decisions
  PlotUtils::MinervaUniverse::SetNuEConstraint(true);
  PlotUtils::MinervaUniverse::SetPlaylist(options.m_plist_string); //TODO: Infer this from the files somehow?
  PlotUtils::MinervaUniverse::SetAnalysisNuPDG(14);
  PlotUtils::MinervaUniverse::SetNFluxUniverses(100); // TODO: CHANGE TO 100 later. For now, testing with no flux systematics. 
  PlotUtils::MinervaUniverse::SetZExpansionFaReweight(false);
 
  //For MnvTunev4.3.1 - Aaron's Tune we need the following:
  //PlotUtils::MinervaUniverse::SetNonResPiReweight(true);
  //PlotUtils::MinervaUniverse::SetDeuteriumGeniePiTune(true);
  PlotUtils::MinervaUniverse::SetReadoutVolume("Tracker");
  PlotUtils::MinervaUniverse::SetMHRWeightNeutronCVReweight( true );
  PlotUtils::MinervaUniverse::SetMHRWeightElastics( true ); 

  //Now that we've defined what a cross section is, decide which sample and model
  //we're extracting a cross section for.
  PlotUtils::Cutter<CVUniverse, MichelEvent>::reco_t  preCuts;
  PlotUtils::Cutter<CVUniverse, MichelEvent>::reco_t  sidebands;
  PlotUtils::Cutter<CVUniverse, MichelEvent>::truth_t signalDefinition, phaseSpace;

  //const double minZ = 5980, maxZ = 8590.07, apothem = 850.; // All in mm
  const double minZ = 5980, maxZ = 8422, apothem = 850; //All in mm
  preCuts.emplace_back(new reco::HasMINOSMatch<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::IsNeutrino<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::MaxMuonAngle<CVUniverse, MichelEvent>(20.));
  preCuts.emplace_back(new reco::NoDeadtime<CVUniverse, MichelEvent>(1, "Deadtime"));
  preCuts.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  preCuts.emplace_back(new reco::Apothem<CVUniverse, MichelEvent>(apothem));
  preCuts.emplace_back(new PTRangeReco<CVUniverse, MichelEvent>(0.0,1.8));
  preCuts.emplace_back(new RecoilERange<CVUniverse, MichelEvent>(0.0,1.2));
  preCuts.emplace_back(new PmuCut<CVUniverse, MichelEvent>(1.5));
  preCuts.emplace_back(new PzmuCut<CVUniverse, MichelEvent>(20.));

  //typedef LowRecoilPion::hasMichel<CVUniverse, MichelEvent> hasMichel;
  //preCuts.emplace_back(new hasMichel<CVUniverse, MichelEvent>());

  preCuts.emplace_back(new LowRecoilPion::hasMichel<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new LowRecoilPion::BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  preCuts.emplace_back(new LowRecoilPion::GetClosestMichel<CVUniverse, MichelEvent>(0));

  //nosidebands.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  //nosidebands.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));

  


  signalDefinition.emplace_back(new truth::IsNeutrino<CVUniverse>());
  signalDefinition.emplace_back(new truth::IsCC<CVUniverse>());
  signalDefinition.emplace_back(new truth::HasPion<CVUniverse>());
  //signalDefinition.emplace_back(new truth::TpiCut<CVUniverse>());
  //signalDefinition.emplace_back(new Q3Limit<CVUniverse>(0.0, 1.20));
  //signalDefinition.emplace_back(new truth::ZRange<CVUniverse>("Tracker", minZ, maxZ));
  //signalDefinition.emplace_back(new truth::Apothem<CVUniverse>(apothem));
  //signalDefinition.emplace_back(new truth::pMuCut<CVUniverse>(1.5));
  //signalDefinition.emplace_back(new truth::EAvailCut<CVUniverse>(1400.)); 
  //signalDefinition.emplace_back(new truth::pTRangeLimit<CVUniverse>(0., 1.0));
  //signalDefinition.emplace_back(new truth::MuonAngle<CVUniverse>(20.));
   //signalDefinition.emplace_back(new hasTruePion<CVUniverse>());                                                                                                              
  
  phaseSpace.emplace_back(new truth::ZRange<CVUniverse>("Tracker", minZ, maxZ));
  phaseSpace.emplace_back(new truth::Apothem<CVUniverse>(apothem));
  phaseSpace.emplace_back(new truth::MuonAngle<CVUniverse>(20.));
  phaseSpace.emplace_back(new truth::PZMuMin<CVUniverse>(1500.));
  phaseSpace.emplace_back(new truth::pTRangeLimit<CVUniverse>(0., 1.8));
  phaseSpace.emplace_back(new truth::pMuCut<CVUniverse>(1.5));
  phaseSpace.emplace_back(new truth::pzMuCut<CVUniverse>(20.));
  phaseSpace.emplace_back(new truth::EavailCut<CVUniverse>());
  //phaseSpace.emplace_back(new truth::TpiCut<CVUniverse>());
  PlotUtils::Cutter<CVUniverse, MichelEvent> mycuts(std::move(preCuts), std::move(sidebands) , std::move(signalDefinition),std::move(phaseSpace));
/*  
  std::vector<std::unique_ptr<PlotUtils::Reweighter<CVUniverse, MichelEvent>>> MnvTunev1;
  
  MnvTunev1.emplace_back(new PlotUtils::FluxAndCVReweighter<CVUniverse, MichelEvent>());
  MnvTunev1.emplace_back(new PlotUtils::GENIEReweighter<CVUniverse, MichelEvent>(true, false));
  MnvTunev1.emplace_back(new PlotUtils::LowRecoil2p2hReweighter<CVUniverse, MichelEvent>());
  MnvTunev1.emplace_back(new PlotUtils::MINOSEfficiencyReweighter<CVUniverse, MichelEvent>());
  MnvTunev1.emplace_back(new PlotUtils::RPAReweighter<CVUniverse, MichelEvent>());
  //TODO: Add my pion reweighter here. - Mehreen S.  Nov 22, 2021
  //MnvTunev1.emplace_back(new PlotUtils::PionReweighter<CVUniverse,MichelEvent>()); 
  PlotUtils::Model<CVUniverse, MichelEvent> model(std::move(MnvTunev1));
  //
  */

   
  std::vector<std::unique_ptr<PlotUtils::Reweighter<CVUniverse, MichelEvent>>> MnvTunev4;
  MnvTunev4.emplace_back(new PlotUtils::FluxAndCVReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::GENIEReweighter<CVUniverse, MichelEvent>(true, true));
  MnvTunev4.emplace_back(new PlotUtils::LowRecoil2p2hReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::MINOSEfficiencyReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::RPAReweighter<CVUniverse, MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::LowQ2PiReweighter<CVUniverse, MichelEvent>("MENU1PI"));
  //MnvTunev4.emplace_back(new PlotUtils::DiffractiveReweighter<CVUniverse, MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::GeantNeutronCVReweighter<CVUniverse, MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::FSIReweighter<CVUniverse, MichelEvent>(true, true)); 
  //MnvTunev4.emplace_back(new PlotUtils::COHPionReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::MnvTunev431Reweighter<CVUniverse, MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::TargetMassReweighter<CVUniverse, MichelEvent>()); 
  MnvTunev4.emplace_back(new PlotUtils::PionReweighter<CVUniverse,MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::BkgSideReweighter<CVUniverse,MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::BkgSigReweighter<CVUniverse,MichelEvent>());
  //MnvTunev4.emplace_back(new PlotUtils::MKReweighter<CVUniverse,MichelEvent>());
  PlotUtils::Model<CVUniverse, MichelEvent> model(std::move(MnvTunev4));
  
   
  // Make a map of systematic universes
  // Leave out systematics when making validation histograms
  const bool doSystematics = (getenv("MNV101_SKIP_SYST") == nullptr);
  if(!doSystematics){
    std::cout << "Skipping systematics (except 1 flux universe) because environment variable MNV101_SKIP_SYST is set.\n";
    PlotUtils::MinervaUniverse::SetNFluxUniverses(2); //Necessary to get Flux integral later...  Doesn't work with just 1 flux universe though because _that_ triggers "spread errors".
  }

  std::map< std::string, std::vector<CVUniverse*> > error_bands;
  if(doSystematics) error_bands = GetStandardSystematics(options.m_mc);
  else{
    std::map<std::string, std::vector<CVUniverse*> > band_flux = PlotUtils::GetFluxSystematicsMap<CVUniverse>(options.m_mc, CVUniverse::GetNFluxUniverses());
    error_bands.insert(band_flux.begin(), band_flux.end()); //Necessary to get flux integral later...
  }
  error_bands["cv"] = {new CVUniverse(options.m_mc)};
  std::map<const std::string, int> error_name;
  std::map<std::string, std::vector<CVUniverse*> >::iterator itr;
  for(itr = error_bands.begin(); itr != error_bands.end(); ++itr) error_name.insert(std::pair<std::string, const int>((itr->second)[0]->ShortName(), (itr->second).size()));
  std::map< std::string, std::vector<CVUniverse*> > truth_bands;
  if(doSystematics) truth_bands = GetStandardSystematics(options.m_truth);
  truth_bands["cv"] = {new CVUniverse(options.m_truth)};

  //std::vector<double> muthetabins = {-1.2,-1, -.80, -.70, -.60, -.50, -.40, -.30, -.20, -.10, .10, .20, .30, .40,.50,.60,.70,.80,.90,1.,1.2};
  std::vector<double> ptbins = {0, 0.075, 0.10, 0.15, 0.20, 0.25,0.30,0.35, 0.4, 0.45, 0.50, 0.55, 0.60 , 0.7, 0.80,0.9, 1.};
  std::vector<double> dansPTBins = {0.01, 0.015, 0.020, 0.025, 0.030, 0.035, 0.040, 0.045, 0.05, 0.075, 0.10, 0.125, 0.15, 0.175,0.20, 0.25,0.30,0.35, 0.4, 0.45, 0.50, 0.55, 0.60 , 0.7, 0.80,0.9, 1.,1.1, 1.2, 1.4, 1.6},// 1.3, 1.4, 1.5, 2.0, 3.0, 4.0},
                      dansPzBins = {1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 15, 20},//, 40, 60},
                      robsEmuBins = {0,1,2,3,4,5,7,9,12,15,18,22,36,50,75,80},
                      mehreenQ3Bins = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6},// 1.2},
		      mehreenpmubins = {0.0, 0.5, 1.0, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 15, 20, 40}, 
		     robsRecoilBins;
  
  		 
  int nq3mbins = mehreenQ3Bins.size() -1;
  std::vector<double> tpibins = {0.0, 10., 15., 20., 25., 30., 36., 42., 48., 54.,60., 66., 72., 78.,  84., 90., 96., 102., 110., 125., 140., 155., 175., 200., 225., 250., 275., 300., 325., 350., 400., 500., 700., 1000.}; 
  
  //std::vector<double> tpibins = {0.0, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 325., 350., 400., 500., 700., 1000.};   
  //std::vector<double> rangebins = {0.1, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 44., 50., 56., 62., 70., 80.,90., 100., 110.,  120., 140., 160., 180., 200., 220., 240., 260., 280., 300., 325., 350., 375., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1200., 1400., 1800., 2400.};             
  std::vector<double> rangebins = {0.00, 0.1, 1.0, 8., 16., 24., 32., 40., 50., 65., 80.,95., 110., 140., 170., 200., 230., 260., 290., 310., 360., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1200., 1400., 1800., 2400.};
  std::vector<double> recoilbins = {0.0, 200., 300., 400., 500., 600., 800., 1000., 1200., 1400., 1600.};
  std::vector<double> recoilbins2 = {0.0, 150., 225., 300., 400., 500., 600., 700., 800., 900., 1000.};//, 1200.};// 2200., 2600., 3000., 6000.};
  std::vector<double> wexpbins = {0.0,0.2, 0.4, .6, .8, 1.0, 1.1, 1.2, 1.3, 1.5, 1.7, 2.0, 2.5, 3.0, 4.0};
  std::vector<double> eavtpibins = {0.0, 75., 150., 225., 300., 400., 500., 600., 700., 800., 900., 1000.};
  std::vector<double> eavbinsGEV = {0.00, 0.075, 0.150, 0.225, 0.300, 0.400, 0.500, 0.600, 0.700, 0.800, 0.900, 1.000};
  std::vector<double> tpibinsGEV = {0.001, .010, .015, .020, .025, .030, .036, .042, .048, .054,.060, .066, 0.072, 0.078,.084, 0.09, 0.096, .102, .110, .125 , .140 , .155, .175, .200, .225, .250, .275, .300, .325, .350, .400, .500,0.700, 1.0}; 
  //std::vector<double> tbins = {0.001,0.01, 0.015, 0.020, 0.025, 0.030, 0.036, 0.042, 0.048, 0.054, 0.06, 0.075, 0.10, 0.125, 0.15, 0.175,0.20, 0.25,0.30,0.35, 0.4, 0.45, 0.50, 0.55, 0.60 , 0.7, 0.80,0.9, 1.,1.5, 2.0, 2.5, 3.0};
  std::vector<double> tbins = {0.000, 0.01, 0.020, 0.030, 0.045, 0.06, 0.080, 0.10, 0.125, 0.15, 0.175, 0.20, 0.25, 0.30, 0.35, 0.4, 0.45, 0.50, 0.60 , 0.80, 1.}; //TODO: remove bins above 0.500  
  //std::vector<double> tbins = {0.001, 0.01, 0.02, 0.05, 0.15, 0.3, 0.5, 0.75, 1.0};   

  std::vector<double> tpibinsGEV2 = {0.000,0.017, 0.023, 0.030, 0.040, 0.050, 0.060,0.070, 0.90, 0.10,0.160, 0.200, 0.230, 0.300, 0.350, 0.400, 0.500};


  std::vector<double> eavbinsGEV2 = {0.00, 0.075, 0.150, 0.225, 0.300, 0.400, 0.500, 0.600, 0.700, 0.800, 0.900, 1.000};

  std::vector<double> eavbin2D = {0.0, .075, .150, .225, .300, .500, .700, 1.000};
  std::vector<double> tpibins2D = {0.0, 0.03, 0.06, 0.1, 0.15, 0.2, 0.25, 0.3, .500}; // binning for Tpi vs |t| in GeV 
  std::vector<double> eavtpibins2D = {0.0, 0.025, 0.050, .075, .150, .225, .300, 1.000};

  //const double robsRecoilBinWidth = 75; //MeV
  //for(int whichBin = 0; whichBin < 20 + 1; ++whichBin) robsRecoilBins.push_back(robsRecoilBinWidth * whichBin);

  std::vector<Variable*> vars = {
    /*
    new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),
    new Variable("AvailableE", "AvailableE", eavbinsGEV, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEAvailGEV),
    new Variable("Tpi", "Tpi", tpibinsGEV2, &CVUniverse::GetDefault, &CVUniverse::GetTrueLowestTpiEventGEV),
    new Variable("EavMinusTpi", "EavMinusTpi", eavbinsGEV, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEavMinusTpiGEV), 
    new Variable("PionRange", "PionRange", rangebins, &CVUniverse::GetDefault, &CVUniverse::GetDefault), 
    */
    // Nov 09 2023 - NEed to run only xsec plots 
    
    new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),
    new Variable("q3bins", "q3bins", dansPTBins,  &CVUniverse::Getq3, &CVUniverse::GetTrueQ3),
    new Variable("AvailableE", "AvailableE", eavbinsGEV, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEAvailGEV),
    new Variable("pzmu", "p_{||, #mu} [GeV/c]", dansPzBins, &CVUniverse::GetMuonPz, &CVUniverse::GetMuonPzTrue),    
    new Variable("pTmu", "p_{T, #mu} [GeV/c]", ptbins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue), //0
    new Variable("q2", "q2 [GeV^2]", dansPTBins, &CVUniverse::GetQ2Reco, &CVUniverse::GetTrueQ2GeV), //2
    new Variable("Tpi", "Tpi", tpibinsGEV2, &CVUniverse::GetDefault, &CVUniverse::GetTrueLowestTpiEventGEV),
    new Variable("EavMinusTpi", "EavMinusTpi", eavbinsGEV2, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEavMinusTpiGEV), 
    new Variable("texp", "texp", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),
    new Variable("eavbins", "eavbins", eavbin2D, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEAvailGEV),
    new Variable("Tpibins", "Tpibins", tpibins2D, &CVUniverse::GetDefault, &CVUniverse::GetTrueLowestTpiEventGEV),
    new Variable("EavMinTpibins", "EavMinTpibins", eavtpibins2D, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEavMinusTpiGEV),

    new Variable("PionRange", "PionRange", rangebins, &CVUniverse::GetDefault, &CVUniverse::GetDefault), 

    new Variable("Abin", "Abin", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),  

    new Variable("Bbin", "Bbin", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),

    new Variable("Cbin", "Cbin", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),

    new Variable("Dbin", "Dbin", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),
     
   
    // Uncomment above after only cross-section plots. Nov 09 2023

    //new Variable("AvailableE", "AvailableE", recoilbins2, &CVUniverse::NewEavail, &CVUniverse::GetTrueEAvail),//6 
    //new Variable("Pmu", "pmu", mehreenpmubins, &CVUniverse::GetMuonP, &CVUniverse::GetPmuTrue), //7
    //new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue), //8
    //new Variable("EHadronic", "EHadronic", recoilbins2, &CVUniverse::GetEHad, &CVUniverse::GetTrueEHad), // 9
    //new Variable("Wexp", "Wexp", wexpbins, &CVUniverse::GetWExp, &CVUniverse::GetTrueWexp), //10
    //new Variable("q3bins", "q3bins", dansPTBins,  &CVUniverse::Getq3, &CVUniverse::GetTrueQ3) //11
    /*new Variable("NewRecoilE", "NewRecoilE", robsRecoilBins, &CVUniverse::NewRecoilE, &CVUniverse::GetTrueQ0),//7
    new Variable("NewEAvailvq0", "NewEavailvq0", robsRecoilBins, &CVUniverse::NewEavail,&CVUniverse::GetTrueQ0),//8
    new Variable("pTmubins", "pTmuy", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),//9
    new Variable("NewEAvailbins", "NewEAvaily", recoilbins, &CVUniverse::NewEavail, &CVUniverse::GetTrueEAvail),//10
    new Variable("NewRecoilEbins", "NewRecoilEy", recoilbins, &CVUniverse::NewRecoilE, &CVUniverse::GetTrueQ0),//11
    new Variable("NewEAvailvq0bins", "NewEavailvq0y", recoilbins, &CVUniverse::NewEavail,&CVUniverse::GetTrueQ0),//12
    new Variable("q3bins", "q3y", mehreenQ3Bins,&CVUniverse::Getq3, &CVUniverse::GetTrueQ3), //13
    new Variable("Pmu", "pmu", mehreenpmubins, &CVUniverse::GetMuonP, &CVUniverse::GetPmuTrue),
    new Variable("LowTpi", "LowTpi", tpibins, &CVUniverse::GetTrueLowestTpiEvent, &CVUniverse::GetTrueLowestTpiEvent)
    */
 };

 std::vector<Variable2D*> vars2D;
 //vars2D.push_back(new Variable2D(*vars[2], *vars[1]));// q3bins vs Eavail
 //vars2D.push_back(new Variable2D(*vars[1], *vars[0]));//Eavail vs pTmubins
 //vars2D.push_back(new Variable2D(*vars[2], *vars[0]));//Tpi vs pTmu
 //vars2D.push_back(new Variable2D(*vars[3], *vars[0]));//EavMinTpi vs ptMu
 // November 09., 20203 - Uncomment below after making xsec plots
 
 vars2D.push_back(new Variable2D(*vars[2], *vars[0]));// pTbins vs Eavail
 vars2D.push_back(new Variable2D(*vars[6], *vars[0]));//pTbins vs Tpi
 vars2D.push_back(new Variable2D(*vars[7], *vars[0]));//pTbins vs (Eavail - Tpi)
 vars2D.push_back(new Variable2D(*vars[8], *vars[9]));//Eavail vs |t| in GeV. Available E is on y axis
 vars2D.push_back(new Variable2D(*vars[8], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis
 vars2D.push_back(new Variable2D(*vars[8], *vars[11]));//Eav - Tpi vs |t| in GeV >>> Eav - Tpi on y axis
 
 vars2D.push_back(new Variable2D(*vars[13], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis - EavMinusTpi < 25 MeV
 vars2D.push_back(new Variable2D(*vars[14], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis - 25 < EavMinusTpi < 50 MeV
 vars2D.push_back(new Variable2D(*vars[15], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis - 50 < EavMinusTpi < 75 MeV
 vars2D.push_back(new Variable2D(*vars[16], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis - 75 < EavMinusTpi < 150 MeV
 
 // Uncomment above after making xsec plots. 
 


 //vars2D.push_back(new Variable2D(*vars[7],*vars[12]));// Tpi vs Pion Range: pion range on y axis 
 //vars2D.push_back(new Variable2D(*vars[6], *vars[10]));// Wexp vs Eavail
 //vars2D.push_back(new Variable2D(*vars[11], *vars[10])); // Wexp vs q3
 //vars2D.push_back(new Variable2D(*vars[9], *vars[9])); // q2 vs q0 (eHadronic)
 /* // TODO: make 2D plots for cross-section
 vars2D.push_back(new Variable2D(*vars[6], *vars[9])); //pTmubins(y) vs Eavail
 vars2D.push_back(new Variable2D(*vars[7], *vars[9])); //pTmubins(y) vs RecoilE
 vars2D.push_back(new Variable2D(*vars[1], *vars[9])); //pTmubins(y) vs q3
 vars2D.push_back(new Variable2D(*vars[3], *vars[9])); //pTmubins(y) vs pz
 vars2D.push_back(new Variable2D(*vars[2], *vars[9])); //pTmubins(y) vs q2
 vars2D.push_back(new Variable2D(*vars[6], *vars[10])); //Eavailbins(y) vs Eavail
 vars2D.push_back(new Variable2D(*vars[8], *vars[12])); //Eavailq0bins(y) vs Eavail
 vars2D.push_back(new Variable2D(*vars[7], *vars[11])); //RecoilEq0bins(y) vs RecoilE
 */
 //vars2D.push_back(new Variable2D(*vars[6], *vars[13])); //q3bins(y) vs Eavail
 //vars2D.push_back(new Variable2D(*vars[7], *vars[13])); //q3bins(y) vs RecoilE
 //vars2D.push_back(new Variable2D(*vars[0], *vars[13])); //q3bins(y) vs pTmu
 //vars2D.push_back(new Variable2D(*vars[3], *vars[13])); //q3bins(y) vs pz
 //vars2D.push_back(new Variable2D(*vars[2], *vars[13])); //q3bins(y) vs q2

 std::vector<Variable*> sidevars;// = vars; 

 std::vector<Variable2D*> sidevars2D;// = vars2D;

 std::vector<Study*> studies;
 std::vector<Study*> sideband_studies;

 std::function<double(const CVUniverse&, const MichelEvent&, const int)> getnclusters = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
        				//int matchtype = evt.m_nmichels[whichMichel].BestMatch;      
		                        double nclus = univ.GetNonMuonClusters(); //evt.m_nmichels[whichMichel].nnonmuclusters; //(double)univ.GetNClusters();
					return nclus;
					//if (matchtype == 3 or matchtype == 4) return nclus;
					//else return -9999.;
				};
 std::function<double(const CVUniverse&, const MichelEvent&, const int)> getmichelenergy = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel){
					//int matchtype = evt.m_nmichels[whichMichel].BestMatch;
					//if (matchtype == 3 or matchtype == 4) return evt.m_nmichels[whichMichel].energy; //MeV
					//else return -9999.;
					return evt.m_nmichels[whichMichel].energy; //MeV
				};
				       
 std::function<double(const CVUniverse&, const MichelEvent&, const int)> getmicheltime = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel){
                                        //int matchtype = evt.m_nmichels[whichMichel].BestMatch;
					
					return (evt.m_nmichels[whichMichel].time - univ.GetVertex().T());
					//if (matchtype == 3 or matchtype == 4) return evt.m_nmichels[whichMichel].time; //microseconds 
					//else return -9999.;
                                };
  std::function<double(const CVUniverse&, const MichelEvent&, const int)> getxpos1 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel){
                                        return evt.m_nmichels[whichMichel].m_x1; //mm 
                                };

  std::function<double(const CVUniverse&, const MichelEvent&, const int)> permichel_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 { 
                                   //double pdg = evt.m_nmichels[whichMichel].true_parentpdg;
                                   //std::cout << " Primary Parent KE: " << evt.m_nmichels[whichMichel].pionKE << std::endl;
                                   return univ.GetTrueLowestTpiEvent();
				   //return evt.m_nmichels[whichMichel].pionKE;
				 };
  std::function<double(const CVUniverse&, const MichelEvent&, const int)> permichel_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 { 
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   //std::cout << " Range: " << micheldist << std::endl;
				   return micheldist;
                                 };


  std::vector<double> nclusbins = {0,1, 5, 15, 25, 50, 75, 100, 150, 200, 250, 300, 350, 400, 450, 500, 600, 700, 1000};
  int nclusterbins = nclusbins.size() -1; 
  std::vector<double> timebins = {-1.0,-0.5,0.01, 0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4, 2.7, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 7.0, 8.0, 9.0, 10. };
  int ntimebins = timebins.size() -1; //micro seconds
  std::vector<double> energybins = {1., 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80.};
  int nenergybins = energybins.size() -1; //MeV
  int nbinsrange = rangebins.size()-1;
  int nbinstpi = tpibins.size()-1;

 

  studies.push_back(new PerMichelVarVecFSPart(getnclusters, "nonmu_clusters", "n_clusters",  nclusterbins, nclusbins, error_bands));
  studies.push_back(new PerMichelVarVecFSPart(getmichelenergy, "michele_energy", "michelenergy",  nenergybins, energybins, error_bands));
  studies.push_back(new PerMichelVarVecFSPart(getmicheltime, "michel_time", "micheltime",  ntimebins, timebins, error_bands));
  
  VarConfig2D rangeconfig{"Pion_Range", "mm", nbinsrange, rangebins};
  VarConfig2D tpiconfig{"Pion_KE", "MeV", nbinstpi, tpibins}; 
  studies.push_back(new PerMichel2DVarbin(permichel_tpi, permichel_range, tpiconfig, rangeconfig, error_bands));
  studies.push_back(new PerMichelVarVecFSPart(permichel_range, "permichel_pirange", "mm", nbinsrange, rangebins, error_bands));
  studies.push_back(new PerMichelVarVecFSPart(permichel_tpi,"permichel_tpi", "MeV", nbinstpi, tpibins, error_bands));


  CVUniverse* data_universe = new CVUniverse(options.m_data);
  std::vector<CVUniverse*> data_band = {data_universe};
  std::map<std::string, std::vector<CVUniverse*> > data_error_bands;
  data_error_bands["cv"] = data_band;
 
  std::vector<Study*> data_studies;
  data_studies.push_back(new PerMichelVarVecFSPart(getnclusters, "nonmu_clusters", "n_clusters",  nclusterbins, nclusbins,  data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(getmichelenergy, "michele_energy", "michelenergy",  nenergybins, energybins, data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(getmicheltime, "michel_time", "micheltime",  ntimebins, timebins, data_error_bands)); 
  data_studies.push_back(new PerMichelVarVecFSPart(permichel_range, "permichel_pirange", "mm", nbinsrange, rangebins, data_error_bands));
 
  for(auto& var: vars) var->InitializeMCHists(error_bands, truth_bands);
  for(auto& var: vars) var->InitializeDATAHists(data_band);
  for(auto& var: vars2D){
	var->InitializeMCHists(error_bands, truth_bands);
	var->SetupResponse(error_name);
  }
  for(auto& var: vars2D) var->InitializeDATAHists(data_band);

  //for(auto& var: sidevars) var->InitializeDATAHists(data_band);
  //for(auto& var: sidevars) var->InitializeMCHists(error_bands, truth_bands);
  //for(auto& var: sidevars2D) var->InitializeMCHists(error_bands, truth_bands);
  //for(auto& var: sidevars2D) var->InitializeDATAHists(data_band);  

  // Loop entries and fill
  try
  {
    CVUniverse::SetTruth(true);
    LoopAndFillEventSelection(options.m_mc, error_bands, vars, vars2D,studies, sidevars, sidevars2D, mycuts, model);
    std::cout << "MC cut summary:\n" << mycuts << "\n";
    CVUniverse::SetTruth(true);
    LoopAndFillEffDenom(options.m_truth, truth_bands, vars, vars2D, mycuts, model);
    
    options.PrintMacroConfiguration(argv[0]);
    std::cout << "MC Cut Summary:\n" << mycuts << "\n";
    mycuts.resetStats();
    CVUniverse::SetTruth(false);
    CVUniverse::SetTruth(false);
    LoopAndFillData(options.m_data, data_band, vars, vars2D, data_studies,sidevars, sidevars2D, mycuts);
    std::cout << "Data Cut summary:\n" << mycuts << "\n";
    
    std::cout  << "Opening the MCOUTPUT for EVENT hists" << std::endl;
    //Write MC results
    TFile* mcOutDir = TFile::Open(MC_OUT_FILE_NAME, "RECREATE");
    //TFile* mcSideDir = TFile::Open(MC_SIDE_FILE_NAME, "RECREATE");
    if(!mcOutDir)
    {
      std::cerr << "Failed to open a file named " << MC_OUT_FILE_NAME << " in the current directory for writing histograms.\n";
      return badOutputFile;
    }
    
   // if(!mcSideDir)
   // {
   //   std::cerr << "Failed to open a file named " << MC_SIDE_FILE_NAME << " in the current directory for writing histograms.\n";
   //   return badOutputFile;
  //  }
    
    std::cout << "Saving Histos to MC Files" << std::endl;
    for(auto& var: vars) var->WriteMC(*mcOutDir);
    std::cout << "WRiting 2D VARS to event level file " << std::endl;
    for(auto& var: vars2D) var->Write(*mcOutDir);
    std::cout << "Saving Histos to Data Files" << std::endl;
   
    //
    //for(auto& var: sidevars) var->WriteMC(*mcSideDir);
    //std::cout << "WRiting 2D VARS to event level file " << std::endl;
    //for(auto& var: sidevars2D) var->Write(*mcSideDir);
    //std::cout << "Saving Histos to Data Files" << std::endl;

    // Nov 09 - Uncomment later. 
    for(auto& study: studies) study->SaveOrDraw(*mcOutDir);


    //Protons On Target
   
    std::cout << "Printing POT MC " << std::endl;
    auto mcPOT = new TParameter<double>("POTUsed", options.m_mc_pot);
    //mcPOT->Write();
    mcOutDir->cd();
    mcPOT->Write();
    
    //mcSideDir->cd();
    //mcPOT->Write();
    

    PlotUtils::TargetUtils targetInfo;
    assert(error_bands["cv"].size() == 1 && "List of error bands must contain a universe named \"cv\" for the flux integral.");
    std::cout << "Looping over Vars to fill systematics" << std::endl;
    for(const auto& var: vars)
    {
      //Flux integral only if systematics are being done (temporary solution)
      util::GetFluxIntegral(*error_bands["cv"].front(), var->efficiencyNumerator->hist)->Write((var->GetName() + "_reweightedflux_integrated").c_str());
      //Always use MC number of nucleons for cross section
      auto nNucleons = new TParameter<double>((var->GetName() + "_fiducial_nucleons").c_str(), targetInfo.GetTrackerNNucleons(minZ, maxZ, true, apothem));
      nNucleons->Write();
    }
    for(const auto& var: vars2D)
    {
      auto nNucleons = new TParameter<double>((var->GetName() + "_fiducial_nucleons").c_str(), targetInfo.GetTrackerNNucleons(minZ, maxZ, true, apothem));
      nNucleons->Write();
      util::GetFluxIntegral(*error_bands["cv"].front(), var->efficiencyNumerator->hist)->Write((var->GetName() + "_reweightedflux_integrated").c_str()); 
      
    }
    
    std::cout << "Writing Data Results" << std::endl;
    //Write data results
    TFile* dataOutDir = TFile::Open(DATA_OUT_FILE_NAME, "RECREATE");
    //TFile* dataSideDir = TFile::Open(DATA_SIDE_FILE_NAME, "RECREATE");
    if(!dataOutDir)
    {
      std::cerr << "Failed to open a file named " << DATA_OUT_FILE_NAME << " in the current directory for writing histograms.\n";
      return badOutputFile;
    }
    //if(!dataSideDir)
    //{
    //  std::cerr << "Failed to open a file named " << DATA_SIDE_FILE_NAME << " in the current directory for writing histograms.\n";
    //  return badOutputFile;
   // }

    std::cout << "Writing Data Vars" << std::endl;
    for(auto& var: vars) var->WriteData(*dataOutDir);
    for(auto& var: vars2D) var->Write(*dataOutDir);
    for(auto& study: data_studies) study->SaveOrDraw(*dataOutDir);  

    //for(auto& var: sidevars) var->WriteData(*dataSideDir);
    //for(auto& var: sidevars2D) var->Write(*dataSideDir); 
    //Protons On Target
    auto dataPOT = new TParameter<double>("POTUsed", options.m_data_pot);
    //dataPOT->Write();
    dataOutDir->cd();
    dataPOT->Write();
    dataOutDir->cd();
    //dataSideDir->cd();
    //dataPOT->Write();
    std::cout << "Success" << std::endl;
  }
  catch(const ROOT::exception& e)
  {
    std::cerr << "Ending on a ROOT error message.  No histograms will be produced.\n"
              << "If the message talks about \"TNetXNGFile\", this could be a problem with dCache.  The message is:\n"
              << e.what() << "\n" << USAGE << "\n";
    return badFileRead;
  }

  return success;
}
