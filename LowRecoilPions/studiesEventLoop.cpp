#define MC_OUT_FILE_NAME "studiesMC.root"
#define DATA_OUT_FILE_NAME "studiesData.root"
#define MC_SIDE_FILE_NAME "runEventLoopMC_July272022_Sideband_q3bin6_reweight.root"
#define DATA_SIDE_FILE_NAME "runEventLoopDATA_July272022_Sideband_q3bin6_reweight.root"


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
#include "cuts/SignalDefinition.h"
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
#include "cuts/hasTruePion.h"
#include "cuts/PTRangeReco.h"
#include "cuts/GetClosestMichel.h"
#include "cuts/Distance2DSideband.h"
#include "cuts/RecoilERange.h"
#include "cuts/PmuCut.h"
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
#include "util/PionReweighter.h"
#include "cuts/LowRecPionSignal.h"
#include "PlotUtils/LowQ2PiReweighter.h"
#include "util/DiffractiveReweighter.h"
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


std::vector<std::vector<CVUniverse*>> groupCompatibleUniverses(const std::map<std::string, std::vector<CVUniverse*>> bands)
{
   std::vector<std::vector<CVUniverse*>> groupedUnivs;
   std::vector<CVUniverse*> vertical;
   for(const auto& band: bands)
   {
     if(band.first == "cv") vertical.insert(vertical.begin(), band.second.begin(), band.second.end());
     else
     {
       for(const auto univ: band.second)
       {
         if(univ->IsVerticalOnly()) vertical.push_back(univ);
         else groupedUnivs.push_back(std::vector<CVUniverse*>{univ});
       }
     }
   }
   groupedUnivs.insert(groupedUnivs.begin(), vertical);
   return groupedUnivs;
}

//==============================================================================
// Loop and Fill
//==============================================================================
void LoopAndFillEventSelection(
    PlotUtils::ChainWrapper* chain,
    std::map<std::string, std::vector<CVUniverse*> > error_bands,
    //std::vector<Variable*> vars,
    //std::vector<Variable2D*> vars2D,
    std::vector<Study*> studies,
    //std::vector<Variable*> sidevars,
    //std::vector<Variable2D*> sidevars2D,
    std::vector<Study*> sideband_studies,
    PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
    PlotUtils::Model<CVUniverse, MichelEvent>& model)
{
  assert(!error_bands["cv"].empty() && "\"cv\" error band is empty!  Can't set Model weight.");
  auto& cvUniv = error_bands["cv"].front();
  
  std::cout << "Starting MC reco loop...\n";
  const int nEntries = chain->GetEntries(); // TODO: July 10 CHANGE BACK TO GETENTRIES
  for (int i=0; i < nEntries; ++i)
  {
    if(i%1000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
    //std::cout << "Now Printing for Event " << i << std::endl;
    cvUniv->SetEntry(i);
    MichelEvent cvEvent;
    model.SetEntry(*cvUniv, cvEvent);
    const double cvWeight =model.GetWeight(*cvUniv, cvEvent);// TODO: Put this model weight back. model.GetWeight(*cvUniv, cvEvent);
    //michelcuts.isMCSelected(*cvUniv, cvEvent, cvWeight);  // Hacking event loop so cutter doesn't screw up Michel Event for Vertical Universes 
    //=========================================
    // Systematics loop(s)
    //=========================================
    for (auto band : error_bands)
    {
      std::vector<CVUniverse*> error_band_universes = band.second;
        
      for (auto universe : error_band_universes)
      {
        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);
        //if (universe->ShortName() != "cv") continue;
        MichelEvent myevent; // make sure your event is inside the error band loop. 
        
        //if(universe->IsVerticalOnly()) myevent = cvEvent; // Hacking event loop so cutter doesn't screw up Michel Event for Vertical Universes 
        //if (universe->ShortName() != "cv") continue;
        const double weight2 = model.GetWeight(*universe, myevent); 
        const auto cutResults = michelcuts.isMCSelected(*universe, myevent, cvWeight);
        //const auto cutResults = michelcuts.isDataSelected(*universe, myevent);       
        //if (universe->ShortName() != "cv") continue;
	if (!cutResults.all()) continue;
        if (cutResults.all()){
            //std::cout << "Universe Name: " << universe->ShortName() << " Weight is : " << weight2 << std::endl; 
            
            for(auto& study: studies) study->SelectedSignal(*universe, myevent, weight2);
            const bool isSignal = michelcuts.isSignal(*universe, weight2);
            if(isSignal){ int i =0;}
            else if(!isSignal){
               for(auto& study: studies) study->BackgroundSelected(*universe, myevent, weight2);
	    }
         	
        } // If event passes PreCuts
      } // End band's universe loop
    } // End Band loop
  } //End entries loop
  std::cout << "Finished MC reco loop.\n";
}


