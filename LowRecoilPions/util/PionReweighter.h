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

        /*
	std::vector<double> q3bin1weights = {0.82435, 0.830887, 0.862543, 0.917496, 0.991634, 1.08006, 1.17502, 1.2697, 1.35885, 1.43734, 1.49575, 1.51875, 1.47963, 1.34423, 1.13559, 0.918846, 0.788976, 0.735919, 0.71303, 0.706644, 0.70802, 0.710867, 0.711998};

        std::vector<double> q3bin2weights = {0.169059, 0.182445, 0.242976, 0.339379, 0.459126, 0.586182, 0.708931, 0.82085, 0.924898, 1.03088, 1.14148, 1.24816, 1.32363, 1.32895, 1.24746, 1.06005, 0.868318, 0.767249, 0.771477, 0.835023, 0.913111, 0.971778, 0.987021};

        std::vector<double> q3bin3weights = {0.406167, 0.400722, 0.381415, 0.359252, 0.345346, 0.355835, 0.406526, 0.510034, 0.666337, 0.857511, 1.04733 , 1.20275 , 1.311   , 1.36494 , 1.34045 , 1.23871 , 1.09418 , 0.959903, 0.908177, 0.930722, 0.995452, 1.05769 , 1.07703 };

        std::vector<double> q3bin4weights = {0.465274 ,0.475479 ,0.521774 ,0.59605 ,0.685829 ,0.781784 ,0.876495 ,0.967061 ,1.05796 ,1.15474 ,1.25674 ,1.35469 ,1.43084 ,1.47119 ,1.47522 ,1.41 ,1.25184 ,1.07685 ,0.968015 ,0.941743 ,0.966135 ,1.00764 ,1.02239};

        std::vector<double> q3bin5weights = {0.549138, 0.562134, 0.624496, 0.72724, 0.859891,  1.00808, 1.15921, 1.30858, 1.45383, 1.5935, 1.7235, 1.83011, 1.88988, 1.88183, 1.80408, 1.63456, 1.38423, 1.11548, 0.902733, 0.778054, 0.732044, 0.731376, 0.738204};

        std::vector<double> tpibin = {.002, .006, .010, .014, .018, .022, .026, .030, .034, .038, .043, .049, .061, .075, .090, .125, .175, .225, .275, .325, .375, .450, .550};

    
        double weight2 = 1.0;
        std::vector<double> currentbins;// =  q3bin1weights;
        if (isnan(univ.GetTrueQ3())) return 1.0;
        double q3_mecAna = univ.GetTrueQ3(); //univ.Getq3();
        if (q3_mecAna < 0.40 ) currentbins = q3bin1weights;
        else if (q3_mecAna >= 0.40 || q3_mecAna < 0.60) currentbins = q3bin2weights;
        else if (q3_mecAna >= 0.60 || q3_mecAna < 0.80) currentbins = q3bin3weights;
        else if (q3_mecAna >= 0.80 || q3_mecAna < 1.00) currentbins = q3bin4weights;
        else if (q3_mecAna >= 1.00 || q3_mecAna < 1.20) currentbins = q3bin5weights;

        int nbins = currentbins.size();*/
        double weight2 = 1.0;
        //std::vector<double> tpiweights = {0.208893,0.252472,0.322854,0.386204,0.483906,0.625299,0.800117,0.982495,1.02912,0.931458,0.867728,0.884766,1.46193,1.16028,1.22921,1.40481,1.27895,1.47147,1.12997,1.24676,0.775835,0.749377,0.627021,0.353792,0.349399,0.146828,0.122889,0.29762,0.277732,1.48114};
        /*
        std::vector<double> tpiweights = {0.17186754,  0.22816274,  0.25754658,  0.35430382,  0.41819581,
        0.57069057,  0.74975401,  0.98005212,  1.04860269,  0.95733922,
        0.8749787 ,  0.92433546,  1.52590533,  1.02857955,  1.06844248,
        1.44728495,  1.25597756,  1.38530834,  1.1476707 ,  1.12358202,
        0.84248435,  0.9307836 ,  0.77521319,  0.33602477,  0.21990108,
        0.05296071, -0.04128534,  0.18793639,  0.26660532,  1.13225302}; // Reweights from numpy with bkg subtracted data range 
        */
        //Tpi reweights for mnvtunev4
        //The Following is a reweight for MnvTunev4 with background subtracted and tolerance of 0.06
        std::vector<double> tpiweights = {-0.00215655,  0.11053229,  0.0091617 ,  0.18465904,  0.32759373,
        				  0.65192503,  0.77486729,  0.94197336,  0.69163669,  0.46433796,
        				  0.87231944,  1.18933422,  0.95321934,  1.31251752,  0.79164138,
        				  1.31379807,  1.14339395,  1.19134192,  1.19375002,  0.99949744,
        				  0.60126032,  0.49300099,  0.45267546,  0.6169398 ,  1.07933653,
        				  0.75220218,  1.01279084,  0.84404324,  0.62274187,  0.64837269};         

        /*
        //The following is a reweight for MnvTunev4 with background subtracted and tolerance of 0.08
         std::vector<double> tpiweights = {0.1200114 , 0.18887331, 0.1870642 , 0.27444119, 0.34557614,
                                          0.46779323, 0.60092253, 0.72694922, 0.81382415, 0.71488692,
                                          0.90503891, 0.91575718, 1.12166593, 1.25086901, 0.82234499,
                                          1.30784105, 1.14822407, 1.19278088, 1.19592509, 0.98847615,
                                          0.62373223, 0.38251533, 0.46220911, 0.60804838, 1.09106715,
                                          0.78382882, 1.04735398, 0.8434568 , 0.65095178, 0.65611836}; 
        */
        std::vector<double> tpilowbins = {0.0, 4.0, 8.0, 12.0, 16.0, 20.0, 24.0, 28.0, 32.0, 36.0, 40.0, 46.0, 52.0, 60.0, 70.0, 80.0, 100.0, 125.0, 150.0, 175.0, 200.0, 225.0, 250.0, 275.0, 300.0, 350.0, 400.0, 500.0, 700.0, 1000.0};
        if (univ.GetTrueNPionsinEvent() == 0) return 1.0;
        double tpi = univ.GetTrueLowestTpiEvent();
        //std::cout << "Printing the q3 of the event " << q3_mecAna << std::endl;	
	//std::cout << "Printing the lowest Tpi in Event " << tpi << std::endl;
      	
        //double tpi = myevent.m_nmichels[0].pionKE/1000.;
        for (int i = 0; i< tpilowbins.size(); i++){
                if (i < tpilowbins.size() and tpi >= tpilowbins[i] and tpi < tpilowbins[i+1]){
		   weight2 = tpiweights[i];
 		   break;
		}
		else if (tpi >= 1000.){
	           weight2 = 1.0; //tpiweights[i];
       		   break; 
                }
        }
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
