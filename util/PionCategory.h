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
        bool pion = univ.GetTrueIsFSPartInEvent(211); //univ.GetTrueNPionsinEvent();
	bool k0 = univ.GetTrueIsFSPartInEvent(311);//univ.GetTrueNKaonsinEvent();
	bool kplus = univ.GetTrueIsFSPartInEvent(321);
	bool pi0 = univ.GetTrueIsFSPartInEvent(111); //univ.GetTrueNPi0inEvent();
	int qelike = 0;
 	
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

		if (pion == true && fForbidden.count(9999) == 1) return true;
		else if ((k0 == true || kplus == true) && fRequired.count(321) == 1 ) return true;
		else if (pi0 == true && fRequired.count(111) == 1) return true;
		else if (pions == false && k0 == false && kplus == false && pi0 == false && fRequired.count(9999) == 1) return true;
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
						      new FSCategory("Non-COH Pi Plus", {9999}, {211}),
                                                      new FSCategory("Neutral Pi Only", {321, 311, 211}, {111}),
                                                      new FSCategory("Kaons Only",{211, 111, 311}, {321}),
						      					      				      
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      //new FSCategory("COH", {0}, {0}, {4})
						  };


#endif //PionFSCategory_H