void LoopAndFillData( PlotUtils::ChainWrapper* data,
			        std::vector<CVUniverse*> data_band,
				//std::vector<Variable*> vars,
                                //std::vector<Variable2D*> vars2D,
                                std::vector<Study*> studies,
				//std::vector<Variable*> sidevars,
                                //std::vector<Variable2D*> sidevars2D,
                                std::vector<Study*> data_sidebands,
				PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts)

{
  std::cout << "Starting data loop...\n";
  const int nEntries = data->GetEntries(); // TODO: July 10 CHANGE BACK TO GEtENTRIES
  for (int i=0; i <nEntries; ++i) {
    //std::cout << "Now Printing for Event " << i << std::endl;
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
      	//setDistanceMichelSelection(*universe, myevent, 150.);
        //setClosestMichel(*universe, myevent,0);
        //if (!myevent.m_nmichelspass.empty() and myevent.selection ==1){
      		//std::cout << "Filling Data STudies" << std::endl;
      	 for(auto& study: studies) study->Selected(*universe, myevent, 1); 
       }	 // End of if (michelcuts.isDataSelected(*universe, myevent).all()) -> filling Selection
        /*else{
          setDistanceMichelSidebands(*universe, myevent, 150., 500.);
          setClosestMichel(*universe, myevent,1);
          if (!myevent.m_sidebandpass.empty() and myevent.sideband == 1){
        	for(auto& study: data_sidebands) study->Selected(*universe, myevent, 1); 

     	  } // End of else for Filling Sideband Variables
        } */// End of  else if no michelspass
     // }// End of PreCuts Pass
    //} // End of Data Band 
   } // End of Entries
  std::cout << "Finished data loop.\n";
}

