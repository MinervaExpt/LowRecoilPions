//PlotUtils includes
#include "PlotUtils/Cut.h"

////Package includes
#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class PTRangeReco: public PlotUtils::Cut<CVUniverse, EVENT>

{
  public:
    PTRangeReco(const double pTmin,const double pTmax): PlotUtils::Cut<CVUniverse, EVENT>(std::to_string(pTmin)+"pT reco < " + std::to_string(pTmax)), 
    fpTmin(pTmin),
    fpTmax(pTmax)
    {
    }
  private:
    double fpTmin; //minimum pT allowed in GeV;
    double fpTmax; //maximum pT allowed in GeV;
    bool checkCut(const CVUniverse& univ, EVENT&) const
    {
      const double pTreco = univ.GetMuonPT();
      return (pTreco > fpTmin) && (pTreco < fpTmax); 
    }

};
