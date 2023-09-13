#ifndef MNV_NUKEHISTOGRAMUTILS_cxx
#define MNV_NUKEHISTOGRAMUTILS_cxx 1

#include "NukeHistogramUtils.h"

//#include "IncludeNukeCCPion.h"

#include "LateralSystematics.h"
#include "SidebandFitUniverse.h"
#include "CohDiffractiveSystematics.h"

#include "TLeaf.h"
#include <fstream>
#include <stdio.h>

#include "EnumTypes.h"
#include "PlotUtils/ChainWrapper.h"
#include "PlotUtils/FluxReweighter.h"

#include "PlotUtils/NSFDefaults.h"
#include "PlotUtils/AngleSystematics.h"
#include "PlotUtils/FluxSystematics.h"
#include "PlotUtils/GenieSystematics.h"
#include "PlotUtils/GeantHadronSystematics.h"
#include "PlotUtils/MinosEfficiencySystematics.h"
#include "PlotUtils/MnvTuneSystematics.h"
#include "PlotUtils/MuonSystematics.h"
#include "PlotUtils/MuonResolutionSystematics.h"
#include "PlotUtils/MichelSystematics.h"
#include "PlotUtils/ResponseSystematics.h"
#include "PlotUtils/TargetMassSystematics.h"

//#include "MinervaUnfold/MnvResponse.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"
#include "PlotUtils/MnvHadronReweight.h"
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"
#include "PlotUtils/POTCounter.h"
//#include "PlotUtils/AnaBinning.h"

#include "NukeCCPionCuts.h"
#include "SidebandType1Pi.h"

//DELETE IF YOU READ THIS
//double CVUniverse::m_muon_momentum_cv_offset = 0.0;
//bool CVUniverse::_is_muon_momentum_cv_offset_set = false;
//bool CVUniverse::_has_muon_error_been_thrown = false;

//ClassImp(NukeHistogramUtils);

//NukeHistogramUtils& NukeHistogramUtils::Get()
//{
//   static NukeHistogramUtils singleton;
//   return singleton;
//};

//! get the sub names of the histograms to book
std::vector< string > NukeHistogramUtils::getHistosNames( string title, string tar_spec )
{
  std::vector< string > names;
  names.clear();
  names.push_back( "data" );
  if( title.find("nukecut") != std::string::npos ){
     names.clear();
     names.push_back( "nukecut1_data" );
     names.push_back( "nukecut2_data" );
     names.push_back( "nukecut3_data" );
     names.push_back( "nukecut4_data" );
     names.push_back( "nukecut5_data" );
     names.push_back( "nukecut6_data" );
     names.push_back( "nukecut7_data" );
     names.push_back( "data" );
     names.push_back( "nukecut1_mc" );
     names.push_back( "nukecut2_mc" );
     names.push_back( "nukecut3_mc" );
     names.push_back( "nukecut4_mc" );
     names.push_back( "nukecut5_mc" );
     names.push_back( "nukecut6_mc" );
     names.push_back( "nukecut7_mc" );
     names.push_back( "mc" );
  }
  if( title.find("newmichel") != std::string::npos ){
    names.push_back( "newmichel_mc"       );
    names.push_back( "newmichel_bkgd"  );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "newmichel_pion_end" );
      names.push_back( "newmichel_pion_vtx" );
      names.push_back( "newmichel_other" );
    }
  }
  if( title.find("phase_space") != std::string::npos ){
    names.push_back("phase_space_mc");
    if(!m_MCBkgdOnly)
    {
      names.push_back("phase_space_thetamu");
      names.push_back("phase_space_W");
      names.push_back("phase_space_tpi50");
      names.push_back("phase_space_tpi350");
    }
  }
  if( title.find("background") != std::string::npos ){
    names.push_back("background_mc");
    if(!m_MCBkgdOnly)
    {
      names.push_back("background_target_hexagon");    
      names.push_back("background_target_nuclei");    
      names.push_back("background_target_US");    
      names.push_back("background_target_DS");    
      names.push_back("background_target_sum");    
      names.push_back("background_channel_only");   
      names.push_back("background_W_range_only");         
      names.push_back("background_W_and_channel");         
      names.push_back("background_pion_pid");
      names.push_back("background_pion_range");
      names.push_back("background_thetamu");   
    }
  }
  if( title.find("event_channel")  != std::string::npos ) {
    names.push_back( "event_channel_mc"               ) ;
    names.push_back( "event_channel_bkgd"             ) ;
    names.push_back( "event_channel_signal"           ) ;
    names.push_back( "event_channel_truth_sig"     ) ;
    names.push_back( "event_channel_bg_sig"           ) ;
    names.push_back( "event_channel_comp_coh"           ) ;
    names.push_back( "event_channel_bg_phase"         ) ;
    if(!m_MCBkgdOnly)
    {
      names.push_back( "event_channel_numuCC_1piplus"      ) ;
      names.push_back( "event_channel_numuCC_1pipNpi"      ) ;
      names.push_back( "event_channel_numuCC_1pipOther"    ) ;
      names.push_back( "event_channel_numuCC_Npiplus"      ) ;
      names.push_back( "event_channel_numuCC_NpipNpi"      ) ;
      names.push_back( "event_channel_numuCC_NpipOther"    ) ;
      names.push_back( "event_channel_numuCC_0pipNpiOther" ) ;
      names.push_back( "event_channel_numuNC_Npi"          ) ;
      names.push_back( "event_channel_numubar"             ) ;
      names.push_back( "event_channel_nue"                 ) ;
      names.push_back( "event_channel_nopi"                ) ;
    }
  }
  if( title.find("pi_channel")  != std::string::npos ) {
    names.push_back( "pi_channel_mc"               ) ;
    names.push_back( "pi_channel_bkgd"             ) ;
    names.push_back( "pi_channel_signal"           ) ;
    names.push_back( "pi_channel_truth_sig"     ) ;
    names.push_back( "pi_channel_bg_sig"           ) ;
    names.push_back( "pi_channel_comp_coh"           ) ;
    if(!m_MCBkgdOnly)
    {
      names.push_back( "pi_channel_numuCC_1piplus"      ) ;
      names.push_back( "pi_channel_numuCC_1pipNpiOther" ) ;
      names.push_back( "pi_channel_numuCC_2piplus"      ) ;
      names.push_back( "pi_channel_numuCC_2pipNpiOther" ) ;
      //names.push_back( "pi_channel_numuCC_2pipNpi"      ) ;
      //names.push_back( "pi_channel_numuCC_2pipOther"    ) ;
      names.push_back( "pi_channel_numuCC_0pipNpiOther" ) ;
      names.push_back( "pi_channel_other"               ) ;
    }
  }
  if( title.find("tarplastic") != std::string::npos ) {
    names.push_back("tarplastic_mc");
    names.push_back("tarplastic_signal");
    names.push_back("tarplastic_tar_W");
    names.push_back("tarplastic_tar_other");
    names.push_back("tarplastic_plas_sig");
    names.push_back("tarplastic_plas_W");
    names.push_back("tarplastic_plas_other");
  } 
  if( title.find("production_channel") != std::string::npos ){
    names.push_back( "production_channel_mc" );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "production_channel_delta_res" );
      names.push_back( "production_channel_higher_res" );
      names.push_back( "production_channel_non_res" );
      names.push_back( "production_channel_coh" );
      names.push_back( "production_channel_ccqe" );
      names.push_back( "production_channel_dis" );
      names.push_back( "production_channel_other" );
      names.push_back( "production_channel_othtar" );
    }
  }
  if( title.find("nucleon_channel") != std::string::npos ){
    names.push_back( "nucleon_channel_mc" );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "nucleon_channel_proton" );
      names.push_back( "nucleon_channel_neutron" );
      names.push_back( "nucleon_channel_heavy_baryon" );
      names.push_back( "nucleon_channel_coh" );
      names.push_back( "nucleon_channel_other" );
    }
  }
  if( title.find("true_W_channel") != std::string::npos ){
    names.push_back( "true_W_channel_mc" );
    names.push_back( "true_W_channel_signal"       );
    names.push_back( "true_W_channel_truth_sig" );
    names.push_back( "true_W_channel_bg_sig"       );
    names.push_back( "true_W_channel_comp_coh"       );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "true_W_channel_lt_14_bkgd" );
      names.push_back( "true_W_channel_14_18" );
      names.push_back( "true_W_channel_gt_18" );
      names.push_back( "true_W_channel_scintillator" );
      names.push_back( "true_W_channel_other_target" );
    }
  }
  if( title.find("pip_number") != std::string::npos ){
    names.push_back( "pip_number_mc" );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "pip_number_0pi" );
      names.push_back( "pip_number_1pi" );
      names.push_back( "pip_number_2pi" );
      names.push_back( "pip_number_Npi" );
      names.push_back( "pip_number_other_target" );
    }
  }
  if( title.find("pi0_number") != std::string::npos ){
    names.push_back( "pi0_number_mc" );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "pi0_number_0pi" );
      names.push_back( "pi0_number_1pi" );
      names.push_back( "pi0_number_2pi" );
      names.push_back( "pi0_number_Npi" );
      names.push_back( "pi0_number_other_target" );
    }
  }
  if( title.find("particle") != std::string::npos ){
    names.push_back( "particle_mc"           ) ;
    names.push_back( "particle_signal"       ) ;
    names.push_back( "particle_truth_sig" ) ;
    names.push_back( "particle_bkgd" ) ;
    if(!m_MCBkgdOnly)
    {
      names.push_back( "particle_piplus"  ) ;
      names.push_back( "particle_piminus" ) ;
      names.push_back( "particle_pi0"     ) ;
      names.push_back( "particle_proton"  ) ;
      names.push_back( "particle_neutron" ) ;
      names.push_back( "particle_meson"   ) ;
      names.push_back( "particle_other"   ) ;
      names.push_back( "particle_overlay" ) ;
      names.push_back( "particle_em"      ) ; 
      names.push_back( "particle_muon"    ) ;  
      names.push_back( "particle_baryon"  ) ;
      names.push_back( "particle_nuclei"  ) ;
      names.push_back( "particle_carbon"  ) ;
      names.push_back( "particle_iron"    ) ;
      names.push_back( "particle_lead"    ) ;
    }
  }
  if( title.find("twopart") != std::string::npos ){
    names.push_back( "twopart_mc"           ) ;
    names.push_back( "twopart_signal"       ) ;
    names.push_back( "twopart_truth_sig" ) ;
    names.push_back( "twopart_bkgd" ) ;
    if(!m_MCBkgdOnly)
    {
      names.push_back( "twopart_piplus"  ) ;
      names.push_back( "twopart_proton"  ) ;
      names.push_back( "twopart_other"   ) ;
    }
  }
  if( title.find("plasCont") != std::string::npos ) {
    names.push_back( "plasCont_mc" );
    names.push_back( "plasCont_bkgd" );
  }
  if( title.find("nucleus") != std::string::npos ) {
    //We may need to worry about reco/true distinction
    names.push_back( "nucleus_mc" );
    names.push_back( "nucleus_bkgd" );
    if( tar_spec.find("tar1") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_iron_tar1" );
      names.push_back( "nucleus_lead_tar1" );
      names.push_back( "nucleus_plastic_tar1" );
      names.push_back( "nucleus_plas_us_tar1" );
      names.push_back( "nucleus_plas_ds_tar1" );
      names.push_back( "nucleus_iron_plas_tar1" );
      names.push_back( "nucleus_lead_plas_tar1" );
      names.push_back( "nucleus_other_tar1" );
    }
    if( tar_spec.find("tar2") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_iron_tar2" );
      names.push_back( "nucleus_lead_tar2" );
      names.push_back( "nucleus_plastic_tar2" );
      names.push_back( "nucleus_plas_us_tar2" );
      names.push_back( "nucleus_plas_ds_tar2" );
      names.push_back( "nucleus_iron_plas_tar2" );
      names.push_back( "nucleus_lead_plas_tar2" );
      names.push_back( "nucleus_other_tar2" );
    }
    if( tar_spec.find("tar3") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_carb_tar3" );
      names.push_back( "nucleus_iron_tar3" );
      names.push_back( "nucleus_lead_tar3" );
      names.push_back( "nucleus_plastic_tar3" );
      names.push_back( "nucleus_plas_us_tar3" );
      names.push_back( "nucleus_plas_ds_tar3" );
      names.push_back( "nucleus_carb_plas_tar3" );
      names.push_back( "nucleus_iron_plas_tar3" );
      names.push_back( "nucleus_lead_plas_tar3" );
      names.push_back( "nucleus_other_tar3" );
    }
    if( tar_spec.find("watertar") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_watertar" );
      names.push_back( "nucleus_plastic_watertar" );
      names.push_back( "nucleus_plas_us_watertar" );
      names.push_back( "nucleus_plas_ds_watertar" );
      names.push_back( "nucleus_water_plas_watertar" );
      names.push_back( "nucleus_other_watertar" );
    }
    if( tar_spec.find("tar4") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_lead_tar4" );
      names.push_back( "nucleus_plastic_tar4");
      names.push_back( "nucleus_plas_us_tar4" );
      names.push_back( "nucleus_plas_ds_tar4" );
      names.push_back( "nucleus_lead_plas_tar4" );
      names.push_back( "nucleus_other_tar4" );
    }
    if( tar_spec.find("tar5") != std::string::npos || tar_spec.size() == 0 )
    { 
      names.push_back( "nucleus_iron_tar5" );
      names.push_back( "nucleus_lead_tar5" );
      names.push_back( "nucleus_plastic_tar5");
      names.push_back( "nucleus_plas_us_tar5" );
      names.push_back( "nucleus_plas_ds_tar5" );
      names.push_back( "nucleus_iron_plas_tar5" );
      names.push_back( "nucleus_lead_plas_tar5" );
      names.push_back( "nucleus_other_tar5" );
    }
    names.push_back( "nucleus_true_water" );
    names.push_back( "nucleus_true_carb" );
    names.push_back( "nucleus_true_iron" );
    names.push_back( "nucleus_true_lead" );
    names.push_back( "nucleus_true_plas" );
    names.push_back( "nucleus_reco_carb" );
    names.push_back( "nucleus_reco_iron" );
    names.push_back( "nucleus_reco_lead" );
    names.push_back( "nucleus_plas_tracker" );
    if( m_IncludeNucleusOutside )
    {
      if( tar_spec.find("tar1") != std::string::npos || tar_spec.size() == 0 )
      { 
        names.push_back( "nucleus_iron1_ds_not_fid" );
        names.push_back( "nucleus_lead1_ds_not_fid" );
        names.push_back( "nucleus_iron1_plas_ds_not_fid" );
        names.push_back( "nucleus_lead1_plas_ds_not_fid" );
      }
      if( tar_spec.find("tar5") != std::string::npos || tar_spec.size() == 0 )
      { 
        names.push_back( "nucleus_iron2_ds_not_fid" );
        names.push_back( "nucleus_lead2_ds_not_fid" );
        names.push_back( "nucleus_iron2_plas_ds_not_fid" );
        names.push_back( "nucleus_lead2_plas_ds_not_fid" );
      }
      if( tar_spec.find("tar5") != std::string::npos || tar_spec.size() == 0 )
      { 
        names.push_back( "nucleus_carb3_ds_not_fid" ); 
        names.push_back( "nucleus_iron3_ds_not_fid" ); 
        names.push_back( "nucleus_lead3_ds_not_fid" ); 
        names.push_back( "nucleus_carb3_plas_ds_not_fid" );
        names.push_back( "nucleus_iron3_plas_ds_not_fid" );
        names.push_back( "nucleus_lead3_plas_ds_not_fid" );
      }
      if( tar_spec.find("tar5") != std::string::npos || tar_spec.size() == 0 )
      { 
        names.push_back( "nucleus_lead4_ds_not_fid" ); 
        names.push_back( "nucleus_lead4_plas_ds_not_fid" );
      }
      if( tar_spec.find("tar5") != std::string::npos || tar_spec.size() == 0 )
      { 
        names.push_back( "nucleus_iron5_ds_not_fid" ); 
        names.push_back( "nucleus_lead5_ds_not_fid" ); 
        names.push_back( "nucleus_iron5_plas_ds_not_fid" );
        names.push_back( "nucleus_lead5_plas_ds_not_fid" );
      }
      names.push_back( "nucleus_plas1_ds_not_fid" );
      names.push_back( "nucleus_plas2_ds_not_fid" );
      names.push_back( "nucleus_plas3_ds_not_fid" );
      names.push_back( "nucleus_plas_tracker_split" ); 
    }
  }
  if( title.find("neutrino") != std::string::npos ) {
    names.push_back( "neutrino_mc"                    );
    names.push_back( "neutrino_bkgd"               );
    if(!m_MCBkgdOnly)
    {
      names.push_back( "neutrino_ccqe"                  );
      names.push_back( "neutrino_ccres"                 );
      names.push_back( "neutrino_ccdis"                 );
      names.push_back( "neutrino_cccoh"                 );
      names.push_back( "neutrino_nunc"                  );
      names.push_back( "neutrino_nubar"                 );
      names.push_back( "neutrino_electrons"             );
      names.push_back( "neutrino_other"                 );
      names.push_back( "neutrino_ccqe_or_like"          );
      names.push_back( "neutrino_notccqe_or_like"       );
      names.push_back( "neutrino_ccqe_like"             );
      names.push_back( "neutrino_notccqe_like"          );
      names.push_back( "neutrino_genie_signal"          );
      names.push_back( "neutrino_genie_nonSignal"       );
      names.push_back( "neutrino_signal"                );
      names.push_back( "neutrino_nonSignal"             );
      names.push_back( "neutrino_notccqe_like_piplus"   );
      names.push_back( "neutrino_notccqe_like_piminus"  );
      names.push_back( "neutrino_notccqe_like_pizero"   );
      names.push_back( "neutrino_notccqe_like_else"     );
      names.push_back( "neutrino_ccqe_like_and_qe"      );
      names.push_back( "neutrino_ccqe_like_and_res"     );
      names.push_back( "neutrino_ccqe_like_and_else"    );
      names.push_back( "neutrino_notccqe_like_and_res"  );
      names.push_back( "neutrino_notccqe_like_and_dis"  );
      names.push_back( "neutrino_notccqe_like_and_else" ); 
    }
  }
  if( title.find("topology") != std::string::npos ){
    names.push_back( "topology_mc"          ) ;
    names.push_back( "topology_signal"      ) ;
    names.push_back( "topology_truth_sig") ;
    names.push_back( "topology_bkgd"        ) ;
    if(!m_MCBkgdOnly)
    {
      names.push_back( "topology_piminus_stop"     ) ;
      names.push_back( "topology_piminus_interact" ) ;
      names.push_back( "topology_piplus_stop"      ) ;
      names.push_back( "topology_piplus_interact"  ) ;
      names.push_back( "topology_proton_stop"      ) ;
      names.push_back( "topology_proton_interact"  ) ;
      names.push_back( "topology_pi0"              ) ;
      names.push_back( "topology_other"            ) ;
    }
  }
  if( title.find("cliffTop") != std::string::npos ){
    names.push_back("cliffTop_mc");
    names.push_back("cliffTop_bkgd");
    if(!m_MCBkgdOnly)
    {
      names.push_back("cliffTop_pi_0");
      names.push_back("cliffTop_pi_1");
      names.push_back("cliffTop_pi_2");
      names.push_back("cliffTop_pi_3");
      names.push_back("cliffTop_pi_4");
      names.push_back("cliffTop_pi_5");
      names.push_back("cliffTop_proton_0");
      names.push_back("cliffTop_proton_1");
      names.push_back("cliffTop_proton_2");
      names.push_back("cliffTop_proton_3");
      names.push_back("cliffTop_proton_4");
      names.push_back("cliffTop_proton_5");
    }
  }
  if( title.find("test") != std::string::npos) {
    names.clear();
    names.push_back("");
  }
  if( title.find("DATAMC") != std::string::npos ){
    names.push_back("mc");
  }
  return names;
}

