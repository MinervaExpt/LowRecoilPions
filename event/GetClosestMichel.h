#include "PlotUtils/LowRecoilPionReco.h"

template <class UNIVERSE, class EVENT>
void setClosestMichel(const UNIVERSE& univ, EVENT& evt, const int michelgroup) //michelgroup is 0 for the selected Michels and 1 for Michels that are too far from their match points (AKA sideband Michels)
{
  //TODO: Put Mehreen's code here!
  if (michelgroup == 0) evt.m_nmichels = evt.m_nmichelspass;
      else if (michelgroup == 1) evt.m_nmichels = evt.m_sidebandpass;
      int nmichels = evt.m_nmichels.size();

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
      }
      for (int i = 0; i < nmichels; ++i)
      {
        double dist = evt.m_nmichels[i].Best3Ddist;
        if (evt.m_nmichels[i].OrderOfMichel == 1) {
           evt.m_bestdist = dist;
           evt.m_idx = i;

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
         }
         else {
           evt.m_idx = -1;
         }

      } // End of for loop over nmichels

}
