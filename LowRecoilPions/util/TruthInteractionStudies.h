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
      }

      FSCategory(const std::string& name, const std::unordered_set<int> forbidden): fName(name), fForbidden(forbidden)
      {
      }


      const std::string& name() const { return fName; }

      bool operator ()(const CVUniverse& univ) const
      {
	const int interaction = univ.GetInteractionType();
	if (interaction==4){ return false;}
	else {
		int nforbidden =0;
		int nrequired=0;
		for(const int pdg: univ.GetTrueFSPDGCodes())
        	{
			if(fForbidden.count(fabs(pdg)) == 1) nforbidden++;
			if(fRequired.size() != 0 && fRequired.count(fabs(pdg)) == 1) nrequired++;
          		//if(fForbidden.count(fabs(pdg)) == 1) return false;
	 		//if(fRequired.size() == 0 || fRequired.count(fabs(pdg)) == 0) return false;
        	 	//if(fRequired.size() != 0 && fRequired.count(fabs(pdg)) == 1) return true;	
		}
		if (nforbidden > 0) return false; // Don't pass events for this category if forbidden pdg is in event
		else if (fRequired.size() != 0 && nrequired >= fRequired.size()) return true; // pass event for this category if no forbidden particles AND have required particles 
		else if (nforbidden == 0) return true; // Pass events that dont have forbidden particles but also dont have a required set - QE-Like for example
		else return false; //if none of the above, dont fill the category
        	//return true;
	}
      }

    private:
      const std::string fName; //Name to satisfy util::Categorized<>
      const std::unordered_set<int> fForbidden;  
      const std::unordered_set<int> fRequired;  
      const std::unordered_set<int> fIntType;
};

const std::vector<FSCategory*> pionFSCategories = {   new FSCategory("QE-like", {211, 111, 321, 311}),
                                                      new FSCategory("Charged Pions Only", {111, 321, 311}, {211}),
                                                      new FSCategory("Neutral Pions Only", {321, 311, 211}, {111}),
                                                      new FSCategory("Kaon Plus Only",{211, 111, 311}, {321}),
						      new FSCategory("Both Pion Types", {311,321}, {111, 211}),
						      
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      //new FSCategory("COH", {0}, {0}, {4})
						  };


#endif //TRUTHINTERACTIONSTUDIES_H