int NukeHistogramUtils::getNHistos( string title, string tar_spec )
{
   int nhistos = int( getHistosNames(title,tar_spec).size() );
   return nhistos;
}

UniverseMap NukeHistogramUtils::getSystematicUniversesMap(PlotUtils::ChainWrapper* chw, std::string flux_playlist, bool is_truth, bool do_full_systematics, int iSBWeight, bool bNeutronCV, bool bElastics ) 
{
  // return map
  UniverseMap error_bands;

  if(m_SetMnvUni)//Just want to call this one
  {
    //static CVUniverse options.  Should be applied uniformly to all universes 
    PlotUtils::MinervaUniverse::SetNuEConstraint(true);
    PlotUtils::MinervaUniverse::SetPlaylist(flux_playlist);
    PlotUtils::MinervaUniverse::SetAnalysisNuPDG(14);

    if( m_bkgd_fit_enum <= kNominal ) PlotUtils::MinervaUniverse::SetNonResPiReweight(false); 
    else                              PlotUtils::MinervaUniverse::SetNonResPiReweight(true);
    if( m_bkgd_fit_enum <= kTune121 ) PlotUtils::MinervaUniverse::SetDeuteriumGeniePiTune(false);
    else                              PlotUtils::MinervaUniverse::SetDeuteriumGeniePiTune(true);

    //MHRW options
    //Not needed right now
    //SetMHRWeightFilename( std::string process_name, std::string directory  );
    //SetReadoutVolume( double minZ, double maxZ, double apothem );

    PlotUtils::MinervaUniverse::SetReadoutVolume( m_plasticFlag ? "Tracker" : "Nuke");
    PlotUtils::MinervaUniverse::SetMHRWeightNeutronCVReweight( bNeutronCV );
    PlotUtils::MinervaUniverse::SetMHRWeightElastics( bElastics );

    m_SetMnvUni = false;
  }
  //
  //kNominal = 0, 
  //kTune121 = 1,
  //kNoLowQ2 = 2,
  //kMENU1PI = 3

  // CV
  error_bands[std::string("CV")].push_back(new CVUniverse(chw, 0));

  // Systematics
  if(do_full_systematics){
    // DETECTOR
    std::vector<double> sigmas = {-1., +1.};
    for (auto sigma : sigmas){
      error_bands[std::string("MEU")].         push_back(new MEUShiftUniverse(                chw, sigma));
      error_bands[std::string("Birks")].       push_back(new BirksShiftUniverse(              chw, sigma));
      error_bands[std::string("BetheBloch")].  push_back(new BetheBlochShiftCVUniverse(       chw, sigma));
      error_bands[std::string("DetMass")].     push_back(new DetectorMassShiftCVUniverse(     chw, sigma));
      error_bands[std::string("NodeCutEff")].  push_back(new NodeCutEffUniverse(       chw, sigma));
    }

    UniverseMap geant_bands = PlotUtils::GetGeantHadronSystematicsMap<CVUniverse>( chw );
    error_bands.insert(geant_bands.begin(), geant_bands.end());

    //Particle Response systematics
    UniverseMap part_resp_error_bands = PlotUtils::GetResponseSystematicsMap<CVUniverse>( chw, false, true );
    error_bands.insert(part_resp_error_bands.begin(), part_resp_error_bands.end());

    //Angle systematics
    UniverseMap angle_error_bands = PlotUtils::GetAngleSystematicsMap<CVUniverse>(chw);
    error_bands.insert(angle_error_bands.begin(), angle_error_bands.end());

    //Muon Angle systematics
    UniverseMap muon_angle_error_bands = PlotUtils::GetMuonAngleResolutionSystematicsMap<CVUniverse>(chw);
    error_bands.insert(muon_angle_error_bands.begin(), muon_angle_error_bands.end());
  
    //Muon P Shift systematics
    UniverseMap mnv_muon_error_bands = PlotUtils::GetMinervaMuonSystematicsMap<CVUniverse>(chw);
    error_bands.insert(mnv_muon_error_bands.begin(), mnv_muon_error_bands.end());

    UniverseMap minos_muon_error_bands = PlotUtils::GetMinosMuonSystematicsMap<CVUniverse>(chw);
    error_bands.insert(minos_muon_error_bands.begin(), minos_muon_error_bands.end());

    //Muon Resolution systematics
    UniverseMap muon_res_error_bands = PlotUtils::GetMuonResolutionSystematicsMap<CVUniverse>(chw);
    error_bands.insert(muon_res_error_bands.begin(), muon_res_error_bands.end());

    //// Minos Efficiency Correction
    std::vector<CVUniverse*> error_bands_minos = PlotUtils::GetMinosEfficiencySystematics<CVUniverse>(chw);
    error_bands[std::string("MinosEfficiency")] = error_bands_minos;

    //Michel Efficiency Error bands
    UniverseMap michel_error_bands = PlotUtils::GetMichelEfficiencySystematicsMap<CVUniverse>(chw);
    error_bands.insert(michel_error_bands.begin(), michel_error_bands.end());

    // FLUX
    //int n_flux_universes = 100;
    //std::vector<CVUniverse*> flux_error_bands = PlotUtils::GetFluxSystematics<CVUniverse>(chw, n_flux_universes);
    std::vector<CVUniverse*> flux_error_bands = PlotUtils::GetFluxSystematics<CVUniverse>(chw, CVUniverse::GetNFluxUniverses());
    error_bands[std::string("Flux")] = flux_error_bands;

    //// GENIE
    UniverseMap genie_error_bands = PlotUtils::GetGenieSystematicsMap<CVUniverse>(chw, false);//No including the new fitted values
    error_bands.insert(genie_error_bands.begin(), genie_error_bands.end());

    //New GENIE MaRES and NormCCRes error bands in Covariance
    //UniverseMap new_res_genie_error_bands = PlotUtils::GetGenieResPionFitCovSystematicsMap<CVUniverse>(chw);
    //New GENIE MaRES and NormCCRes error bands No Covariance
    UniverseMap new_res_genie_error_bands = PlotUtils::GetGenieResPionFitSystematicsMap<CVUniverse>(chw);
    error_bands.insert(new_res_genie_error_bands.begin(), new_res_genie_error_bands.end());

    //New GENIE MvRES
    UniverseMap new_ep_genie_error_bands = PlotUtils::GetGenieEPMvResSystematicsMap<CVUniverse>(chw);
    error_bands.insert(new_ep_genie_error_bands.begin(), new_ep_genie_error_bands.end());

    //TODO Remove.  Plot making
    //error_bands["MvRES"].push_back(new PlotUtils::GenieUniverse<CVUniverse>(chw, -1, "MvRES"));
    //error_bands["MvRES"].push_back(new PlotUtils::GenieUniverse<CVUniverse>(chw, 1, "MvRES"));

    //// RPA
    UniverseMap rpa_error_bands = PlotUtils::GetRPASystematicsMap<CVUniverse>(chw);
    error_bands.insert(rpa_error_bands.begin(), rpa_error_bands.end());

    //// 2P2H
    std::vector<CVUniverse*> error_bands_2p2h = PlotUtils::Get2p2hSystematics<CVUniverse>(chw);
    error_bands[std::string("2p2h")] = error_bands_2p2h;

    //// SB Fit errors
    UniverseMap sb_fit_error_bands = GetSBFitMap(chw);
    error_bands.insert(sb_fit_error_bands.begin(), sb_fit_error_bands.end());

    //Target Mass errors
    UniverseMap error_bands_tarmass = GetTargetMassSystematicsMap<CVUniverse>( chw );
    error_bands.insert(error_bands_tarmass.begin(), error_bands_tarmass.end());
    
    //Diffractive pion production unc
    //UniverseMap error_bands_cohdiff = GetCohDiffractiveSystematicsMap<CVUniverse>( chw );
    UniverseMap error_bands_cohdiff = GetCohDiffractiveSystematicsMap( chw );
    error_bands.insert(error_bands_cohdiff.begin(), error_bands_cohdiff.end());

    //// NON-RES PI
    //Obsolete
    //UniverseMap nonrespi_error_band = PlotUtils::GetNonResPiSystematicsMap<CVUniverse>(chw);
    //error_bands.insert(nonrespi_error_band.begin(), nonrespi_error_band.end());

    // LowQ2Pi 
    //std::vector<CVUniverse*> error_bands_lowq2pi = PlotUtils::GetLowQ2PiSystematics<CVUniverse>(chw);
    //error_bands[std::string("LowQ2Pi")] = error_bands_lowq2pi;
  }
  cout<<"Setting CVUniverse variables:"<<endl;
  cout<<"Is Truth? "<<is_truth<<endl;
  cout<<"NeutronCVReweight "<<bNeutronCV<<endl;
  cout<<"SB Weight "<<iSBWeight<<endl;
  cout<<"Bkgd Fit Enum " <<m_bkgd_fit_enum<<endl;
  for( auto &band : error_bands )
  {
    for( auto &universe : band.second ) 
    {
      universe->SetTruth(is_truth); 
      universe->SetSBWeight( iSBWeight ); 
      universe->SetBkgdFitType( m_bkgd_fit_enum );
    }
  }

  return error_bands;
}

SidebandType1PiMap NukeHistogramUtils::getSideband1PiMap( UniverseMap syst_bands, NukeCCPionCuts* cuts, bool useVertexZ ) 
{
  // return map
  SidebandType1PiMap sideband_map;
  for( auto& band : syst_bands )
  {
    std::vector<CVUniverse*> universes = band.second;
    for( auto& universe : universes )
    {
      sideband_map[band.first].push_back( new SidebandType1Pi( universe, cuts, useVertexZ ) );
    }
  }
  return sideband_map;
}

//This type of systematic bands don't have support anymore    
std::vector<CVUniverse*> NukeHistogramUtils::getSystematicUniverses( PlotUtils::ChainWrapper* chw )
{
  std::vector<CVUniverse*> systematic_universes;

  //CV
  systematic_universes.push_back(new CVUniverse(chw));

  ////Detector
  //systematic_universes.push_back(new MuonPRangeCurvatureShiftUniverse(chw, -1));
  //systematic_universes.push_back(new MuonPRangeCurvatureShiftUniverse(chw, +1));
  systematic_universes.push_back( new BirksShiftUniverse(chw, +1) );
  systematic_universes.push_back( new BirksShiftUniverse(chw, -1) );

  systematic_universes.push_back( new BetheBlochShiftCVUniverse(chw, +1) );
  systematic_universes.push_back( new BetheBlochShiftCVUniverse(chw, -1) );

  systematic_universes.push_back( new DetectorMassShiftCVUniverse(chw, +1) );
  systematic_universes.push_back( new DetectorMassShiftCVUniverse(chw, -1) );

  // GENIE
  //std::vector<CVUniverse*> genie_universes = PlotUtils::GetGenieSystematics<CVUniverse>(chw, -1);
  //systematic_universes.insert(systematic_universes.end(), genie_universes.begin(), genie_universes.end());
  //genie_universes.clear();
  //genie_universes = PlotUtils::GetGenieSystematics<CVUniverse>(chw, +1.);
  //systematic_universes.insert(systematic_universes.end(), genie_universes.begin(), genie_universes.end());

//    MnvTuneSystematics

  return systematic_universes;
} 