void LoopAndFillEffDenom( PlotUtils::ChainWrapper* truth,
    				std::map<std::string, std::vector<CVUniverse*> > truth_bands,
    				std::vector<Variable*> vars,
                                std::vector<Variable2D*> vars2D,
 			        std::vector<Study*> studies,
    				PlotUtils::Cutter<CVUniverse, MichelEvent>& michelcuts,
                                PlotUtils::Model<CVUniverse, MichelEvent>& model)
{
  assert(!truth_bands["cv"].empty() && "\"cv\" error band is empty!  Could not set Model entry.");
  auto& cvUniv = truth_bands["cv"].front();

  std::cout << "Starting efficiency denominator loop...\n";
  const int nEntries = truth->GetEntries();
  for (int i=0; i<nEntries; ++i)
  {
    if(i%1000==0) std::cout << i << " / " << nEntries << "\r" << std::flush;
    MichelEvent cvEvent;
    cvUniv->SetEntry(i);
    model.SetEntry(*cvUniv, cvEvent);
    const double cvWeight = model.GetWeight(*cvUniv, cvEvent);

    //=========================================
    // Systematics loop(s)
    //=========================================
    for (auto band : truth_bands)
    {
      std::vector<CVUniverse*> truth_band_universes = band.second;
      for (auto universe : truth_band_universes)
      {
        MichelEvent myevent; //Only used to keep the Model happy

        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);

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
  PlotUtils::MinervaUniverse::SetNFluxUniverses(500);
  PlotUtils::MinervaUniverse::SetZExpansionFaReweight(false);


  // For MnvTunev4.3.1 
  PlotUtils::MinervaUniverse::SetNonResPiReweight(true);
  PlotUtils::MinervaUniverse::SetDeuteriumGeniePiTune(true);
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
  //preCuts.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  preCuts.emplace_back(new reco::Apothem<CVUniverse, MichelEvent>(apothem));
  preCuts.emplace_back(new reco::ZRange<CVUniverse, MichelEvent>("Tracker", minZ, maxZ));
  preCuts.emplace_back(new reco::MaxMuonAngle<CVUniverse, MichelEvent>(20.));
  preCuts.emplace_back(new reco::HasMINOSMatch<CVUniverse, MichelEvent>());
  preCuts.emplace_back(new reco::NoDeadtime<CVUniverse, MichelEvent>(1, "Deadtime"));
  preCuts.emplace_back(new reco::IsNeutrino<CVUniverse, MichelEvent>());
  //preCuts.emplace_back(new Q3RangeReco<CVUniverse, MichelEvent>(0.0,1.2));
  preCuts.emplace_back(new PTRangeReco<CVUniverse, MichelEvent>(0.0,1.0));
  preCuts.emplace_back(new RecoilERange<CVUniverse, MichelEvent>(0.0,1.0));
  preCuts.emplace_back(new PmuCut<CVUniverse, MichelEvent>(1.5));
  preCuts.emplace_back(new hasMichel<CVUniverse, MichelEvent>());
  //preCuts.emplace_back(new Distance2DSideband<CVUniverse, MichelEvent>(500.));
  
  preCuts.emplace_back(new BestMichelDistance2D<CVUniverse, MichelEvent>(150.));
  preCuts.emplace_back(new GetClosestMichel<CVUniverse, MichelEvent>(0));


  TFile* mc_MichelStudies = TFile::Open("studiesMC.root", "RECREATE");
  TFile* data_MichelStudies = TFile::Open("studiesData.root", "RECREATE");

  signalDefinition.emplace_back(new truth::IsNeutrino<CVUniverse>());
  signalDefinition.emplace_back(new truth::IsCC<CVUniverse>());
  signalDefinition.emplace_back(new truth::HasPion<CVUniverse>());
  
  phaseSpace.emplace_back(new truth::ZRange<CVUniverse>("Tracker", minZ, maxZ));
  phaseSpace.emplace_back(new truth::Apothem<CVUniverse>(apothem));
  phaseSpace.emplace_back(new truth::MuonAngle<CVUniverse>(20.));
  phaseSpace.emplace_back(new truth::PZMuMin<CVUniverse>(1500.));
  phaseSpace.emplace_back(new truth::pTRangeLimit<CVUniverse>(0., 1.0));
  phaseSpace.emplace_back(new truth::pMuCut<CVUniverse>(1.5));  
  //phaseSpace.emplace_back(new truth::q0RangeLimit<CVUniverse>(0.0, .7));

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
  */
  
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
  
  // Make a map of systematic universes
  // Leave out systematics when making validation histograms
  const bool doSystematics = (getenv("MNV101_SKIP_SYST") == nullptr);
  if(!doSystematics){
    std::cout << "Skipping systematics (except 1 flux universe) because environment variable MNV101_SKIP_SYST is set.\n";
    PlotUtils::MinervaUniverse::SetNFluxUniverses(2); //Necessary to get Flux integral later...  Doesn't work with just 1 flux universe though because _that_ triggers "spread errors".
  }

  //Group univeress here. Accesses the groupCompatibleUniverses function
  std::map< std::string, std::vector<CVUniverse*> > error_bands;
  std::vector<std::vector<CVUniverse*>> groupedUnivs;
 
  if(doSystematics){
     error_bands = GetStandardSystematics(options.m_mc);
     groupedUnivs = groupCompatibleUniverses(error_bands); //For running with Systematics 
  }
  else{
    std::map<std::string, std::vector<CVUniverse*> > band_flux = PlotUtils::GetFluxSystematicsMap<CVUniverse>(options.m_mc, CVUniverse::GetNFluxUniverses());
    error_bands.insert(band_flux.begin(), band_flux.end()); //Necessary to get flux integral later...
  }
  error_bands["cv"] = {new CVUniverse(options.m_mc)};
  std::map< std::string, std::vector<CVUniverse*> > truth_bands;
  if(doSystematics) truth_bands = GetStandardSystematics(options.m_truth);
  truth_bands["cv"] = {new CVUniverse(options.m_truth)};
 
  std::vector<double> dansPTBins = {0, 0.075, 0.10, 0.15, 0.20, 0.30, 0.4, 0.50,0.60 , 0.7, 0.80,0.9, 1.,1.1, 1.2, 1.3, 1.4, 1.5},
                      dansPzBins = {1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 6, 7, 8, 9, 10, 15, 20, 40, 60},
                      robsEmuBins = {0,1,2,3,4,5,7,9,12,15,18,22,36,50,75,80},
                      mehreenQ3Bins = {0.001, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4},
		      robsRecoilBins;
  
   
  int nq3mbins = mehreenQ3Bins.size() -1; 
  std::vector<double> tpibins = {1., 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 350., 400., 500., 700.,1000.};//, 1300., 1600., 2000.};   
   std::vector<double> rangebins = {1., 8., 16., 24., 32., 40., 50., 65., 80.,95., 110., 140., 170., 200., 230., 260., 290., 310., 360., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1200., 1400., 1800., 2400.};

  std::vector<double> recoilbins = {0.0, 150., 225., 300., 400., 500., 600.,700., 800., 900., 1000.};// 1200., 1400., 1600.};
  const double robsRecoilBinWidth = 50; //MeV
  for(int whichBin = 0; whichBin < 30 + 1; ++whichBin) robsRecoilBins.push_back(robsRecoilBinWidth * whichBin);

//This is where your list of Studies go for PerMichel variables -> Accessed through MichelEvent

  std::vector<Study*> studies;
  std::vector<Study*> sideband_studies; 
   
  std::function<double(const CVUniverse&, const MichelEvent&)> getq3 = [](const CVUniverse& univ, const MichelEvent& evt)
                                 {
                                   double q3 = evt.q3_reco;
				   //std::cout << "Printing q3 " << q3 << std::endl;
                                   return q3;
                                 }; 
  std::function<double(const CVUniverse&, const MichelEvent&)> getpT = [](const CVUniverse& univ, const MichelEvent& evt)
                                 { 
                                   double pT = evt.pT_reco;
				   //std::cout << "Printing pT " << pT  << std::endl;
                                   return pT;
                                 }; 
  std::function<double(const CVUniverse&, const MichelEvent&)> getEavail = [](const CVUniverse& univ, const MichelEvent& evt)
                                 { 
                                   double eavail = evt.eavail_reco;
				   //std::cout << "Printing eavail " << eavail << std::endl;
                                   return eavail;
                                 }; 
 
  std::function<double(const CVUniverse&, const MichelEvent&, const int)> delta_t = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   int evttype = evt.eventtype;
                                   double micheltime = evt.m_nmichels[whichMichel].time;
                                   double vtxtime = univ.GetVertex().t();
                                   double deltat = (micheltime - vtxtime/1000.); //hopefully this is in microseconds (mus)
                                   //if (evttype == 1) return deltat;
                                   //else return -9999.;
                                   return deltat;
                                 };

   std::function<double(const CVUniverse&, const MichelEvent&, const int)> permichel_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   //if (evt.eventtype == 1) return micheldist;
                                   //else return -9999.;
                                   return micheldist;
                                 };
   std::function<double(const CVUniverse&, const MichelEvent&, const int)> pertruepimichel_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
				   double pdg = evt.m_nmichels[whichMichel].true_parentpdg;
				   if (pdg == 211 || pdg == 321) return micheldist;
 				   else return 9999.;
                                 };
   std::function<double(const CVUniverse&, const MichelEvent&, const int)> permichel_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
				   double pdg = evt.m_nmichels[whichMichel].true_parentpdg;
				   if (pdg == 211 || pdg == 321) return evt.m_nmichels[whichMichel].pionKE;
				   else return 9999.;

				 };

  std::function<double(const CVUniverse&, const MichelEvent&, const int)> overlay_vtx_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
				   double KE = evt.m_nmichels[whichMichel].pionKE;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   if (overlayfrac > .5 && (matchtype == 1 || matchtype == 2 )) return KE;
                                   else return -9999.;
                                 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> overlay_clus_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   double KE = evt.m_nmichels[whichMichel].pionKE;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   if (overlayfrac > .5 && (matchtype == 3 || matchtype == 4 )) return KE;
                                   else return -9999.;
                                 };

 std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_goodvtx_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   double KE = evt.m_nmichels[whichMichel].pionKE;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd == recoEnd && (matchtype == 1 || matchtype == 2)) return KE;
                                   else return -9999.;
                                 };
 std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_goodclus_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   double KE = evt.m_nmichels[whichMichel].pionKE;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd == recoEnd && (matchtype == 3 || matchtype == 4)) return KE;
                                   else return -9999.;
                                 };
  std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_badvtx_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   double KE = evt.m_nmichels[whichMichel].pionKE;
				   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd != recoEnd && (matchtype == 1 || matchtype == 2))
                                   {
                                        //univ.PrintArachneLink();
                                        //std::cout << "Printing Michel Time for bad VTX match type "  << evt.m_nmichels[whichMichel].time << std::endl;
                                        return KE;
                                   }
                                   else return -9999.;
                                 };

 std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_badclus_tpi = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                   double KE = evt.m_nmichels[whichMichel].pionKE;
				   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd != recoEnd && (matchtype == 3 || matchtype == 4))
                                   {
                                         //univ.PrintArachneLink();
                                         //std::cout << "Printing Michel Time for bad CLUS match type "  << evt.m_nmichels[whichMichel].time << std::endl;
                                         return KE;
                                   }
                                   else return -9999.;
                                 };


                                  
   std::function<double(const CVUniverse&, const MichelEvent&, const int)> overlay_vtx_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;

				   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
	                           int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
	 			   if (overlayfrac > .5 && (matchtype == 1 || matchtype == 2 )) return micheldist;
 				   else return -9999.;
				 };

   std::function<double(const CVUniverse&, const MichelEvent&, const int)> overlay_clus_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;

                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   if (overlayfrac > .5 && (matchtype == 3 || matchtype == 4 )) return micheldist;
                                   else return -9999.;
                                 };

    std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_goodvtx_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
			           int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;

                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
		                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd == recoEnd && (matchtype == 1 || matchtype == 2)) return micheldist;
                                   else return -9999.;
                                 }; 

   std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_goodclus_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
				   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd == recoEnd && (matchtype == 3 || matchtype == 4)) return micheldist;
                                   else return -9999.;
                                 }; 

  std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_badvtx_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
				   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd != recoEnd && (matchtype == 1 || matchtype == 2))
			           {
				  //      univ.PrintArachneLink();
				//	std::cout << "Printing Michel Time for bad VTX match type "  << evt.m_nmichels[whichMichel].time << std::endl;
					return micheldist;
                                   }
				   else return -9999.;
                                 }; 
 std::function<double(const CVUniverse&, const MichelEvent&, const int)> truemichel_badclus_range = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
				   int overlayfrac = evt.m_nmichels[whichMichel].overlay_fraction;
                                   int matchtype = evt.m_nmichels[whichMichel].BestMatch;
                                   int trueEnd = evt.m_nmichels[whichMichel].trueEndpoint;
                                   int recoEnd = evt.m_nmichels[whichMichel].recoEndpoint;
                                   if (overlayfrac < .5 && trueEnd != recoEnd && (matchtype == 3 || matchtype == 4))
			           {
				//	 univ.PrintArachneLink();
                                  //       std::cout << "Printing Michel Time for bad CLUS match type "  << evt.m_nmichels[whichMichel].time << std::endl;
					 return micheldist;
                                   }
				   else return -9999.; 
                                 };


