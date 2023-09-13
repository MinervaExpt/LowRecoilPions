#include "PlotUtils/Cut.h"

#include "event/CVUniverse.h"

template <class CVUniverse, class EVENT>

class NPiCut: public PlotUtils::Cut<CVUniverse, EVENT>

{
    public:
        NPiCut(const int npi): PlotUtils::Cut<CVUniverse, EVENT>("Atleast "+std::to_string(npi)+ " pion in Event"), 
        fnpi(npi)
        {

        }
    private:
        int fnpi; // n true pi in event
        bool checkCut(const CVUniverse& univ, EVENT&) const
        {
           const int npievents = univ.GetTrueNPionsinEvent();

            if (npievents >= fnpi) return true;
            else return false;
        }




};
