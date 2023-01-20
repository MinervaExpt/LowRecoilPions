#define MC_OUT_FILE_NAME "runEventLoopMC.root"
#define DATA_OUT_FILE_NAME "runEventLoopData.root"
#define MC_SIDE_FILE_NAME "sidebandEventLoopMC.root"
#define DATA_SIDE_FILE_NAME "sidebandEventLoopData.root"


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
#include "cuts/Distance2DSideband.h"
#include "cuts/RecoilERange.h"
#include "cuts/PmuCut.h"
#include "cuts/WexpRecoCut.h"
#include "event/SetDistanceMichelSideband.h"
#include "event/SetDistanceMichelSelection.h"
#include "event/GetClosestMichel.h"
//#include "Binning.h" //TODO: Fix me

//PlotUtils includes
#include "PlotUtils/makeChainWrapper.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"
#include "PlotUtils/MacroUtil.h"
#include "PlotUtils/MnvPlotter.h"
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
#include "util/PionReweighter.h"
#include "util/DiffractiveReweighter.h"
#include "cuts/LowRecPionSignal.h"
#include "PlotUtils/LowQ2PiReweighter.h"
#include "PlotUtils/GeantNeutronCVReweighter.h"
#include "PlotUtils/FSIReweighter.h"
#include "util/COHPionReweighter.h"
#include "util/TargetMassReweighter.h"
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
    PlotUtils::Cutter<CVUniverse, MichelEvent>& sidecuts,
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
    const double cvWeight =model.GetWeight(*cvUniv, cvEvent);// TODO: Put this model weight back. model.GetWeight(*cvUniv, cvEvent); 
    //std::cout << "Printing Muon Momentum At Beginning of Entry Loop: " << cvUniv->GetMuonPTTrue() << std::endl;
    //std::cout << " ============================================================= " << std::endl;
    //=========================================
    // Systematics loop(s)
    //=========================================
    for (auto band : error_bands)
    {
      std::vector<CVUniverse*> error_band_universes = band.second;
      for (auto universe : error_band_universes)
      {
        //if (universe->ShortName() != "cv") continue;
        //MichelEvent myevent; // make sure your event is inside the error band loop. 
        //if (universe->ShortName() != "cv") continue;
        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);
        MichelEvent myevent;
        const auto cutResults = michelcuts.isMCSelected(*universe, myevent, cvWeight);
	//const auto cutResults = michelcuts.isDataSelected(*universe, myevent);       
        //if (universe->ShortName() != "cv") continue;
	//if (!cutResults.all()) continue;
        const double weight2 = model.GetWeight(*universe, myevent);
	if (cutResults.all()){

		for(auto& var: vars) {
        		var->selectedMCReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //"Fake data" for closure
            		(*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
            		var->FillCategHistos(*universe,var->GetRecoValue(*universe), weight2);
        	}
        	for(auto& var: vars2D) {
                	var->mcTotalHist->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                	(*var->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                	var->FillCategHistos(*universe,var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
                	var->mc_trueHist->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);

         	}
        	const bool isSignal = michelcuts.isSignal(*universe, weight2);
		const bool isPhaseSpace = michelcuts.isPhaseSpace(*universe, weight2);
		if(isSignal){

			for(auto& var: vars)
          		{
				double diff = var->GetTrueValue(*universe) - var->GetRecoValue(*universe);
				

				var->migration->FillUniverse(universe, var->GetRecoValue(*universe), var->GetTrueValue(*universe), weight2);
                                var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValue(*universe), weight2);
				var->selectedSignalReco->FillUniverse(universe, var->GetRecoValue(*universe), weight2); //Efficiency numerator in reco variables.  Useful for warping studies 
			}
			for(auto& var: vars2D)
          		{
            			var->efficiencyNumerator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight2);
          		}
		} // end of if Signal()
                else if (!isSignal){
			int bkgd_ID = -1;
          		if (universe->GetCurrent()==2)bkgd_ID=0;
          		else if( universe->GetCurrent()==1) bkgd_ID=1;

		
          		for(auto& var: vars){

                 		(*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, var->GetRecoValue(*universe), weight2);
          		}
          		for(auto& var: vars2D) (*var->m_backgroundHists)[bkgd_ID].FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), weight2);
        	} // End of else statement for if Signal 
       } // If event passes PreCuts
       else if (!cutResults.all()){
           const auto sideResults = sidecuts.isMCSelected(*universe, myevent, cvWeight);
           if (sideResults.all()){ 
               for(auto& svar: sidevars) {
                        svar->selectedMCReco->FillUniverse(universe, svar->GetRecoValue(*universe), weight2); //"Fake data" for closure
                        (*svar->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, svar->GetRecoValue(*universe), weight2);
                        svar->FillCategHistos(*universe,svar->GetRecoValue(*universe), weight2);
                }
               for(auto& svar: sidevars2D) {
                        svar->mcTotalHist->FillUniverse(universe, svar->GetRecoValueX(*universe), svar->GetRecoValueY(*universe), weight2);
                        (*svar->m_MChists)[universe->GetInteractionType()].FillUniverse(universe, svar->GetRecoValueX(*universe), svar->GetRecoValueY(*universe), weight2);
                        svar->FillCategHistos(*universe,svar->GetRecoValueX(*universe), svar->GetRecoValueY(*universe), weight2);
                        svar->mc_trueHist->FillUniverse(universe, svar->GetTrueValueX(*universe), svar->GetTrueValueY(*universe), weight2);

               }
               const bool isSignal = sidecuts.isSignal(*universe, weight2);
               const bool isPhaseSpace = sidecuts.isPhaseSpace(*universe, weight2);
               if(isSignal){

                        for(auto& svar: sidevars)
                        {
                               // double diff = var->GetTrueValue(*universe) - var->GetRecoValue(*universe);


                                svar->migration->FillUniverse(universe, svar->GetRecoValue(*universe), svar->GetTrueValue(*universe), weight2);
                                svar->efficiencyNumerator->FillUniverse(universe, svar->GetTrueValue(*universe), weight2);
                                svar->selectedSignalReco->FillUniverse(universe, svar->GetRecoValue(*universe), weight2); //Efficiency numerator in reco variables.  Useful for warping studies 
                        }
                        for(auto& svar: sidevars2D)
                        {
                                svar->efficiencyNumerator->FillUniverse(universe, svar->GetTrueValueX(*universe), svar->GetTrueValueY(*universe), weight2);
                        }
               } // end of if Signal()
               else if (!isSignal){
                        int bkgd_ID = -1;
                        if (universe->GetCurrent()==2)bkgd_ID=0;
                        else if( universe->GetCurrent()==1) bkgd_ID=1;


                        for(auto& svar: sidevars){

                                (*svar->m_backgroundHists)[bkgd_ID].FillUniverse(universe, svar->GetRecoValue(*universe), weight2);
                        }
                        for(auto& svar: sidevars2D) (*svar->m_backgroundHists)[bkgd_ID].FillUniverse(universe, svar->GetRecoValueX(*universe), svar->GetRecoValueY(*universe), weight2);
               } // End of else statement for if Signal               
				
           }//End of If passes Sideband Cuts          	             
	   else { continue;}
       }//end of check for Sidebands

      } // End band's universe loop
 
    } // End Band loop
  
  //std::cout << "Printing Muon Momentum At End of Entry Loop: " << cvUniv->GetMuonPTTrue() << std::endl;
  //std::cout << "====================================================== ==========" << std::endl;
  } //End entries loop
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
				PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
				PlotUtils::Cutter<CVUniverse, MichelEvent>& sidecuts)

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
      //if (!cutResults.all()) continue;
      if (cutResults.all()){
      		for(auto& var: vars)
      		{
			if (var->GetName() == "LowTpi") continue;
			var->dataHist->FillUniverse(universe, var->GetRecoValue(*universe), 1);
      		}

      		for(auto& var: vars2D)
      		{
        		var->dataHist->FillUniverse(universe, var->GetRecoValueX(*universe), var->GetRecoValueY(*universe), 1);
      		}
      }// End of PreCuts Pass
      else if (!cutResults.all()){
	   const auto sideResults = sidecuts.isDataSelected(*universe, myevent);
           if (sideResults.all()){
               for(auto& svar: sidevars)
                {
                        if (svar->GetName() == "LowTpi") continue;
                        svar->dataHist->FillUniverse(universe, svar->GetRecoValue(*universe), 1);
                }

                for(auto& svar: sidevars2D)
                {
                        svar->dataHist->FillUniverse(universe, svar->GetRecoValueX(*universe), svar->GetRecoValueY(*universe), 1);
                }
           } // End of if Passes Sideband Cuts
       //    else {continue;}
      }
      else {continue;}
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
        MichelEvent myevent; //Only used to keep the Model happy
        //std::cout << "Set Michel Event " << std::endl;
        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);
	//std::cout << "checking to see if event is in signal and phase space" << std::endl;
        if (!michelcuts.isEfficiencyDenom(*universe, cvWeight)) continue; //Weight is ignored for isEfficiencyDenom() in all but the CV universe 
        const double weight = model.GetWeight(*universe, myevent); //Only calculate the weight for events that will use it

        //Fill efficiency denominator now: 
        for(auto var: vars)
        {
          var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValue(*universe), weight);
        }
        // Fill Studies denominator:
