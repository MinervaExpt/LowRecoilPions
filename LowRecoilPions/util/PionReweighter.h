//File: Reweighter.h
//Brief: A Reweighter changes the CV model into a different model using just a multiplicative
//       constant.  All vertical systematics are implemented by taking ratios to such weights.
//       Some Reweighters are mutually exclusive, and others are only needed for specific systematics.
//Author: Andrew Olivier aolivier@ur.rochester.edu

#ifndef PION_REWEIGHTER_H
#define PION_REWEIGHTER_H

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
  class PionReweighter:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      PionReweighter() = default;
      virtual ~PionReweighter() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{


        double weight2 = 1.0;

	// The following is a reweight for Mnvtunev4 (Aaron's ROOT file) + Mehreen's Cuts (no overflow in range) - NO LOW Q2 - with tolerance of 0.07
	
	/*std::vector<double> tpiweights = {0.1463731, 0.2560693, 0.2574526, 0.3550735, 0.4449296, 0.5424848,
       				     	  0.689206 , 0.8075024, 0.9062778, 0.7989852, 1.0543231, 1.0485164,
       					  1.3478616, 1.2264872, 1.0434803, 1.3833882, 1.1364908, 1.2445642,
        		                  1.1477601, 1.0125145, 0.8575593, 0.5598411, 0.5413836, 0.5520705,
        			          0.9651784, 0.74787  , 0.9644799, 0.975053 , 0.8552795, 0.9439185}; // These are weights from python notebook 
	*/
	// These are weights from Mathematica 
	// {0.145544,0.254611,0.256008,0.35303,0.442377,0.539401,0.685322,0.80295,0.901203,0.7945,1.0482,1.04233,1.34035,1.21939,1.03745,1.37538,1.13002,1.23749,1.14112,1.00627,0.852107,0.557473,0.538646,0.548259,0.958125,0.742081,0.957056,0.968198,0.849366,0.936737};// 1.2 Tolerance
	




       // These are weights with no LowQ2 and with COH 1.4 Scale to account for diffractive events
	/*std::vector<double> tpiweights = {0.1463731, 0.2560693, 0.2574526, 0.3550735, 0.4449296, 0.5424848,
       					  0.689206 , 0.8075024, 0.9062778, 0.7989852, 1.0543231, 1.0485164,
       					  1.3478616, 1.2264872, 1.0434803, 1.3833882, 1.1364908, 1.2445642,
       					  1.1477601, 1.0125145, 0.8575593, 0.5598411, 0.5413836, 0.5520705,
       					  0.9651784, 0.74787  , 0.9644799, 0.975053 , 0.8552795, 0.9439185};

	*/

        //
        
        /* 
        //The following is a reweight for MnvTunev4 WITH LOWQ2 and tolerance of 0.07
        std::vector<double> tpiweights = {0.1343913, 0.2187968, 0.2313141, 0.3063998, 0.3839539, 0.4662279,
       0.5862746, 0.706122 , 0.7974511, 0.7056143, 0.9210527, 0.9020598,
       1.1558561, 1.0895753, 0.8977514, 1.2209612, 1.0269214, 1.1106524,
       1.0232318, 0.8816901, 0.6715675, 0.4520906, 0.4597239, 0.4883855,
       0.8576775, 0.6477114, 0.8632815, 0.8529108, 0.7252371, 0.7618927};
        */
       
        	
 	// The following are weights for MnvTunev4 WITH LOW Q2 and COH 1.4 Scale with Tolerance of 0.07
 	std::vector<double> tpiweights = {0.1191236, 0.2020399, 0.2212095, 0.2770364, 0.3469764, 0.3706979,
      	 0.4700661, 0.5407503, 0.6600101, 0.5904604, 0.8462214, 0.8231438,
       	 0.9762195, 0.9716326, 0.7618331, 1.0650391, 0.9630661, 0.9521869,
       	 0.9175258, 0.8582132, 0.5937072, 0.3543653, 0.3964163, 0.4193023,
       	 0.7045454, 0.5470016, 0.7443208, 0.7370278, 0.6201841, 0.5991364};






	// Before Optimization of the script (prior to March 21, 2023) 
	/*
          {0.1340057, 0.2178327, 0.2308023, 0.3065482, 0.3848411, 0.4667965,
           0.5851043, 0.7044237, 0.7921043, 0.6991538, 0.9244578, 0.9133085,
           1.1511266, 1.1048231, 0.9025478, 1.2282045, 1.0455529, 1.1442468,
           1.0316732, 0.9360767, 0.6802597, 0.4572329, 0.4797465, 0.514938 ,
           0.8897806, 0.6670714, 0.9041721, 0.8814068, 0.7353624, 0.7870013};
         */
         //{0.1349671, 0.2194824, 0.2324611, 0.3074448, 0.385085 , 0.4667326,0.5866438, 0.7062348, 0.7983695, 0.7070036, 0.9233816, 0.9019015, 1.1558886, 1.0958217, 0.8969745, 1.2229454, 1.0284349, 1.1156926, 1.0217027, 0.8876227, 0.6706334, 0.4537697, 0.4615512, 0.488923 ,0.8561042, 0.6461674, 0.8609727, 0.8513815, 0.7240095, 0.762974};

	

	 

	 /*
	// {0.133636,0.217564,0.230035,0.304639,0.381746,0.463561,0.582943,0.702102,0.792977,0.701658,0.915696,0.896688,1.14943,1.08327,0.892555,1.21387,1.02105,1.10431,1.01738,0.876281,0.667025,0.450283,0.457366,0.484879,0.851214,0.642544,0.856433,0.846765,0.720108,0.755901}; Mathematica 1.2 Tol Weights
	*/
 
        std::vector<double> tpilowbins = {1., 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 350., 400., 500., 700.,1000.}; 
        if (univ.GetTrueNPionsinEvent() == 0) return 1.0;
        double tpi = univ.GetTrueLowestTpiEvent();
        //std::cout << "Printing the q3 of the event " << q3_mecAna << std::endl;	
	//std::cout << "Printing the lowest Tpi in Event " << tpi << std::endl;
        //double angle = cos(univ.GetTrueAngleLowTpi());	
        //double tpi = myevent.m_nmichels[0].pionKE/1000.;
        for (int i = 0; i< tpilowbins.size(); i++){
                if (i < tpilowbins.size() and tpi >= tpilowbins[i] and tpi < tpilowbins[i+1]){
		   weight2 = tpiweights[i];
 		   break;
		}
		else if (tpi >= 1000.){
	           weight2 = abs(tpiweights[29]);
       		   break; 
                }
        }
        //if (angle > 0.10) weight2 = 0.90*weight2; //Correcting for Forward going Pions 
	return weight2;
      };
      virtual std::string GetName() const {return "LowRecPionReweight"; }

      virtual bool DependsReco() const {return false;}
      //virtual bool DependsTruth() const {return true;}; //Not needed as of time of writing.
      //PlotUtils::PionReweighter& PionReweighter();
      //virtual bool IsCompatible(const PionReweighter& /*other*/) const { return true; }
      //virtual std::vector<UNIVERSE*> GetRequiredUniverses() const { return std::vector<UNIVERSE*>{}; }
  };
}

#endif //PION_REWEIGHTER_H
