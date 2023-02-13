//#include "CVUniverse.h"
//#include "Michel.h"
//Cutter written to select Michels 
//PlotUtils includes
#include "PlotUtils/Cut.h"

template <class UNIVERSE, class EVENT>
class GetClosestMichel: public PlotUtils::Cut<UNIVERSE, EVENT>
{
 public:
    GetClosestMichel(const int michelgroup): PlotUtils::Cut<UNIVERSE, EVENT>("Getting Closest Michel for Michel Group " + std::to_string(michelgroup))
    {
    }
    //michel group means getting Closest Michel in selection (evt.m_nmichelspass) or sideband group (m_sidebandpass)

  private:
    using Michel = typename std::remove_reference<decltype(std::declval<EVENT>().m_nmichels.front())>::type;
    int michelgroup;
    bool checkCut(const UNIVERSE& univ, EVENT& evt) const
    {
    
      // evt.m_nmichels.clear();
      //if (michelgroup == 0) evt.m_nmichels = evt.m_nmichelspass;
      //else if (michelgroup == 1) evt.m_nmichels = evt.m_sidebandpass;  
      int noverlay = 0.0;
      int nmichels = evt.m_nmichels.size();
      std::vector<Michel> closestMichel;
      if (nmichels == 0) return false;     
      evt.m_bestdist = 9999.; // setting some default value for best distance
      std::vector<double> allmichel3Ddist;
      for (int i = 0; i < nmichels; ++i)
      {
        
	double dist = evt.m_nmichels[i].Best3Ddist; //getting the minimum pion range (vertex to Michel/Clus distance) 
        allmichel3Ddist.push_back(dist);
	
      } // Get all the distances for the michels that pass the 2D dist cut 
      
      std::sort(allmichel3Ddist.begin(), allmichel3Ddist.end());

      for (int i = 0; i < nmichels; ++i)
      {
        double dist = evt.m_nmichels[i].Best3Ddist; 
	int order = i+1;
        if (dist == allmichel3Ddist[i]) evt.m_nmichels[i].OrderOfMichel = order;
         
        //std::cout << "Printing Order of Michels " << order << " , dist: "<< dist << std::endl;
      }
      //std::cout << "Printing Closest Distance in event : " << allmichel3Ddist[0] << std::endl;
      for (int i = 0; i < nmichels; ++i)
      {
        double dist = evt.m_nmichels[i].Best3Ddist; 
	if (evt.m_nmichels[i].OrderOfMichel == 1) {
           //closestMichel.push_back(evt.m_nmichels[i]);
	   if (dist > 2400.) return false; // mimicing aaron's cuts to remove high tpi events > 350 MeV. 1000 - 1200 mm in range is approximately the bin with the most events in that tpi bins. . 
	   
           evt.m_bestdist = dist;
	              
	   evt.m_idx = i;
           if (evt.m_nmichels[i].overlay_fraction > 0.5) evt.ClosestMichelsIsOverlay = 1;      
           evt.m_best_XZ = evt.m_nmichels[i].best_XZ;
           evt.m_best_UZ = evt.m_nmichels[i].best_UZ;
           evt.m_best_VZ = evt.m_nmichels[i].best_VZ;
           evt.m_matchtype = evt.m_nmichels[i].BestMatch;
           int bmatch = evt.m_nmichels[i].BestMatch;
           if(bmatch == 1 || bmatch == 3){
           evt.best_x = evt.m_nmichels[i].m_x1;
           evt.best_y = evt.m_nmichels[i].m_y1;
           evt.best_z = evt.m_nmichels[i].m_z1;
           }
           else if(bmatch == 2 || bmatch == 4){
           evt.best_x = evt.m_nmichels[i].m_x2;
           evt.best_y = evt.m_nmichels[i].m_y2;
           evt.best_z = evt.m_nmichels[i].m_z2;
           }
           evt.b_truex = evt.m_nmichels[i].true_initialx;
           evt.b_truey = evt.m_nmichels[i].true_initialy;
           evt.b_truez = evt.m_nmichels[i].true_initialz;
           closestMichel.push_back(evt.m_nmichels[i]);
         }	
	 else {
    	  // if (evt.m_nmichels[i].OrderOfMichel == 2) closestMichel.push_back(evt.m_nmichels[i]);
	   evt.m_idx = -1;
	 }
        
       }
       if (closestMichel.empty()) return false;
       double lowtpiinevent = closestMichel[0].pionKE;
       //if (closestMichel[0].Best3Ddist > 1200.) continue; //Mimicking Aaron's cuts to remove high Tpi events > 350 MeV. 
       
       //std::cout << "Closest Michel Pion KE is " << lowtpiinevent << std::endl;
       //if (closestMichel[0].BestMatch == 1 || closestMichel[0].BestMatch == 2) univ.PrintTrueArachneLink();
       evt.lowTpi = lowtpiinevent;
       evt.pT_reco = univ.GetMuonPT();
       evt.q3_reco = univ.Getq3();
       evt.eavail_reco = univ.NewEavail();  
       evt.m_nmichels.clear();
       evt.m_nmichels = closestMichel;
       if (univ.GetMuonPT() < .20 and univ.NewEavail() < 50. and !evt.m_nmichels.empty())
       {
	     double vtx_t = univ.GetVertex().T()/pow(10, 3); //mus
	     
	    // univ.PrintTrueArachneLink();
	   //  univ.PrintDataArachneLink();
	   //  std::cout << "Available Energy: " << univ.NewEavail() << " Muon pT Reco: " << univ.GetMuonPT()  << " Primary Vtx time: " << vtx_t << " Best Michel at time: " << evt.m_nmichels[0].time << " range: " << evt.m_nmichels[0].Best3Ddist << " energy: " << evt.m_nmichels[0].energy << " Matched to : " <<  evt.m_nmichels[0].BestMatch << std::endl;
          
       }
       //if (closestMichel[0].Best3Ddist > 2500.) return false;
       return true;       
       //return !evt.m_nmichels.empty();
    };

};
