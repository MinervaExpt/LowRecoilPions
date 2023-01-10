#ifndef STUDY2D_H
#define STUDY2D_H

//ROOT includes
#include "TDirectory.h"
#include "PlotUtils/HistFolio.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Model.h"
class MichelEvent;
class CVUniverse;
class Study2D
{
  public:
    Study2D() {} //TODO: Any base constructor needed?

    void Selected(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt)
    {
      for(const auto univ: univs)
      {
         const double weight = model.GetWeight(*univ, evt);
         fillSelected(*univ, evt, weight);
      }
    }
   
    void Selected(const CVUniverse& univ, const MichelEvent& evt)
    {
         const double weight = 1.; //Only ever do this with Data. Overloading this function to accept filling data loop
         fillSelected(univ, evt, weight);
    }
    
    
    void SelectedSignal(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) //const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      fillSelectedSignal(univs, model, evt);
    }
    //Eventually you need to change this to match Selected signal
    void TruthSignal(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) //const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      fillTruthSignal(univs, model, evt);
    }

    void BackgroundSelected(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) //const CVUniverse& univ, const MichelEvent& evt, const double weight)
    {
      fillBackground(univs, model, evt);
    }


    //Find Andrew if you need to know how to overload functions for drawing.
    //Only need this when you write a new Study.
    virtual void SaveOrDraw(TDirectory& outDir) = 0;
    
  private:
    using Hist = PlotUtils::HistWrapper<CVUniverse>;

    virtual void fillSelected(const CVUniverse& univ, const MichelEvent& evt, const double weight) = 0;
    virtual void fillSelectedSignal(const std::vector<CVUniverse*>& univ, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) = 0;
    virtual void fillTruthSignal(const std::vector<CVUniverse*>& univ, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) = 0;
    virtual void fillBackground(const std::vector<CVUniverse*>& univ, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) = 0;
    virtual void fillHistForAllUniverses(const std::vector<CVUniverse*>& univs, const Model<CVUniverse, MichelEvent>& model, const MichelEvent& evt) = 0; 
};

#endif //STUDY2D_H
