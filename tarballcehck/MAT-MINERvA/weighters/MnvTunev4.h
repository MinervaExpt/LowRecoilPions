//File:MnvTunev4.h Implemeting MinervaTune v4 for Mehreen's Analysis
//Author: Mehreen Sultana msultana@ur.rochester.edu

#ifndef MNVTUNEV4_REWEIGHTER_H
#define MNVTUNEV4_REWEIGHTER_H


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
  class MnvTunev4:public Reweighter<UNIVERSE, EVENT>
  {
    public:
      MnvTunev4() = default;
      virtual ~MnvTunev4() = default;

      virtual double GetWeight(const UNIVERSE& univ, const EVENT& myevent /*event*/) const override{


      };
    virtual std::string GetName() const {return "MnvTunev4"; }

    virtual bool DependsReco() const {return false;}

  };


}

#endif // MNVTUNEV4_REWEIGHTER_H