std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_XZ = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 { 
				   double twoDdist = evt.m_nmichels[whichMichel].best_XZ;
            			   return twoDdist;
		   		 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_UZ = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].best_UZ;
                                   return twoDdist;
                                 };


std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_VZ = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].best_VZ;
                                   return twoDdist;
                                 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_XZ_upvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_vertex_XZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
				   if (trueend == 1) return twoDdist;
  				   else return 9999.;
                                 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_UZ_upvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_vertex_UZ;
				   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 1) return twoDdist;
                                   else return 9999.;
                                 };


std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_VZ_upvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_vertex_VZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 1) return twoDdist;
                                   else return 9999.;
				 };


std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_XZ_upclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_clus_XZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 1) return twoDdist;
                                   else return 9999.;
				 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_UZ_upclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_clus_UZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 1) return twoDdist;
                                   else return 9999.; 
				};

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_VZ_upclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].up_to_clus_VZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 1) return twoDdist;
                                   else return 9999.;
				 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_XZ_downclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_clus_XZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.; 
                                 };
std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_UZ_downclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_clus_UZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.; 
                                 };
std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_VZ_downclus = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_clus_VZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.; 
                                 };
std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_XZ_downvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_vertex_XZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.; 
                                 };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_UZ_downvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_vertex_UZ;
				   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.;
                                 };



