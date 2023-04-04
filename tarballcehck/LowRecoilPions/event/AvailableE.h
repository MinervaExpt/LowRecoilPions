#ifndef EVENTQ_H
#define EVENTQ_H

#include <iostream>
#include "PlotUtils/MinervaUniverse.h"

class EventQ : public PlotUtils::MinervaUniverse {

      public:
      #include "PlotUtils/MuonFunctions.h" // GetMinosEfficiencyWeight
      #include "PlotUtils/TruthFunctions.h" //Getq3True
      #include "PlotUtils/LowRecoilFunctions.h" // GetEAvailable()

      // ========================================================================
      //  Constructor/Destructor
      // ========================================================================
      
      EventQ(PlotUtils::ChainWrapper* chw, double nsigma = 0)
      : PlotUtils::MinervaUniverse(chw, nsigma) {}

      virtual ~EventQ() {}

      // ========================================================================
      //   // Quantities defined here as constants for the sake of below. Definition
      //     // matched to Dan's CCQENuInclusiveME variables from:
      //       // `/minerva/app/users/drut1186/cmtuser/Minerva_v22r1p1_OrigCCQENuInc/Ana/CCQENu/ana_common/include/CCQENuUtils.h`

      static constexpr double M_n = 939.56536;
      static constexpr double M_p = 938.272013;
      static constexpr double M_nucleon = (1.5*M_n+M_p)/2.5;
      static constexpr double M_mu = 105.6583/1000.; //Converting to GeV
      static constexpr int PDG_n = 2112;
      static constexpr int PDG_p = 2212;
      static constexpr double M_pi = 139.57039; //in MeV

      // GetFunctions 
      //
      // ========================================================================
      // Write a "Get" function for all quantities access by your analysis.
      // For composite quantities (e.g. Enu) use a calculator function
      // In order to properly calculate muon variables and systematics use the
      // various functions defined in MinervaUniverse.
      // E.g. GetPmu, GetEmu, etc.
      // ========================================================================

      


}
