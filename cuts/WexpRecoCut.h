//PlotUtils includes
#include "PlotUtils/Cut.h"

////Package includes
#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class WexpRecoCut: public PlotUtils::Cut<CVUniverse, EVENT>

{
  public:
    WexpRecoCut(const double min,const double max): PlotUtils::Cut<CVUniverse, EVENT>(std::to_string(min)+"RecoilE reco < " + std::to_string(max)), 
    fmin(min),
    fmax(max)
    {
    }
  private:
    double fmin; //minimum pT allowed in GeV;
    double fmax; //maximum pT allowed in GeV;
    bool checkCut(const CVUniverse& univ, EVENT&) const
    {
      const double wexp = univ.GetWExp(); //GeV
      //if (wexp > 1.7) return false;
      return (wexp > fmin) && (wexp <= fmax); 
    }

};