std::function<double(const CVUniverse&, const MichelEvent&, const int)> michel_VZ_downvtx = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                 {
                                   double twoDdist = evt.m_nmichels[whichMichel].down_to_vertex_VZ;
                                   int trueend = evt.m_nmichels[whichMichel].trueEndpoint;
                                   if (trueend == 2) return twoDdist;
                                   else return 9999.;  
                                 };


std::function<double(const CVUniverse&, const MichelEvent&, const int)> pion_angle = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
				{
    				  double angle = evt.m_nmichels[whichMichel].best_angle;
 				  return cos(angle);
				};

std::function<double(const CVUniverse&, const MichelEvent&, const int)> pion_angle_range1 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].best_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
   				  if (micheldist <= 150.) return cos(angle);
				  else return 9999.;
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> pion_angle_range2 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].best_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist >150. && micheldist <= 250.) return cos(angle);
                                  else return 9999.;
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> pion_angle_range3 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].best_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist > 250. && micheldist <=500.) return cos(angle);
                                  else return 9999.;
                                };
std::function<double(const CVUniverse&, const MichelEvent&, const int)> pion_angle_range4 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].best_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist > 500.) return cos(angle);
                                  else return 9999.;
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> true_angle = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].true_angle;
                                  return cos(angle);
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> true_angle_range1 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].true_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist <= 150.) return cos(angle);
                                  else return 9999.;
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> true_angle_range2 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].true_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist > 150 && micheldist <= 250.) return cos(angle);
                                  else return 9999.;
                                };
