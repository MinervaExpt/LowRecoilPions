//Cutter written to select Michels 
//PlotUtils includes
#include "PlotUtils/Cut.h"

template <class UNIVERSE, class EVENT>
class hasMichel: public PlotUtils::Cut<UNIVERSE, EVENT>
{
 public:
    hasMichel(): PlotUtils::Cut<UNIVERSE, EVENT>("Event Has Michel ")
    {
    }

  private:
    using Michel = typename std::remove_reference<decltype(std::declval<EVENT>().m_nmichels.front())>::type;

    bool checkCut(const UNIVERSE& univ, EVENT& evt) const
    {
      //if (univ.ShortName() != "cv") return true; //evt.m_allmichels.clear();
      //int nclusters = univ.GetNClusters(); //univ.GetNonMuonClusters(); //univ.GetNClusters();
      //if (nclusters > 500.) return false;
      int nmichels = univ.GetNMichels();
      if (nmichels < 1) return bool(false);
      int nfittedmich = univ.GetFittedMichelsOnly();
      if (nfittedmich < 1) return false;
      int nclusters = univ.GetNClusters(); 
      if (univ.ShortName() != "cv" ){
	 if (!evt.m_allmichels.empty()) return bool(true);
         //else return bool(true); 
      }
      //if (nclusters > 500.) return false;
  
      //std::cout << "Number of Clusters in Event are " << nclusters << std::endl; 
      for (int i = 0; i < nmichels; ++i)
      {
       
        Michel current_michel(univ,i);
        
        //if (univ.ShortName() == "cv") Michel current_michel(univ, i);
        //else {
	//	if (!evt.m_allmichels.empty()) auto current_michel = evt.m_allmichels[i];}


        //Michel* current_michel= new Michel(univ, i);
        if (current_michel.is_fitted != 1) continue;
 	if (abs(current_michel.vtx_michel_timediff) < 0.400) continue; // < 0.400 is to reject dead michels that happen during dead time. >-0.250 is to see what matches look like for michels that happen before neutrino event. 
       

	double z1 = current_michel.m_z1;
	double z2 = current_michel.m_z2;
	//Michel is in Tracker and ECAL	
	if (z1 < 5980. || z2 < 5980.) continue; 
	else if (z1 > 9038. || z2 > 9038.) continue;
        evt.m_allmichels.push_back(current_michel);       
                   	
        
       }
        //std::cout << "Has Michel Cut Done looping over Michels " << std::endl;
        //Filling Event Level Info needed for 2D selection 
        //double lowtpiinevent = univ.GetTrueTpi();
        //evt.lowTpi = lowtpiinevent;     
        evt.nclusters = nclusters;
        evt.pT_reco = univ.GetMuonPT();
	evt.q3_reco = univ.Getq3();
	evt.eavail_reco = univ.NewEavail();
        if (evt.m_allmichels.empty()) return bool(false);
        else return bool(true);
	//return !evt.m_allmichels.empty();
    }

};