void NukeHistogramUtils::setBkgdFitType( std::string bkgd_fit_type )
{
  if( bkgd_fit_type.find("Nominal") != std::string::npos ) m_bkgd_fit_enum = kNominal; 
  if( bkgd_fit_type.find("Tune121") != std::string::npos ) m_bkgd_fit_enum = kTune121;  
  if( bkgd_fit_type.find("NoLowQ2") != std::string::npos ) m_bkgd_fit_enum = kNoLowQ2;  
  if( bkgd_fit_type.find("MENU1PI") != std::string::npos ) m_bkgd_fit_enum = kMENU1PI;  
}


void NukeHistogramUtils::bookHistos( TFile* file, PlotUtils::MnvH1D** h, string var_name, string title )
{
   int nhistos = getNHistos(title);
   std::vector< string > names = getHistosNames(title);

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = (PlotUtils::MnvH1D*)file->Get( hname );
     //if( h[i] ) removeUnfilledErrorBands( h[i] );
     h[i]->UnSumw2Universes();
   }

   return;
}

void NukeHistogramUtils::bookHistos( TFile* file, PlotUtils::MnvH1D** h, int nhistos, string var_name )
{
   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%d",var_name.c_str(),i);
     h[i] = (PlotUtils::MnvH1D*)file->Get( hname );
     //if( h[i] ) removeUnfilledErrorBands( h[i] );
     h[i]->UnSumw2Universes();
   }
   return;
}

void NukeHistogramUtils::bookHistos( PlotUtils::MnvH1D** h, string var_name, string title, int nbins,
                                                          double min, double max )
{
   int nhistos = getNHistos(title);
   std::vector< string > names = getHistosNames(title);

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = new PlotUtils::MnvH1D(hname,title.c_str(),nbins,min,max);
     //XXX HEY!  If things go back, start looking here
     h[i]->UnSumw2Universes();
   }
   return;
}

void NukeHistogramUtils::bookHistos( PlotUtils::MnvH1D** h, string var_name, string title, nuke_axis_binning xbins  )
{
   int nhistos = getNHistos(title);
   std::vector< string > names = getHistosNames(title);
  
   double xbinsarray[xbins.nbins];
   for(int i = 0; i <= xbins.nbins; i++) xbinsarray[i] = xbins.bin_edges[i];

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = new PlotUtils::MnvH1D(hname,title.c_str(),xbins.nbins,xbinsarray,xbins.default_width);
     h[i]->UnSumw2Universes();
   }
   return;
}

void NukeHistogramUtils::bookHistos( PlotUtils::MnvH1D** h, int nhistos, string var_name, string title,
                                                          nuke_axis_binning xbins )
{
   double xbinsarray[xbins.nbins];
   for(int i = 0; i <= xbins.nbins; i++) xbinsarray[i] = xbins.bin_edges[i];

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%d",var_name.c_str(),i);
     h[i] = new PlotUtils::MnvH1D(hname,title.c_str(),xbins.nbins,xbinsarray,xbins.default_width);
     h[i]->UnSumw2Universes();
   }
   return;
}

void NukeHistogramUtils::bookHistos( PlotUtils::MnvH1D** h, string var_name, std::vector<string> names, string title,
                                                          nuke_axis_binning xbins )
{
   double xbinsarray[xbins.nbins];

   for(int i = 0; i <= xbins.nbins; ++i) xbinsarray[i] = xbins.bin_edges[i];

   for(uint i = 0; i < names.size(); ++i) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = new PlotUtils::MnvH1D(hname,title.c_str(),xbins.nbins,xbinsarray,xbins.default_width);
   }
   return;
}

void NukeHistogramUtils::bookHistos( PlotUtils::MnvH2D** h, string var_name, string title, nuke_axis_binning xbins, 
                                                          nuke_axis_binning ybins )
{
   int nhistos = getNHistos(title);
   std::vector< string > names = getHistosNames(title);

   double xbinsarray[xbins.nbins];
   for(int i = 0; i <= xbins.nbins; i++) xbinsarray[i] = xbins.bin_edges[i];

   double ybinsarray[ybins.nbins];
   for(int i = 0; i <= ybins.nbins; i++) ybinsarray[i] = ybins.bin_edges[i];

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = new PlotUtils::MnvH2D(hname,title.c_str(),xbins.nbins,xbinsarray,ybins.nbins,ybinsarray);
     h[i]->UnSumw2Universes();
   }
   return;
}

void NukeHistogramUtils::bookResponses( PlotUtils::MnvH2D** h, string var_name, string title, nuke_axis_binning xbins, 
                                                          nuke_axis_binning ybins )
{
   int nhistos = getNHistos(title);
   std::vector< string > names = getHistosNames(title);

   int nBins = (xbins.nbins+2)*(ybins.nbins+2);

   for(int i = 0; i < nhistos; i++) {
     char hname[1000];
     sprintf(hname,"%s_%s",var_name.c_str(),names[i].c_str());
     h[i] = new PlotUtils::MnvH2D(hname,title.c_str(),nBins,0.0,(double)nBins,nBins,0.0,(double)nBins );
   }
   return;
}

std::vector< PlotUtils::MnvH1D** > NukeHistogramUtils::setupHists(int nTypes, int nHistograms, 
                                                        string name, std::vector<string> tnames, 
                                                        string title, std::vector<nuke_axis_binning> bins)
{
  std::vector< PlotUtils::MnvH1D** > hRet(nTypes);

  //TODO Check tnames, which is the type
  for(int i = 0; i < nTypes; i++){
    hRet[i] = new PlotUtils::MnvH1D*[nHistograms];
    bookHistos(hRet[i], name+tnames[i], title, bins[i]);
  }
  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i)[j]);
    }
  }
  
  return hRet;
}

std::vector< PlotUtils::MnvH1D** > NukeHistogramUtils::setupHists(int nTypes, int nHistograms, 
                                                        string name, std::vector<string> tnames, 
                                                        string title, nuke_axis_binning bins)
{
  std::vector< PlotUtils::MnvH1D** > hRet(nTypes);

  for(int i = 0; i < nTypes; i++){
    hRet[i] = new PlotUtils::MnvH1D*[nHistograms];
    bookHistos(hRet[i], name+tnames[i], title, bins);
  }
  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i)[j]);
    }
  }
  
  return hRet;
}

std::vector< PlotUtils::MnvH2D** > NukeHistogramUtils::setupHists(int nTypes, int nHistograms, 
                                                         string name, std::vector<string> tnames, 
                                                         string title, std::vector<nuke_axis_binning> bins1, std::vector<nuke_axis_binning> bins2)
{
  std::vector< PlotUtils::MnvH2D** > hRet(nTypes);

  for(int i = 0; i < nTypes; i++){
    hRet[i] = new PlotUtils::MnvH2D*[nHistograms];
    bookHistos(hRet[i], name+tnames[i], title, bins1[i], bins2[i]);
  }
  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i)[j]);
    }
  }
  return hRet;
}

std::vector< PlotUtils::MnvH2D** > NukeHistogramUtils::setupHists(int nTypes, int nHistograms, 
                                                         string name, std::vector<string> tnames, 
                                                         string title, nuke_axis_binning bins1, nuke_axis_binning bins2)
{
  std::vector< PlotUtils::MnvH2D** > hRet(nTypes);

  for(int i = 0; i < nTypes; i++){
    hRet[i] = new PlotUtils::MnvH2D*[nHistograms];
    bookHistos(hRet[i], name+tnames[i], title, bins1, bins2);
  }
  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i)[j]);
    }
  }
  return hRet;
}

std::vector< PlotUtils::MnvH2D** > NukeHistogramUtils::setupResponses(int nTypes, int nHistograms, 
                                                         string name, std::vector<string> tnames, 
                                                         string title, nuke_axis_binning bins1, nuke_axis_binning bins2)
{
  std::vector< PlotUtils::MnvH2D** > hRet(nTypes);

  for(int i = 0; i < nTypes; i++){
    hRet[i] = new PlotUtils::MnvH2D*[nHistograms];
    bookResponses(hRet[i], name+tnames[i], title, bins1, bins2);
  }
  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i)[j]);
    }
  }
  return hRet;
}

std::vector< EffHist< PlotUtils::MnvH1D** > > NukeHistogramUtils::setupEffHists(int nTypes, int nHistograms, 
                                                                                  string name, std::vector<string> tnames, 
                                                                                  string title, nuke_axis_binning bins)
{
  //Maybe we setup us as normal...
  //But how do we create numerator/denominator?

  std::vector< EffHist< PlotUtils::MnvH1D** > > hRet; 
  for(int i = 0; i < nTypes; i++){
    PlotUtils::MnvH1D** hist_num = new PlotUtils::MnvH1D*[nHistograms]; 
    PlotUtils::MnvH1D** hist_den = new PlotUtils::MnvH1D*[nHistograms]; 

    bookHistos(hist_num, name+tnames[i]+"_EFF_NUM", title, bins);
    bookHistos(hist_den, name+tnames[i]+"_EFF_DEN", title, bins);

    EffHist< PlotUtils::MnvH1D** > tmp_eff;
    tmp_eff.numerator   = hist_num;
    tmp_eff.denominator = hist_den;
    hRet.push_back( tmp_eff );
  }

  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i).numerator[j]);
      m_histList.Add(hRet.at(i).denominator[j]);
    }
  }
  
  return hRet;
}

std::vector< EffHist< PlotUtils::MnvH1D** > > NukeHistogramUtils::setupEffHists(int nTypes, int nHistograms, 
                                                                                  string name, std::vector<string> tnames, 
                                                                                  string title, std::vector<nuke_axis_binning> bins)
{
  //Maybe we setup us as normal...
  //But how do we create numerator/denominator?

  std::vector< EffHist< PlotUtils::MnvH1D** > > hRet; 
  for(int i = 0; i < nTypes; i++){
    PlotUtils::MnvH1D** hist_num = new PlotUtils::MnvH1D*[nHistograms]; 
    PlotUtils::MnvH1D** hist_den = new PlotUtils::MnvH1D*[nHistograms]; 

    bookHistos(hist_num, name+tnames[i]+"_EFF_NUM", title, bins[i]);
    bookHistos(hist_den, name+tnames[i]+"_EFF_DEN", title, bins[i]);

    EffHist< PlotUtils::MnvH1D** > tmp_eff;
    tmp_eff.numerator   = hist_num;
    tmp_eff.denominator = hist_den;
    hRet.push_back( tmp_eff );
  }

  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i).numerator[j]);
      m_histList.Add(hRet.at(i).denominator[j]);
    }
  }
  
  return hRet;
}

std::vector< EffHist< PlotUtils::MnvH2D** > > NukeHistogramUtils::setupEffHists(int nTypes, int nHistograms, 
                                                                                  string name, std::vector<string> tnames, 
                                                                                  string title, nuke_axis_binning bins1, nuke_axis_binning bins2)
{
  //Maybe we setup us as normal...
  //But how do we create numerator/denominator?

  std::vector< EffHist< PlotUtils::MnvH2D** > > hRet; 
  for(int i = 0; i < nTypes; i++){
    PlotUtils::MnvH2D** hist_num = new PlotUtils::MnvH2D*[nHistograms]; 
    PlotUtils::MnvH2D** hist_den = new PlotUtils::MnvH2D*[nHistograms]; 

    bookHistos(hist_num, name+tnames[i]+"_EFF_NUM", title, bins1, bins2);
    bookHistos(hist_den, name+tnames[i]+"_EFF_DEN", title, bins1, bins2);

    EffHist< PlotUtils::MnvH2D** > tmp_eff;
    tmp_eff.numerator   = hist_num;
    tmp_eff.denominator = hist_den;
    hRet.push_back( tmp_eff );
  }

  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i).numerator[j]);
      m_histList.Add(hRet.at(i).denominator[j]);
    }
  }
  
  return hRet;
}

std::vector< EffHist< PlotUtils::MnvH2D** > > NukeHistogramUtils::setupEffHists(int nTypes, int nHistograms, 
                                                                                  string name, std::vector<string> tnames, 
                                                                                  string title, std::vector<nuke_axis_binning> bins1, std::vector<nuke_axis_binning> bins2)
{
  //Maybe we setup us as normal...
  //But how do we create numerator/denominator?

  std::vector< EffHist< PlotUtils::MnvH2D** > > hRet; 
  for(int i = 0; i < nTypes; i++){
    PlotUtils::MnvH2D** hist_num = new PlotUtils::MnvH2D*[nHistograms]; 
    PlotUtils::MnvH2D** hist_den = new PlotUtils::MnvH2D*[nHistograms]; 

    bookHistos(hist_num, name+tnames[i]+"_EFF_NUM", title, bins1[i], bins2[i]);
    bookHistos(hist_den, name+tnames[i]+"_EFF_DEN", title, bins1[i], bins2[i]);

    EffHist< PlotUtils::MnvH2D** > tmp_eff;
    tmp_eff.numerator   = hist_num;
    tmp_eff.denominator = hist_den;
    hRet.push_back( tmp_eff );
  }

  for(int i = 0; i < nTypes; i++) {
    for(int j = 0; j < nHistograms; j++) {
      m_histList.Add(hRet.at(i).numerator[j]);
      m_histList.Add(hRet.at(i).denominator[j]);
    }
  }
  
  return hRet;
}

std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > NukeHistogramUtils::getHistWrapper( std::vector< PlotUtils::MnvH1D** > h, std::vector<CVUniverse*> &sys_uni, bool bAllHists )
{
  std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0][0]->GetTitle(),h[0][0]->GetXaxis()->GetTitle(),h[0][0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx = 0;

  std::vector<CVUniverse*> data_sys_uni;
  data_sys_uni.push_back(sys_uni[0]);

  for(uint j = 0; j< h.size(); ++j ){ 
    std::vector< PlotUtils::HistWrapper<CVUniverse>* > tmp_hists;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j][i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig") && !hname.Contains("_comp_coh")) continue;

      PlotUtils::HistWrapper<CVUniverse> *tmp_hw = new PlotUtils::HistWrapper<CVUniverse>(h[j][i],hname.Contains("_data") ? data_sys_uni : sys_uni );
      
      h_idx = m_histList.IndexOf(h[j][i]); 
      m_histList.Remove(h[j][i]);
      h[j][i] = tmp_hw->hist;
      m_histList.AddAt(tmp_hw->hist,h_idx);

      tmp_hists.push_back(tmp_hw); 
      m_hwList.push_back(tmp_hw);
    }
    ret_hists.push_back(tmp_hists); 
  }

  return ret_hists;
}

std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > NukeHistogramUtils::getHistWrapper( std::vector< PlotUtils::MnvH2D** > h, std::vector<CVUniverse*> &sys_uni, bool bAllHists )
{
  std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0][0]->GetTitle(),h[0][0]->GetXaxis()->GetTitle(),h[0][0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx = 0;

  std::vector<CVUniverse*> data_sys_uni;
  data_sys_uni.push_back(sys_uni[0]);

  for(uint j = 0; j< h.size(); ++j ){ 
    std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > tmp_hists;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j][i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig")  ) continue;

      PlotUtils::Hist2DWrapper<CVUniverse> *tmp_hw = new PlotUtils::Hist2DWrapper<CVUniverse>(h[j][i],hname.Contains("_data") ? data_sys_uni : sys_uni );

      h_idx = m_histList.IndexOf(h[j][i]); 
      m_histList.Remove(h[j][i]);
      h[j][i] = tmp_hw->hist;
      m_histList.AddAt(tmp_hw->hist,h_idx);

      tmp_hists.push_back(tmp_hw); 
      m_hw2DList.push_back(tmp_hw);
    }
    ret_hists.push_back(tmp_hists); 
  }

  return ret_hists;
}

