//PlotUtils includes
#include "PlotUtils/Cut.h"

////Package includes
#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class PzmuCut: public PlotUtils::Cut<CVUniverse, EVENT>

{
  public:
    PzmuCut(const double pmax): PlotUtils::Cut<CVUniverse, EVENT>("pzmu reco < " + std::to_string(pmax)), 
    fpMax(pmax)
    {
    }
  private:
    double fpMax; //minimum pT allowed in GeV;
    bool checkCut(const CVUniverse& univ, EVENT&) const
    {
      //const double preco = univ.GetMuonP();
      const double pz = univ.GetMuonPz();
      return (pz >  1.5 and pz < fpMax); 
    }

};
