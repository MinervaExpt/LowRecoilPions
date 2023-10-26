#ifndef TRUTHINTERACTIONSTUDIES_H
#define TRUTHINTERACTIONSTUDIES_H

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


const std::map<int, std::string> GENIECategories = {{1, "QE"},
                                                      {8, "2p2h"},
                                                      {2, "RES"},
                                                      {3, "DIS"}
                                              
                                                     };
using GENIECategory = int;


class FSCategory
 {
    public:
      FSCategory(const std::string& name, const std::unordered_set<int> forbidden, const std::unordered_set<int> required): fName(name), fForbidden(forbidden), fRequired(required)
      {
      }

      FSCategory(const std::string& name, const std::unordered_set<int> forbidden): fName(name), fForbidden(forbidden)
      {
      }

      const std::string& name() const { return fName; }

      bool operator ()(const CVUniverse& univ) const
      {
	if(fForbidden.count(fabs(0)) == 1 && univ.GetInteractionType() == 4) return true;
	else if (univ.GetInteractionType() == 4) return false;
	else {
		for(const int pdg: univ.GetTrueFSPDGCodes())
        	{
          		if(fForbidden.count(fabs(pdg)) == 1) return false;
	 		 //if(fRequired.count(fabs(pdg)) == 0) return false;
        	}

        	return true;
	}
      }

    private:
      const std::string fName; //Name to satisfy util::Categorized<>
      const std::unordered_set<int> fForbidden;  
      const std::unordered_set<int> fRequired;  

};

const std::vector<FSCategory*> pionFSCategories = {   new FSCategory("QE-like", {211, 111, 321, 311}),
                                                      new FSCategory("Charged Pions Only", {111, 321, 311}, {211}),
                                                      new FSCategory("Neutral Pions Only", {321, 311, 211}, {111}),
                                                      new FSCategory("Kaon Plus Only",{2212,211, 111, 311}, {321}),
						      new FSCategory("NPions", {2212,311,321}, {111, 211}),
                                                      //new FSCategory("NoneAbove", {2212,211, 111, 321, 311}),
						      new FSCategory("COH", {0})
						      //new FSCategory("No Pions", {111,211}, {111, 211}),
                                                      //new FSCategory("Other", {2212,211, 111, 321, 311})
						  };


#endif //TRUTHINTERACTIONSTUDIES_H
