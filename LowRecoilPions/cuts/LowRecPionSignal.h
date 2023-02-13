//File: LowRecPionSignal.h
//Brief: Signal definition constraints for Mehreen's Low Recoil Pion  analysis.
//Author: Mehreen Sultana 

namespace truth
{

  template<class UNIVERSE>
  class HasPion: public PlotUtils::SignalConstraint<UNIVERSE>
  {
	public: 
	   HasPion():PlotUtils::SignalConstraint<UNIVERSE>("HasPion")
	   {	
	   }
	private:
	   bool checkConstraint(const UNIVERSE& univ) const override
	   {
		int npi = univ.GetTrueNPionsinEvent(); //univ.GetTrueNPiPlus();  //univ.GetTrueNPionsinEvent();
      		if (npi > 0) return true;
      		else return false;	
	
	   }

  };
   
  template <class UNIVERSE>
  class pTRangeLimit: public PlotUtils::SignalConstraint<UNIVERSE>
  {
    public:
      pTRangeLimit(const double pTMin, const double pTMax): PlotUtils::SignalConstraint<UNIVERSE>(std::to_string(pTMin) + "True pT < " + std::to_string(pTMax) + " GeV"),
      fpTMin(pTMin),
      fpTMax(pTMax)
      {
      }

    private:
      double fpTMax; //Maximum pT allowed in GeV/c
      double fpTMin; // Min pT allowed in GeV/c
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double truepT = univ.GetMuonPTTrue();
        return (truepT > fpTMin && truepT < fpTMax);
      }
  };


  template <class UNIVERSE>
  class pMuCut: public PlotUtils::SignalConstraint<UNIVERSE>
  {
    public:
      pMuCut(const double pMin): PlotUtils::SignalConstraint<UNIVERSE>("True pmu > " + std::to_string(pMin) + " GeV"),
      fpMin(pMin)
      {
      }

    private:
      double fpMin; // Min pmu allowed in GeV/c
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double truep = univ.GetPmuTrue();
        return (truep > fpMin and truep < 20.0);
      }
  };


  template <class UNIVERSE>
  class TrueWexpCut: public PlotUtils::SignalConstraint<UNIVERSE>
  {
   public:
      TrueWexpCut(): PlotUtils::SignalConstraint<UNIVERSE>("Wexp < 1.7")
      {
      }

   private:
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double wexp = univ.GetTrueWexp();
        return wexp < 1.7;
      }

  };

  template <class UNIVERSE>
  class EavailCut: public PlotUtils::SignalConstraint<UNIVERSE>
  {
   public:
      EavailCut(): PlotUtils::SignalConstraint<UNIVERSE>("Available Energy < 2.0 GeV")
      {
      }

   private:
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double trueE = univ.GetTrueEAvail()/1000.;
        if (trueE < 1.0) return true;
	else return false;
      }

  };

  template <class UNIVERSE>
  class TpiCut: public PlotUtils::SignalConstraint<UNIVERSE>
  {
   public:
      TpiCut(): PlotUtils::SignalConstraint<UNIVERSE>("Pion Kinetic Energy < .350 GeV")
      {
      }

   private:
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double trueE = univ.GetTrueLowestTpiEvent();
        if (trueE < 350.) return true;
        else return false;
      }

  };



} 
