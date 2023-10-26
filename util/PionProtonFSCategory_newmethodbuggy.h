#ifndef PIONPROTONFSCATEGORY_H
#define PIONPROTONFSCATEGORY_H

#include "studies/Study.h"
#ifndef __CINT__ //PlotUtils/VariableBase.h uses std::function which is from c++11
#include "util/SafeROOTName.h" //TODO: This is a very bad idea
#endif //__CINT__
#include "util/Categorized.h"
#include "event/CVUniverse.h"
#include "PlotUtils/VariableBase.h"
#include "util/SafeROOTName.h"
#include <functional> 
#include <unordered_set>


                                              
using GENIECategory = int;


class FSpCategory
 {
    public:
      
      FSpCategory(const std::string& name, const std::unordered_set<int> forbidden, const std::unordered_set<int> required): fName(name), fForbidden(forbidden), fRequired(required)
      {
		//fEventList.open("OtherEvents.txt");

      }

      FSpCategory(const std::string& name, const std::unordered_set<int> forbidden): fName(name), fForbidden(forbidden)
      {
		//fEventList.open("OtherEvents_Norequired.txt");
      }
      

      const std::string& name() const { return fName; }

      bool operator ()(const CVUniverse& univ) const
      {
	const int interaction = univ.GetInteractionType();
        int npionp = 0;
        int npion0 = 0;
        int nkplus = 0;
        int npionm = 0;
        int nkneut = 0;
        int nproton = 0;
	int nneutron = 0;
        bool leadingproton = false;
	bool leadingneutron = false;
        int npart = 0;
        std::vector<int> protonidx;
	std::vector<int> neutronidx;
        const std::vector<int> fsParts = univ.GetTrueFSPDGCodes();
 	//npionp =  std::count(fsParts.begin(), fsParts.end(), 211); 
	//npionm = std::count(fsParts.begin(), fsParts.end(), -211);
	//npion0 = std::count(fsParts.begin(), fsParts.end(), 111);
	//nkplus = std::count(fsParts.begin(), fsParts.end(), 321);
	//nkneut = std::count(fsParts.begin(), fsParts.end(), 311);
        //nneutron = std::count(fsParts.begin(), fsParts.end(), 2112);
	//nprotonidx = std::count(fsParts.begin(), fsParts.end(), 2212);
	
	
	std::vector<double> neutE;
	std::vector<double> protE;

        for(auto pdgCode: fsParts)
	{
  		if(pdgCode == 211) ++npionp;
		else if(pdgCode == -211) ++npionm;
		else if(pdgCode == 111) ++npion0;
  		else if(abs(pdgCode) == 321) ++nkplus;
                else if(pdgCode == 311) ++nkneut;
	        else if(pdgCode == 2112){
			++nneutron;
			neutE.push_back(univ.GetTrueEpiEvent(npart));
		        neutronidx.push_back(npart);	
		}
		else if(pdgCode == 2212){
			 ++nproton;
			 protE.push_back(univ.GetTrueEpiEvent(npart));
	         	 protonidx.push_back(npart);
		}
		npart++;    
        }

	if (!neutE.empty()) std::sort(neutE.begin(), neutE.end());
	if (!protE.empty()) std::sort(protE.begin(), protE.end());

	bool hasprotonandneutron = false;
        if (nproton == 0 and nneutron == 0) // If no neutron and proton, set booleans to false
	{
	   leadingneutron = false;
           leadingproton = false;
	}
        else if (nproton > 0 and nneutron == 0){ // If there is a proton and no neutron, 
             leadingproton = true;
             leadingneutron = false;
        }
	else if (nproton == 0 and nneutron > 0){ // If there is a proton and no neutron, 
             leadingproton = false;
             leadingneutron = true;
        }
	else if (protE.back() > neutE.back()){
	   leadingproton = true;
           leadingneutron = false;
	}
	else if (protE.back() < neutE.back()){
	   leadingproton = false;
           leadingneutron = true;
	}

	/*
	else if (protonidx.size() == 1 and neutronidx.empty()){ // If there is a proton and no neutron, 
	     leadingproton = true;
	     leadingneutron = false;
	}
	else if (protonidx.size() == 1 and neutronidx.size() == 1){ //If there is 1 proton and 1 neutron. Check which has greater energy
	     double protonE = univ.GetTrueEpiEvent(protonidx[0]);
 	     double neutronE = univ.GetTrueEpiEvent(neutronidx[0]);
	     hasprotonandneutron = true; 
	     if (protonE > neutronE) leadingproton = true;
	     else if(neutronE > protonE) leadingneutron = true;
	}
	else if(protonidx.size() > 1 and neutronidx.size() >= 1){ // IF there are more than 1 proton and 1 or more neutron, get the highest energy proton and neutron and compare.
	     double highEproton = 0.;
	     double highEneutron = 0.;
             hasprotonandneutron = true;
             for(auto p:protonidx){
		double energy = univ.GetTrueEpiEvent(p);
		if (energy > highEproton) highEproton = energy;
	     }
	     for(auto n:neutronidx){
		double energy = univ.GetTrueEpiEvent(n);
		if (energy > highEneutron) highEneutron = energy;
	     }
	     if (highEproton > highEneutron) leadingproton = true;
	     else if (highEneutron > highEproton) leadingneutron = true;
	}
	else if(protonidx.empty() and !neutronidx.empty()){ // If there are no protons and only neutrons
	     leadingneutron = true;
	     leadingproton = false;
        }
	else if(protonidx.size() == 0 and neutronidx.size() == 0){
	     leadingneutron = false;
	     leadingproton = false;
        } 
	*/   

        bool pion = (npionp > 0); //univ.GetTrueIsFSPartInEvent(211); //univ.GetTrueNPionsinEvent();
	bool k0 = (nkneut > 0);//univ.GetTrueIsFSPartInEvent(311);//univ.GetTrueNKaonsinEvent();
	bool kplus = (nkplus > 0);//univ.GetTrueIsFSPartInEvent(321);
	bool pi0 = (npion0 > 0); //univ.GetTrueIsFSPartInEvent(111); //univ.GetTrueNPi0inEvent();
	//bool lam = univ.GetTrueIsFSPartInEvent(3122); //lambda baryons
	//bool sigma = univ.GetTrueIsFSPartInEvent(3222);
	int qelike = 0;
 	int npions = npionp; //univ.GetTrueNPionsinEvent();
        bool omesons = (kplus == true or k0 == true or pi0 == true or npionm > 0);
	//if (interaction == 4) std::cout << "THIS EVENT IS COH " << std::endl;
 	if (interaction==4 && fForbidden.count(4) == 1 ){ return true;} // Put all Coherent Events in 1 category
	//else if (interaction==3){return false;}
	else {

                /*
		for(const int pdg: univ.GetTrueFSPDGCodes())
        	{
			if(pdg == 211) pions++;
		        else if(pdg == 321) kplus++;
			else if(pdg == 111) pi0++;
			else if(pdg == 311) k0++;
		}
                */
		//if (npions == 1) std::cout << "THIS EVENT HAS 1 PION " << std::endl;
		if (npionp == 1 && npionm == 0 && k0 == false && pi0 == false && kplus == false && nproton > 0 && nneutron == 0 && fForbidden.count(9990) == 1) return true; // 1pi+ w/ Leading proton
	        else if (npionp == 1 && npionm == 0 && k0 == false && pi0 == false && kplus == false && leadingneutron == false && leadingproton == true && fForbidden.count(9990) == 1) return true; // 1pi+ w/ Leading proton
		else if (npionp == 1 && npionm == 0 && k0 == false && pi0 == false && kplus == false && leadingneutron == true && leadingproton == false && fForbidden.count(9991) == 1) return true; // 1pi+ w/ Leading neutron
		else if (npionp == 1 && npionm == 0 && k0 == false && pi0 == false && kplus == false && nproton == 0 && nneutron == 0 && fForbidden.count(9992) == 1) return true; // 1pi+ w/o Leading proton or neutron
		else if (npionp > 1 && npionm == 0 &&  k0 == false && pi0 == false && kplus == false && fForbidden.count(9980) == 1) return true; //Npi+ only;
 		
                else if (npionp > 0 and omesons == true and interaction==2 and fForbidden.count(2) == 1 ) return true; //npi+ with mesons RES
		else if (npionp > 0 and omesons == true and interaction==3 and fForbidden.count(3) == 1 ) return true; //npi+ with mesons DIS
		else if (pion == false && (kplus == true) && fRequired.count(321) == 1 ) return true; //Events with Kaons
		else if (pion == false && pi0 == true && fRequired.count(111) == 1) return true; // Events with Neutral Pi only
		else if (npionm == 0 && pion == false && k0 == false && kplus == false && pi0 == false && fRequired.count(9999) == 1) return true; // QE-Like events
		else return false;
        	//return true;
	}
      }

    private:
      const std::string fName; //Name to satisfy util::Categorized<>
      const std::unordered_set<int> fForbidden;  
      const std::unordered_set<int> fRequired;  
      mutable std::ofstream fEventList;

};

