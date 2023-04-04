//PlotUtils includes
#include "PlotUtils/Cut.h"

////Package includes
#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class PmuCut: public PlotUtils::Cut<CVUniverse, EVENT>

{
  public:
    PmuCut(const double pmin): PlotUtils::Cut<CVUniverse, EVENT>("pmu reco > " + std::to_string(pmin)), 
    fpMin(pmin)
    {
    }
  private:
    double fpMin; //minimum pT allowed in GeV;
    bool checkCut(const CVUniverse& univ, EVENT&) const
    {
      const double preco = univ.GetMuonP();
      const double pz = univ.GetMuonPz();
      return (preco > fpMin && pz > fpMin); 
    }

};
