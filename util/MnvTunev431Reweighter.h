//File: COHPionReweighter.h
//Brief: A Reweighter changes the CV model into a different model using just a multiplicative
//       constant.  All vertical systematics are implemented by taking ratios to such weights.
//       Some Reweighters are mutually exclusive, and others are only needed for specific systematics.


#ifndef MNVTUNEV431_REWEIGHTER_H
#define MNVTUNEV431_REWEIGHTER_H

#if __cplusplus < 201103L
  #define override
#endif

//c++ includes
#include <string>
#include <vector>
#include "PlotUtils/Reweighter.h"
#include "systematics/Systematics.h"
#include "PlotUtils/TargetMassSystematics.h"
#include "PlotUtils/MichelSystematics.h"
#include "PlotUtils/WeightFunctions.h"
#include "PlotUtils/MnvTuneSystematics.h"
namespace PlotUtils
{

  template <class UNIVERSE, class EVENT = MichelEvent>
  class MnvTunev431Reweighter:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      MnvTunev431Reweighter() = default;
      virtual ~MnvTunev431Reweighter() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{
	double weight = 1.0;
        double wgt_lowq2pi = univ.GetLowQ2PionWeight(); //"MENU1PI"); // univ.GetQ2True() / 1000000., "MENU1PI"); //univ.GetLowQ2PionWeight();        
        double wgt_diff = 1.0; //univ.GetDiffractiveWeight();
        double wgt_mich = univ.GetMichelEfficiencyReWeight();
        double wgt_target = univ.GetTargetMassReWeight(); //GetTargetMassWeight();; 
        double wgt_fsi = univ.GetFSIReWeight();
        double wgt_geant = univ.GetGeantHadronReWeight();
        double wgt_coh = univ.GetCOHPionWeight();
	double wgt_cohm = 1.0;//univ.GetCOHTpiCorrection();
	double wgt_neutpi = 1.0; //univ.GetWeightForOnePiWNeutron();
	//double q2Gev = univ.GetQ2True()/1000000.;
	if(univ.GetInt("mc_intType") == 4){
	  //std::cout << "Printing Scale APplied ot COH events in MnvTuneReweighter: " << 1.4 << std::endl;
          wgt_diff = 1.436;
     	  wgt_cohm = 3.0; 
	  //std::cout << "Coherent Weight Added by Mehreen is" << wgt_cohm << std::endl;
	 
        } // This is a correction to COH events Aaron's adds to account for missing diffractive model in our simulation
        /*
	std::cout << "Printing LowQ2 weight for Q2: " << q2Gev << " and weight: " << wgt_lowq2pi << "\n"
		  << "Printing Diffractive weight: " << wgt_diff <<  "\n"
		  << "Printing MichelEff weight: " << wgt_mich << "\n"
		  << "Printing TargetMass weight: " << wgt_target << "\n"
		  << "Printing FSI weight: " << wgt_fsi << "\n"
		  << "Printing GeantHadron weight: " << wgt_geant << "\n"
		  << "Printing COH weight: " << wgt_coh << "\n" << std::endl;
        */
	double wgt_mk = 1.0;// univ.GetMKReWeight();
 	return weight*wgt_diff*wgt_mich*wgt_target*wgt_fsi*wgt_coh*wgt_geant*wgt_lowq2pi*wgt_mk*wgt_cohm*wgt_neutpi;
                
	
      };
      virtual std::string GetName() const {return "MnvTunev431Reweighter"; }

      virtual bool DependsReco() const {return false;}
      //virtual bool DependsTruth() const {return true;}; //Not needed as of time of writing.
      //PlotUtils::PionReweighter& PionReweighter();
      //virtual bool IsCompatible(const PionReweighter& /*other*/) const { return true; }
      //virtual std::vector<UNIVERSE*> GetRequiredUniverses() const { return std::vector<UNIVERSE*>{}; }
  };
}

#endif //MnvTunev431_REWEIGHTER_H
