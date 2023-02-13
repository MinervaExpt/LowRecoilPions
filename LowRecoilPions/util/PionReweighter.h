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
        /*
        //The Following is a reweight for MnvTunev4 with background subtracted and tolerance of 0.06
        std::vector<double> tpiweights = {-0.00215655,  0.11053229,  0.0091617 ,  0.18465904,  0.32759373,
        				  0.65192503,  0.77486729,  0.94197336,  0.69163669,  0.46433796,
        				  0.87231944,  1.18933422,  0.95321934,  1.31251752,  0.79164138,
        				  1.31379807,  1.14339395,  1.19134192,  1.19375002,  0.99949744,
        				  0.60126032,  0.49300099,  0.45267546,  0.6169398 ,  1.07933653,
        				  0.75220218,  1.01279084,  0.84404324,  0.62274187,  0.64837269};         

        */
        /*
        //The following is a reweight for MnvTunev4 with background subtracted and tolerance of 0.08
         std::vector<double> tpiweights = {0.1200114 , 0.18887331, 0.1870642 , 0.27444119, 0.34557614,
                                          0.46779323, 0.60092253, 0.72694922, 0.81382415, 0.71488692,
                                          0.90503891, 0.91575718, 1.12166593, 1.25086901, 0.82234499,
                                          1.30784105, 1.14822407, 1.19278088, 1.19592509, 0.98847615,
                                          0.62373223, 0.38251533, 0.46220911, 0.60804838, 1.09106715,
                                          0.78382882, 1.04735398, 0.8434568 , 0.65095178, 0.65611836}; 
        */
        //The following is reweight for MnvTunev431 with background subtracted and tolerance of 0.06 with full ME1A statistics
        /*std::vector<double>tpiweights = {0.1902855,  0.2954966,  0.3703688,  0.3903857,  0.473193 ,
        				0.5182093,  0.6041358,  0.6818155,  0.7368678,  0.7260622,
        				1.0200225,  0.9287505,  1.0881722,  1.2070503,  0.9951518,
        				1.3066559,  1.1821288,  1.1417606,  0.9864009,  1.1229612,
        				0.999276 ,  0.8574656,  0.7296579,  0.6093314,  0.8987519,
        				0.5927742,  0.7078378,  0.7067878,  0.5828136, -0.0004369};
        */

        //The following is a reweight for MnvTunev431 with fixes to LowQ2 and COH pion implementation. Range cut < 1000mm. Signal Tpi cut < 350 MeV. 
        /*std::vector<double>tpiweights = {0.1727731,  0.2648498,  0.3024206,  0.3654409,  0.4399542,
        0.4648256,  0.5480141,  0.6236805,  0.7197562,  0.6610829,
        0.9579538,  0.9148854,  1.1541735,  1.1104433,  0.870103 ,
        1.2277846,  1.0469355,  1.0605834,  0.9115713,  0.7011701,
        0.363619 ,  0.3702355,  0.3012748,  0.2264294,  0.3344879,
        0.2065797,  0.2612956,  0.3321888,  0.2875124, -0.0000552};
        */

	// The following is a reweight for Mnvtunev4 bug fix (coh applied twice) and with low q2 turned on. Tolerance of 0.07
	std::vector<double> tpiweights = {0.1623999,  0.2532403,  0.2868589,  0.3483384,  0.4167944,
        				0.458954 ,  0.5535843,  0.6406514,  0.7842327,  0.7314507,
        				1.0782849,  0.9666569,  1.1400208,  1.1620875,  0.9457339,
        				1.2891274,  1.1025859,  1.2414063,  0.9547479,  0.9850241,
        				0.9252908,  0.8733355,  0.7545873,  0.5580611,  0.8883108,
        				0.5754092,  0.7220107,  0.7224896,  0.5922406, -0.001366 };
        
        /*
        //The following is a reweight for MnvTunev4 without LowQ2 and tolerance of 0.07
        std::vector<double> tpiweights = {0.1740693,  0.2836968,  0.3152313,  0.3890271,  0.4754425,
        0.5146361,  0.6296602,  0.7122324,  0.8777017,  0.8275259,
        1.2257956,  1.0909433,  1.2560052,  1.2901227,  1.0463555,
        1.4124964,  1.1655215,  1.349213 ,  1.0538344,  1.1087648,
        1.1036029,  0.9979603,  0.8691629,  0.644745 ,  1.0185651,
        0.6825277,  0.8367527,  0.8308716,  0.697864 , -0.0013543};
	*/
 
        std::vector<double> tpilowbins = {1., 4., 8., 12., 16., 20., 24., 28., 32., 36., 40., 46., 52.,60., 70., 80., 100., 125.,150., 175., 200., 225., 250., 275., 300., 350., 400., 500., 700.,1000.}; 
        if (univ.GetTrueNPionsinEvent() == 0) return 1.0;
        double tpi = univ.GetTrueLowestTpiEvent();
        //std::cout << "Printing the q3 of the event " << q3_mecAna << std::endl;	
	//std::cout << "Printing the lowest Tpi in Event " << tpi << std::endl;
        double angle = cos(univ.GetTrueAngleLowTpi());	
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
