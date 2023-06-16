//File: COHPionReweighter.h
//Brief: A Reweighter changes the CV model into a different model using just a multiplicative
//       constant.  All vertical systematics are implemented by taking ratios to such weights.
//       Some Reweighters are mutually exclusive, and others are only needed for specific systematics.


#ifndef COHPion_REWEIGHTER_H
#define COHPion_REWEIGHTER_H

#if __cplusplus < 201103L
  #define override
#endif

//c++ includes
#include <string>
#include <vector>
#include "PlotUtils/Reweighter.h"
#include "PlotUtils/WeightFunctions.h"
namespace PlotUtils
{

  template <class UNIVERSE, class EVENT = MichelEvent>
  class Pi0BkgScale:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      Pi0BkgScale() = default;
      virtual ~Pi0BkgScale() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{
	if( univ.GetInt("mc_intType") != 4){
	   
 	    int npi = univ.GetTrueNPionsinEvent();
            bool npi0 = GetTrueIsFSPartInEvent(111);
	    if (npi == 1) return 1.0;
   	    else{
		if (npi0 == true){
		   std::vector<double> eavailbins = {};
		   std::vector<double> scalfactors = {1.20, 1.14, 1.28, 0.98, 1.05, 0.95, 1.04, 1.02};
	           

		}

 	    } 
	    double angle = univ.GetTrueAngleHighTpi(); 
	    double KE = univ.GetTrueHighTpi();
	 }
         else return 1.0;

                
	
      };
      virtual std::string GetName() const {return "COHPionReweighter"; }

      virtual bool DependsReco() const {return false;}
      //virtual bool DependsTruth() const {return true;}; //Not needed as of time of writing.
      //PlotUtils::PionReweighter& PionReweighter();
      //virtual bool IsCompatible(const PionReweighter& /*other*/) const { return true; }
      //virtual std::vector<UNIVERSE*> GetRequiredUniverses() const { return std::vector<UNIVERSE*>{}; }
  };
}

#endif //DIFFRACTIVE_REWEIGHTER_H