std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > NukeHistogramUtils::getHistWrapper( std::vector< PlotUtils::MnvH1D** > h, std::map< std::string,std::vector<CVUniverse*> > &sys_band, bool bAllHists  )
{
  std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0][0]->GetTitle(),h[0][0]->GetXaxis()->GetTitle(),h[0][0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx = 0;

  std::map< std::string,std::vector<CVUniverse*> > data_sys_band;
  data_sys_band[std::string("CV")].push_back(sys_band[std::string("CV")][0]);

  for(uint j = 0; j< h.size(); ++j ){ 
    std::vector< PlotUtils::HistWrapper<CVUniverse>* > tmp_hists;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j][i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig")  ) continue;
      //Histwrapper has it's own copy of MnvH1D.  We need to put this into the vector of hists AND m_histList
      PlotUtils::HistWrapper<CVUniverse> *tmp_h = new PlotUtils::HistWrapper<CVUniverse>(h[j][i],hname.Contains("_data") ? data_sys_band : sys_band);

      h_idx = m_histList.IndexOf(h[j][i]); 
      m_histList.Remove(h[j][i]);
      h[j][i] = tmp_h->hist;
      m_histList.AddAt(tmp_h->hist,h_idx);

      tmp_hists.push_back(tmp_h); 
      m_hwList.push_back(tmp_h);
    }
    ret_hists.push_back(tmp_hists); 
  }

  return ret_hists;
}

std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > NukeHistogramUtils::getHistWrapper( std::vector< PlotUtils::MnvH2D** > h, std::map< std::string,std::vector<CVUniverse*> > &sys_band, bool bAllHists )
{
  std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0][0]->GetTitle(),h[0][0]->GetXaxis()->GetTitle(),h[0][0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx = 0;

  std::map< std::string,std::vector<CVUniverse*> > data_sys_band;
  data_sys_band[std::string("CV")].push_back(sys_band[std::string("CV")][0]);

  for(uint j = 0; j< h.size(); ++j ){ 
    std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > tmp_hists;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j][i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig") ) continue;

      PlotUtils::Hist2DWrapper<CVUniverse> *tmp_h = new PlotUtils::Hist2DWrapper<CVUniverse>(h[j][i],hname.Contains("_data") ? data_sys_band : sys_band);

      h_idx = m_histList.IndexOf(h[j][i]); 
      m_histList.Remove(h[j][i]);
      h[j][i] = tmp_h->hist;
      m_histList.AddAt(tmp_h->hist,h_idx);

      tmp_hists.push_back(tmp_h); 
      m_hw2DList.push_back(tmp_h);
    }
    ret_hists.push_back(tmp_hists); 
  }

  return ret_hists;
}

std::vector< EffHist< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > > NukeHistogramUtils::getEffHistWrapper( std::vector< EffHist< PlotUtils::MnvH1D** > > h, UniverseMap &mc_sys_band, UniverseMap &truth_sys_band, bool bAllHists  )
{
  std::vector< EffHist< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0].numerator[0]->GetTitle(),h[0].numerator[0]->GetXaxis()->GetTitle(),h[0].numerator[0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx_num = 0;
  uint h_idx_den = 0;

  for(uint j = 0; j< h.size(); ++j ){ 
    EffHist< std::vector<PlotUtils::HistWrapper<CVUniverse>* > > tmp_eff;
    std::vector< PlotUtils::HistWrapper<CVUniverse>* > tmp_vec_num;
    std::vector< PlotUtils::HistWrapper<CVUniverse>* > tmp_vec_den;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j].numerator[i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig") && !hname.Contains("_comp_coh") ) continue;

      PlotUtils::HistWrapper<CVUniverse> *tmp_num = new PlotUtils::HistWrapper<CVUniverse>(h[j].numerator[i]  ,mc_sys_band   , bAllHists );
      PlotUtils::HistWrapper<CVUniverse> *tmp_den = new PlotUtils::HistWrapper<CVUniverse>(h[j].denominator[i],truth_sys_band, bAllHists );

      /////////////////////////////////////////////////////
      h_idx_num = m_histList.IndexOf(h[j].numerator[i]);
      h_idx_den = m_histList.IndexOf(h[j].denominator[i]);

      m_histList.Remove(h[j].numerator[i]);
      m_histList.Remove(h[j].denominator[i]);

      h[j].numerator[i]   =  tmp_num->hist; 
      h[j].denominator[i] =  tmp_den->hist;

      m_histList.AddAt(tmp_num->hist,h_idx_num); 
      m_histList.AddAt(tmp_den->hist,h_idx_den);
      /////////////////////////////////////////////////////

      tmp_eff.numerator.push_back(tmp_num);
      tmp_eff.denominator.push_back(tmp_den);

      m_hwList.push_back(tmp_num);
      m_hwList.push_back(tmp_den);
    }
    ret_hists.push_back(tmp_eff); 
  }

  return ret_hists;
}

std::vector< EffHist< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > > NukeHistogramUtils::getEffHistWrapper( std::vector< EffHist< PlotUtils::MnvH2D** > > h, UniverseMap &mc_sys_band, UniverseMap &truth_sys_band, bool bAllHists  )
{
  std::vector< EffHist< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > > ret_hists;
  
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0].numerator[0]->GetTitle(),h[0].numerator[0]->GetXaxis()->GetTitle(),h[0].numerator[0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  uint h_idx_num = 0;
  uint h_idx_den = 0;

  for(uint j = 0; j< h.size(); ++j ){ 
    EffHist< std::vector<PlotUtils::Hist2DWrapper<CVUniverse>* > > tmp_eff;
    std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > tmp_vec_num;
    std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > tmp_vec_den;
    for(int i = 0; i < nhistos; ++i)
    {
      TString hname(h[j].numerator[i]->GetName());
      if( !bAllHists && !hname.Contains("_mc") && !hname.Contains("_bg_sig") && !hname.Contains("_signal") && !hname.Contains("_truth_sig") && !hname.Contains("_comp_coh") ) continue;

      PlotUtils::Hist2DWrapper<CVUniverse> *tmp_num = new PlotUtils::Hist2DWrapper<CVUniverse>(h[j].numerator[i]  ,mc_sys_band   ,bAllHists);
      PlotUtils::Hist2DWrapper<CVUniverse> *tmp_den = new PlotUtils::Hist2DWrapper<CVUniverse>(h[j].denominator[i],truth_sys_band,bAllHists);

      /////////////////////////////////////////////////////
      h_idx_num = m_histList.IndexOf(h[j].numerator[i]);
      h_idx_den = m_histList.IndexOf(h[j].denominator[i]);

      m_histList.Remove(h[j].numerator[i]);
      m_histList.Remove(h[j].denominator[i]);

      h[j].numerator[i]   =  tmp_num->hist; 
      h[j].denominator[i] =  tmp_den->hist;

      m_histList.AddAt(tmp_num->hist,h_idx_num); 
      m_histList.AddAt(tmp_den->hist,h_idx_den);
      /////////////////////////////////////////////////////
      tmp_eff.numerator.push_back(tmp_num);
      tmp_eff.denominator.push_back(tmp_den);

      m_hw2DList.push_back(tmp_num);
      m_hw2DList.push_back(tmp_den);
    }
    ret_hists.push_back(tmp_eff); 
  }

  return ret_hists;
}

std::vector<TString> NukeHistogramUtils::getGridInputFiles( std::string input_dir, int process, std::string &run_str, bool bGridTest )
{
  /*cout<<input_playlist<<endl;
  ifstream fin(input_playlist.c_str());
  assert(fin.is_open());
  std::string filename;

  //Where do we want to get the files?
  std::string merged_dir;
  if(bGridTest) merged_dir ="/pnfs/minerva/persistent/users/abercell/MergedFiles";//Use if you just want pnfs
  else          merged_dir = "root://fndca1.fnal.gov:1094/pnfs/fnal.gov/usr/minerva/persistent/users/abercell/MergedFiles";//xrootd.  Safer
  //else          mergtged_dir = "root://fndcagpvm01.fnal.gov:1094/pnfs/fnal.gov/usr/minerva/persistent/users/abercell/MergedFiles";//xrootd.  Safer

  //Grab the files from the playlist
  std::vector< TString > input_files;
  while (getline(fin, filename))
  {
    input_files.push_back(TString::Format("%s/%s/%s",merged_dir.c_str(),input_dir.c_str(),filename.c_str()));
    TString fileidx = TString(filename);
    TString run = fileidx(fileidx.Index("run")+3,8);
    run_str = run_str + std::string(run.Data());
  }
  fin.close();

  return input_files;*/

  ifstream fin(Form("%s/include/playlists/NukeCCPion_%s.txt", getenv("NUKECCPION_MACRO"),input_dir.c_str()));
  assert(fin.is_open());

  if( process < 0 ) run_str = "0";

  int iProcess = 0;
  std::string filename;
  std::vector< TString > input_files;
  while(getline(fin,filename))  
  {
    if( filename.empty() ) continue;
    if( filename.find( "#" )!=string::npos ) continue;

    if( process < 0 )
    {
      input_files.push_back( TString::Format("%s",filename.c_str()) );
    }
    else
    {
      if( process == iProcess ) 
      {
        input_files.push_back( TString::Format("%s",filename.c_str()) );
        TString fileidx = TString(filename);
        TString run = fileidx(fileidx.Index("run")+3,8);
        run_str = run_str + std::string(run.Data());
        break;
      }
    }
    ++iProcess;
  } 
  fin.close(); 
  return input_files;

}

void NukeHistogramUtils::writeHists(TString outname, bool bMCBkgd )
{
  TFile *output_file=new TFile(outname.Data(),"RECREATE"); 
  if( !output_file ){  cout << "NULL file! returning!" << endl; return;} 
  else{ cout<<endl; cout<<TString::Format("Writing output to %s",outname.Data())<<endl; }
  output_file->cd();
  if( bMCBkgd )
  {
    TIter next(&m_histList);
    TObject* object = 0;
    while((object=next()))
    {
      TString objName(object->GetName());
      if( objName.Contains("data") || objName.Contains("mc") || objName.Contains("signal") || 
          objName.Contains("bkgd") || objName.Contains("h_POT") ) object->Write();
    }
  }
  else
  {
    //TIter next(&m_histList);
    //TObject* object = 0;
    //while((object=next()))
    //{
    //  TString objName(object->GetName());
    //  if(objName.Contains("mig_")) continue;
    //  MnvH1D* hist = (MnvH1D*)object;
    //  cout<<hist->GetName()<<" "<<hist->Integral()<<" "<<hist->GetNErrorSources()<<endl; 
    //}
    m_histList.Write();
  } 

  output_file->Close();
}

void NukeHistogramUtils::syncHistWrappers()
{
  for(uint i = 0; i<m_hwList.size(); ++i)
  {
    m_hwList[i]->SyncCVHistos();
    //cout<<m_hwList[i]->hist->GetNVertErrorBands()<<endl;
  }
  for(uint i = 0; i<m_hw2DList.size(); ++i)
  {
    m_hw2DList[i]->SyncCVHistos();
  }
}

void NukeHistogramUtils::makePOTHists( TChain *meta, std::string datamc, std::string modifier )
{
  MnvH1D* h_Data_POT = new PlotUtils::MnvH1D(Form("h_Data_POT%s",modifier.c_str()),";Batches;POT",7,0.,7.);
  MnvH1D* h_MC_POT   = new PlotUtils::MnvH1D(Form("h_MC_POT%s",modifier.c_str()),";Batches;POT",7,0.,7.);

  if(datamc.find("Truth")==string::npos)
  {
    cout<<"Getting meta entries"<<endl;
    int nEntries = meta->GetEntries();
    double pot_used_all = 0;
    std::vector<double> pot_batch(6, 0);

    cout<<"Getting POT"<<endl;
    char leafName[50];
    for(int iEntry = 0; iEntry < nEntries; ++iEntry)
    {
      meta->GetEntry(iEntry);
      TLeaf* POT_Used = meta->GetLeaf("POT_Used");
      if(POT_Used) pot_used_all += POT_Used->GetValue();
      for( int batch = 1; batch <= 6; ++ batch )
      {
        sprintf(leafName,"POT_Used_batch%d",batch);
        TLeaf* POT_Used_batch = meta->GetLeaf(leafName);
        if(POT_Used_batch) pot_batch[batch+1]+= POT_Used_batch->GetValue();
      }
    }

    cout<<"Filling POT hists"<<endl;
    //Need to copy the POT code, because we can't use the 
    if(datamc.find("Data")!=string::npos) 
    {
      h_Data_POT->SetBinContent(1, pot_used_all);
      for( int batch = 1; batch <=6; ++batch )
      {
        h_Data_POT->SetBinContent( batch+1, pot_batch[batch-1] );
      }
    }
    else if(datamc.find("MC")!=string::npos) 
    {
      h_MC_POT->SetBinContent(1, pot_used_all);
      for( int batch = 1; batch <=6; ++batch )
      {
        h_MC_POT->SetBinContent( batch+1, pot_batch[batch-1] );
      }
    }
  }

  m_histList.Add(h_Data_POT);
  m_histList.Add(h_MC_POT);
}

void NukeHistogramUtils::addErrorBands( PlotUtils::MnvH1D** h, int nhistos ) 
{
//   int nUniverses = NukeFunctionUtils::Get().getNUniverses();
//
//   //bool include = false;
//   //if(  NukeFileUtils::Get().getMCVariation() == "none" || 
//   //    (NukeFileUtils::Get().getMCVariation() == "no_fsi" && NukeFunctionUtils::Get().getGenieVariation() ) ) include = true;
//
//   bool include = true;
//
//   if( NukeFunctionUtils::Get().getIncludeSystematics() && include ) {
//     addVerticalErrorBands(h,nhistos,nUniverses);
//   }
//
//   if( NukeFunctionUtils::Get().getIncludeSystematics() && NukeFileUtils::Get().getMCVariation() == "none" ) {
//     addLateralErrorBands(h,nhistos,nUniverses);
//   } 
   return;
}

void NukeHistogramUtils::addErrorBands( PlotUtils::MnvH2D** h, int nhistos )
{
//   int nUniverses = NukeFunctionUtils::Get().getNUniverses();
//   bool include = false;
//   if(  NukeFileUtils::Get().getMCVariation() == "none" ||
//       (NukeFileUtils::Get().getMCVariation() == "no_fsi" && NukeFunctionUtils::Get().getGenieVariation() ) ) include = true;
//
//   if( NukeFunctionUtils::Get().getIncludeSystematics() && include ) {
//     addVerticalErrorBands(h,nhistos,nUniverses);
//   }
//
//   if( NukeFunctionUtils::Get().getIncludeSystematics() && NukeFileUtils::Get().getMCVariation() == "none" ) {
//     addLateralErrorBands(h,nhistos,nUniverses);
//   } 
   return;
}

