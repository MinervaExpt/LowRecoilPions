#define MC_OUT_FILE_NAME "SideEventLoopMC.root"
#define DATA_OUT_FILE_NAME "SideEventLoopData.root"
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
#include "event/MichelEvent.h"
#include "systematics/Systematics.h"
#include "cuts/MaxPzMu.h"
#include "util/Variable.h"
#include "util/Variable2D.h"
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
#include "cuts/hasMichel.h"
#include "event/Michel.h"
#include "cuts/BestMichelDistance2D.h"
#include "cuts/VtxMatchFirst.h"
//#include "cuts/hasTruePion.h"
#include "cuts/PTRangeReco.h"
#include "cuts/GetClosestMichel.h"
//#include "cuts/Distance2DSideband.h"
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
#include "cuts/RemoveSignalEvents.h"
#include "cuts/GetClosestMichelSide.h"
#include "cuts/Distance2DSideband.h"


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

//==============================================================================
// Loop and Fill
//==============================================================================
void LoopAndFillEventSelection(
    PlotUtils::ChainWrapper* chain,
    std::map<std::string, std::vector<CVUniverse*> > error_bands,
    std::vector<Variable*> vars,
    std::vector<Variable2D*> vars2D,
    //std::vector<Study*> studies,
    std::vector<Variable*> sidevars,
    std::vector<Variable2D*> sidevars2D,
    //std::vector<Study*> sideband_studies,
    PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
    PlotUtils::Model<CVUniverse, MichelEvent>& model)
{
  assert(!error_bands["cv"].empty() && "\"cv\" error band is empty!  Can't set Model weight.");
  auto& cvUniv = error_bands["cv"].front();

  std::cout << "Starting MC reco loop...\n";
  const int nEntries = chain->GetEntries(); // TODO: July 10 CHANGE BACK TO GETENTRIES
  for (auto i=0; i < nEntries; ++i)
  {
    if(i%1000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
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
	        double dist = myevent.m_nmichels[0].Best3Ddist;
   	        double recotpi = universe->GetTpiFromRange(dist)/1000.; // in GeV
		TVector3 p_pi(myevent.m_nmichels[0].reco_ppix, myevent.m_nmichels[0].reco_ppiy, myevent.m_nmichels[0].reco_ppiz); // MeV
                int idxpi = universe->GetTrueLowestTpiIdx(); // integer         
                double t_truth = universe->GetTruet(idxpi);
                double t_reco = universe->Gettreco(recotpi,p_pi);
	        double EavMinusTpi = -9999.;
		//std::cout << "reco |t| = " << t_reco << std::endl;
		//std::cout << "truth |t| = " << t_truth << std::endl;	
		//std::cout << "Printing relevant vars: recotpi " << recotpi << std::endl;
   		for(auto& var: vars) {
        		if (var->GetName() == "Tpi"){
			   var->selectedMCReco->FillUniverse(universe, recotpi, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, recotpi, weight2);
                           var->FillCategHistos(*universe,recotpi, weight2);
			}
			else if (var->GetName() == "EavMinusTpi"){
			   double eav = var->GetRecoValue(*universe);
			   double eavtpidiff = eav - recotpi;
			   if (recotpi > eav){ 
				eavtpidiff = 0.0;
				EavMinusTpi = 0.0;
			   }
			   EavMinusTpi = eavtpidiff;
			   //std::cout << "Printing relevant vars: EavMinusTpi " << eavtpidiff << std::endl;
			   var->selectedMCReco->FillUniverse(universe, eavtpidiff, weight2); //"Fake data" for closure
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, eavtpidiff, weight2);
                           var->FillCategHistos(*universe,eavtpidiff, weight2);
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
				var->selectedMCReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //"Fake data" for closure
                                (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
                                var->FillCategHistos(*universe,var->GetRecoValue(*universe), weight2);
			}
			
  		}
        	for(auto& var: vars2D) {
  			const std::string keyName = var->GetName();
    			const size_t endOfPrefix1 = keyName.find("EavMinusTpi_");
    			const size_t endOfPrefix2 = keyName.find("Tpi_");
			const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");

		        //std::cout << "looping over 2D vars" << std::endl;
			if(endOfPrefix1 != std::string::npos){
			  double eav = var->GetRecoValueX(*universe);
			  double eavtpidiff = eav - recotpi;
			  if (recotpi > eav) eavtpidiff = 0.0;
			  var->mcTotalHist->FillUniverse(universe, eavtpidiff, var->GetRecoValueY(*universe), weight2);
                          (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, eavtpidiff, var->GetRecoValueY(*universe), weight2);
                          var->FillCategHistos(*universe,eavtpidiff, var->GetRecoValueY(*universe), weight2); 
			}
			else if(endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos){
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
			   var->mcTotalHist->FillUniverse(universe, t_reco, recotpi, weight2);
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, recotpi, weight2);
                           var->FillCategHistos(*universe,t_reco, recotpi, weight2);
			}
			else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
		 	   //double eav = var->GetRecoValueY(*universe);
                           //double eavtpidiff = eav - recotpi;
                           //if (recotpi > eav) eavtpidiff = 0.0;
			   var->mcTotalHist->FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                           (*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                           var->FillCategHistos(*universe,t_reco, EavMinusTpi, weight2);

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

			for(auto& var: vars)
          		{
				double diff = var->GetTrueValue(*universe) - var->GetRecoValue(*universe);
				

				if (var->GetName() == "Tpi"){
					var->migration->FillUniverse(universe, recotpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, recotpi, weight2); //Ef
				}
				else if (var->GetName() == "EavMinusTpi"){ // Get the Variable that is the difference between Available Energy and Tpi. Non pion available energy. Not sure yet what to do about case with multiple pions. 
                           		//double eav = var->GetRecoValue(*universe);
                           		//double eavtpidiff = eav - recotpi;
					//if (recotpi > eav) eavtpidiff = 0.0;
					var->migration->FillUniverse(universe, EavMinusTpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, weight2);
				}
				else if (var->GetName() == "texp"){
					var->migration->FillUniverse(universe, t_reco, t_truth, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, weight2);
                                        var->selectedSignalReco->FillUniverse(universe, t_reco, weight2);
					
				}
				else if (var->GetName() == "Tpibins"){
					var->migration->FillUniverse(universe, recotpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, recotpi, weight2);
				}
				else if (var->GetName() == "EavMinTpibins"){
					//double eav = var->GetRecoValue(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
                                        var->migration->FillUniverse(universe, EavMinusTpi, var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, weight2);
				}
				else {
					var->migration->FillUniverse(universe, var->GetRecoValue(*universe), var->GetTrueValue(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
                                        var->selectedSignalReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //Efficiency numerator in reco variables.  Useful for warping studies 

				}
				//if (var->GetName() == "AvailableE" and var->GetTrueValue(*universe) > 2000.) universe->PrintTrueArachneLink();			
			}
			for(auto& var: vars2D)
          		{
                 		const std::string keyName = var->GetName();
                        	const size_t endOfPrefix1 = keyName.find("EavMinusTpi");
                        	const size_t endOfPrefix2 = keyName.find("Tpi_");
				const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        	const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        	const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");

				if(endOfPrefix1 != std::string::npos){
                          		//double eav = var->GetRecoValueX(*universe);
                          		//double eavtpidiff = eav - recotpi;
                          		//if (recotpi > eav) eavtpidiff = 0.0;
					var->selectedSignalReco->FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
					var->FillResponse(EavMinusTpi, var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
				}
				else if (endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos){
				        //std::cout << keyName << std::endl;
					var->selectedSignalReco->FillUniverse(universe, recotpi, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(recotpi, var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);

				}
				else if(endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					var->selectedSignalReco->FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(t_reco, var->GetRecoValueY(*universe),t_truth, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
				}
				else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					var->selectedSignalReco->FillUniverse(universe, t_reco, recotpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(t_reco, recotpi, t_truth, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);

				}
				else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					//double eav = var->GetRecoValueY(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
					var->selectedSignalReco->FillUniverse(universe, t_reco, EavMinusTpi, weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight2);
                                        var->FillResponse(t_reco, EavMinusTpi, t_truth, var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
				}
				else{
					var->selectedSignalReco->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                                        var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
					var->FillResponse(var->GetRecoValueX(*universe), var->GetRecoValueY(*universe),var->GetTrueValueX(*universe), var->GetTrueValueY(*universe),universe->ShortName(),weight2,unv_count);
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
			int bkgd_ID = -1;
          		if (universe->GetCurrent()==2)bkgd_ID=0;
          		else if( universe->GetCurrent()==1) bkgd_ID=1;
			
			
		
          		for(auto& var: vars){

          			if (var->GetName() == "Tpi"){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, recotpi, weight2);
				}
				else if (var->GetName() == "EavMinusTpi"){
                                        double eav = var->GetRecoValue(*universe);
                                        double eavtpidiff = eav - recotpi;
					if (recotpi > eav) eavtpidiff = 0.0;
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, eavtpidiff, weight2);
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
				        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
				}	
			}
          		for(auto& var: vars2D){
				const std::string keyName = var->GetName();
                                const size_t endOfPrefix1 = keyName.find("EavMinusTpi");
                                const size_t endOfPrefix2 = keyName.find("Tpi_");
				const size_t endOfPrefix3 = keyName.find("texp_eavbins");
                        	const size_t endOfPrefix4 = keyName.find("texp_Tpibins");
                        	const size_t endOfPrefix5 = keyName.find("texp_EavMinTpibins");
				if(endOfPrefix1 != std::string::npos){
                                        //double eav = var->GetRecoValueX(*universe);
                                        //double eavtpidiff = eav - recotpi;
                                        //if (recotpi > eav) eavtpidiff = 0.0;
				        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), weight2);

				}
				else if (endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos){
					(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe,recotpi, var->GetRecoValueY(*universe), weight2);
				}
				else if(endOfPrefix3 != std::string::npos && endOfPrefix4 == std::string::npos  && endOfPrefix5 == std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){

				        (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, var->GetRecoValueY(*universe), weight2);

				}
				else if(endOfPrefix4 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					 (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, recotpi, weight2);
			
				}
				else if(endOfPrefix5 != std::string::npos &&  endOfPrefix2 == std::string::npos && endOfPrefix1 == std::string::npos){
					 //double eav = var->GetRecoValueY(*universe);
                                         //double eavtpidiff = eav - recotpi;
                                         //if (recotpi > eav) eavtpidiff = 0.0;
					 (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, t_reco, EavMinusTpi, weight2);

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
  for (auto v : vars2D) v->getResponseObjects(error_bands);
  std::cout << "Finished MC reco loop.\n";
}


void LoopAndFillData( PlotUtils::ChainWrapper* data,
			        std::vector<CVUniverse*> data_band,
				std::vector<Variable*> vars,
                                std::vector<Variable2D*> vars2D,
                                //std::vector<Study*> studies,
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
      if(i%1000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
      //std::cout << "Creating Michel Event" << std::endl;
      //if (universe->ShortName() != "cv") continue;
      MichelEvent myevent; 
      //std::cout << "Applying Cuts to Data Event " << std::endl;
      const auto cutResults = michelcuts.isDataSelected(*universe, myevent);
      if (!cutResults.all()) continue;
      if (cutResults.all()){
		double dist = myevent.m_nmichels[0].Best3Ddist; // mm
                double recotpi = universe->GetTpiFromRange(dist)/1000.; //GeV
		TVector3 p_pi(myevent.m_nmichels[0].reco_ppix, myevent.m_nmichels[0].reco_ppiy, myevent.m_nmichels[0].reco_ppiz); // MeV
		double t_reco;
	       	t_reco = universe->Gettreco(recotpi,p_pi);
	        double EavMinusTpi = -9999.;		
		for(auto& var: vars)
      		{
      			if (var->GetName() == "Tpi"){
			   var->dataHist->FillUniverse(universe, recotpi, 1);
			}
			else if (var->GetName() == "EavMinusTpi"){
                           double eav = var->GetRecoValue(*universe);
                           double eavtpidiff = (eav - recotpi);
			   if (recotpi > eav){
				eavtpidiff = 0.0;
				EavMinusTpi = 0.0;
			   }
			   EavMinusTpi = eavtpidiff;
			   var->dataHist->FillUniverse(universe, eavtpidiff, 1);
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
		        if(endOfPrefix1 != std::string::npos){
				//double eav = var->GetRecoValueX(*universe);
                                //double eavtpidiff = eav - recotpi;
                                //if (recotpi > eav) eavtpidiff = 0.0;
				var->dataHist->FillUniverse(universe, EavMinusTpi, var->GetRecoValueY(*universe), 1);
			}
			else if(endOfPrefix2 != std::string::npos && endOfPrefix1 == std::string::npos){
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
    if(i%1000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
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
	if (!isEffDenom) continue;
        const double weight = model.GetWeight(*universe, myevent);
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

        for(auto var: vars)
        {
          if (var->GetName() == "texp"){
		var->efficiencyDenominator->FillUniverse(universe, t_truth, weight);
	  }
	  else {
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
	  const size_t endOfPrefix3 = keyName.find("texp_");
  	  if(endOfPrefix3 != std::string::npos){
		var->efficiencyDenominator->FillUniverse(universe, t_truth, var->GetTrueValueY(*universe), weight);
	  }
          else{
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
  preCuts.emplace_back(new reco::IsNeutrino<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  preCuts.emplace_back(new reco::Apothem<CVUniverse, MichelEvent>(apothem));
  preCuts.emplace_back(new reco::MaxMuonAngle<CVUniverse, MichelEvent>(20.));
  preCuts.emplace_back(new reco::HasMINOSMatch<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::NoDeadtime<CVUniverse, MichelEvent>(1, "Deadtime"));
  preCuts.emplace_back(new PTRangeReco<CVUniverse, MichelEvent>(0.0,1.8));
  preCuts.emplace_back(new RecoilERange<CVUniverse, MichelEvent>(0.0,1.2));
  preCuts.emplace_back(new PmuCut<CVUniverse, MichelEvent>(1.5));
  preCuts.emplace_back(new PzmuCut<CVUniverse, MichelEvent>(20.));
  preCuts.emplace_back(new hasMichel<CVUniverse, MichelEvent>());
   
  //preCuts.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  //preCuts.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));
  //nosidebands.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  //nosidebands.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));

  preCuts.emplace_back(new RemoveSignalEvents<CVUniverse, MichelEvent>(150.));
  preCuts.emplace_back(new Distance2DSideband<CVUniverse, MichelEvent>(1000.));
  preCuts.emplace_back(new GetClosestMichelSide<CVUniverse, MichelEvent>(1));
  


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
  std::vector<double> tpibins = {1., 10., 15., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 325., 350., 400., 500., 700., 1000., 1500., 2000., 3000.}; 

  //std::vector<double> tpibins = {0.0, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 325., 350., 400., 500., 700., 1000.};   
  std::vector<double> rangebins = {0.01, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 44., 50., 56., 62., 70., 80.,90., 100., 110.,  120., 140., 160., 180., 200., 220., 240., 260., 280., 300., 325., 350., 375., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1200., 1400., 1800., 2400.};             
  std::vector<double> recoilbins = {0.0, 200., 300., 400., 500., 600., 800., 1000., 1200., 1400., 1600.};
  std::vector<double> recoilbins2 = {0.0, 150., 225., 300., 400., 500., 600., 700., 800., 900., 1000.};//, 1200.};// 2200., 2600., 3000., 6000.};
  std::vector<double> wexpbins = {0.0,0.2, 0.4, .6, .8, 1.0, 1.1, 1.2, 1.3, 1.5, 1.7, 2.0, 2.5, 3.0, 4.0};
  std::vector<double> eavtpibins = {0.0, 75., 150., 225., 300., 400., 500., 600., 700., 800., 900., 1000.};
  std::vector<double> eavbinsGEV = {0.0, .075, .150, .225, .300, .400, .500, .600, .700, .800, .900, 1.000};
  std::vector<double> tpibinsGEV = {.001, .010, .015, .020, .024, .028, .032, .036, .040, .046, .052,.060, .070, .080, .100, .125,.150, .175, .200, .225, .250, .275, .300, .325, .350, .400, .500, 1.0, 1.5, 2.0, 2.5, 3.0};  
  std::vector<double> tbins = {0.005,0.01, 0.015, 0.020, 0.025, 0.030, 0.035, 0.040, 0.045, 0.05, 0.075, 0.10, 0.125, 0.15, 0.175,0.20, 0.25,0.30,0.35, 0.4, 0.45, 0.50, 0.55, 0.60 , 0.7, 0.80,0.9, 1.,1.5, 2.0, 2.5, 3.0};
  std::vector<double> eavbin2D = {0.0, .075, .150, .225, .300, .500, .700, 1.000};
  std::vector<double> tpibins2D = {0.0, 0.03, 0.06, 0.1, 0.15, 0.2, 0.25, 0.3, .500, 1.0}; // binning for Tpi vs |t| in GeV 
  std::vector<double> eavtpibins2D = {0.0, 0.025, 0.050, .075, .150, .225, .300, 1.000};

  //const double robsRecoilBinWidth = 75; //MeV
  //for(int whichBin = 0; whichBin < 20 + 1; ++whichBin) robsRecoilBins.push_back(robsRecoilBinWidth * whichBin);

  std::vector<Variable*> vars = {
    new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),
    new Variable("q3bins", "q3bins", dansPTBins,  &CVUniverse::Getq3, &CVUniverse::GetTrueQ3),
    new Variable("AvailableE", "AvailableE", eavbinsGEV, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEAvailGEV),
    new Variable("pzmu", "p_{||, #mu} [GeV/c]", dansPzBins, &CVUniverse::GetMuonPz, &CVUniverse::GetMuonPzTrue),    
    new Variable("pTmu", "p_{T, #mu} [GeV/c]", ptbins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue), //0
    //new Variable("q3", "q3 [GeV]", mehreenQ3Bins, &CVUniverse::Getq3, &CVUniverse::GetTrueQ3), //1
    new Variable("q2", "q2 [GeV^2]", dansPTBins, &CVUniverse::GetQ2Reco, &CVUniverse::GetTrueQ2GeV), //2
    new Variable("EavMinusTpi", "EavMinusTpi", eavbinsGEV, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEavMinusTpiGEV), 
    //new Variable("pxmu" "px_{#mu} [GeV/c]", dansPTBins,&CVUniverse::GetPXmuGeV, &CVUniverse::GetMuPxTrue),
    //new Variable("pymu" "py_{#mu} [GeV/c]", dansPTBins,&CVUniverse::GetPYmuGeV, &CVUniverse::GetMuPyTrue),
    //new Variable("pzmu" "px_{#mu} [GeV/c]", dansPTBins,&CVUniverse::GetPZmuGeV, &CVUniverse::GetMuPzTrue),

    //new Variable("pzmu", "p_{||, #mu} [GeV/c]", dansPzBins, &CVUniverse::GetMuonPz, &CVUniverse::GetMuonPzTrue),//3
    //new Variable("Emu", "E_{#mu} [GeV]", robsEmuBins, &CVUniverse::GetEmuGeV, &CVUniverse::GetElepTrueGeV),//4
    //new Variable("q3pTdiff","[GeV]", dansPTBins, &CVUniverse::Recoq3pTdiff, &CVUniverse::GetTrueq3pTdiff),//5
    new Variable("Tpi", "Tpi", tpibinsGEV, &CVUniverse::GetDefault, &CVUniverse::GetTrueLowestTpiEventGEV), 
    new Variable("texp", "texp", tbins,  &CVUniverse::GetDefault,  &CVUniverse::GetDefault),
    new Variable("eavbins", "eavbins", eavbin2D, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEAvailGEV),
    new Variable("Tpibins", "Tpibins", tpibins2D, &CVUniverse::GetDefault, &CVUniverse::GetTrueLowestTpiEventGEV),
    new Variable("EavMinTpibins", "EavMinTpibins", eavtpibins2D, &CVUniverse::NewEavailGEV, &CVUniverse::GetTrueEavMinusTpiGEV),

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
 vars2D.push_back(new Variable2D(*vars[2], *vars[0]));// pTbins vs Eavail
 vars2D.push_back(new Variable2D(*vars[6], *vars[0]));//pTbins vs (Eavail - Tpi)
 vars2D.push_back(new Variable2D(*vars[7], *vars[0]));//pTbins vs Tpi
 vars2D.push_back(new Variable2D(*vars[8], *vars[9]));//Eavail vs |t| in GeV. Available E is on y axis
 vars2D.push_back(new Variable2D(*vars[8], *vars[10]));//Tpi vs |t| in GeV >>> Tpi on y axis
 vars2D.push_back(new Variable2D(*vars[8], *vars[11]));//Eav - Tpi vs |t| in GeV >>> Eav - Tpi on y axis

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

  CVUniverse* data_universe = new CVUniverse(options.m_data);
  std::vector<CVUniverse*> data_band = {data_universe};
  std::map<std::string, std::vector<CVUniverse*> > data_error_bands;
  data_error_bands["cv"] = data_band;
  
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
    CVUniverse::SetTruth(false);
    LoopAndFillEventSelection(options.m_mc, error_bands, vars, vars2D, sidevars, sidevars2D, mycuts, model);
    std::cout << "MC cut summary:\n" << mycuts << "\n";
    //mycuts.resetStats();
    CVUniverse::SetTruth(true);
    LoopAndFillEffDenom(options.m_truth, truth_bands, vars, vars2D, mycuts, model);
    options.PrintMacroConfiguration(argv[0]);
    std::cout << "MC Cut Summary:\n" << mycuts << "\n";
    mycuts.resetStats();
    CVUniverse::SetTruth(false);
    CVUniverse::SetTruth(false);
    LoopAndFillData(options.m_data, data_band, vars, vars2D, sidevars, sidevars2D, mycuts);
    std::cout << "Data Cut summary:\n" << mycuts << "\n";
    //mycuts.resetStats();
    
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