std::function<double(const CVUniverse&, const MichelEvent&, const int)> true_angle_range3 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].true_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist > 250 && micheldist <= 500.) return cos(angle);
                                  else return 9999.;
                                };

std::function<double(const CVUniverse&, const MichelEvent&, const int)> true_angle_range4 = [](const CVUniverse& univ, const MichelEvent& evt, const int whichMichel)
                                {
                                  double angle = evt.m_nmichels[whichMichel].true_angle;
                                  double  micheldist = evt.m_nmichels[whichMichel].Best3Ddist;
                                  if (micheldist > 500) return cos(angle);
                                  else return 9999.;
                                };

  //std::vector<double> rangebins = {0., 15., 30., 45., 60., 75., 100., 130., 160., 190., 220., 260., 300., 350., 400., 450., 500., 550., 600., 650., 700., 800., 900., 1000., 1400., 1800., 2400.};
   std::vector<double> anglebins = {-1.2,-1, -.80, -.70, -.60, -.50, -.40, -.30, -.20, -.10, .10, .20, .30, .40,.50,.60,.70,.80,.90,1.,1.2};
   int nbinsrange = rangebins.size()-1;
   int nbinstpi = tpibins.size()-1;
   int nanglebin = anglebins.size() -1;

   //VarConfig deltat_config{"deltat", "#mus", 30, 0., 9.};
   VarConfig pirange_config{"pirange", "mm", 100, 0.0, 2000.0};
   VarConfig tpi_config{"KE", "MeV", 100, 0.0, 1000.};
   VarConfig2D rangeconfig{"Pion_Range", "mm", nbinsrange, rangebins};
   VarConfig2D tpiconfig{"Pion_KE", "MeV", nbinstpi, tpibins};
   int nbinq3 = dansPTBins.size()-1;
   eVarConfig2D q3config{"q3_reco", "GeV", nq3mbins, mehreenQ3Bins};
   eVarConfig2D pTconfig{"pT_reco", "GeV", nq3mbins, mehreenQ3Bins};
   eVarConfig2D erangeconfig{"ePirange", "mm", nbinsrange, rangebins};
   eVarConfig2D angleconfig{"PiAngle", "cos", nanglebin, anglebins};
   eVarConfig2D etpiconfig{"ePion_KE", "MeV", nbinstpi, tpibins};
   //std::vector<double> anglebins = {-1.2,-1, -.80, -.70, -.60, -.50, -.40, -.30, -.20, -.10, .10, .20, .30, .40,.50,.60,.70,.80,.90,1.,1.2};
   int nbinsangle = anglebins.size() -1;
   studies.push_back(new PerMichelVarVecFSPart(pion_angle, "pion_angle", "cos(#theta)", nbinsangle,anglebins, error_bands));
   studies.push_back(new PerMichelVarVecFSPart(pion_angle_range1, "pion_angle_range1", "cos(#theta)", nbinsangle,anglebins, error_bands));
   studies.push_back(new PerMichelVarVecFSPart(pion_angle_range2, "pion_angle_range2", "cos(#theta)", nbinsangle,anglebins, error_bands));
   studies.push_back(new PerMichelVarVecFSPart(pion_angle_range3, "pion_angle_range3", "cos(#theta)", nbinsangle,anglebins, error_bands));
   studies.push_back(new PerMichelVarVecFSPart(pion_angle_range4, "pion_angle_range4", "cos(#theta)",nbinsangle,anglebins , error_bands));

   studies.push_back(new PerMichelVarVecFSPart(permichel_range, "permichel_pirange", "mm", nbinsrange, rangebins, error_bands));
   studies.push_back(new PerMichelVarVecFSPart(permichel_tpi,"permichel_tpi", "MeV", nbinstpi, tpibins, error_bands));


   studies.push_back(new PerMichel2DVarbin(permichel_tpi, pertruepimichel_range, tpiconfig, rangeconfig, error_bands));
   //studies.push_back(new PerMichel2DVarbin(permichel_range, permichel_tpi, rangeconfig,tpiconfig, error_bands));
  
   std::function<double(const CVUniverse&, const MichelEvent&)> event_angle = [](const CVUniverse& univ, const MichelEvent& evt)
                                { 
                                  double angle = evt.m_nmichels[0].best_angle;
                                  return cos(angle);
                                };
   std::function<double(const CVUniverse&, const MichelEvent&)> event_range = [](const CVUniverse& univ, const MichelEvent& evt)
                                { 
                                  double range = evt.m_nmichels[0].Best3Ddist;
                                  return range;
                                };
   
   studies.push_back(new PerMichelEvent2DVarbin(event_angle, getpT, angleconfig, pTconfig, error_bands));
   studies.push_back(new PerMichelEvent2DVarbin(event_range, getpT, erangeconfig, pTconfig, error_bands));



