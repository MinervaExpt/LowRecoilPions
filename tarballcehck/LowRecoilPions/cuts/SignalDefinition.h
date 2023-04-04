//This is what we'll do for reco
/*template <class UNIVERSE>
using q3Signal = PlotUtils::Maximum<UNIVERSE, double, &UNIVERSE::GetTrueq3>;*/ //TODO: Except I didn't actually write this for SignalConstraints :(

//PlotUtils includes
#include "PlotUtils/Cut.h"

//Package includes
#include "event/CVUniverse.h"

template <class UNIVERSE>
class Q3Limit: public PlotUtils::SignalConstraint<UNIVERSE>
{
  public:
    Q3Limit(const double q3Min, const double q3Max): PlotUtils::SignalConstraint<UNIVERSE>(std::to_string(q3Min) + "True q3 < " + std::to_string(q3Max) + " GeV"),
  fQ3Min(q3Min),
  fQ3Max(q3Max)
    {
    }

  private:
    double fQ3Max; //Maximum q3 allowed in GeV/c
    double fQ3Min; // Min q3 allowed in GeV/c
    bool checkConstraint(const UNIVERSE& univ) const //override
    {
      double trueq3 = univ.Getq3True();
      return trueq3 > fQ3Min && trueq3 <= fQ3Max;
    }
};
template <class UNIVERSE>
class hasTruePion: public PlotUtils::SignalConstraint<UNIVERSE>
{
  public:
    hasTruePion(): PlotUtils::SignalConstraint<UNIVERSE>("has at least 1 True Pion")
    {
    }

  private:
    bool checkConstraint(const UNIVERSE& univ) const //override
    {

      int npi = univ.GetTrueNPionsinEvent();

      if (npi > 0) return true;
      else return false;
    }
};

/*
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
class TrueEavailCut: public PlotUtils::SignalConstraint<UNIVERSE>
{  
   public:
      TrueEavailCut(): PlotUtils::SignalConstraint<UNIVERSE>("Available Energy < 2.0 GeV")
      {
      }
   
   private:
      bool checkConstraint(const UNIVERSE& univ) const //override
      {
        double trueE = univ.GetTrueEAvail();
        return trueE < 2.0;
      }

};
*/

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
      double truepT = univ.GetMuonPTTrue()/1000.;
      return truepT > fpTMin && truepT < fpTMax;
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
      
      return (truep > fpMin && truep < 20.0);
    }
};

template <class UNIVERSE>
class q0RangeLimit: public PlotUtils::SignalConstraint<UNIVERSE>
{
  public:
    q0RangeLimit(const double min, const double max): PlotUtils::SignalConstraint<UNIVERSE>(std::to_string(min) + "True q0 < " + std::to_string(max) + " GeV"),
    fMin(min),
    fMax(max)
    {
    }

  private:
    double fMax; //Maximum q0 allowed in GeV/c
    double fMin; //Min q0 allowed in GeV/c
    bool checkConstraint(const UNIVERSE& univ) const //override
    {
      double trueq0 = univ.GetTrueQ0GeV();
      return trueq0 > fMin && trueq0 < fMax;
    }
};