void NukeHistogramUtils::debugCuts( std::map<string, bool>& Cuts )
{
  int numBool=0;
  std::vector< string > names;
  for(std::map<string,bool>::iterator it=Cuts.begin(); it!=Cuts.end(); ++it){
    string name  = it->first;
    bool passcut = it->second;
    if( name.find("mc") != std::string::npos || name.find("bkgd") != std::string::npos ||
        name.find("signal") != std::string::npos || name.find("bg_sig") != std::string::npos ||
        name.find("comp_coh") != std::string::npos || name.find("truth_sig") != std::string::npos ||
        name.find("bg_phase") != std::string::npos ) continue; 
    if(passcut){ numBool++; names.push_back(name);}
  }
  if(numBool>1){
    cout<<endl;
    for(uint i=0;i<names.size();++i){ cout<< names[i]<<endl;}
  }
  return;
}

PlotUtils::MnvH1D* NukeHistogramUtils::subtractBkgdFromData( PlotUtils::MnvH1D* h_data, PlotUtils::MnvH1D* h_bkgd, double pot_scale )
{
  TString hname(h_data->GetName());
  TString hprefix = hname(0,hname.Index("_data"));
  PlotUtils::MnvH1D* h_data_BkgdSub = (PlotUtils::MnvH1D*)h_data->Clone( Form( "%s_BkgdSubData", hprefix.Data() ) );
  h_data_BkgdSub->AddMissingErrorBandsAndFillWithCV( *h_bkgd );
  h_data_BkgdSub->Add( h_bkgd, -pot_scale );
  return h_data_BkgdSub;
} 

PlotUtils::MnvH2D* NukeHistogramUtils::subtractBkgdFromData( PlotUtils::MnvH2D* h_data, PlotUtils::MnvH2D* h_bkgd, double pot_scale )
{
  TString hname(h_data->GetName());
  TString hprefix = hname(0,hname.Index("_data"));
  PlotUtils::MnvH2D* h_data_BkgdSub = (PlotUtils::MnvH2D*)h_data->Clone( Form( "%s_BkgdSubData", hprefix.Data() ) );
  h_data_BkgdSub->AddMissingErrorBandsAndFillWithCV( *h_bkgd );
  h_data_BkgdSub->Add( h_bkgd, -pot_scale );
  return h_data_BkgdSub;
} 

void NukeHistogramUtils::fillHisto( PlotUtils::MnvH1D** h, double var, double wgt, int index)
{
  h[index]->Fill(var, wgt);
}

void NukeHistogramUtils::fillDataHisto( PlotUtils::MnvH1D** h, double var, double wgt)
{
  h[0]->Fill(var,wgt);
}

void NukeHistogramUtils::fillHisto( PlotUtils::MnvH2D** h, double var1, double var2, double wgt, int index)
{
  h[index]->Fill(var1, var2, wgt);
}

void NukeHistogramUtils::fillDataHisto( PlotUtils::MnvH2D** h, double var1, double var2, double wgt)
{
  h[0]->Fill(var1, var2, wgt);
}

void NukeHistogramUtils::fill1DHistos( PlotUtils::MnvH1D** h, string classification, map< string, bool> &Cuts, double var, double wgt )
{
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  for(int i = 0; i < nhistos; ++i)
  {
    TString hname(h[i]->GetName());
    TString htitle = TString::Format("%s %s %s",h[i]->GetTitle(),h[i]->GetXaxis()->GetTitle(),h[i]->GetYaxis()->GetTitle());
    if( !(bool)hname.Contains(classification) || !(bool)htitle.Contains(classification) ) continue;//If isn't the right classification, move on
    for(std::map<string,bool>::iterator it=Cuts.begin(); it!=Cuts.end(); ++it){
      string name  = it->first;
      bool passcut = it->second;
      if( (bool)hname.EndsWith(name.c_str()) ){
        if( passcut ){
          h[i]->Fill( var, wgt );
        }
        break;  //Only breaks if it finds a histogram with a matched name 
      }
    }//-- loop through map with cuts
  }//-- loop through histograms
}

void NukeHistogramUtils::fill2DHistos( PlotUtils::MnvH2D** h, string classification, map< string, bool> &Cuts, double var1, double var2, double wgt )
{
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  for(int i = 0; i < nhistos; ++i)
  {
    TString hname(h[i]->GetName());
    TString htitle = TString::Format("%s %s %s",h[i]->GetTitle(),h[i]->GetXaxis()->GetTitle(),h[i]->GetYaxis()->GetTitle());
    if( !hname.Contains(classification) || !htitle.Contains(classification) ) continue;//If isn't the right classification, move on
    for(std::map<string,bool>::iterator it=Cuts.begin(); it!=Cuts.end(); ++it){
      string name  = it->first;
      bool passcut = it->second;
      if( hname.EndsWith(name.c_str()) ){
        if( passcut ) h[i]->Fill( var1, var2, wgt );
        break;  //Only breaks if it finds a histogram with a matched name 
      }
    }//-- loop through map with cuts
  }//-- loop through histograms
}

//PROTOTYPE Revisit this if you want to get things specific for nucleus
//void NukeHistogramUtils::fill1DHistwrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, string classification, map< string, bool> &Cuts, double var, double wgt, int iType )
//{
//  char htitle[200];
//  sprintf(htitle,"%s_%s_%s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
//  int nhistos = getNHistos(htitle);
//  TString htmpname(hws[0]->hist->GetName());
//  //Nucleus override
//  if( classification.find("nucleus") != string::npos && htmpname.Contains(TString::Format("tar%d_",iType).Data()) ) nhistos = getNHistos(htitle,TString::Format("tar%d",iType).Data());
//}

void NukeHistogramUtils::fill1DHistwrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, string classification, map< string, bool> &Cuts, double var, double wgt )
{
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  for(int i = 0; i < nhistos; ++i)
  {
    TString hname(hws[i]->hist->GetName());
    TString htitle = TString::Format("%s %s %s",hws[i]->hist->GetTitle(),hws[i]->hist->GetXaxis()->GetTitle(),hws[i]->hist->GetYaxis()->GetTitle());
    if( !(bool)hname.Contains(classification) || !(bool)htitle.Contains(classification) ) continue;//If isn't the right classification, move on
    for(std::map<string,bool>::iterator it=Cuts.begin(); it!=Cuts.end(); ++it){
      string name  = it->first;
      bool passcut = it->second;
      if( (bool)hname.EndsWith(name.c_str()) ){
        if( passcut ){
          hws[i]->univHist(cv)->Fill( var, wgt );
        }
        break;  //Only breaks if it finds a histogram with a matched name 
      }
    }//-- loop through map with cuts
  }//-- loop through histograms
}

void NukeHistogramUtils::fill2DHistwrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, string classification, map< string, bool> &Cuts, double var1, double var2, double wgt )
{
  char htitle[200];
  sprintf(htitle,"%s_%s_%s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  int nhistos = getNHistos(htitle);

  for(int i = 0; i < nhistos; ++i)
  {
    TString hname(hws[i]->hist->GetName());
    TString htitle = TString::Format("%s %s %s",hws[i]->hist->GetTitle(),hws[i]->hist->GetXaxis()->GetTitle(),hws[i]->hist->GetYaxis()->GetTitle());
    if( !(bool)hname.Contains(classification) || !(bool)htitle.Contains(classification) ) continue;//If isn't the right classification, move on
    for(std::map<string,bool>::iterator it=Cuts.begin(); it!=Cuts.end(); ++it){
      string name  = it->first;
      bool passcut = it->second;
      if( (bool)hname.EndsWith(name.c_str() )){
        if( passcut ){
          //cout<<hws[i]->hist->GetName()<<" "<<cv->ShortName()<<endl;
          //cout<<wgt<<" "<<hws[i]->univHist(cv)->Integral()<<endl;
          hws[i]->univHist(cv)->Fill( var1, var2, wgt );
        }
        break;  //Only breaks if it finds a histogram with a matched name 
      }
    }//-- loop through map with cuts
  }//-- loop through histograms
}

void NukeHistogramUtils::fillNukeCutsHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt)
{
  //!!! DO NOT USE
  
  string classification = "nukecuts";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  if(!histo_cuts->passReconstructorCuts(chw,ievent)) cout<<"Does not pass cut"<<endl;
  
  //This should only call each cut once per call
  std::map< string, bool > PassNukeCuts;
  PassNukeCuts["nukecut1_data"] = histo_cuts->passNukeCut1(chw,ievent);
  PassNukeCuts["nukecut2_data"] = histo_cuts->passNukeCut2(chw,ievent);
  PassNukeCuts["nukecut3_data"] = histo_cuts->passNukeCut3(chw,ievent);
  PassNukeCuts["nukecut4_data"] = histo_cuts->passNukeCut4(chw,ievent);
  PassNukeCuts["nukecut5_data"] = histo_cuts->passNukeCut5(chw,ievent);
  PassNukeCuts["nukecut6_data"] = histo_cuts->passNukeCut6(chw,ievent);
  PassNukeCuts["nukecut7_data"] = histo_cuts->passNukeCut7(chw,ievent);
  PassNukeCuts["data"]          = true;
  PassNukeCuts["nukecut1_mc"]   = histo_cuts->passNukeCut1(chw,ievent);       
  PassNukeCuts["nukecut2_mc"]   = histo_cuts->passNukeCut2(chw,ievent);    
  PassNukeCuts["nukecut3_mc"]   = histo_cuts->passNukeCut3(chw,ievent);    
  PassNukeCuts["nukecut4_mc"]   = histo_cuts->passNukeCut4(chw,ievent);    
  PassNukeCuts["nukecut5_mc"]   = histo_cuts->passNukeCut5(chw,ievent);    
  PassNukeCuts["nukecut6_mc"]   = histo_cuts->passNukeCut6(chw,ievent);    
  PassNukeCuts["nukecut7_mc"]   = histo_cuts->passNukeCut7(chw,ievent);    
  PassNukeCuts["mc"]            = true;

  fill1DHistos( h, classification, PassNukeCuts, var, wgt );
  
  return;
}

void NukeHistogramUtils::fillMCHistWrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var, double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  bool bSignal      = false;
  bool bTruthSignal = false;
  bool bCoh         = false;

  TString AllClassify(hws[0]->hist->GetTitle());
  //If you use this for a truth loop, bSignal is always False
  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bSignal = histo_cuts->isSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bTruthSignal = histo_cuts->isTruthSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bCoh = histo_cuts->isNuMuCCCoh( cv ); 

  for(uint i = 0; i<hws.size(); ++i)
  {
    TString hname(hws[i]->hist->GetName());
    if( hname.Contains("_mc") ) hws[i]->univHist(cv)->Fill(var, wgt*wgt_bkgd); 
    if( hname.Contains("_signal") && bSignal ) hws[i]->univHist(cv)->Fill(var, wgt*wgt_bkgd); 
    if( hname.Contains("_truth_sig") && bTruthSignal ) hws[i]->univHist(cv)->Fill(var, wgt*wgt_bkgd); 
    if( hname.Contains("_comp_coh") && bCoh && bTruthSignal ) hws[i]->univHist(cv)->Fill(var, wgt*wgt_bkgd); 
    if( ( hname.Contains("event_channel") || hname.Contains("pi_channel") || hname.Contains("true_W_channel") ) &&
        ( hname.Contains("_bg_sig") && !bSignal ) ) hws[i]->univHist(cv)->Fill(var, wgt*wgt_bkgd); 
  }
}

void NukeHistogramUtils::fillMCHistWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var1, double var2, double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  bool bSignal      = false;
  bool bTruthSignal = false;
  bool bCoh         = false;

  TString AllClassify(hws[0]->hist->GetTitle());
  //If you use this for a truth loop, bSignal is always False
  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bSignal = histo_cuts->isSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bTruthSignal = histo_cuts->isTruthSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bCoh = histo_cuts->isNuMuCCCoh( cv ); 

  for(uint i = 0; i<hws.size(); ++i)
  {
    TString hname(hws[i]->hist->GetName());
    if( hname.Contains("_mc") )  hws[i]->univHist(cv)->Fill(var1, var2, wgt*wgt_bkgd);
    if( hname.Contains("_signal") && bSignal ) hws[i]->univHist(cv)->Fill(var1, var2, wgt*wgt_bkgd); 
    if( hname.Contains("_truth_sig") && bTruthSignal ) hws[i]->univHist(cv)->Fill(var1, var2, wgt*wgt_bkgd); 
    if( hname.Contains("_comp_coh") && bCoh && bTruthSignal ) hws[i]->univHist(cv)->Fill(var1, var2, wgt*wgt_bkgd); 
    if( ( hname.Contains("event_channel") || hname.Contains("pi_channel") || hname.Contains("true_W_channel") ) &&
        ( hname.Contains("_bg_sig") && !bSignal ) ) hws[i]->univHist(cv)->Fill(var1, var2, wgt*wgt_bkgd); 
  }
}

void NukeHistogramUtils::fillResponseWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, PlotUtils::MnvH2D** h, CVUniverse* cv, int index, double reco_var1, double reco_var2, double true_var1, double true_var2, double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  bool bSignal      = false;
  bool bTruthSignal = false;
  bool bCoh         = false;

  TString AllClassify(hws[0]->hist->GetTitle());
  //If you use this for a truth loop, bSignal is always False
  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bSignal = histo_cuts->isSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bTruthSignal = histo_cuts->isTruthSignal( cv, index, bSignalOnePi ); 

  if( AllClassify.Contains("event_channel") || AllClassify.Contains("pi_channel")
                                            || AllClassify.Contains("true_W_channel") ) bCoh = histo_cuts->isNuMuCCCoh( cv ); 

  //Find the bin from h
  double binX = hws[0]->hist->GetXaxis()->GetBinCenter( h[0]->FindFixBin( reco_var1, reco_var2 ) );
  double binY = hws[0]->hist->GetYaxis()->GetBinCenter( h[0]->FindFixBin( true_var1, true_var2 ) );

  for(uint i = 0; i<hws.size(); ++i)
  {
    TString hname(hws[i]->hist->GetName());
    if( hname.Contains("_mc") ) hws[i]->univHist(cv)->Fill(binX, binY, wgt*wgt_bkgd);
    if( hname.Contains("_signal") && bSignal ) hws[i]->univHist(cv)->Fill(binX, binY, wgt*wgt_bkgd); 
    if( hname.Contains("_truth_sig") && bTruthSignal ) hws[i]->univHist(cv)->Fill(binX, binY, wgt*wgt_bkgd); 
    if( hname.Contains("_comp_coh") && bCoh && bTruthSignal ) hws[i]->univHist(cv)->Fill(binX, binY, wgt*wgt_bkgd); 
    if( ( hname.Contains("event_channel") || hname.Contains("pi_channel") || hname.Contains("true_W_channel") ) &&
        ( hname.Contains("_bg_sig") && !bSignal ) ) hws[i]->univHist(cv)->Fill(binX, binY, wgt*wgt_bkgd); 
  }
}

void NukeHistogramUtils::fillHistWrappersAndClassifyHistos( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, int iProng, bool bSignalOnePi )
{
  if( cv->ShortName().find("cv") != string::npos )
  {
    fillClassifyCVHistos( h, index, var, wgt, wgt_bkgd, cv, iProng, bSignalOnePi );
  }
  else
  {
    fillMCHistWrappers( hws, cv, index, var, wgt, bSignalOnePi );
  }
}

