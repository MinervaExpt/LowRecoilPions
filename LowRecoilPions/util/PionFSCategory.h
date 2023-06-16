#ifndef PIONFSCATEGORY_H
#define PIONFSCATEGORY_H

#include "studies/Study.h"
#ifndef __CINT__ //PlotUtils/VariableBase.h uses std::function which is from c++11
#include "util/SafeROOTName.h" //TODO: This is a very bad idea
#endif //__CINT__
#include "event/MichelEvent.h"
#include "util/Categorized.h"
#include "event/CVUniverse.h"
#include "PlotUtils/VariableBase.h"
#include "util/SafeROOTName.h"
#include <functional> 
#include <unordered_set>


                                              
using GENIECategory = int;


class FSCategory
 {
    public:
      
      FSCategory(const std::string& name, const std::unordered_set<int> forbidden, const std::unordered_set<int> required): fName(name), fForbidden(forbidden), fRequired(required)
      {
		//fEventList.open("OtherEvents.txt");

      }

      FSCategory(const std::string& name, const std::unordered_set<int> forbidden): fName(name), fForbidden(forbidden)
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
        const std::vector<int> fsParts = univ.GetTrueFSPDGCodes();
        for(auto pdgCode: fsParts)
	{
  		if(pdgCode == 211) ++npionp;
		else if(pdgCode == -211) ++npionm;
		else if(pdgCode == 111) ++npion0;
  		else if(abs(pdgCode) == 321) ++nkplus;
                else if(pdgCode == 311) ++nkneut;
	} 
        bool pion = (npionp > 0); //univ.GetTrueIsFSPartInEvent(211); //univ.GetTrueNPionsinEvent();
	bool k0 = (nkneut > 0);//univ.GetTrueIsFSPartInEvent(311);//univ.GetTrueNKaonsinEvent();
	bool kplus = (nkplus > 0);//univ.GetTrueIsFSPartInEvent(321);
	bool pi0 = (npion0 > 0); //univ.GetTrueIsFSPartInEvent(111); //univ.GetTrueNPi0inEvent();
	//bool lam = univ.GetTrueIsFSPartInEvent(3122); //lambda baryons
	//bool sigma = univ.GetTrueIsFSPartInEvent(3222);
	int qelike = 0;
 	int npions = npionp; //univ.GetTrueNPionsinEvent();
	//if (interaction == 4) std::cout << "THIS EVENT IS COH " << std::endl;
 	if (interaction==4 && fForbidden.count(4) == 1 ){ return true;}
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
		if (pion == true && npions >= 1 && k0 == false && pi0 == false && kplus == false && fForbidden.count(9999) == 1) return true;
		else if (pion == true && npions >= 1 && (k0 == true or pi0 == true or kplus == true ) && fForbidden.count(2) == 1) return true;
		else if (pion == false && (kplus == true) && fRequired.count(321) == 1 ) return true;
		else if (pion == false && pi0 == true && fRequired.count(111) == 1) return true;
		//else if (pion == false && (sigma == true or lam == true) && fRequired.count(3122) == 1) return true;
		else if (pion == false && k0 == false && kplus == false && pi0 == false && fRequired.count(9999) == 1) return true;
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

const std::vector<FSCategory*> pionFSCategories = {   new FSCategory("QE-like", {211, 111, 321, 311}, {9999}),
                                                      //new FSCategory("Single Pi Plus", {-1}, {211}),
						      new FSCategory("COH Pions", {4}, {211}),
						      new FSCategory("NPi Plus Only", {9999}, {211}),
						      new FSCategory("NPi and Other Mesons", {2}, {211}),
						      new FSCategory("Neutral Pi Only", {321, 311, 211}, {111}),
                                                      new FSCategory("Kaons Only",{211, 111, 311}, {321}),
						      //new FSCategory("Strange Baryons", {211},{3122, 3222}),    					      				      
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      //new FSCategory("COH", {0}, {0}, {4})
						  };


#endif //PionFSCategory_H
