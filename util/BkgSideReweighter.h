//File: Reweighter.h
//Brief: A Reweighter changes the CV model into a different model using just a multiplicative
//       constant.  All vertical systematics are implemented by taking ratios to such weights.
//       Some Reweighters are mutually exclusive, and others are only needed for specific systematics.
//Author: Andrew Olivier aolivier@ur.rochester.edu

#ifndef BKGSIG_REWEIGHTER_H
#define BKGSIG_REWEIGHTER_H

#if __cplusplus < 201103L
  #define override
#endif

//c++ includes
#include <string>
#include <vector>
#include "PlotUtils/Reweighter.h"

namespace PlotUtils
{

  template <class UNIVERSE, class EVENT = MichelEvent>
  class BkgSideReweighter:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      BkgSideReweighter() = default;
      virtual ~BkgSideReweighter() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{


        double weight = 1.0;
        std::vector<double> bkgScaleFactors = {1.19244383771, 1.09013470332, 1.12069371873, 0.983542302985, 0.914772435765, 0.809941914184, 0.694502935553, 0.832308771075, 0.522963786348, 0.955402552231};

        std::vector<double>sigScaleFactors = {1.47609549374,1.04737115921,1.01859300236,0.941330399358,0.913565649665,0.817960763676,0.990615578846,0.953137009493,1.10526207567,1.1081843998};
        bool isSignal = false;	
        bool isBackground = false;
        if (univ.GetTrueNPionsinEvent() >= 1) isSignal = true;
        else{ isBackground = true;}
	//else if (univ.GetTrueNPi0inEvent() > 0) isBackground = true;
        //else if (univ.GetTrueNPionsinEvent() == 0 and univ.GetTrueNPi0inEvent() == 0 and univ.GetTrueNKaonsinEvent() == 0 and univ.GetTrueNNeutralKaonsinEvent() == 0) isBackground = true;

        double eavail = univ.GetTrueEAvailGEV();  //GetTrueEAvail();//NewEavail();
        std::vector<double> eavbinsGEV = {0.0, .075, .150, .225, .300, .400, .500, .600, .700, .800, .900, 1.000};
	/*        
        for (int i =0; i < eavailbins.size(); i++){
	     if (i < eavailbins.size() and eavail >= eavailbins[i] and eavail < eavailbins[i+1]){
                   if(isSignal) weight = sigScaleFactors[i];
	           else if(isBackground) weight = bkgScaleFactors[i];
                   break;
	     }
	     else if (eavail > 1000.){
		   if(isSignal) weight = sigScaleFactors[9];
		   else if(isBackground) weight = bkgScaleFactors[9];
		   break;
	     }


        }*/

	//mehreenpTBins = {0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6}	
	double pT = univ.GetMuonPTTrue();
	double p0 = 1.0;
	double p1 = 1.0;
	if (isBackground == true and eavail < .250){
		if (pT <= 0.2){
			p0 = 1.21456;
			p1 = 1.35223;
		}
		else if (pT > 0.2 and pT <= 0.4){
			p0 = 1.30892;
			p1 = -1.11561;

		}
		else if (pT > 0.4 and pT <= 0.6){
			p0 =  1.35141;
			p1 = -0.999961;
		}
		else if (pT > 0.6 and pT <= 0.8){
			p0 = 1.54076;
			p1 =  -1.42152;

		}
	        weight = p0 + p1*eavail;
		return weight;
	}
	else{
		return 1.0;	
	}
	
      };
      virtual std::string GetName() const {return "BkgSideReweight"; }

      virtual bool DependsReco() const {return false;}
      //virtual bool DependsTruth() const {return true;}; //Not needed as of time of writing.
      //PlotUtils::PionReweighter& PionReweighter();
      //virtual bool IsCompatible(const PionReweighter& /*other*/) const { return true; }
      //virtual std::vector<UNIVERSE*> GetRequiredUniverses() const { return std::vector<UNIVERSE*>{}; }
  };
}

#endif //PION_REWEIGHTER_H
