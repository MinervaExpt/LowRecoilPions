//File: COHPionReweighter.h
//Brief: A Reweighter changes the CV model into a different model using just a multiplicative
//       constant.  All vertical systematics are implemented by taking ratios to such weights.
//       Some Reweighters are mutually exclusive, and others are only needed for specific systematics.


#ifndef TARGETMASS_REWEIGHTER_H
#define TARGETMASS_REWEIGHTER_H

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
  class TargetMassReweighter:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      TargetMassReweighter() = default;
      virtual ~TargetMassReweighter() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{
	double weight = univ.GetTargetMassWeight();//*univ.GetTrueMichelEfficiencyWeight(); //GetTargetMassWeight();; 
	
        return weight;
                
	
      };
      virtual std::string GetName() const {return "TargetMassReweighter"; }

      virtual bool DependsReco() const {return false;}
      //virtual bool DependsTruth() const {return true;}; //Not needed as of time of writing.
      //PlotUtils::PionReweighter& PionReweighter();
      //virtual bool IsCompatible(const PionReweighter& /*other*/) const { return true; }
      //virtual std::vector<UNIVERSE*> GetRequiredUniverses() const { return std::vector<UNIVERSE*>{}; }
  };
}

#endif //DIFFRACTIVE_REWEIGHTER_H