const std::vector<FSpCategory*> pionprotonFSCategories = {   new FSpCategory("QE-like", {211, 111, 321, 311}, {9999}),
                                                      //new FSCategory("Single Pi Plus", {-1}, {211}),
						      new FSpCategory("COH Pions", {4}, {211}), // COH events only
						      new FSpCategory("1pip w LProton", {9990}, {211}), 
						      new FSpCategory("1pip w LNeutron", {9991}, {211}),
						      new FSpCategory("1pip wo p or n", {9992}, {211}),
						      new FSpCategory("Npip Only", {9980}, {221}),
						      //new FSpCategory("Npip w LProton", {9980}, {211}),
						      //new FSpCategory("Npip w LNeutron", {9981}, {211}),
					  	      //new FSpCategory("Npip wo p or n", {9982}, {211}),
						      //new FSCategory("NPi Plus Only", {9999}, {211}),
						      new FSpCategory("NPi and Mesons RES", {2}, {211}), //Npi+ w/ other mesons from Resonant Events
						      new FSpCategory("NPi and Mesons DIS", {3}, {211}), //Npi+ w/ other mesons from DIS events
						      //new FSpCategory("Npi and Mesons (Other)", {5}, {211}), //Npi+ w/ other mesons from anything else
						      new FSpCategory("Neutral Pi Only", {321, 311, 211}, {111}),
                                                      new FSpCategory("Kaons Only",{211, 111, 311}, {321}),
						      //new FSCategory("Strange Baryons", {211},{3122, 3222}),    					      				      
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      //new FSCategory("COH", {0}, {0}, {4})
						  };


#endif //PionProtonFSCategory_H
