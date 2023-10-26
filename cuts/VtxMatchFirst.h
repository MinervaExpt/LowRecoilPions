//File: BestMichelDistance2D.h
//Brief: Require the the distance to vertex for the best Michel electron is below some value.
//Author: Andrew Olivier aolivier@ur.rochester.edu, Mehreen Sultana msultana@ur.rochester.edu

//PlotUtils includes
#include "PlotUtils/Cut.h"
#include "PlotUtils/LowRecoilPionReco.h"
typedef LowRecoilPion::Michel<CVUniverse> Michel;
typedef LowRecoilPion::MichelEvent<CVUniverse> MichelEvent;

template <class UNIVERSE, class EVENT>
class VtxMatchFirst: public PlotUtils::Cut<UNIVERSE, EVENT>
{
  public:
    VtxMatchFirst(const double maxDistance1, const double maxDistance2): PlotUtils::Cut<UNIVERSE, EVENT>("Per Michel 2D Distance in at least two views is < " + std::to_string(maxDistance1) + "mm For Vertex Matches. And < " + std::to_string(maxDistance2) + "mm For Cluster Matches"), m_maxDistance1(maxDistance1),  m_maxDistance2(maxDistance2)  
    {
    }

  private:
    double m_maxDistance1; //Maximum distance from the vertex that the best Michel can have in mm
    double m_maxDistance2;
    bool checkCut(const UNIVERSE& univ, EVENT& evt) const
    {
      std::vector<Michel> nmichelspass;
      for (unsigned int i = 0; i < evt.m_nmichels.size(); i++)
      {
        double vtx_XZ1 = evt.m_nmichels[i].up_to_vertex_XZ; //this values is the 2D distance for the closest type of match to the michel. (can be either endpoint 1-vertex or endpoint1-cluster or endpoint2 - vertex or endpoint2 - cluster) 
        double vtx_UZ1 = evt.m_nmichels[i].up_to_vertex_UZ;//this values is the 2D distance for the closest type of match to the michel. (can be either endpoint 1-vertex or endpoint1-cluster or endpoint2 - vertex or endpoint2 - cluster) 
        double vtx_VZ1 = evt.m_nmichels[i].up_to_vertex_VZ;//this values is the 2D distance for the closest type of match to the michel. (can be either endpoint 1-vertex or endpoint1-cluster or endpoint2 - vertex or endpoint2 - cluster) 
        double vtx_XZ2 = evt.m_nmichels[i].down_to_vertex_XZ;
        double vtx_UZ2 = evt.m_nmichels[i].down_to_vertex_UZ;
        double vtx_VZ2 = evt.m_nmichels[i].down_to_vertex_VZ;
        double vtx_3D1 = evt.m_nmichels[i].up_to_vertex_dist3D;
        double vtx_3D2 = evt.m_nmichels[i].down_to_vertex_dist3D;
        bool pass1 = false;
        bool pass2 = false;
        bool passvtx = false;
        bool passclus = false;
        bool passc1 = false;
        bool passc2 = false;
        double clus_XZ1 = evt.m_nmichels[i].up_to_clus_XZ;       
        double clus_UZ1 = evt.m_nmichels[i].up_to_clus_UZ;
        double clus_VZ1 = evt.m_nmichels[i].up_to_clus_VZ;
        double clus_XZ2 = evt.m_nmichels[i].down_to_clus_XZ;
        double clus_UZ2 = evt.m_nmichels[i].down_to_clus_UZ;
        double clus_VZ2 = evt.m_nmichels[i].down_to_clus_VZ;
        double clus_upmich3D = evt.m_nmichels[i].up_clus_michel_dist3D; // Distnce between Clusters and Michel Endpoint 1
        double clus_downmich3D = evt.m_nmichels[i].down_clus_michel_dist3D; // Distance between Clusters and Michel Endpoint 2
        double clus_upvtx3D = evt.m_nmichels[i].up_to_cluster_dist3D; // Distance between vertex - Clusters Matched to  Michel  Endpoint 1 
        double clus_downvtx3D = evt.m_nmichels[i].down_to_cluster_dist3D; // Distance between vertex - Clusters Matched to  Michel  Endpoint 2       
   

        if (vtx_XZ1 < m_maxDistance1 && (vtx_UZ1 < m_maxDistance1 || vtx_VZ1 < m_maxDistance1)) pass1 = true;
        else if (vtx_UZ1 < m_maxDistance1 && (vtx_XZ1 < m_maxDistance1 || vtx_VZ1 < m_maxDistance1)) pass1 = true;
        else if (vtx_VZ1 < m_maxDistance1 && (vtx_XZ1 < m_maxDistance1 || vtx_UZ1 < m_maxDistance1)) pass1 = true;
        else if (vtx_XZ2 < m_maxDistance1 && (vtx_UZ2 < m_maxDistance1 || vtx_VZ2 < m_maxDistance1)) pass2 = true;
        else if (vtx_UZ2 < m_maxDistance1 && (vtx_XZ2 < m_maxDistance1 || vtx_VZ2 < m_maxDistance1)) pass2 = true;
        else if (vtx_VZ2 < m_maxDistance1 && (vtx_XZ2 < m_maxDistance1 || vtx_UZ2 < m_maxDistance1)) pass2 = true;
        
        if (pass1 == true || pass2 == true)
	{

           passvtx = true;
	   if (vtx_3D1 < vtx_3D2) {
		evt.m_nmichels[i].best_XZ = evt.m_nmichels[i].up_to_vertex_XZ;
                evt.m_nmichels[i].best_UZ = evt.m_nmichels[i].up_to_vertex_UZ;
                evt.m_nmichels[i].best_VZ = evt.m_nmichels[i].up_to_vertex_VZ;
                evt.m_nmichels[i].BestMatch = 1;
                evt.m_nmichels[i].Best3Ddist = evt.m_nmichels[i].up_to_vertex_dist3D;
	   }
	   else if (vtx_3D2 < vtx_3D1)
           {
                evt.m_nmichels[i].best_XZ = evt.m_nmichels[i].down_to_vertex_XZ;
                evt.m_nmichels[i].best_UZ = evt.m_nmichels[i].down_to_vertex_UZ;
                evt.m_nmichels[i].best_VZ = evt.m_nmichels[i].down_to_vertex_VZ;
                evt.m_nmichels[i].BestMatch = 2;
                evt.m_nmichels[i].Best3Ddist = evt.m_nmichels[i].down_to_vertex_dist3D;
	   }

	}
       
        else {
             
                if (vtx_XZ1 < m_maxDistance2 && (vtx_UZ1 < m_maxDistance2 || vtx_VZ1 < m_maxDistance2)) passc1 = true;
        	else if (clus_UZ1 < m_maxDistance2 && (clus_XZ1 < m_maxDistance2 || clus_VZ1 < m_maxDistance2)) passc1 = true;
        	else if (clus_VZ1 < m_maxDistance2 && (clus_XZ1 < m_maxDistance2 || clus_UZ1 < m_maxDistance2)) passc1 = true;
        	else if (clus_XZ2 < m_maxDistance2 && (clus_UZ2 < m_maxDistance2 || clus_VZ2 < m_maxDistance2)) passc2 = true;
        	else if (clus_UZ2 < m_maxDistance2 && (clus_XZ2 < m_maxDistance2 || clus_VZ2 < m_maxDistance2)) passc2 = true;
        	else if (clus_VZ2 < m_maxDistance2 && (clus_XZ2 < m_maxDistance2 || clus_UZ2 < m_maxDistance2)) passc2 = true;           
  	        
		if (passc1 == true || passc2 == true){
	 		passclus = true;
		       	if (clus_upmich3D < clus_downmich3D)
                        {
				evt.m_nmichels[i].best_XZ = evt.m_nmichels[i].up_to_clus_XZ;
                	        evt.m_nmichels[i].best_UZ = evt.m_nmichels[i].up_to_clus_UZ;
                                evt.m_nmichels[i].best_VZ = evt.m_nmichels[i].up_to_clus_VZ;
                                evt.m_nmichels[i].BestMatch = 3;
                                evt.m_nmichels[i].Best3Ddist = clus_upvtx3D;

                        } 
                        else if (clus_downmich3D < clus_upmich3D)
                        {
                                evt.m_nmichels[i].best_XZ = evt.m_nmichels[i].down_to_clus_XZ;
                                evt.m_nmichels[i].best_UZ = evt.m_nmichels[i].down_to_clus_UZ;
                                evt.m_nmichels[i].best_VZ = evt.m_nmichels[i].down_to_clus_VZ;
                                evt.m_nmichels[i].BestMatch = 4;
                                evt.m_nmichels[i].Best3Ddist = clus_downvtx3D;

	                }

                }

        }

         if (passvtx == true || passclus == true) nmichelspass.push_back(evt.m_nmichels[i]);
            
      }
      evt.m_nmichels.clear(); //empty existing vector of Michels
      evt.m_nmichels = nmichelspass; // replace vector of michels with the vector of michels that passed the above cut
      evt.m_bestdist = 9999.;
      if (!nmichelspass.empty()){
      for (int j = 0; j < nmichelspass.size(); ++j)
      {
        double dist = nmichelspass[j].Best3Ddist; //getting the minimum pion range (vertex to Michel/Clus distance) 
        if (dist <= evt.m_bestdist) {
           evt.m_bestdist = dist;
           evt.m_idx = j;

           evt.m_best_XZ = nmichelspass[j].best_XZ;
           evt.m_best_UZ = nmichelspass[j].best_UZ;
           evt.m_best_VZ = nmichelspass[j].best_VZ;
           evt.m_matchtype = nmichelspass[j].BestMatch;
           int bmatch = nmichelspass[j].BestMatch;
           if(bmatch == 1 || bmatch == 3){
           evt.best_x = nmichelspass[j].m_x1;
           evt.best_y = nmichelspass[j].m_y1;
           evt.best_z = nmichelspass[j].m_z1;
           }
           else if(bmatch == 2 || bmatch == 4){
           evt.best_x = nmichelspass[j].m_x2;
           evt.best_y = nmichelspass[j].m_y2;
           evt.best_z = nmichelspass[j].m_z2;
           }
           else {continue;}
           evt.b_truex = nmichelspass[j].true_initialx;
           evt.b_truey = nmichelspass[j].true_initialy;
           evt.b_truez = nmichelspass[j].true_initialz;
           }
      }}
          
      return !evt.m_nmichels.empty();
    }
};
