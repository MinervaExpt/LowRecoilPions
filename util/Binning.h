#ifndef BINNING_H
#define BINNING_H

#include <iostream>
#include <vector>


class Binning{
  public:
      //! Constructor
      Binning(){};
      //! Destructor
      ~Binning(){};

      std::vector<double> biasbins;
      std::vector<double> absresbins;
      

};

/*
      const double diffBinWidth = 25; //MeV
      for(int whichBin = 0; whichBin < 600 + 1; ++whichBin) {
        double max = diffBinWidth*whichBin;
        if (max >= 3000.) break;
        absresbins.push_back(-3000.+diffBinWidth * whichBin);
      }
      const double biaswidth = 0.2;
       for(int whichBin = 0; whichBin < 50 + 1; ++whichBin) {
        double max = biaswidth*whichBin;
        if (max >= 5.) break;
        biasbins.push_back(-3000.+diffBinWidth * whichBin);
      }
*/





#endif