// Set Up Data Universe

  CVUniverse* data_universe = new CVUniverse(options.m_data);
  std::vector<CVUniverse*> data_band = {data_universe};
  std::map<std::string, std::vector<CVUniverse*> > data_error_bands;
  data_error_bands["cv"] = data_band;
  
  std::vector<Study*> data_studies;
  std::vector<Study*> data_sidebands;
  data_studies.push_back(new PerMichelVarVecFSPart(pion_angle, "pion_angle", "cos(#theta)", nbinsangle,anglebins, data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(pion_angle_range1, "pion_angle_range1", "cos(#theta)", nbinsangle,anglebins, data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(pion_angle_range2, "pion_angle_range2", "cos(#theta)", nbinsangle,anglebins, data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(pion_angle_range3, "pion_angle_range3", "cos(#theta)", nbinsangle,anglebins, data_error_bands));
  data_studies.push_back(new PerMichelVarVecFSPart(pion_angle_range4, "pion_angle_range4", "cos(#theta)",nbinsangle,anglebins , data_error_bands)); 
  data_studies.push_back(new PerMichelVarVecFSPart(permichel_range, "permichel_pirange", "mm", nbinsrange, rangebins, data_error_bands));
 
  data_studies.push_back(new PerMichelEvent2DVarbin(event_angle, getpT, angleconfig, pTconfig, data_error_bands));
  data_studies.push_back(new PerMichelEvent2DVarbin(event_range, getpT, erangeconfig, pTconfig, data_error_bands));
   

  // Loop entries and fill
  try
  {
    CVUniverse::SetTruth(true);
    LoopAndFillEventSelection(options.m_mc, error_bands, studies, sideband_studies, mycuts, model);
    std::cout << "MC cut summary:\n" << mycuts << "\n";
    //mycuts.resetStats();
    CVUniverse::SetTruth(true);
    //LoopAndFillEffDenom(options.m_truth, truth_bands, vars, vars2D,studies, mycuts, model);
    //options.PrintMacroConfiguration(argv[0]);
    //std::cout << "Sideband Cut Summary:\n" << mysides << "\n";
    mycuts.resetStats();
    CVUniverse::SetTruth(false);
    LoopAndFillData(options.m_data, data_band, data_studies, data_sidebands, mycuts);
    std::cout << "Data cut summary:\n" << mycuts << "\n";
    //mycuts.resetStats();
    
    std::cout  << "Opening the MCOUTPUT for EVENT hists" << std::endl;
    
    
    std::cout << "Saving Histos to MC Files" << std::endl;
    //TFile* mc_MichelStudies = TFile::Open("AllMichel_hasFittedMichel_500mm.root", "RECREATE");
    //"ALL2DDistprinted_OnlyPionMichels_tpimorethan80meV_forceendpointmatch_2Ddistcut_mc.root", "RECREATE");
    for(auto& study: studies) study->SaveOrDraw(*mc_MichelStudies);
    std::cout << "WRiting STUDIES to michel level file" << std::endl;
    std::cout << "Printing POT MC " << std::endl;
    auto mcPOT = new TParameter<double>("POTUsed", options.m_mc_pot);
    mc_MichelStudies->cd();
    mcPOT->Write(); 
    //for(auto& study: sideband_studies) study->SaveOrDraw(*mc_SidebandStudies);


    //CVUniverse::SetTruth(false);
    //LoopAndFillData(options.m_data, data_band, vars, vars2D, data_studies,sidevars, sidevars2D, data_sidebands, mycuts);
    //std::cout << "Data cut summary:\n" << mycuts << "\n";
    //mycuts.resetStats();

    for(auto& study: data_studies) study->SaveOrDraw(*data_MichelStudies);
    //for(auto& study: data_sidebands) study->SaveOrDraw(*data_SidebandStudies);


   
    
    //mc_SidebandStudies->cd();
    //mcPOT->Write();
    

    PlotUtils::TargetUtils targetInfo;
    assert(error_bands["cv"].size() == 1 && "List of error bands must contain a universe named \"cv\" for the flux integral.");
    std::cout << "Looping over Vars to fill systematics" << std::endl;

    auto dataPOT = new TParameter<double>("POTUsed", options.m_data_pot);
    //dataPOT->Write();
    data_MichelStudies->cd();
    dataPOT->Write();
    //data_SidebandStudies->cd();
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