//        for(auto& study: studies) study->SelectedSignal(*universe, cvEvent, weight);

        for(auto var: vars2D)
        {
          var->efficiencyDenominator->FillUniverse(universe, var->GetTrueValueX(*universe), var->GetTrueValueY(*universe), weight);
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
  //PlotUtils::MinervaUniverse::SetMHRWeightNeutronCVReweight( true );
  //PlotUtils::MinervaUniverse::SetMHRWeightElastics( true ); 

  //Now that we've defined what a cross section is, decide which sample and model
  //we're extracting a cross section for.
  PlotUtils::Cutter<CVUniverse, MichelEvent>::reco_t  preCuts;
  PlotUtils::Cutter<CVUniverse, MichelEvent>::reco_t  sideband;
  PlotUtils::Cutter<CVUniverse, MichelEvent>::reco_t  sidebands;
  PlotUtils::Cutter<CVUniverse, MichelEvent>::truth_t signalDefinition, phaseSpace;

  //const double minZ = 5980, maxZ = 8590.07, apothem = 850.; // All in mm
  const double minZ = 5980, maxZ = 8422, apothem = 850; //All in mm
  preCuts.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  preCuts.emplace_back(new reco::Apothem<CVUniverse, MichelEvent>(apothem));
  preCuts.emplace_back(new reco::MaxMuonAngle<CVUniverse, MichelEvent>(20.));
  preCuts.emplace_back(new reco::HasMINOSMatch<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::NoDeadtime<CVUniverse, MichelEvent>(1, "Deadtime"));
  preCuts.emplace_back(new reco::IsNeutrino<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new PTRangeReco<CVUniverse, MichelEvent>(0.0,1.0));
  preCuts.emplace_back(new RecoilERange<CVUniverse, MichelEvent>(0.0,1.5));
  preCuts.emplace_back(new PmuCut<CVUniverse, MichelEvent>(1.5));
  preCuts.emplace_back(new hasMichel<CVUniverse, MichelEvent>());
   
  preCuts.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  preCuts.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));
  //nosidebands.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  //nosidebands.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));

  sidebands.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  sidebands.emplace_back(new reco::Apothem<CVUniverse, MichelEvent>(apothem));
  sidebands.emplace_back(new reco::MaxMuonAngle<CVUniverse, MichelEvent>(20.));
  sidebands.emplace_back(new reco::HasMINOSMatch<CVUniverse, MichelEvent>());
  sidebands.emplace_back(new reco::NoDeadtime<CVUniverse, MichelEvent>(1, "Deadtime"));
  sidebands.emplace_back(new reco::IsNeutrino<CVUniverse, MichelEvent>());
  sidebands.emplace_back(new PTRangeReco<CVUniverse, MichelEvent>(0.0,1.0));
  sidebands.emplace_back(new RecoilERange<CVUniverse, MichelEvent>(0.0,1.5));
  sidebands.emplace_back(new PmuCut<CVUniverse, MichelEvent>(1.5));
  sidebands.emplace_back(new hasMichel<CVUniverse, MichelEvent>()); 
  sidebands.emplace_back(new Distance2DSideband<CVUniverse, MichelEvent>(1000.));
  sidebands.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(1));

  signalDefinition.emplace_back(new truth::IsNeutrino<CVUniverse>());
  signalDefinition.emplace_back(new truth::IsCC<CVUniverse>());
  signalDefinition.emplace_back(new truth::HasPion<CVUniverse>());
  //signalDefinition.emplace_back(new truth::EavailCut<CVUniverse>());
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
  phaseSpace.emplace_back(new truth::pTRangeLimit<CVUniverse>(0., 1.0));
  phaseSpace.emplace_back(new truth::pMuCut<CVUniverse>(1.5));
  //phaseSpace.emplace_back(new truth::TrueEavailCut<CVUniverse>());
  //phaseSpace.emplace_back(new truth::EAvailCut<CVUniverse>(1500.));

  PlotUtils::Cutter<CVUniverse, MichelEvent> mycuts(std::move(preCuts), std::move(sideband) , std::move(signalDefinition),std::move(phaseSpace));
  PlotUtils::Cutter<CVUniverse, MichelEvent> sidecuts(std::move(sidebands), std::move(sideband) , std::move(signalDefinition),std::move(phaseSpace));

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
 

  /*  
  std::vector<std::unique_ptr<PlotUtils::Reweighter<CVUniverse, MichelEvent>>> MnvTunev4;
  MnvTunev4.emplace_back(new PlotUtils::FluxAndCVReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::GENIEReweighter<CVUniverse, MichelEvent>(true, true));
  MnvTunev4.emplace_back(new PlotUtils::LowRecoil2p2hReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::MINOSEfficiencyReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::RPAReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::LowQ2PiReweighter<CVUniverse, MichelEvent>("MENU1PI"));
  MnvTunev4.emplace_back(new PlotUtils::DiffractiveReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::GeantNeutronCVReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::FSIReweighter<CVUniverse, MichelEvent>(true, true)); 
  MnvTunev4.emplace_back(new PlotUtils::COHPionReweighter<CVUniverse, MichelEvent>());
  MnvTunev4.emplace_back(new PlotUtils::TargetMassReweighter<CVUniverse, MichelEvent>()); 
  //MnvTunev4.emplace_back(new PlotUtils::PionReweighter<CVUniverse,MichelEvent>());
  PlotUtils::Model<CVUniverse, MichelEvent> model(std::move(MnvTunev4));
  */
   
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
  std::map< std::string, std::vector<CVUniverse*> > truth_bands;
  if(doSystematics) truth_bands = GetStandardSystematics(options.m_truth);
  truth_bands["cv"] = {new CVUniverse(options.m_truth)};



  std::vector<double> dansPTBins = {0, 0.075, 0.10, 0.15, 0.20, 0.30, 0.4, 0.50,0.60 , 0.7, 0.80,0.9, 1.,1.1, 1.2, 1.3, 1.4, 1.5, 2.0, 3.0, 4.0},
                      dansPzBins = {1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 15, 20, 40, 60},
                      robsEmuBins = {0,1,2,3,4,5,7,9,12,15,18,22,36,50,75,80},
                      mehreenQ3Bins = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2},
		      mehreenpmubins = {0.0, 0.5, 1.0, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 15, 20, 40}, 
		     robsRecoilBins;
  
  			 
  int nq3mbins = mehreenQ3Bins.size() -1; 
  std::vector<double> tpibins = {0, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 325., 350., 400., 500., 700., 1000.};   
  std::vector<double> rangebins = {0, 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 44., 50., 56., 62., 70., 80.,90., 100., 110.,  120., 140., 160., 180., 200., 220., 240., 260., 280., 300., 325., 350., 375., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1200., 1400., 1800., 2400.};             
  std::vector<double> recoilbins = {0.0, 200., 300., 400., 500., 600., 800., 1000., 1200., 1400., 1600.};
  std::vector<double> recoilbins2 = {0.0, 150., 225., 300., 400., 500., 600., 700., 800., 900., 1000.};//, 1200.};// 2200., 2600., 3000., 6000.};
  std::vector<double> wexpbins = {0.0,0.2, 0.4, .6, .8, 1.0, 1.1, 1.2, 1.3, 1.5, 1.7, 2.0, 2.5, 3.0, 4.0};
  
  //const double robsRecoilBinWidth = 75; //MeV
  //for(int whichBin = 0; whichBin < 20 + 1; ++whichBin) robsRecoilBins.push_back(robsRecoilBinWidth * whichBin);

  std::vector<Variable*> vars = {
    new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),
    new Variable("q3bins", "q3bins", dansPTBins,  &CVUniverse::Getq3, &CVUniverse::GetTrueQ3),
    new Variable("AvailableE", "AvailableE", recoilbins2, &CVUniverse::NewEavail, &CVUniverse::GetTrueEAvail),
    new Variable("pzmu", "p_{||, #mu} [GeV/c]", dansPzBins, &CVUniverse::GetMuonPz, &CVUniverse::GetMuonPzTrue)    
 };

 std::vector<Variable2D*> vars2D;
 vars2D.push_back(new Variable2D(*vars[2], *vars[1]));// q3bins vs Eavail
 vars2D.push_back(new Variable2D(*vars[2], *vars[0]));// pTbins vs Eavail

 std::vector<Variable*> sidevars = {
    new Variable("pTmubins", "pTmubins", mehreenQ3Bins, &CVUniverse::GetMuonPT, &CVUniverse::GetMuonPTTrue),
    new Variable("q3bins", "q3bins", dansPTBins,  &CVUniverse::Getq3, &CVUniverse::GetTrueQ3),
    new Variable("AvailableE", "AvailableE", recoilbins2, &CVUniverse::NewEavail, &CVUniverse::GetTrueEAvail),
    new Variable("pzmu", "p_{||, #mu} [GeV/c]", dansPzBins, &CVUniverse::GetMuonPz, &CVUniverse::GetMuonPzTrue)    
 };
 
 std::vector<Variable2D*> sidevars2D;
 sidevars2D.push_back(new Variable2D(*sidevars[2], *sidevars[1]));// q3bins vs Eavail
 sidevars2D.push_back(new Variable2D(*sidevars[2], *sidevars[0]));// pTbins vs Eavail

 //std::vector<Study*> studies;
 //std::vector<Study*> sideband_studies;

  CVUniverse* data_universe = new CVUniverse(options.m_data);
  std::vector<CVUniverse*> data_band = {data_universe};
  std::map<std::string, std::vector<CVUniverse*> > data_error_bands;
  data_error_bands["cv"] = data_band;
  //Initiliaze selection
  for(auto& var: vars) var->InitializeMCHists(error_bands, truth_bands);
  for(auto& var: vars) var->InitializeDATAHists(data_band);
  for(auto& var: vars2D) var->InitializeMCHists(error_bands, truth_bands);
  for(auto& var: vars2D) var->InitializeDATAHists(data_band);
  //Initialize for sidebands
  for(auto& svar: sidevars) svar->InitializeDATAHists(data_band);
  for(auto& svar: sidevars) svar->InitializeMCHists(error_bands, truth_bands);
  for(auto& svar: sidevars2D) svar->InitializeMCHists(error_bands, truth_bands);
  for(auto& svar: sidevars2D) svar->InitializeDATAHists(data_band);  

  // Loop entries and fill
  try
  {
    CVUniverse::SetTruth(false);
    LoopAndFillEventSelection(options.m_mc, error_bands, vars, vars2D, sidevars, sidevars2D, mycuts, sidecuts, model);
    std::cout << "MC cut summary:\n" << mycuts << "\n";
    std::cout << "MC Side cuts summary: \n" << sidecuts << "\n";
    //mycuts.resetStats();
    CVUniverse::SetTruth(true);
    LoopAndFillEffDenom(options.m_truth, truth_bands, vars, vars2D, mycuts, model);
    options.PrintMacroConfiguration(argv[0]);
    std::cout << "MC Cut Summary:\n" << mycuts << "\n";
    mycuts.resetStats();
    sidecuts.resetStats();
    CVUniverse::SetTruth(false);
    //CVUniverse::SetTruth(false);
    LoopAndFillData(options.m_data, data_band, vars, vars2D, sidevars, sidevars2D, mycuts, sidecuts);
    std::cout << "Data Cut summary:\n" << mycuts << "\n";
    std::cout << "Data Side Cut summary: \n" << sidecuts << "\n";
    //mycuts.resetStats();
    
    std::cout  << "Opening the MCOUTPUT for EVENT hists" << std::endl;
    //Write MC results
    TFile* mcOutDir = TFile::Open(MC_OUT_FILE_NAME, "RECREATE");
    TFile* mcSideDir = TFile::Open(MC_SIDE_FILE_NAME, "RECREATE");
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
    //std::cout << "Saving Histos to Data Files" << std::endl;
   
    std::cout << "Writing Sideband Variables" << std::endl;
    for(auto& var: sidevars) var->WriteMC(*mcSideDir);
    std::cout << "WRiting 2D VARS to event level file " << std::endl;
    for(auto& var: sidevars2D) var->Write(*mcSideDir);
    //std::cout << "Saving Histos to Data Files" << std::endl;





    //Protons On Target
   
    std::cout << "Printing POT MC " << std::endl;
    auto mcPOT = new TParameter<double>("POTUsed", options.m_mc_pot);
    mcPOT->Write();
    mcOutDir->cd();
    mcPOT->Write();
    
    mcSideDir->cd();
    mcPOT->Write();
    

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



    std::cout << "Writing Data Results" << std::endl;
    //Write data results
    TFile* dataOutDir = TFile::Open(DATA_OUT_FILE_NAME, "RECREATE");
    TFile* dataSideDir = TFile::Open(DATA_SIDE_FILE_NAME, "RECREATE");
    if(!dataOutDir)
    {
      std::cerr << "Failed to open a file named " << DATA_OUT_FILE_NAME << " in the current directory for writing histograms.\n";
      return badOutputFile;
    }
    if(!dataSideDir)
    {
      std::cerr << "Failed to open a file named " << DATA_SIDE_FILE_NAME << " in the current directory for writing histograms.\n";
      return badOutputFile;
    }

    std::cout << "Writing Data Vars" << std::endl;
    for(auto& var: vars) var->WriteData(*dataOutDir);
    for(auto& var: vars2D) var->Write(*dataOutDir);
  
    for(auto& var: sidevars) var->WriteData(*dataSideDir);
    for(auto& var: sidevars2D) var->Write(*dataSideDir); 
    //Protons On Target
    auto dataPOT = new TParameter<double>("POTUsed", options.m_data_pot);
    dataPOT->Write();
    dataOutDir->cd();
    dataPOT->Write();
    dataOutDir->cd();
    dataSideDir->cd();
    dataPOT->Write();
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
