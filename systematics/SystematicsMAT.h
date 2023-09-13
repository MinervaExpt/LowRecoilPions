#ifndef Systematics_h
#define Systematics_h

//==============================================================================
// Get Several standard MINERvA systematics
//==============================================================================

#include "event/CVUniverse.h"
#include "PlotUtils/FluxSystematics.h"
#include "PlotUtils/GenieSystematics.h"
#include "PlotUtils/MinosEfficiencySystematics.h"
#include "PlotUtils/MuonSystematics.h"
#include "PlotUtils/MnvTuneSystematics.h"
#include "PlotUtils/MuonResolutionSystematics.h"
#include "PlotUtils/AngleSystematics.h"
#include "PlotUtils/TargetMassSystematics.h"
#include "PlotUtils/MichelSystematics.h"
#include "PlotUtils/MuonSystematics.h"
#include "PlotUtils/NSFDefaults.h"
#include "PlotUtils/ResponseSystematics.h"
#include "PlotUtils/ChainWrapper.h"
#include "PlotUtils/GeantHadronSystematics.h"
#include "CohDiffractiveSystematics.h"


typedef std::map<std::string, std::vector<CVUniverse*>> UniverseMap;

UniverseMap GetStandardSystematics(PlotUtils::ChainWrapper* chain)
{
  // return map
  UniverseMap error_bands;

  // CV
  error_bands[std::string("cv")].push_back(new CVUniverse(chain));

  // Detector 
   
  

  //========================================================================
  // FLUX
  //========================================================================
  UniverseMap bands_flux =
      PlotUtils::GetFluxSystematicsMap<CVUniverse>(chain, CVUniverse::GetNFluxUniverses());
  error_bands.insert(bands_flux.begin(), bands_flux.end());

  //========================================================================
  // GENIE
  //========================================================================
  // Standard
  UniverseMap bands_genie =
      PlotUtils::GetGenieSystematicsMap<CVUniverse>(chain, false); //PlotUtils::GetStandardGenieSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_genie.begin(), bands_genie.end());

  //Ma Res
  UniverseMap new_res_genie_error_bands =
        PlotUtils::GetGenieResPionFitSystematicsMap<CVUniverse>(chain);
    error_bands.insert(new_res_genie_error_bands.begin(),
    new_res_genie_error_bands.end());
  
  //MvRES
  UniverseMap new_ep_genie_error_bands =
        PlotUtils::GetGenieEPMvResSystematicsMap<CVUniverse>(chain);
    error_bands.insert(new_ep_genie_error_bands.begin(),
    new_ep_genie_error_bands.end());

  

  //========================================================================
  // MnvTunes
  //========================================================================
  // RPA
  UniverseMap bands_rpa = PlotUtils::GetRPASystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_rpa.begin(), bands_rpa.end());

  // 2P2H
  UniverseMap bands_2p2h = PlotUtils::Get2p2hSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_2p2h.begin(), bands_2p2h.end());

  //========================================================================
  // Muons
  //========================================================================
  // Muon reco in MINERvA -- Catchall systematic for pmu reco in minerva.
  // Lateral-only. Shifts pmu.
  UniverseMap bands_muon_minerva =
      PlotUtils::GetMinervaMuonSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_muon_minerva.begin(), bands_muon_minerva.end());

  // Muons in MINOS -- Catchall systematic for wiggle solution -- correlates
  // flux universes and minos muon momentum reco.
  // Lateral AND Vertical systematic. Shifts Pmu and GetFluxAndCVUniverse.
  //
  // Expect a non-zero systematic even when no pmu involved.
  UniverseMap bands_muon_minos =
     PlotUtils::GetMinosMuonSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_muon_minos.begin(), bands_muon_minos.end());

  // Vertical only
  UniverseMap bands_minoseff =
      PlotUtils::GetMinosEfficiencySystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_minoseff.begin(), bands_minoseff.end());

  UniverseMap bands_muon_resolution = PlotUtils::GetMuonResolutionSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_muon_resolution.begin(), bands_muon_resolution.end());

  UniverseMap bands_geant = PlotUtils::GetGeantHadronSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_geant.begin(), bands_geant.end());

  // Beam angle
  UniverseMap bands_angle = PlotUtils::GetAngleSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_angle.begin(), bands_angle.end());

  // Hadron inelastics cross sections
  //TODO: There's some special recoil function I need to write for the response systematics to work correctly
  /*UniverseMap bands_response = PlotUtils::GetResponseSystematicsMap<CVUniverse>(chain);
  error_bands.insert(bands_response.begin(), bands_response.end());*/
  const bool use_neutron = false;
    const bool use_new = true;
    UniverseMap bands_response =
        PlotUtils::GetResponseSystematicsMap<CVUniverse>(chain, use_neutron,
                                                         use_new);
    error_bands.insert(bands_response.begin(), bands_response.end());

  
  //========================================================================
  //TargetMassSystematics
  //========================================================================

  UniverseMap error_bands_tarmass = PlotUtils::GetTargetMassSystematicsMap<CVUniverse>( chain );
  error_bands.insert(error_bands_tarmass.begin(),error_bands_tarmass.end());

  //========================================================================
  //Michel Efficiency Error Bands
  //========================================================================
  UniverseMap michel_error_bands = PlotUtils::GetMichelEfficiencySystematicsMap<CVUniverse>(chain);
  error_bands.insert(michel_error_bands.begin(),michel_error_bands.end());  
  
  //Diffractive Error Bands
  UniverseMap error_bands_cohdiff =
        GetCohDiffractiveSystematicsMap( chain );
    error_bands.insert(error_bands_cohdiff.begin(),
    error_bands_cohdiff.end());
  
  for (auto band : error_bands) {
    std::vector<CVUniverse*> universes = band.second;
    for (auto universe : universes) universe->SetTruth(is_truth);
  }

  //Adding systematics according to what Ben has

  //UniverseMap new_res_genie_error_bands = PlotUtils::GetGenieResPionFitSystematicsMap<CVUniverse>(chain);
  //error_bands.insert(new_res_genie_error_bands.begin(), new_res_genie_error_bands.end());

  //UniverseMap new_ep_genie_error_bands = PlotUtils::GetGenieEPMvResSystematicsMap<CVUniverse>(chain);
  //error_bands.insert(new_ep_genie_error_bands.begin(), new_ep_genie_error_bands.end());
  


  return error_bands;
}

#endif  // Systematics_h
