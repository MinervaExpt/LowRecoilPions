#include "event/MichelEvent.h"

template <class UNIVERSE, class EVENT>
void setDistanceMichelSelection(const UNIVERSE& univ, EVENT& evt, const double m_maxDistance)
{
      std::vector<Michel> nmichelspass;
      int npass = 0;
      for (unsigned int i = 0; i < evt.m_allmichels.size(); i++)
      {

	double upvtxXZ = evt.m_allmichels[i].up_to_vertex_XZ;
        double downvtxXZ = evt.m_allmichels[i].down_to_vertex_XZ;
        double upvtxUZ = evt.m_allmichels[i].up_to_vertex_UZ;
        double downvtxUZ = evt.m_allmichels[i].down_to_vertex_UZ;
        double upvtxVZ = evt.m_allmichels[i].up_to_vertex_VZ;
        double downvtxVZ = evt.m_allmichels[i].down_to_vertex_VZ;


        std::vector<double> upvtx = {upvtxXZ, upvtxUZ, upvtxVZ};
        std::vector<double> downvtx = {upvtxXZ, upvtxUZ, upvtxVZ};

        std::sort(upvtx.begin(), upvtx.end());
        std::sort(downvtx.begin(), downvtx.end());

	if (upvtxXZ < m_maxDistance && (upvtxUZ < m_maxDistance || upvtxVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(0) = 1;
        else if (upvtxUZ < m_maxDistance && (upvtxXZ < m_maxDistance || upvtxVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(0) = 1;
        else if (upvtxVZ < m_maxDistance && (upvtxXZ < m_maxDistance || upvtxUZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(0) = 1;
        else evt.m_allmichels[i].passable_matchtype.at(0) = -1;

        if (downvtxXZ < m_maxDistance && (downvtxUZ < m_maxDistance || downvtxVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(1) = 2;
        else if (downvtxUZ < m_maxDistance && (downvtxXZ < m_maxDistance || downvtxVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(1) = 2;
        else if (downvtxVZ < m_maxDistance && (downvtxXZ < m_maxDistance || downvtxUZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(1) = 2;
        else evt.m_allmichels[i].passable_matchtype.at(1) = -1;

        double upclusXZ = evt.m_allmichels[i].up_to_clus_XZ;
        double upclusUZ = evt.m_allmichels[i].up_to_clus_UZ;
        double upclusVZ = evt.m_allmichels[i].up_to_clus_VZ;
        double downclusXZ = evt.m_allmichels[i].down_to_clus_XZ;
        double downclusUZ = evt.m_allmichels[i].down_to_clus_UZ;
        double downclusVZ = evt.m_allmichels[i].down_to_clus_VZ;

        std::vector<double> upclus = {upclusXZ, upclusUZ, upclusVZ};
        std::vector<double> downclus = {downclusXZ, downclusUZ,downclusVZ};

        std::sort(upclus.begin(),upclus.end());
        std::sort(downclus.begin(),downclus.end());

	if (upclusXZ < m_maxDistance && (upclusUZ < m_maxDistance || upclusVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(2) = 3;
        else if (upclusUZ < m_maxDistance && (upclusXZ < m_maxDistance || upclusVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(2) = 3;
        else if (upclusVZ < m_maxDistance && (upclusXZ < m_maxDistance || upclusUZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(2) = 3;
        else evt.m_allmichels[i].passable_matchtype.at(2) = -1;

        if (downclusXZ < m_maxDistance && (downclusUZ < m_maxDistance || downclusVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(3) = 4;
        else if (downclusUZ < m_maxDistance && (downclusXZ < m_maxDistance || downclusVZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(3) = 4;
        else if (downclusVZ < m_maxDistance && (downclusXZ < m_maxDistance || downclusUZ < m_maxDistance)) evt.m_allmichels[i].passable_matchtype.at(3) = 4;
        else evt.m_allmichels[i].passable_matchtype.at(3) = -1;

        if (evt.m_allmichels[i].passable_matchtype.at(1) == -1 && evt.m_allmichels[i].passable_matchtype.at(2) == -1 && evt.m_allmichels[i].passable_matchtype.at(3) == -1 && evt.m_allmichels[i].passable_matchtype.at(0) == -1) continue;

        std::vector<double> distances3D;

	if (evt.m_allmichels[i].passable_matchtype[0] == 1) distances3D.push_back(evt.m_allmichels[i].up_to_vertex_dist3D); //Distance between michel to vertex
	if (evt.m_allmichels[i].passable_matchtype[1] == 2) distances3D.push_back(evt.m_allmichels[i].down_to_vertex_dist3D); //distancebetween michel to vertex 
	if (evt.m_allmichels[i].passable_matchtype[2] == 3) distances3D.push_back(evt.m_allmichels[i].up_clus_michel_dist3D); //distnace between michel to cluster
	if (evt.m_allmichels[i].passable_matchtype[3] == 4) distances3D.push_back(evt.m_allmichels[i].down_clus_michel_dist3D); // distance between michel to cluster
	if (distances3D.empty()) distances3D = {9999.,9999.,9999.,9999.};

	std::sort(distances3D.begin(), distances3D.end());

        if (distances3D[0] == 9999.) continue;
        else if (distances3D[0] == evt.m_allmichels[i].up_to_vertex_dist3D){
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].up_to_vertex_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].up_to_vertex_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].up_to_vertex_VZ;
         evt.m_allmichels[i].BestMatch = 1;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].up_to_vertex_dist3D;
	 npass++;
	}
	else if (distances3D[0] == evt.m_allmichels[i].down_to_vertex_dist3D){
         evt.m_allmichels[i].BestMatch = 2;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].down_to_vertex_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].down_to_vertex_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].down_to_vertex_VZ;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].down_to_vertex_dist3D;
	 npass++;
	}
	else if (distances3D[0] == evt.m_allmichels[i].up_clus_michel_dist3D){
         evt.m_allmichels[i].BestMatch = 3;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].up_to_clus_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].up_to_clus_VZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].up_to_clus_UZ;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].up_clus_michvtx_dist3D;
	 npass++;
	}
	else if (distances3D[0] == evt.m_allmichels[i].down_clus_michel_dist3D){
         evt.m_allmichels[i].BestMatch = 4;
         evt.m_allmichels[i].Best3Ddist = evt.m_allmichels[i].down_clus_michvtx_dist3D;
         evt.m_allmichels[i].best_XZ = evt.m_allmichels[i].down_to_clus_XZ;
         evt.m_allmichels[i].best_UZ = evt.m_allmichels[i].down_to_clus_UZ;
         evt.m_allmichels[i].best_VZ = evt.m_allmichels[i].down_to_clus_VZ;
	 npass++;
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
        evt.m_nmichelspass.push_back(evt.m_allmichels[i]);


      }
      if (!nmichelspass.empty() and npass > 0){
      	evt.selection = 1;
      	evt.m_nmichels.clear();
      	evt.m_nmichels = nmichelspass;
      }
      else evt.selection = 0; // has no michels with 2D distance < 150 to matched object

}
