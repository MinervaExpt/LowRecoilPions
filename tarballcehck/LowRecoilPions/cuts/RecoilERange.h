//PlotUtils includes
#include "PlotUtils/Cut.h"

////Package includes
#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class RecoilERange: public PlotUtils::Cut<CVUniverse, EVENT>

{
  public:
    RecoilERange(const double min,const double max): PlotUtils::Cut<CVUniverse, EVENT>(std::to_string(min)+"RecoilE reco < " + std::to_string(max)), 
    fmin(min),
    fmax(max)
    {
    }
  private:
    double fmin; //minimum pT allowed in GeV;
    double fmax; //maximum pT allowed in GeV;
    bool checkCut(const CVUniverse& univ, EVENT&) const
    {
      
      
      const double reco = univ.NewEavail()/1000.0; //MeV->GeV
      //if (odE > 100.) return false;
      return (reco > fmin) && (reco < fmax); 
    }

};
