#ifndef TRUTHINTERACTIONSTUDIES_H
#define TRUTHINTERACTIONSTUDIES_H

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


const std::map<int, std::string> GENIECategories = {{1, "QE"},
                                                      {8, "2p2h"},
                                                      {2, "RES"},
                                                      {3, "DIS"}
                                              
                                                     };
using GENIECategory = int;


class FSCategory
 {
    public:
      FSCategory(const std::string& name, const std::unordered_set<int> forbidden, const std::unordered_set<int> required, const std::unordered_set<int> intType): fName(name), fForbidden(forbidden), fRequired(required), fIntType(intType)
      {
		
      }
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
	bool failedevent = false;
	int npi = 0;
	if (interaction==4){ return false;}
	//else if (interaction==3){return false;}
	else {
		int nforbidden =0;
		int nrequired=0;
		for(const int pdg: univ.GetTrueFSPDGCodes())
        	{
			if(fForbidden.count(fabs(pdg)) == 1) nforbidden++;
			if(fRequired.size() != 0 && fRequired.count(pdg) == 1) nrequired++;
          		if (pdg == 211) npi++;
			//if(fForbidden.count(fabs(pdg)) == 1) return false;
	 		//if(fRequired.size() == 0 || fRequired.count(fabs(pdg)) == 0) return false;
        	 	//if(fRequired.size() != 0 && fRequired.count(fabs(pdg)) == 1) return true;	
		}
		
		if (nforbidden > 0) failedevent = true;
		else if (fRequired.size() != 0 && nrequired >= fRequired.size()) failedevent = false;
		else if (nforbidden == 0 && fRequired.empty()) failedevent = false;
		else failedevent = true;
		 
		//if (failedevent = true) univ.PrintArachneLink();
		
		
		if (nforbidden > 0) return false; // Don't pass events for this category if forbidden pdg is in event
		//else if (fForbidden.count(-1) == 1 && fRequired.count(211) == 1 && npi == 1) return true;
		else if (fRequired.size() != 0 && nrequired >= fRequired.size()) return true; // pass event for this category if no forbidden particles AND have required particles 
		else if (nforbidden == 0 && fRequired.empty()) return true; // Pass events that dont have forbidden particles but also dont have a required set - QE-Like for example
		else return false; //if none of the above, dont fill the category
        	//return true;
	}
      }

    private:
      const std::string fName; //Name to satisfy util::Categorized<>
      const std::unordered_set<int> fForbidden;  
      const std::unordered_set<int> fRequired;  
      const std::unordered_set<int> fIntType;
      mutable std::ofstream fEventList;

};

const std::vector<FSCategory*> pionFSCategories = {   new FSCategory("QE-like", {211, 111, 321, 311}),
                                                      //new FSCategory("Single Pi Plus", {-1}, {211}),
						      new FSCategory("Pi Plus Only", {111, 321, 311}, {211}),
                                                      new FSCategory("Neutral Pi Only", {321, 311, 211}, {111}),
                                                      new FSCategory("K Plus Only",{211, 111, 311}, {321}),
						      new FSCategory("All Pi Types", {311,321}, {111, 211, -211})
						      					      				      
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      //new FSCategory("COH", {0}, {0}, {4})
						  };


#endif //TRUTHINTERACTIONSTUDIES_H