void NukeHistogramUtils::fillClassifyHistWrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var, double wgt, double wgt_bkgd, int iProng, bool bSignalOnePi )
{
  std::string AllClassify = hws[0]->hist->GetTitle();

  //cout<<"WGT "<<wgt<<" Bkgd "<<wgt_bkgd<<endl;
  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( hws, index, var, cv, wgt, wgt_bkgd, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( hws, index, var, cv, wgt, wgt_bkgd, bSignalOnePi ); 
  if( AllClassify.find("tarplastic") != std::string::npos )    fillTarPlasticHistos( hws, index, var, cv, wgt, wgt_bkgd,bSignalOnePi );

  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( hws, var, cv, index, wgt, wgt_bkgd ); 
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( hws, var, cv, index, wgt, wgt_bkgd, bSignalOnePi );

  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( hws, var, cv, index, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( hws, var, cv, index, wgt ); 
  }

  if( AllClassify.find("nucleus") != std::string::npos )  fillNucleiHistos(   hws, var, cv, wgt, wgt_bkgd ); 
  if( AllClassify.find("plasCont") != std::string::npos ) fillPlasContHistos( hws, var, cv, wgt, wgt_bkgd ); 

  if( iProng == -1 ){ 
    std::vector<int> pdgVec = cv->GetVec<int>("NukeCCPion_pion_trueParticlePDG");
    std::vector<int> destructCodeVec = cv->GetVec<int>("NukeCCPion_hadron_destructCode");
    for( uint prong = 0; prong< pdgVec.size(); ++prong){
      double reco_pion_p     = cv->GetVecElem("NukeCCPion_pion_p",prong);
      if(reco_pion_p<0) break;

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( hws, var, cv, prong, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos )  fillTwoPartHistos( hws, var, cv, prong, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( hws, var, cv, prong, wgt*wgt_bkgd ); 
    }
  }

  else if( iProng > -1 ) 
  {
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( hws, var, cv, iProng, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( hws, var, cv, iProng, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( hws, var, cv, iProng, wgt*wgt_bkgd ); 
  }
}

void NukeHistogramUtils::fillClassifyHistWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var1, double var2, double wgt, double wgt_bkgd, int iProng, bool bSignalOnePi )
{
  std::string AllClassify = hws[0]->hist->GetTitle();

  //cout<<"WGT "<<wgt<<" Bkgd "<<wgt_bkgd<<endl;
  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( hws, index, var1, var2, cv, wgt, wgt_bkgd, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( hws, index, var1, var2, cv, wgt, wgt_bkgd, bSignalOnePi ); 
  if( AllClassify.find("tarplastic") != std::string::npos )    fillTarPlasticHistos( hws, index, var1, var2, cv, wgt, wgt_bkgd,bSignalOnePi );

  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( hws, var1, var2, cv, index, wgt, wgt_bkgd ); 
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( hws, var1, var2, cv, index, wgt, wgt_bkgd, bSignalOnePi );

  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( hws, var1, var2, cv, index, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( hws, var1, var2, cv, index, wgt ); 
  }

  if( AllClassify.find("nucleus") != std::string::npos )  fillNucleiHistos(   hws, var1, var2, cv, wgt, wgt_bkgd ); 
  if( AllClassify.find("plasCont") != std::string::npos ) fillPlasContHistos( hws, var1, var2, cv, wgt, wgt_bkgd ); 

  if( iProng == -1 ){ 
    std::vector<int> pdgVec = cv->GetVec<int>("NukeCCPion_pion_trueParticlePDG");
    std::vector<int> destructCodeVec = cv->GetVec<int>("NukeCCPion_hadron_destructCode");
    for( uint prong = 0; prong< pdgVec.size(); ++prong){
      double reco_pion_p     = cv->GetVecElem("NukeCCPion_pion_p",prong);
      if(reco_pion_p<0) break;

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( hws, var1, var2, cv, prong, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( hws, var1, var2, cv, prong, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( hws, var1, var2, cv, prong, wgt*wgt_bkgd ); 
    }
  }

  else if( iProng > -1 ) 
  {
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( hws, var1, var2, cv, iProng, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( hws, var1, var2, cv, iProng, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( hws, var1, var2, cv, iProng, wgt*wgt_bkgd ); 
  }
}

void NukeHistogramUtils::fillDataVtxHistos( VtxHists< MnvH1D**> vtx_hists, int index, int_vtx_info int_vtx, int plastic )   
{
  fillDataHisto( vtx_hists.vtx_x[index], int_vtx.x );
  fillDataHisto( vtx_hists.vtx_y[index], int_vtx.y );
  fillDataHisto( vtx_hists.vtx_z[index], int_vtx.z );

  if( plastic == 0 ) 
  {
    if( int_vtx.target == 1 || int_vtx.target == 5 ) fillDataHisto( vtx_hists.per_u[index], int_vtx.u );
    else if( int_vtx.target == 2 )                   fillDataHisto( vtx_hists.per_d[index], int_vtx.d );
    else if( int_vtx.target == 3 ) {
                                                     fillDataHisto( vtx_hists.per_c[index], int_vtx.c );
      if( int_vtx.c <= 0.0 )                         fillDataHisto( vtx_hists.per_x[index], int_vtx.x );
    }
    if( int_vtx.target < 1 || int_vtx.target > 5)
    {
      if( index == 1 || index == 5 ) fillDataHisto( vtx_hists.per_u_sb[index], int_vtx.u );
      else if( index == 2 )          fillDataHisto( vtx_hists.per_d_sb[index], int_vtx.d );
      else if( index == 3 ) {
                                     fillDataHisto( vtx_hists.per_c_sb[index], int_vtx.c );
        if( int_vtx.c <= 0.0 )       fillDataHisto( vtx_hists.per_x_sb[index], int_vtx.x );
      }
    }
  }
}

void NukeHistogramUtils::fillMCVtxHistWrappers( VtxHists< std::vector< HistWrapper<CVUniverse>* > > vtx_hws, CVUniverse* cv, int index, int_vtx_info int_vtx, double wgt, double wgt_bkgd, bool bSignalOnePi, int plastic ) 
{
  std::string AllClassify = vtx_hws.vtx_z[index][0]->hist->GetTitle();

  if( AllClassify.find("event_channel") != string::npos ) 
  {
    fillEventChannelHistos( vtx_hws.vtx_x[index], index, int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillEventChannelHistos( vtx_hws.vtx_y[index], index, int_vtx.y, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillEventChannelHistos( vtx_hws.vtx_z[index], index, int_vtx.z, cv, wgt, wgt_bkgd, bSignalOnePi );

    if( plastic == 0 ) {
      if( int_vtx.target == 1 || int_vtx.target == 5 ) fillEventChannelHistos( vtx_hws.per_u[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 2 )                   fillEventChannelHistos( vtx_hws.per_d[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 3 ) 
      {
                                                       fillEventChannelHistos( vtx_hws.per_c[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
        if( int_vtx.c <= 0.0 )                         fillEventChannelHistos( vtx_hws.per_x[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
      } 
      if( int_vtx.target < 1 || int_vtx.target > 5)
      {
        if( index == 1 || index == 5 )   fillEventChannelHistos( vtx_hws.per_u_sb[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 2 )            fillEventChannelHistos( vtx_hws.per_d_sb[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 3 ) 
        {
                                         fillEventChannelHistos( vtx_hws.per_c_sb[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
          if( int_vtx.c <= 0.0 )         fillEventChannelHistos( vtx_hws.per_x_sb[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
        } 
      }
    }//End of int_vtx.targets only
  }
  if( AllClassify.find("pi_channel") != string::npos ) 
  {
    fillPiChannelHistos( vtx_hws.vtx_x[index], index, int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillPiChannelHistos( vtx_hws.vtx_y[index], index, int_vtx.y, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillPiChannelHistos( vtx_hws.vtx_z[index], index, int_vtx.z, cv, wgt, wgt_bkgd, bSignalOnePi );

    if( plastic == 0 ) {
      if( int_vtx.target == 1 || int_vtx.target == 5 ) fillPiChannelHistos( vtx_hws.per_u[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 2 )                   fillPiChannelHistos( vtx_hws.per_d[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 3 ) 
      {
                                                       fillPiChannelHistos( vtx_hws.per_c[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
        if( int_vtx.c <= 0.0 )                         fillPiChannelHistos( vtx_hws.per_x[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
      } 
      if( int_vtx.target < 1 || int_vtx.target > 5)
      {
        if( index == 1 || index == 5 )   fillPiChannelHistos( vtx_hws.per_u_sb[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 2 )            fillPiChannelHistos( vtx_hws.per_d_sb[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 3 ) 
        {
                                         fillPiChannelHistos( vtx_hws.per_c_sb[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
          if( int_vtx.c <= 0.0 )         fillPiChannelHistos( vtx_hws.per_x_sb[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
        }
      } 
    }//End of int_vtx.targets only
  }
  if( AllClassify.find("tarplastic") != string::npos ) 
  {
    fillTarPlasticHistos( vtx_hws.vtx_x[index], index, int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillTarPlasticHistos( vtx_hws.vtx_y[index], index, int_vtx.y, cv, wgt, wgt_bkgd, bSignalOnePi );
    fillTarPlasticHistos( vtx_hws.vtx_z[index], index, int_vtx.z, cv, wgt, wgt_bkgd, bSignalOnePi );

    if( plastic == 0 ) {
      if( int_vtx.target == 1 || int_vtx.target == 5 ) fillTarPlasticHistos( vtx_hws.per_u[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 2 )                   fillTarPlasticHistos( vtx_hws.per_d[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
      else if( int_vtx.target == 3 ) 
      {
                                                       fillTarPlasticHistos( vtx_hws.per_c[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
        if( int_vtx.c <= 0.0 )                         fillTarPlasticHistos( vtx_hws.per_x[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
      } 
      if( int_vtx.target < 1 || int_vtx.target > 5)
      {
        if( index == 1 || index == 5 )   fillTarPlasticHistos( vtx_hws.per_u_sb[index], index, int_vtx.u, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 2 )            fillTarPlasticHistos( vtx_hws.per_d_sb[index], index, int_vtx.d, cv, wgt, wgt_bkgd, bSignalOnePi );
        else if( index == 3 ) 
        {
                                         fillTarPlasticHistos( vtx_hws.per_c_sb[index], index,  int_vtx.c, cv, wgt, wgt_bkgd, bSignalOnePi );
          if( int_vtx.c <= 0.0 )         fillTarPlasticHistos( vtx_hws.per_x_sb[index], index,  int_vtx.x, cv, wgt, wgt_bkgd, bSignalOnePi );
        }
      } 
    }//End of int_vtx.targets only
  }
  if( AllClassify.find("nucleus") != string::npos ) {
    fillNucleiHistos( vtx_hws.vtx_x[index], int_vtx.x, cv, wgt, wgt_bkgd );
    fillNucleiHistos( vtx_hws.vtx_y[index], int_vtx.y, cv, wgt, wgt_bkgd );
    fillNucleiHistos( vtx_hws.vtx_z[index], int_vtx.z, cv, wgt, wgt_bkgd );

    if( plastic == 0 ) {
      if( int_vtx.target == 1 || int_vtx.target == 5 ) fillNucleiHistos( vtx_hws.per_u[index], int_vtx.u, cv, wgt, wgt_bkgd );
      else if( int_vtx.target == 2 )                   fillNucleiHistos( vtx_hws.per_d[index], int_vtx.d, cv, wgt, wgt_bkgd );
      else if( int_vtx.target == 3 ) {
                                                       fillNucleiHistos( vtx_hws.per_c[index], int_vtx.c, cv, wgt, wgt_bkgd );
        if( int_vtx.c <= 0.0 )                         fillNucleiHistos( vtx_hws.per_x[index], int_vtx.x, cv, wgt, wgt_bkgd );
      } 
      if( int_vtx.target < 1 || int_vtx.target > 5)
      {
        if( index == 1 || index == 5 )   fillNucleiHistos( vtx_hws.per_u_sb[index], int_vtx.u, cv, wgt, wgt_bkgd );
        else if( index == 2 )            fillNucleiHistos( vtx_hws.per_d_sb[index], int_vtx.d, cv, wgt, wgt_bkgd );
        else if( index == 3 ) {
                                         fillNucleiHistos( vtx_hws.per_c_sb[index], int_vtx.c, cv, wgt, wgt_bkgd );
          if( int_vtx.c <= 0.0 )         fillNucleiHistos( vtx_hws.per_x_sb[index], int_vtx.x, cv, wgt , wgt_bkgd );
        }
      } 
    }//End of int_vtx.targets only
  }//End of nucleus
}
 
//void NukeHistogramUtils::fillHist2DWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, double var1, double var2, double wgt )
//{
//  for(uint i = 0; i<hws.size(); ++i)
//  {
//    hws[i]->univHist(cv)->Fill(var1, var2, wgt); 
//  }
//}

//ugh.. you should reorder all of the fills if you use this
void NukeHistogramUtils::fillClassifyHistos( PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, 
                                               PlotUtils::ChainWrapper* chw, int ievent, int iProng, bool bSignalOnePi, int MichelPDG, bool fill_outside_fid_histos )
{
    //std::string AllClassify = getAllClassify();
  std::string AllClassify = h[0]->GetTitle();

  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( h, index, var, chw, ievent, wgt, wgt_bkgd, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( h, index, var, chw, ievent, wgt, wgt_bkgd, bSignalOnePi ); 
  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    std::vector<int>  mc_FSPartPDG = chw->GetValueVector<int>("mc_FSPartPDG",ievent);
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( h, var, mc_FSPartPDG, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( h, var, mc_FSPartPDG, wgt ); 
  }
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( h, var, chw, ievent, index, wgt, wgt_bkgd, bSignalOnePi );
  
  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( h, var, chw, ievent, wgt, wgt_bkgd ); 

  if( AllClassify.find("nucleus") != std::string::npos ){
    std::vector<double> mc_vtx = chw->GetValueVector<double>("mc_vtx",ievent);
    true_info mc;
    mc.nuclei   = chw->GetInt("mc_targetZ",ievent);
    mc.vtx_x    = mc_vtx[0];
    mc.vtx_y    = mc_vtx[1];
    mc.vtx_z    = mc_vtx[2];

    fillNucleiHistos( h, var, mc, wgt, wgt_bkgd, fill_outside_fid_histos ); 
  }

  if( AllClassify.find("plasCont") != std::string::npos ){
    std::vector<double> mc_vtx = chw->GetValueVector<double>("mc_vtx",ievent);
    true_info mc;
    mc.nuclei   = chw->GetInt("mc_targetZ",ievent);
    mc.vtx_x    = mc_vtx[0];
    mc.vtx_y    = mc_vtx[1];
    mc.vtx_z    = mc_vtx[2];

    fillPlasContHistos( h, var, mc, wgt, wgt_bkgd ); 
  }

  if( iProng == -1 ){ 
    std::vector<int> pdgVec          = chw->GetValueVector<int>("NukeCCPion_pion_trueParticlePDG", ievent);
    std::vector<double> hadron_E     = chw->GetValueVector<double>("NukeCCPion_hadron_E", ievent);
    std::vector<int> destructCodeVec = chw->GetValueVector<int>("NukeCCPion_hadron_destructCode", ievent);
    for( uint prong = 0; prong< pdgVec.size(); ++prong){
      int pdg = pdgVec[prong];
      int destructCode = destructCodeVec[prong];
      //Aaron, find something better please.  
      //I did, but I don't use this
      double reco_pion_p     = chw->GetValue("NukeCCPion_pion_p",ievent,prong);
      if(reco_pion_p<0) break;
      double true_tpi = hadron_E[prong] - histo_cuts->getParticleMass(pdg);

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
      if( AllClassify.find("cliffTop") != std::string::npos ) fillCliffTopologyHistos( h, var, pdg, destructCode, wgt); 
    }
  }
  //UGGHH Think how to do this better
  else if( iProng == -2 ) {
    int pdg          = MichelPDG; 
    int destructCode = 0;//this might have something similar in michel
    double true_tpi = 50; 
   
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
    if( AllClassify.find("cliffTop") != std::string::npos ) fillCliffTopologyHistos( h, var, pdg, destructCode, wgt); 

  }

  else if( iProng > -1 ) {
    int pdg          = chw->GetInt("NukeCCPion_pion_trueParticlePDG", ievent, iProng);
    int destructCode = chw->GetInt("NukeCCPion_hadron_destructCode", ievent, iProng);
    double true_tpi  = chw->GetValue("NukeCCPion_hadron_destructCode", ievent, iProng) - histo_cuts->getParticleMass(pdg);
    
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
    if( AllClassify.find("cliffTop") != std::string::npos ) fillCliffTopologyHistos( h, var, pdg, destructCode, wgt); 

  }
}

void NukeHistogramUtils::fillClassifyCVHistos( PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, CVUniverse *cv, int iProng, bool bSignalOnePi )
{
  std::string AllClassify = h[0]->GetTitle();

  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( h, index, var, cv, wgt, 1, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( h, index, var, cv, wgt, 1, bSignalOnePi ); 
  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( h, var, mc_FSPartPDG, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( h, var, mc_FSPartPDG, wgt ); 
  }
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( h, var, cv, index, wgt, wgt_bkgd, bSignalOnePi );

  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( h, var, cv, index, wgt, 1 ); 

  if( AllClassify.find("background") != std::string::npos )  fillBackgroundHistos( h, index, var, cv, wgt, 1, bSignalOnePi ); 
  if( AllClassify.find("phase_space") != std::string::npos ) fillPhaseSpaceHistos( h, index, var, cv, wgt, 1, bSignalOnePi ); 

  if( iProng == -1 ){ 
    std::vector<int> pdgVec = cv->GetVec<int>("NukeCCPion_pion_trueParticlePDG");
    std::vector<int> destructCodeVec = cv->GetVec<int>("NukeCCPion_hadron_destructCode");
    for( uint prong = 0; prong< pdgVec.size(); ++prong){
      int pdg = pdgVec[prong];
      int destructCode = destructCodeVec[prong];
      //Aaron, find something better please
      double reco_pion_p     = cv->GetVecElem("NukeCCPion_pion_p",prong);
      if(reco_pion_p<0) break;
      double true_tpi = cv->GetTpiTruthMatch(prong); 

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
    }
  }

  else if( iProng > -1 ) {
    int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
    int destructCode = cv->GetVecElemInt("NukeCCPion_hadron_destructCode", iProng);
    double true_tpi  = cv->GetTpiTruthMatch(iProng); 

    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
  }
}

void NukeHistogramUtils::fillClassifyHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, double wgt, double wgt_bkgd, 
                                               PlotUtils::ChainWrapper* chw, int ievent, int iProng, bool bSignalOnePi, int MichelPDG, bool fill_outside_fid_histos )
{
    //std::string AllClassify = getAllClassify();
  std::string AllClassify = h[0]->GetTitle();

  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( h, index, var1, var2, chw, ievent, wgt, wgt_bkgd, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( h, index, var1, var2, chw, ievent, wgt, wgt_bkgd, bSignalOnePi ); 
  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    std::vector<int>  mc_FSPartPDG = chw->GetValueVector<int>("mc_FSPartPDG",ievent);
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( h, var1, var2, mc_FSPartPDG, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( h, var1, var2, mc_FSPartPDG, wgt ); 
  }
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( h, var1, var2, chw, ievent, index, wgt, wgt_bkgd, bSignalOnePi);
  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( h, var1, var2, chw, ievent, wgt, wgt_bkgd ); 

  if( AllClassify.find("nucleus") != std::string::npos ){
    std::vector<double> mc_vtx = chw->GetValueVector<double>("mc_vtx",ievent);
    true_info mc;
    mc.nuclei   = chw->GetInt("mc_targetZ",ievent);
    mc.vtx_x    = mc_vtx[0];
    mc.vtx_y    = mc_vtx[1];
    mc.vtx_z    = mc_vtx[2];

    fillNucleiHistos( h, var1, var2, mc, wgt, wgt_bkgd, fill_outside_fid_histos ); 
  }

  if( AllClassify.find("plasCont") != std::string::npos ){
    std::vector<double> mc_vtx = chw->GetValueVector<double>("mc_vtx",ievent);
    true_info mc;
    mc.nuclei   = chw->GetInt("mc_targetZ",ievent);
    mc.vtx_x    = mc_vtx[0];
    mc.vtx_y    = mc_vtx[1];
    mc.vtx_z    = mc_vtx[2];

    fillPlasContHistos( h, var1, var2, mc, wgt, wgt_bkgd ); 
  }
  if( iProng == -1 ){ 
    std::vector<int> pdgVec = chw->GetValueVector<int>("NukeCCPion_pion_trueParticlePDG", ievent);
    std::vector<int> destructCodeVec = chw->GetValueVector<int>("NukeCCPion_hadron_destructCode", ievent);
    std::vector<double> hadron_E     = chw->GetValueVector<double>("NukeCCPion_hadron_E", ievent);
    for( uint prong = 0; prong< pdgVec.size(); prong++){
      int pdg = pdgVec[prong];
      int destructCode = destructCodeVec[prong];
      //Aaron, find something better please
      double reco_pion_p     = chw->GetValue("NukeCCPion_pion_p",ievent,prong);
      if(reco_pion_p<0) break;
      double true_tpi = hadron_E[prong] - histo_cuts->getParticleMass(pdg);

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var1, var2, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
      if( AllClassify.find("cliffTop") != std::string::npos ) fillCliffTopologyHistos( h, var1, var2, pdg, destructCode, wgt); 
    }
  }
  else if( iProng > -1 ) {
    int pdg          = chw->GetInt("NukeCCPion_pion_trueParticlePDG", ievent, iProng);
    int destructCode = chw->GetInt("NukeCCPion_hadron_destructCode", ievent, iProng);
    double true_tpi  = chw->GetValue("NukeCCPion_hadron_destructCode", ievent, iProng) - histo_cuts->getParticleMass(pdg);
    
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var1, var2, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
    if( AllClassify.find("cliffTop") != std::string::npos ) fillCliffTopologyHistos( h, var1, var2, pdg, destructCode, wgt); 
  }
}

void NukeHistogramUtils::fillClassifyCVHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, double wgt, double wgt_bkgd, CVUniverse* cv, int iProng, bool bSignalOnePi )
{
  std::string AllClassify = h[0]->GetTitle();

  //Need pdg, destructCode, mc, fill_outside_fid_histos
  if( AllClassify.find("event_channel") != std::string::npos ) fillEventChannelHistos( h, index, var1, var2, cv, wgt, 1, bSignalOnePi ); 

  if( AllClassify.find("pi_channel") != std::string::npos )    fillPiChannelHistos( h, index, var1, var2, cv, wgt, 1, bSignalOnePi ); 
  if( AllClassify.find("pip_number") != std::string::npos || AllClassify.find("pi0_number") != std::string::npos )
  {
    std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
    if( AllClassify.find("pip_number") != std::string::npos ) fillNPiPlusHistos( h, var1, var2, mc_FSPartPDG, wgt ); 
    if( AllClassify.find("pi0_number") != std::string::npos ) fillNPiZeroHistos( h, var1, var2, mc_FSPartPDG, wgt ); 
  }
  if( AllClassify.find("true_W_channel") != std::string::npos ) fillWHistos( h, var1, var2, cv, index, wgt, wgt_bkgd, bSignalOnePi);

  if( AllClassify.find("production_channel") != std::string::npos ) fillProductionChannelHistos( h, var1, var2, cv, index, wgt, 1 ); 

  if( iProng == -1 ){ 
    std::vector<int> pdgVec = cv->GetVec<int>("NukeCCPion_pion_trueParticlePDG");
    std::vector<int> destructCodeVec = cv->GetVec<int>("NukeCCPion_hadron_destructCode");
    for( uint prong = 0; prong< pdgVec.size(); prong++){
      int pdg = pdgVec[prong];
      int destructCode = destructCodeVec[prong];
      //Aaron, find something better please
      double reco_pion_p     = cv->GetVecElem("NukeCCPion_pion_p",prong);
      if(reco_pion_p<0) break;
      double true_tpi = cv->GetTpiTruthMatch(prong); 

      if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
      if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var1, var2, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
    }
  }
  else if( iProng > -1 ) {
    int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
    int destructCode = cv->GetVecElemInt("NukeCCPion_hadron_destructCode", iProng);
    double true_tpi  = cv->GetTpiTruthMatch(iProng); 
    
    if( AllClassify.find("particle") != std::string::npos ) fillParticleHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("twopart") != std::string::npos ) fillTwoPartHistos( h, var1, var2, pdg, true_tpi, wgt*wgt_bkgd );
    if( AllClassify.find("topology") != std::string::npos ) fillTopologyHistos( h, var1, var2, pdg, destructCode, true_tpi, wgt*wgt_bkgd ); 
  }
}

void NukeHistogramUtils::fillEventChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse* cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassEventChannelCuts );
  fill1DHistwrappers( hws, cv, classification, PassEventChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillEventChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse* cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassEventChannelCuts );
  fill2DHistwrappers( hws, cv, classification, PassEventChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillEventChannelHistos( PlotUtils::MnvH1D** h, int index, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassEventChannelCuts );
  fill1DHistos( h, classification, PassEventChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillEventChannelHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassEventChannelCuts );
  fill1DHistos( h, classification, PassEventChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillEventChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly );   
 
 //debugCuts( PassEventChannelCuts );
  fill2DHistos( h, classification, PassEventChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillEventChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, CVUniverse* cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "event_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassEventChannelCuts = histo_cuts->fillEventChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   
 
  //debugCuts( PassEventChannelCuts );
  fill2DHistos( h, classification, PassEventChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPhaseSpaceHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "phase_space"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPhaseSpaceCuts = histo_cuts->fillPhaseSpaceCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassEventChannelCuts );
  fill1DHistos( h, classification, PassPhaseSpaceCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillBackgroundHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "background"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassBackgroundCuts = histo_cuts->fillBackgroundCuts( cv, index, bSignalOnePi );   

  //debugCuts( PassEventChannelCuts );
  fill1DHistos( h, classification, PassBackgroundCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassPiChannelCuts );
  fill1DHistwrappers( hws, cv, classification, PassPiChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassPiChannelCuts );
  fill2DHistwrappers( hws, cv, classification, PassPiChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( PlotUtils::MnvH1D** h, int index, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassPiChannelCuts );
  fill1DHistos( h, classification, PassPiChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   

  //debugCuts( PassPiChannelCuts );
  fill1DHistos( h, classification, PassPiChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly );   
 
 //debugCuts( PassPiChannelCuts );
  fill2DHistos( h, classification, PassPiChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillPiChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, CVUniverse* cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "pi_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPiChannelCuts = histo_cuts->fillPiChannelCuts( cv, index, bSignalOnePi, m_MCBkgdOnly );   
 
  //debugCuts( PassPiChannelCuts );
  fill2DHistos( h, classification, PassPiChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillTarPlasticHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "tarplastic"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassTarPlasticCuts = histo_cuts->fillTarPlasticCuts( cv, index, bSignalOnePi );   

  //debugCuts( PassTarPlasticCuts );
  fill1DHistwrappers( hws, cv, classification, PassTarPlasticCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillTarPlasticHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse *cv, double wgt, double wgt_bkgd, bool bSignalOnePi)
{
  string classification = "tarplastic"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassTarPlasticCuts = histo_cuts->fillTarPlasticCuts( cv, index, bSignalOnePi );   

  //debugCuts( PassTarPlasticCuts );
  fill2DHistwrappers( hws, cv, classification, PassTarPlasticCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper *chw, int ievent, int index, double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill1DHistos( h, classification, PassWCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( PlotUtils::MnvH2D** h, double var1, double var2, PlotUtils::ChainWrapper *chw, int ievent, int index,  double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( chw, ievent, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill2DHistos( h, classification, PassWCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int index,  double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( cv, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill1DHistwrappers( hws, cv, classification, PassWCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int index,  double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( cv, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill2DHistwrappers( hws, cv, classification, PassWCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( PlotUtils::MnvH1D** h, double var, CVUniverse *cv, int index,  double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( cv, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill1DHistos( h, classification, PassWCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillWHistos( PlotUtils::MnvH2D** h, double var1, double var2, CVUniverse *cv, int index,  double wgt, double wgt_bkgd, bool bSignalOnePi )
{
  string classification = "true_W_channel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassWCuts = histo_cuts->fillWCuts( cv, index, bSignalOnePi, m_MCBkgdOnly  );

  //debugCuts( PassWCuts );

  fill2DHistos( h, classification, PassWCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( cv, ivec, m_MCBkgdOnly );   

  debugCuts( PassProductionChannelCuts );
  fill1DHistwrappers( hws, cv, classification, PassProductionChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( cv, ivec, m_MCBkgdOnly );   

  debugCuts( PassProductionChannelCuts );
  fill2DHistwrappers( hws, cv, classification, PassProductionChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( chw, ievent, m_MCBkgdOnly );   

  //debugCuts( PassProductionChannelCuts );
  fill1DHistos( h, classification, PassProductionChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( PlotUtils::MnvH1D** h, double var, CVUniverse *cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( cv, ivec, m_MCBkgdOnly );   

  debugCuts( PassProductionChannelCuts );
  fill1DHistos( h, classification, PassProductionChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( PlotUtils::MnvH2D** h, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( chw, ievent, m_MCBkgdOnly );   
 
 //debugCuts( PassProductionChannelCuts );
  fill2DHistos( h, classification, PassProductionChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillProductionChannelHistos( PlotUtils::MnvH2D** h, double var1, double var2, CVUniverse* cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "production_channel"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;

  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassProductionChannelCuts = histo_cuts->fillProductionChannelCuts( cv, ivec, m_MCBkgdOnly );   
 
  //debugCuts( PassProductionChannelCuts );
  fill2DHistos( h, classification, PassProductionChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillNucleonChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "nucleon_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNucleonChannelCuts = histo_cuts->fillNucleonChannelCuts( cv, ivec, m_MCBkgdOnly );   

  debugCuts( PassNucleonChannelCuts );
  fill1DHistwrappers( hws, cv, classification, PassNucleonChannelCuts, var, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillNucleonChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int ivec, double wgt, double wgt_bkgd)
{
  string classification = "nucleon_channel"; 
  std::string AllClassify = getAllClassify();
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNucleonChannelCuts = histo_cuts->fillNucleonChannelCuts( cv, ivec, m_MCBkgdOnly );   

  debugCuts( PassNucleonChannelCuts );
  fill2DHistwrappers( hws, cv, classification, PassNucleonChannelCuts, var1, var2, wgt*wgt_bkgd );

  return; 
}

void NukeHistogramUtils::fillNPiPlusHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int ivec, double wgt )
{
  string classification = "pip_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  bool bTrueTarget = histo_cuts->passTrueTargetCut( cv, ivec );
  std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
  std::map< string, bool > PassNPiPlusCuts = histo_cuts->fillNPiPlusCuts( mc_FSPartPDG, bTrueTarget, m_MCBkgdOnly  );

  //debugCuts( PassNPiPlusCuts );

  fill1DHistwrappers( hws, cv, classification, PassNPiPlusCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiPlusHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int ivec, double wgt )
{
  string classification = "pip_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  bool bTrueTarget = histo_cuts->passTrueTargetCut( cv, ivec );
  std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
  std::map< string, bool > PassNPiPlusCuts = histo_cuts->fillNPiPlusCuts( mc_FSPartPDG, bTrueTarget, m_MCBkgdOnly  );

  //debugCuts( PassNPiPlusCuts );

  fill2DHistwrappers( hws, cv, classification, PassNPiPlusCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiPlusHistos( PlotUtils::MnvH1D** h, double var, std::vector<int>& FS_PDG, double wgt )
{
  string classification = "pip_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassNPiPlusCuts = histo_cuts->fillNPiPlusCuts( FS_PDG, true, m_MCBkgdOnly  );

  //debugCuts( PassNPiPlusCuts );

  fill1DHistos( h, classification, PassNPiPlusCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiPlusHistos( PlotUtils::MnvH2D** h, double var1, double var2, std::vector<int>& FS_PDG, double wgt )
{
  string classification = "pip_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNPiPlusCuts = histo_cuts->fillNPiPlusCuts( FS_PDG, true, m_MCBkgdOnly  );

  //debugCuts( PassNPiPlusCuts );
  fill2DHistos( h, classification, PassNPiPlusCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiZeroHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int ivec, double wgt )
{
  string classification = "pi0_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  bool bTrueTarget = histo_cuts->passTrueTargetCut( cv, ivec );
  std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
  std::map< string, bool > PassNPiZeroCuts = histo_cuts->fillNPiZeroCuts( mc_FSPartPDG, bTrueTarget, m_MCBkgdOnly  );

  //debugCuts( PassNPiZeroCuts );

  fill1DHistwrappers( hws, cv, classification, PassNPiZeroCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiZeroHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int ivec, double wgt )
{
  string classification = "pi0_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  bool bTrueTarget = histo_cuts->passTrueTargetCut( cv, ivec );
  std::vector<int>  mc_FSPartPDG = cv->GetVec<int>("mc_FSPartPDG");
  std::map< string, bool > PassNPiZeroCuts = histo_cuts->fillNPiZeroCuts( mc_FSPartPDG, bTrueTarget, m_MCBkgdOnly  );

  //debugCuts( PassNPiZeroCuts );

  fill2DHistwrappers( hws, cv, classification, PassNPiZeroCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiZeroHistos( PlotUtils::MnvH1D** h, double var, std::vector<int>& FS_PDG, double wgt )
{
  string classification = "pi0_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassNPiZeroCuts = histo_cuts->fillNPiZeroCuts( FS_PDG, true, m_MCBkgdOnly  );

  //debugCuts( PassNPiZeroCuts );

  fill1DHistos( h, classification, PassNPiZeroCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillNPiZeroHistos( PlotUtils::MnvH2D** h, double var1, double var2, std::vector<int>& FS_PDG, double wgt )
{
  string classification = "pi0_number";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNPiZeroCuts = histo_cuts->fillNPiZeroCuts( FS_PDG, true, m_MCBkgdOnly  );

  //debugCuts( PassNPiZeroCuts );
  fill2DHistos( h, classification, PassNPiZeroCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillTwoPartHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt )
{
  string classification = "twopart";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //This should only call each cut once per call
  std::map< string, bool > PassTwoPartCuts = histo_cuts->fillTwoPartCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTwoPartCuts );

  fill1DHistwrappers( hws, cv, classification, PassTwoPartCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillTwoPartHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt )
{
  string classification = "twopart";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //This should only call each cut once per call
  std::map< string, bool > PassTwoPartCuts = histo_cuts->fillTwoPartCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTwoPartCuts );

  fill2DHistwrappers( hws, cv, classification, PassTwoPartCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillTwoPartHistos( PlotUtils::MnvH1D** h, double var, int pdg, double true_tpi, double wgt )
{
  string classification = "twopart";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassTwoPartCuts = histo_cuts->fillTwoPartCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTwoPartCuts );

  fill1DHistos( h, classification, PassTwoPartCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillTwoPartHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, double true_tpi, double wgt )
{
  string classification = "twopart";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassTwoPartCuts = histo_cuts->fillTwoPartCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTwoPartCuts );
  fill2DHistos( h, classification, PassTwoPartCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillParticleHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt )
{
  string classification = "particle";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //This should only call each cut once per call
  std::map< string, bool > PassParticleCuts = histo_cuts->fillParticleCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassParticleCuts );

  fill1DHistwrappers( hws, cv, classification, PassParticleCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillParticleHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt )
{
  string classification = "particle";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //This should only call each cut once per call
  std::map< string, bool > PassParticleCuts = histo_cuts->fillParticleCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassParticleCuts );

  fill2DHistwrappers( hws, cv, classification, PassParticleCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillParticleHistos( PlotUtils::MnvH1D** h, double var, int pdg, double true_tpi, double wgt )
{
  string classification = "particle";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;
  
  //This should only call each cut once per call
  std::map< string, bool > PassParticleCuts = histo_cuts->fillParticleCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassParticleCuts );

  fill1DHistos( h, classification, PassParticleCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillParticleHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, double true_tpi, double wgt )
{
  string classification = "particle";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassParticleCuts = histo_cuts->fillParticleCuts( pdg, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassParticleCuts );
  fill2DHistos( h, classification, PassParticleCuts, var1, var2, wgt );

  return; 
}

//Production channel (production_channel) Double check nucleus

//void NukeHistogramUtils::fillTopologyHistos( PlotUtils::MnvH1D** h, double var, int pdg, int destructCode, double wgt)
//{
//
//  string classification = "topology"; 
//  std::string AllClassify = getAllClassify();
//  //return if this function's classification wasn't in the overall classification
//  if( AllClassify.find(classification) == std::string::npos ) return;
//  
//  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
//  if( !htitle.Contains(classification) ) return;
//
//  //Gets the topology once, no need to fill it multiple times
//  std::map< string, bool > PassTopologyCuts = histo_cuts->fillTopologyCuts( pdg, destructCode, m_MCBkgdOnly  );
//
//  //debugCuts( PassTopologyCuts )
//  fill1DHistos( h, classification, PassTopologyCuts, var, wgt );
//
//  return; 
//}

void NukeHistogramUtils::fillPlasContHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, double wgt, double wgt_bkgd )
{
  string classification = "plasCont"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  true_info mc;
  mc.nuclei   = cv->GetInt("mc_targetZ");
  std::vector<double> mc_vtx = cv->GetVecDouble("mc_vtx");
  mc.vtx_x = mc_vtx[0];  mc.vtx_y = mc_vtx[1];  mc.vtx_z = mc_vtx[2];

  std::map< string, bool > PassPlasContCuts = histo_cuts->fillPlasContCuts( mc );
  fill1DHistwrappers( hws, cv, classification, PassPlasContCuts, var, wgt*wgt_bkgd );

  return;
}

void NukeHistogramUtils::fillPlasContHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, double wgt, double wgt_bkgd )
{
  string classification = "plasCont"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  true_info mc;
  mc.nuclei   = cv->GetInt("mc_targetZ");
  std::vector<double> mc_vtx = cv->GetVecDouble("mc_vtx");
  mc.vtx_x = mc_vtx[0];  mc.vtx_y = mc_vtx[1];  mc.vtx_z = mc_vtx[2];

  std::map< string, bool > PassPlasContCuts = histo_cuts->fillPlasContCuts( mc );
  fill2DHistwrappers( hws, cv, classification, PassPlasContCuts, var1, var2, wgt*wgt_bkgd );

  return;
}

void NukeHistogramUtils::fillPlasContHistos( PlotUtils::MnvH1D** h, double var, true_info mc, double wgt, double wgt_bkgd  )
{
  string classification = "plasCont"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPlasContCuts = histo_cuts->fillPlasContCuts( mc );
  fill1DHistos( h, classification, PassPlasContCuts, var, wgt*wgt_bkgd );

  return;
}

void NukeHistogramUtils::fillPlasContHistos( PlotUtils::MnvH2D** h, double var1, double var2, true_info mc, double wgt, double wgt_bkgd  )
{
  string classification = "plasCont"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassPlasContCuts = histo_cuts->fillPlasContCuts( mc );
  fill2DHistos( h, classification, PassPlasContCuts, var1, var2, wgt );
  
  return;
}

void NukeHistogramUtils::fillNucleiHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, double wgt, double wgt_bkgd, 
                                                                bool fill_outside_fid_histos )
{
  string classification = "nucleus"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  true_info mc;
  mc.nuclei   = cv->GetInt("mc_targetZ");
  std::vector<double> mc_vtx = cv->GetVecDouble("mc_vtx");
  mc.vtx_x = mc_vtx[0];  mc.vtx_y = mc_vtx[1];  mc.vtx_z = mc_vtx[2];

  std::map< string, bool > PassNucleiCuts = histo_cuts->fillNucleiCuts( mc );
  fill1DHistwrappers( hws, cv, classification, PassNucleiCuts, var, wgt*wgt_bkgd );

  if( fill_outside_fid_histos && m_IncludeNucleusOutside ) {
    std::map< string, bool > PassNucleiOutsideCuts = histo_cuts->fillNucleiOutsideCuts( mc );
    fill1DHistwrappers( hws, cv, classification, PassNucleiOutsideCuts, var, wgt );
  }

  return;
}

void NukeHistogramUtils::fillNucleiHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, double wgt, double wgt_bkgd, 
                                                                bool fill_outside_fid_histos )
{
  string classification = "nucleus"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  true_info mc;
  mc.nuclei   = cv->GetInt("mc_targetZ");
  std::vector<double> mc_vtx = cv->GetVecDouble("mc_vtx");
  mc.vtx_x = mc_vtx[0];  mc.vtx_y = mc_vtx[1];  mc.vtx_z = mc_vtx[2];

  std::map< string, bool > PassNucleiCuts = histo_cuts->fillNucleiCuts( mc );
  fill2DHistwrappers( hws, cv, classification, PassNucleiCuts, var1, var2, wgt*wgt_bkgd );

  if( fill_outside_fid_histos && m_IncludeNucleusOutside ) {
    std::map< string, bool > PassNucleiOutsideCuts = histo_cuts->fillNucleiOutsideCuts( mc );
    fill2DHistwrappers( hws, cv, classification, PassNucleiOutsideCuts, var1, var2, wgt );
  }

  return;
}

void NukeHistogramUtils::fillNucleiHistos( PlotUtils::MnvH1D** h, double var, true_info mc, double wgt, double wgt_bkgd, 
                                                                bool fill_outside_fid_histos )
{
  string classification = "nucleus"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNucleiCuts = histo_cuts->fillNucleiCuts( mc );
  fill1DHistos( h, classification, PassNucleiCuts, var, wgt*wgt_bkgd );

  if( fill_outside_fid_histos && m_IncludeNucleusOutside ) {
    std::map< string, bool > PassNucleiOutsideCuts = histo_cuts->fillNucleiOutsideCuts( mc );
    fill1DHistos( h, classification, PassNucleiOutsideCuts, var, wgt );
  }

  return;
}

void NukeHistogramUtils::fillNucleiHistos( PlotUtils::MnvH2D** h, double var1, double var2, true_info mc, 
                                           double wgt, double wgt_bkgd, bool fill_outside_fid_histos)
{
string classification = "nucleus"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  std::map< string, bool > PassNucleiCuts = histo_cuts->fillNucleiCuts( mc );
  fill2DHistos( h, classification, PassNucleiCuts, var1, var2, wgt );
  
  if( fill_outside_fid_histos && m_IncludeNucleusOutside ) {
    std::map< string, bool > PassNucleiOutsideCuts = histo_cuts->fillNucleiOutsideCuts( mc );
    fill2DHistos( h, classification, PassNucleiOutsideCuts, var1, var2, wgt );
  }
  
  return;
}

void NukeHistogramUtils::fillTopologyHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt  )
{
  string classification = "topology"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  int destructCode = cv->GetVecElemInt("NukeCCPion_hadron_destructCode", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassTopologyCuts = histo_cuts->fillTopologyCuts( pdg, destructCode, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTopologyCuts );
  fill1DHistwrappers( hws, cv, classification, PassTopologyCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillTopologyHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt  )
{
  string classification = "topology"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",hws[0]->hist->GetTitle(),hws[0]->hist->GetXaxis()->GetTitle(),hws[0]->hist->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  int pdg          = cv->GetVecElemInt("NukeCCPion_pion_trueParticlePDG", iProng);
  int destructCode = cv->GetVecElemInt("NukeCCPion_hadron_destructCode", iProng);
  double true_tpi  = cv->GetTpiTruthMatch(iProng); 

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassTopologyCuts = histo_cuts->fillTopologyCuts( pdg, destructCode, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTopologyCuts );
  fill2DHistwrappers( hws, cv, classification, PassTopologyCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillTopologyHistos( PlotUtils::MnvH1D** h, double var, int pdg, int destructCode, double true_tpi, double wgt)
{
  string classification = "topology"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassTopologyCuts = histo_cuts->fillTopologyCuts( pdg, destructCode, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTopologyCuts );
  fill1DHistos( h, classification, PassTopologyCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillTopologyHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, int destructCode, double true_tpi, double wgt)
{
  string classification = "topology"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassTopologyCuts = histo_cuts->fillTopologyCuts( pdg, destructCode, true_tpi, m_MCBkgdOnly  );

  //debugCuts( PassTopologyCuts );
  fill2DHistos( h, classification, PassTopologyCuts, var1, var2, wgt );
 
  return; 
}

void NukeHistogramUtils::fillCliffTopologyHistos( PlotUtils::MnvH1D** h, double var, int pdg, int destructCode, double wgt)
{
  string classification = "cliffTop"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassCliffTopologyCuts = histo_cuts->fillCliffTopologyCuts( pdg, destructCode );

  //debugCuts( PassCliffTopologyCuts );
  fill1DHistos( h, classification, PassCliffTopologyCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillCliffTopologyHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, int destructCode, double wgt)
{
  string classification = "cliffTop"; 
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //Gets the topology once, no need to fill it multiple times
  std::map< string, bool > PassCliffTopologyCuts = histo_cuts->fillCliffTopologyCuts( pdg, destructCode );

  //debugCuts( PassCliffTopologyCuts );
  fill2DHistos( h, classification, PassCliffTopologyCuts, var1, var2, wgt );

  return; 
}

void NukeHistogramUtils::fillNewMichelHistos( PlotUtils::MnvH1D** h, double var, michel_info& michel, int iprong, double wgt )
{
  string classification = "newmichel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //This should only call each cut once per call
  std::map< string, bool > PassNewMichelCuts = histo_cuts->fillNewMichelCuts( michel, iprong );
  
  //debugCuts( PassNewMichelCuts );

  fill1DHistos( h, classification, PassNewMichelCuts, var, wgt );

  return; 
}

void NukeHistogramUtils::fillNewMichelHistos( PlotUtils::MnvH2D** h, double var1, double var2, michel_info& michel, int iprong, double wgt)
{
  string classification = "newmichel";
  std::string AllClassify = getAllClassify();
  //return if this function's classification wasn't in the overall classification
  if( AllClassify.find(classification) == std::string::npos ) return;
  
  TString htitle = TString::Format("%s %s %s",h[0]->GetTitle(),h[0]->GetXaxis()->GetTitle(),h[0]->GetYaxis()->GetTitle());
  if( !htitle.Contains(classification) ) return;

  //This should only call each cut once per call
  std::map< string, bool > PassNewMichelCuts = histo_cuts->fillNewMichelCuts( michel, iprong );
 
  //debugCuts( PassNewMichelCuts );

  fill2DHistos( h, classification, PassNewMichelCuts, var1, var2, wgt );

  return; 
}

#endif

