#include "PlotUtils/LowRecoilPionReco.h"
typedef LowRecoilPion::Michel<CVUniverse> Michel;

template <class UNIVERSE, class EVENT>
void setDistanceMichelSidebands(const UNIVERSE& univ, EVENT& evt, const double selectedDist, const double sidebandDist)
{
      std::vector<Michel> nmichelspass;
      int nside;
      int npass = 0;
      for (unsigned int i = 0; i < evt.m_allmichels.size(); i++)
      {

        bool upvtx = false;
        bool downvtx = false;
        bool upclus = false;
        bool downclus = false;  
	
	double upvtxXZ = evt.m_allmichels[i].up_to_vertex_XZ;
        double downvtxXZ = evt.m_allmichels[i].down_to_vertex_XZ;
        double upvtxUZ = evt.m_allmichels[i].up_to_vertex_UZ;
        double downvtxUZ = evt.m_allmichels[i].down_to_vertex_UZ;
        double upvtxVZ = evt.m_allmichels[i].up_to_vertex_VZ;
        double downvtxVZ = evt.m_allmichels[i].down_to_vertex_VZ;

        std::vector<double> up2Ddist = {upvtxXZ, upvtxUZ, upvtxVZ};
        std::vector<double> down2Ddist = {downvtxXZ, downvtxUZ, downvtxVZ};

        std::sort(up2Ddist.begin(), up2Ddist.end());
        std::sort(down2Ddist.begin(),down2Ddist.end());

	if (up2Ddist[0] < selectedDist && up2Ddist[1] < selectedDist)
        {
                npass++;
                continue;
        }
        if (down2Ddist[0] < selectedDist && down2Ddist[1] < selectedDist)
        {
                npass++;
                continue;
        }


        if (up2Ddist[0] > sidebandDist && up2Ddist[1] > sidebandDist) upvtx = true;
        if (down2Ddist[0] > sidebandDist && down2Ddist[1] > sidebandDist) downvtx = true;

        if (upvtx == true) evt.m_allmichels[i].passable_matchtype.at(0) = 1;
	else evt.m_allmichels[i].passable_matchtype.at(0) = -1;

	if (downvtx == true) evt.m_allmichels[i].passable_matchtype.at(1) = 2;
	else evt.m_allmichels[i].passable_matchtype.at(1) = -1;
	
	double upclusXZ = evt.m_allmichels[i].up_to_clus_XZ;
        double upclusUZ = evt.m_allmichels[i].up_to_clus_UZ;
        double upclusVZ = evt.m_allmichels[i].up_to_clus_VZ;
        double downclusXZ = evt.m_allmichels[i].down_to_clus_XZ;
        double downclusUZ = evt.m_allmichels[i].down_to_clus_UZ;
        double downclusVZ = evt.m_allmichels[i].down_to_clus_VZ;

        std::vector<double> upclusdist = {upclusXZ,upclusUZ,upclusVZ};
        std::vector<double> downclusdist = {downclusXZ, downclusUZ,downclusVZ};

        std::sort(upclusdist.begin(), upclusdist.end());
        std::sort(downclusdist.begin(),downclusdist.end());

        if (upclusdist[0] < selectedDist && upclusdist[1] < selectedDist)
        {
                npass++;
                continue;
        }
        if (downclusdist[0] < selectedDist && downclusdist[1] < selectedDist)
        {
                npass++;
                continue;
        }


        if (upclusdist[0] > sidebandDist && upclusdist[1] > sidebandDist) upclus = true;
        if (downclusdist[0] > sidebandDist && downclusdist[1] > sidebandDist) downclus = true;


        if (upclus == true) evt.m_allmichels[i].passable_matchtype.at(2) = 3;
        else evt.m_allmichels[i].passable_matchtype.at(2) = -1;
        if (downclus == true)  evt.m_allmichels[i].passable_matchtype.at(3) = 4;
        else evt.m_allmichels[i].passable_matchtype.at(3) = -1;

	std::vector<double> distances3D;

        if (evt.m_allmichels[i].passable_matchtype[0] == 1) distances3D.push_back(evt.m_allmichels[i].up_to_vertex_dist3D);
        if (evt.m_allmichels[i].passable_matchtype[1] == 2) distances3D.push_back(evt.m_allmichels[i].down_to_vertex_dist3D);
        if (evt.m_allmichels[i].passable_matchtype[2] == 3) distances3D.push_back(evt.m_allmichels[i].up_clus_michel_dist3D);
        if (evt.m_allmichels[i].passable_matchtype[3] == 4) distances3D.push_back(evt.m_allmichels[i].down_clus_michel_dist3D);
        if (distances3D.empty()) distances3D = {9999.,9999.,9999.,9999.};
        if (distances3D[0] == 9999.) continue;

	std::sort(distances3D.begin(), distances3D.end());


        if (distances3D[0] == evt.m_allmichels[i].up_to_vertex_dist3D){
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].up_to_vertex_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].up_to_vertex_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].up_to_vertex_VZ;
         evt.m_allmichels[i].BestMatch = 1;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].up_to_vertex_dist3D;
	 nside++;
        }
	else if (distances3D[0] == evt.m_allmichels[i].down_to_vertex_dist3D){
         evt.m_allmichels[i].BestMatch = 2;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].down_to_vertex_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].down_to_vertex_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].down_to_vertex_VZ;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].down_to_vertex_dist3D;
	 nside++;
	}
	else if (distances3D[0] == evt.m_allmichels[i].up_clus_michel_dist3D){
         evt.m_allmichels[i].BestMatch = 3;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].up_to_clus_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].up_to_clus_VZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].up_to_clus_UZ;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].up_clus_michvtx_dist3D;
	 nside++;
	}
	else if (distances3D[0] == evt.m_allmichels[i].down_clus_michel_dist3D){
         evt.m_allmichels[i].BestMatch = 4;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].down_clus_michvtx_dist3D;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].down_to_clus_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].down_to_clus_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].down_to_clus_VZ;
	 nside++;
	}
	else{
                evt.m_allmichels[i].BestMatch = 0;
                evt.m_allmichels[i].Best3Ddist = 9999.;
                evt.m_allmichels[i].best_XZ = 9999.;
                evt.m_allmichels[i].best_UZ = 9999.;
                evt.m_allmichels[i].best_VZ = 9999.;
                continue;
        }
	nmichelspass.push_back(evt.m_allmichels[i]);
        evt.m_sidebandpass.push_back(evt.m_allmichels[i]);
      }

      if (npass == 0 && !nmichelspass.empty() && nside > 0){ 
	evt.sideband = 1;
 	evt.m_nmichels.clear();
	evt.m_nmichels = nmichelspass;
      }

}
