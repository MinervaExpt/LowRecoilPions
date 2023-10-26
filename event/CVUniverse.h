// =============================================================================
// Base class for an un-systematically shifted (i.e. CV) universe. Implement
// "Get" functions for all the quantities that you need for your analysis.
//
// This class inherits from PU::MinervaUniverse, which in turn inherits from
// PU::BaseUniverse. PU::BU defines the interface with anatuples.
// 
// Within the class, "WeightFunctions" and "MuonFunctions" are included to gain
// access to standardized weight and muon variable getters. See:
// https://cdcvs.fnal.gov/redmine/projects/minerva-sw/wiki/MinervaUniverse_Structure_
// for a full list of standardized functions you can use. In general, if a
// standard version of a function is available, you should be using it.
// =============================================================================
#ifndef CVUNIVERSE_H
#define CVUNIVERSE_H

#include <iostream>
#include <chrono>
#include "PlotUtils/MinervaUniverse.h"
#include "TVector.h"
#include "PlotUtils/LowRecoilPionReco.h"
#include "TLorentzVector.h"
typedef LowRecoilPion::Cluster Cluster;

//#include "event/Cluster.h"
class CVUniverse : public PlotUtils::MinervaUniverse {

  public:
  #include "PlotUtils/MuonFunctions.h" // GetMinosEfficiencyWeight
  #include "PlotUtils/TruthFunctions.h" //Getq3True
  #include "PlotUtils/LowRecoilFunctions.h" // GetEAvailable()
  #include "PlotUtils/WeightFunctions.h" // Get COH Weight functions  
  #include "PlotUtils/MichelFunctions.h"
  #include "RecoilEnergyFunctions.h"
  #include "PlotUtils/MnvTuneSystematics.h"
  // ========================================================================
  // Constructor/Destructor
  // ========================================================================
  CVUniverse(PlotUtils::ChainWrapper* chw, double nsigma = 0)
      : PlotUtils::MinervaUniverse(chw, nsigma) {}

  virtual ~CVUniverse() {}

  // ========================================================================
  // Quantities defined here as constants for the sake of below. Definition
  // matched to Dan's CCQENuInclusiveME variables from:
  // `/minerva/app/users/drut1186/cmtuser/Minerva_v22r1p1_OrigCCQENuInc/Ana/CCQENu/ana_common/include/CCQENuUtils.h`
  // ========================================================================
  static constexpr double M_n = 939.56536;
  static constexpr double M_p = 938.272013;
  static constexpr double M_nucleon = (1.5*M_n+M_p)/2.5;
  static constexpr double M_mu = 105.6583/1000.; //Converting to GeV
  static constexpr int PDG_n = 2112;
  static constexpr int PDG_p = 2212;
  static constexpr double M_pi = 139.57039; //in MeV
  
  
  // ========================================================================
  // Write a "Get" function for all quantities access by your analysis.
  // For composite quantities (e.g. Enu) use a calculator function.
  //
  // In order to properly calculate muon variables and systematics use the
  // various functions defined in MinervaUniverse.
  // E.g. GetPmu, GetEmu, etc.
  // ========================================================================

  // Quantities only needed for cuts
  // Although unlikely, in principle these quanties could be shifted by a
  // systematic. And when they are, they'll only be shifted correctly if we
  // write these accessor functions.
  double GetTrueQ3() const
  {
 	return Getq3True()/pow(10,3);
  } 
  
  double GetTrueQ2GeV() const
  {
        return GetQ2True()/pow(10, 6);
  }

  double GetTrueQ2MeV() const
  {
        return GetQ2True();
  }

  double GetTrueQ0GeV() const
  {
        return Getq0True()/pow(10,3);
  }
  double GetTrueQ0() const
  {     
        return Getq0True();
  }

  //Muon kinematics
  double GetMuonPT() const //GeV/c
  {
    return GetPmu()/1000. * sin(GetThetamu());
  }

  double GetMuonPz() const //GeV/c
  {
    return GetPmu()/1000. * cos(GetThetamu());
  }

  double GetMuonVecX() const // MeV/c
  {
    return GetMuon4V().Px();

  }

  double GetPXmu() const { return GetMuon4V().Px(); } //MeV
  double GetPXmuGeV() const { return GetMuon4V().Px()/1000.; } //GeV
  double GetPYmu() const { return GetMuon4V().Py(); } // MeV
  double GetPYmuGeV() const { return GetMuon4V().Py()/1000.; } //GeV
  double GetPZmu() const { return GetMuon4V().Pz(); } //MeV
  double GetPZmuGeV() const { return GetMuon4V().Pz()/1000.; } //GeV

  double GetPXmuTrue() const {
        //std::cout << GetVecElem("mc_primFSLepton", 0)  << std::endl;
   	return GetVecElem("mc_primFSLepton", 0);
  }

  double GetPYmuTrue() const {
  	return GetVecElem("mc_primFSLepton", 1);
  }

  double GetPZmuTrue() const {
        return GetVecElem("mc_primFSLepton", 1);
  }

  virtual ROOT::Math::PxPyPzEVector GetPmuTrueWRTBeam() const {
     
     TVector3 pmu(GetVecElem("mc_primFSLepton", 0), GetVecElem("mc_primFSLepton", 1), GetVecElem("mc_primFSLepton", 1));
     //pmu.RotateX(MinervaUnits::numi_beam_angle_rad);
     const double numi_beam_angle_rad = -0.05887;
     //double theta = GetThetalepTrue(); //thetaWRTBeam(p3lep.X(), p3lep.Y(), p3lep.Z());
     //double phi = GetPhilepTrue(); //phiWRTBeam(p3lep.X(), p3lep.Y(), p3lep.Z()); 
     double px = pmu.X(); //GetPlepTrue()*sin(theta) * cos(phi);  
     double py =  -1.0 *sin( numi_beam_angle_rad )*pmu.Z() + cos( numi_beam_angle_rad )*pmu.Y();;//GetPlepTrue()*sin(theta) * sin(phi);
     double pz = cos( numi_beam_angle_rad )*pmu.Z() + sin( numi_beam_angle_rad )*pmu.Y();//GetPlepTrue()*cos(theta);
     double E = GetElepTrue(); 
     return ROOT::Math::PxPyPzEVector(px, py, pz, E);
  }

  double GetMuPxTrue() const { return  GetPmuTrueWRTBeam().Px()/1000.;}
  double GetMuPyTrue() const { return  GetPmuTrueWRTBeam().Py()/1000.;}
  double GetMuPzTrue() const { return  GetPmuTrueWRTBeam().Pz()/1000.;}
  
  double GetMuonP() const
  {
	return GetPmu()/1000.; //GeV/c
  }
  double GetPmuTrue() const
  {
	return GetPlepTrue()/1000.; //GeV/c

  }



  double GetTrueExpq3() const
  {
     return calcq3(GetTrueExperimentersQ2(), GetEnuTrue()/pow(10,3), GetElepTrue()/pow(10,3)); // In GeV
  }
  double GetMuonPTTrue() const //GeV/c
  {
    return GetPlepTrue()/1000. * sin(GetThetalepTrue());
  }

  double GetMuonPzTrue() const //GeV/c
  {
    return GetPlepTrue()/1000. * cos(GetThetalepTrue());
  }

  double GetMuonPxTrue() const //GeV/c
  {
    return GetPlepTrue()/1000. * sin(GetThetalepTrue()) * cos(GetPhilepTrue()) ;
  }
 
  double GetMuonPyTrue() const //GeV/c
  {
    return GetPlepTrue()/1000. * sin(GetThetalepTrue()) * sin(GetPhilepTrue()) ;
  }


  double GetEmuGeV() const //GeV
  {
    return GetEmu()/1000.;
  }

  double GetElepTrueGeV() const //GeV
  {
    return GetElepTrue()/1000.;
  }

  int GetInteractionType() const {
    return GetInt("mc_intType");
  }

  int GetTargetNucleon() const {
    return GetInt("mc_targetNucleon");
  }
  
  double GetBjorkenXTrue() const {
    return GetDouble("mc_Bjorkenx");
  }
  double GetZero() const{
    return 0.0;
  }
 
  double GetDefault() const{
    return 99999.;
  }

  double GetBjorkenYTrue() const {
    return GetDouble("mc_Bjorkeny");
  }

  virtual bool IsMinosMatchMuon() const {
    int ismatch = GetInt("isMinosMatchTrack");
   
    if (ismatch > 0) return true;
    else return false;
  }
 
  virtual bool isMuonCharge() const{
   double charge = GetDouble("MasterAnaDev_minos_trk_qp");
   return charge < 0.0;
  }
  
  ROOT::Math::XYZTVector GetVertex() const
  {
    ROOT::Math::XYZTVector result;
    result.SetCoordinates(GetVec<double>("vtx").data());
    return result;
  }

  ROOT::Math::XYZTVector GetTrueVertex() const
  {
    ROOT::Math::XYZTVector result;
    result.SetCoordinates(GetVec<double>("mc_vtx").data());
    return result;
  }
  
  virtual double  GetIntVtxXTrue() const
  {
    return GetTrueVertex().X();
  }
  virtual double  GetIntVtxYTrue() const
  {
    return GetTrueVertex().Y();
  }
  virtual double  GetIntVtxZTrue() const
  {
    return GetTrueVertex().Z();
  }

  virtual int GetTDead() const {
    return GetInt("phys_n_dead_discr_pair_upstream_prim_track_proj");
  }
  
  //=== Response Systematic Definitions
  virtual double GetCalRecoilEnergy() const {
    return NewEavail(); // Use calorimetric recoil energy used. 	
  }
 /* 
  virtual double GetNonCalRecoilEnergy() {
    if (GetNMichels() == 0) {
#ifndef NDEBUG
      std::cout << "CVU::GetNonCalRecoilEnergy WARNING: no michel candidates!\n";
#endif
      return 0.;
    }
    else{
      return ;
    }
    
  }
  */

  virtual double GetEventPionTpi(double range){
     double tpi = GetTpiFromRange(range);
     double energy = tpi + M_pi;
     return energy;
  }
  
  // =============== 

 
  //TODO: If there was a spline correcting Eavail, it might not really be Eavail.
  //      Our energy correction spline, one of at least 2 I know of, corrects q0
  //      so that we get the right neutrino energy in an inclusive sample.  So,
  //      this function could be correcting for neutron energy which Eavail should
  //      not do.
  /*
  double GetEavailCorr() const {
       return GetEAvailable();
  }  

  
  virtual double GetEavail() const {
    //if (GetEAvailable() > 1500.) PrintArachneLink(); 
    return GetEAvailable();
    //return eavail;
    //return GetEAvailable();
   }
 
  virtual double GetNoPolylineE() const{
	return GetDouble("recoil_E_nopolyline");
  }
  virtual double GetTrueEHad() const{
    return GetEnuTrue() - GetElepTrue();

  }
  virtual double GetPolylineE() const {
	return GetDouble("recoil_E_polylinecorrected");	
  }
 
  virtual double GetRecoilECorr() const {
    double eavail = GetEAvailable() + M_pi;///GetDouble("recoil_E_nopolyline");
    return eavail;
    //if (eavail < 400.) return 50.+1.7*eavail;
    //if (eavail > 400.) return 1.7*eavail;
  } 
  virtual double GetRecoilE() const {
    double eavail = GetEAvailable()+M_pi; //GetDouble("recoil_E_nopolyline");
    return eavail;
  }  

  
  virtual double GetEHad() const {
     return GetCalRecoilE();
  }

  virtual double GetCalRecoilE() const{
    const double calE = GetDouble("MasterAnaDev_hadron_recoil_default");
    if (calE > 1000.){
        return GetCalRecoilE_CCPiSPline();
    }
    else {
        return GetCalRecoilE_Corrected();
    }

  }

  virtual double GetCalRecoilE_CCPiSPline() const{
    return GetDouble("MasterAnaDev_hadron_recoil_two_track");

  }

  virtual double GetCalRecoilE_Corrected() const {
    double calE = GetDouble("MasterAnaDev_hadron_recoil_default");
    double calE_corrected = calE; //make correction to this
    std::vector<double> calE_bins = {0.0, 0.025e3, 0.05e3, 0.075e3, 0.1e3, 0.15e3, 0.2e3, 0.25e3, 0.3e3, 0.4e3, 0.5e3,0.6e3, 0.7e3,   0.8e3,  0.9e3,   1.0e3, 2.0e3};
    const std::vector<double> corrections = {-0.060, -0.050, -0.210, -0.180,
                                           -0.165, -0.180, -0.180, -0.180,
                                           -0.195, -0.360, -0.400};
    for (int i_bin = 0; i_bin < calE_bins.size() - 1; ++i_bin) {
      if (((1e3) * calE_bins[i_bin] < calE) &&
        (calE < (1e3) * calE_bins[i_bin + 1])) {
        if (calE < 1000.) {
          calE_corrected = calE - (1e3) * corrections[i_bin];
          break;
        }
      }
     }
    return calE_corrected;
  } 
  */

  virtual double Recoq3pTdiff() const{
     double q3 = Getq3();
     double pT = GetMuonPT(); 
     return sqrt( (q3*q3) - (pT*pT));

  }

  virtual double GetTrueq3pTdiff() const {
     double q3 = GetTrueQ3();
     double pT =  GetMuonPTTrue(); 
     return sqrt( (q3*q3) - (pT*pT));

  }
  /*
  virtual double GetRecoilESum() const {
     double ecal = GetDouble("blob_recoil_E_ecal");
     double hcal = GetDouble("blob_recoil_E_hcal");
     double tracker = GetDouble("blob_recoil_E_tracker");
     double od = GetDouble("blob_recoil_E_od");
     return (ecal+hcal+tracker); //Get these in GeV
  }

  virtual double GetRecoilECorrSum() const {
     //These correction values are obtained from docdb 7712
     double ecal = 1.73*GetDouble("blob_recoil_E_ecal"); // Adding calorimetric correction for ECAL (not doing side ecal)
     double hcal = 9.32*GetDouble("blob_recoil_E_hcal"); //Calorimetric correction for HCAL
     double tracker = GetDouble("blob_recoil_E_tracker"); // Calorimetric correction for tracker
     double od = 9.32*GetDouble("blob_recoil_E_od"); // Calorimetric correction for OD
     return 1.70*(ecal+hcal+tracker);
    
  }
  
  virtual double GetRecoilESumPi() const {
     double ecal = GetDouble("blob_recoil_E_ecal");
     double hcal = GetDouble("blob_recoil_E_hcal");
     double tracker = GetDouble("blob_recoil_E_tracker");
     double od = GetDouble("blob_recoil_E_od");
     return (ecal+hcal+tracker)/pow(10,3); //Get these in GeV
  }
  */
  virtual double GetQ2Reco() const{
    //double q2reco = GetDouble("MasterAnaDev_Q2_Inclusive")/pow(10,6);
     
    double enu = GetEmuGeV() + (NewRecoilE())/1000.;
    //double q2reco =  CalcQ2(enu,  GetEmuGeV(), GetThetamu()); 
    //std::cout << " Q2 from Eavail is " << q2reco << std::endl;
    double pmucos = (GetPmu()/1000.)*cos(GetThetamu());
    double q2reco = 2.*enu*(GetEmuGeV() - pmucos ) - (M_mu*M_mu);
    //std::cout << "Print RecoilSummedE" << GetRecolE() <<  " Print Ehad: " << GetEHad() << " Print q2reco: " << q2reco << " Print EAvail: " << GetEavail() << std::endl;
    //double q2reco = GetDouble("qsquared_recoil");
    
    //std::cout << " Q2 from Tree is " << GetDouble("MasterAnaDev_Q2_Inclusive")/pow(10,6) << std::endl; // TODO: July 20, 2022 Check Using Hang Su's Low recoil function; //q2reco;
    return q2reco;//GetDouble("MasterAnaDev_Q2_Inclusive")/pow(10,6);
  }
  
  virtual double GetQ2RecoMeV() const{
    
    return GetQ2Reco()*pow(10,6);

  }

  //This is for RecoilFunction Calculations
  virtual double GetLowQ2PionWeight() const
  {   
      //double weight = GetLowQ2PiWeight("MENU1PI");
      //if (weight != 1.0) PrintTrueArachneLink();
      //double w = GetDouble("mc_w");
      //int npion = 0.0;
      //int pdgsize = GetInt("mc_nFSPart");
      //for (int i = 0; i< pdgsize; i++)
      //{
      //     int pdg = GetVecElem("mc_FSPartPDG", i);
      //     if (pdg == 211) npion++;
      //}
      return GetLowQ2PiWeight("MENU1PI");
      //else return 1.0; 
  }

  virtual double GetMichelEfficiencyReWeight() const{
    return GetMichelEfficiencyWeight();
  }

  virtual double GetGeantHadronReWeight() const{
    return GetGeantHadronWeight();

  }

  virtual double GetFSIReWeight() const{
    return GetFSIWeight(0);
  }

  virtual double GetTargetMassReWeight() const {
    return GetTargetMassWeight();
  }
  virtual double GetMKReWeight() const {
    return GetMKWeight();

  }
  
  virtual double Gettreco(double tpi, TVector3 p_pi) const {
	double Emu = GetEmuGeV(); // in GeV
	double epi = tpi + 139.57/1000.; // in GeV
	double plmu = GetPZmu()/1000.; // GeV
	double plpi = p_pi.Z()/1000.; //GeV Should be Z component of the pion momentum
  	double epsub = (Emu - plmu) + (epi - plpi); // GeV
	double pxtot = GetPXmu()/1000. + p_pi.X()/1000.;
	double pytot = GetPYmu()/1000. + p_pi.Y()/1000.;
	double pTmag2 = pxtot*pxtot + pytot*pytot; // GeV squared
	double treco = epsub*epsub + pTmag2;
	return treco;
  }

  virtual double GetTruet(int idx) const {
	if (idx == 9999) return -9999.;
	double ppix = GetVecElem("mc_FSPartPx", idx)/1000.;
	TVector3 ppi(ppix, GetTruePpiyEvent(idx)/1000., GetTruePpizEvent(idx)/1000.); 
	TVector3 pmu(GetMuonPxTrue(), GetMuonPyTrue(), GetMuonPzTrue()); //GeV
	double emu = GetElepTrueGeV();
	double epi = GetVecElem("mc_FSPartE", idx)/1000.; //GetTrueEpiEvent(idx);
	double epsub = (emu - pmu.Z()) + (epi - ppi.Z());
        double pxtot = ppi.X() + pmu.X();
	double pytot = ppi.Y() + pmu.Y();
	double pTmag2 = pxtot*pxtot + pytot*pytot; // GeV squared
	double ttrue = epsub*epsub + pTmag2; 
	return ttrue;
  }

 
  virtual double GetCOHPionWeight() const { // This is Alejandro Ramirez's Correction
    double weight = 1.0;
    if(GetInt("mc_intType") != 4) return 1.0;
    if(GetInt("mc_intType") == 4){
       //int npi = GetTrueNPionsinEvent();
       //if (npi == 0) return 1.0;
       double angle = GetTrueAngleHighTpi();//*180./M_PI; //this is now in degrees
       //Angle is already in degrees GetTrueAngleHighTpi()
       //std::cout << "Printing Angle for COH event " << angle << std::endl;
       double epi = GetTrueHighEpi()/1000.; // This is supposed to be the energy of the pion!!!! 
       if (epi < 0) return 1.0;
       else {
	weight *= GetCoherentPiWeight(angle, epi); //Inputs are in Degrees and GeV
        //std::cout << "Printing COHerent weight for COH event for angle: " << angle << " degrees and KE : " << KE << " GeV. And weight: " << weight << std::endl;
	return weight;
       }
     }
     else return 1.0;
  }

  virtual double GetCOHTpiCorrection() const{ // This is Mehreen's Correction. 
     double weight = 1.0;
     
     if(GetInt("mc_intType") != 4) return 1.0;
     else if(GetInt("mc_intType") == 4){
        return 3.0;
	//double tpi = GetTrueLowestTpiEvent();
        //if (tpi < 0) return 1.0;
        //double eavl =  GetTrueEAvail();
	//std::cout << "True E available is " << GetTrueEAvail() << " And True Lowest Tpi is " << GetTrueLowestTpiEvent() << std::endl;
	//std::cout << "True Eav - Tpi is " << GetTrueEavMinusTpi() << " MeV" << std::endl;
	
        //if (GetTrueEavMinusTpi() >= 0. && GetTrueEavMinusTpi() < 25.0){
	        //PrintTrueArachneLink();
	        //weight = 3.0;
	//        return 3.0;
		/*
                if (GetTrueLowestTpiEvent() >= 60. && GetTrueLowestTpiEvent() < 200.){
                   //PrintTrueArachneLink();
		   weight = 3.0;
		   return 3.0;
                }
		else if (GetTrueLowestTpiEvent() >= 200. && GetTrueLowestTpiEvent() < 300.){
		    weight = ;
		    return 1.5;
		}
		*/
        //}
        //else return 1.0;
     }

  }


   virtual double GetTrueAngleHighTpi() const {
     int nFSpi = GetInt("mc_nFSPart");
     double angle = -9999.; //WRTbeam and in degrees
     double pionKE = 0.0;
     int idk = -9999;
     for (int i = 0; i < nFSpi; i++){
         int pdg = GetVecElem("mc_FSPartPDG",i);
         if(pdg != 211) continue;
         double energy = GetVecElem("mc_FSPartE", i);
         
	 double mass = 139.569;
         double tpi = energy - mass;
         if (tpi >= pionKE) {
               pionKE = tpi;
               TVector3 pimomentumvec(GetVecElem("mc_FSPartPx", i), GetVecElem("mc_FSPartPz", i),GetVecElem("mc_FSPartPz", i));
               double deg_wrtb = thetaWRTBeam(pimomentumvec.X(), pimomentumvec.Y(), pimomentumvec.Z()); //rad
          
               angle = deg_wrtb; //*180./M_PI;
         }
      }
      //Making sure angle is only between 0 and pi
      if (angle < 0.0) angle = -1.0*angle;
      if (angle > M_PI) angle = 2.0*M_PI - angle; 
      return angle*180./M_PI; // Degrees
   }

   virtual double GetTrueAngleLowTpi() const {
     int nFSpi = GetInt("mc_nFSPart");
     double angle = 9999.; //WRTbeam and in degrees
     double pionKE = 9999.;
     int idk = -9999;
     for (int i = 0; i < nFSpi; i++){
         int pdg = GetVecElem("mc_FSPartPDG",i);
         if(pdg != 211) continue;
         double energy = GetVecElem("mc_FSPartE", i);
         double mass = 139.569;
         double tpi = energy - mass;
         if (tpi <= pionKE) {
               pionKE = tpi;
               TVector3 pimomentumvec(GetVecElem("mc_FSPartPx", i), GetVecElem("mc_FSPartPz", i),GetVecElem("mc_FSPartPz", i));
               double deg_wrtb = thetaWRTBeam(pimomentumvec.X(), pimomentumvec.Y(), pimomentumvec.Z()); //rad
               angle = deg_wrtb*180./M_PI;
         }
      }

      return angle;
   }
   
  
   virtual double GetTrueHighEpi() const {
     int nFSpi = GetInt("mc_nFSPart");
     double pionE = -1.0;
     double pionKE = -1.0;
     for (int i = 0; i < nFSpi; i++){
          int pdg = GetVecElem("mc_FSPartPDG",i);
          if(pdg != 211) continue;
          double energy = GetVecElem("mc_FSPartE", i);
          double mass = 139.569;
          double tpi = energy - mass;
	  if (tpi >= pionKE){
	     pionKE = tpi;
             pionE = energy;
          }
      } 
      //std::cout << "Printing energy of pion " << pionE << std::endl;
      return pionE; // MeV
   }


  //GetRecoilE is designed to match the NSF validation suite
  //virtual double GetRecoilE() const {
  //  return GetVecElem("recoil_summed_energy", 0);
  //}
  double CalcQ2(const double Enu, const double Emu,
                          const double Thetamu) const {
      double Q2 =
             2.0 * Enu *
             (Emu - sqrt(pow(Emu, 2.0) - pow(M_mu, 2.0)) *
             cos(Thetamu)) - pow(M_mu, 2.0);
      if (Q2 < 0.) Q2 = 0.0;
      return Q2;
  } 


 
  virtual double Getq3() const{
    double eavail = NewRecoilE()/1000.;
    double q2 = GetQ2Reco();
    //std::cout << " Dan's EHad is " << eavail << " Q2 is " << q2 << std::endl;
    double q3mec = sqrt(eavail*eavail + q2);
    //std::cout << "Print TrueEavail: " << GetTrueEAvail() <<  " Print RecoilE: " << GetRecoilE() << " Print q2reco: " << GetQ2Reco() << " Print EAvail: " << GetEavail() << " Print q3: " << q3mec  << std::endl;
    return  q3mec; // Using Hang's q3 definition TODO: check to see if using Hang's definition is ok. //q3mec;
  }
   
  virtual int GetCurrent() const { return GetInt("mc_current"); }

  virtual int GetTruthNuPDG() const { return GetInt("mc_incoming"); }

  virtual double GetMuonQP() const {
    return GetDouble((GetAnaToolName() + "_minos_trk_qp").c_str());
  }

  //Some functions to match CCQENuInclusive treatment of DIS weighting. Name matches same Dan area as before.
  virtual double GetTrueExperimentersQ2() const {
    double Enu = GetEnuTrue()/pow(10,3); //GeV
    double Emu = GetElepTrue()/pow(10,3); //GeV
    double thetaMu = GetThetalepTrue();
    return 4.0*Enu*Emu*pow(sin(thetaMu/2.0),2.0);//GeV^2
  }

  virtual double CalcTrueExperimentersQ2(double Enu, double Emu, double thetaMu) const{
    return 4.0*Enu*Emu*pow(sin(thetaMu/2.0),2.0);//MeV^2
  }

  virtual double GetTrueExperimentersW() const {
    double nuclMass = M_nucleon;
    int struckNucl = GetTargetNucleon();
    if (struckNucl == PDG_n){
      nuclMass=M_n;
    }
    else if (struckNucl == PDG_p){
      nuclMass=M_p;
    }
    double Enu = GetEnuTrue();
    double Emu = GetElepTrue();
    double thetaMu = GetThetalepTrue();
    double Q2 = CalcTrueExperimentersQ2(Enu, Emu, thetaMu);
    return TMath::Sqrt(pow(nuclMass,2) + 2.0*(Enu-Emu)*nuclMass - Q2);
  }

  // Adding Branches for Mehreen's Michels
  virtual int GetNMichels() const{
      return GetInt("FittedMichel_michel_fitPass_sz");
  }

  virtual double GetWeightForOnePiWNeutron() const{
    if (GetTrueEavMinusTpi() > 50) return 1.0;
    if (GetTrueEavMinusTpi() <= 50){
	const int interaction = GetInteractionType();
        if (interaction == 4 || interaction == 1) return 1.0;
	int npionp = 0;
        int npion0 = 0;
        int nkplus = 0;
        int npionm = 0;
        int nkneut = 0;
        int nproton = 0;
        int nneutron = 0;
        bool leadingproton = false;
        bool leadingneutron = false;
        int npart = -1;
	std::vector<int> protonidx;
        std::vector<int> neutronidx;
	std::vector<double> neutE, protE;
	const std::vector<int> fsParts = GetTrueFSPDGCodes();	
        //double weight = 1.3 - 0.8729*GetMuonPT() + 0.2825*GetMuonPT()*GetMuonPT();	
    	for(auto pdgCode: fsParts)
       	{
		npart++;
                if(pdgCode == 211) ++npionp;
                else if(pdgCode == -211) return 1.0; //++npionm;
                else if(pdgCode == 111) return 1.0; //++npion0;
                else if(abs(pdgCode) == 321) return 1.0; //++nkplus;
                else if(pdgCode == 311) return 1.0;
                else if(pdgCode == 2112){
             		++nneutron;
	                neutE.push_back(GetTrueEpiEvent(npart));
                        neutronidx.push_back(npart);
                }
                else if(pdgCode == 2212){
                 	++nproton;
		        protE.push_back(GetTrueEpiEvent(npart));
                        protonidx.push_back(npart);
                }

		if (npionp > 1) return 1.0;	
         }

	 if (npionp == 0 || npionp > 1 || nneutron == 0) return 1.0;
	
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
	     return 1.0;
         }
         else if (nproton == 0 and nneutron > 0){ // If there is a proton and no neutron, 
             leadingproton = false;
             leadingneutron = true;
         }
         else if (protE.back() > neutE.back()){
           leadingproton = true;
           leadingneutron = false;
	   return 1.0;
         }
         else if (protE.back() < neutE.back()){
           leadingproton = false;
           leadingneutron = true;
         }	
	 
	 if (leadingneutron == true && npionp == 1) return 1.3 - 0.8729*GetMuonPT() + 0.2825*GetMuonPT()*GetMuonPT();
	 
    }//end if EavMinusTpi > 75 MeV 
    else return 1.0;

  } 
 
  virtual double GetTpiFromRange(double range) const{
     /*
     char *mweightsfilelocation = std::getenv("UTILPATH");
     std::string f = std::string(mweightsfilelocation) + "TpiRangeFit_allbinTest.root"; // "/TpiRange_Mediangraph.root";
     TFile* file = TFile::Open(f.c_str(),"READONLY"); 
     TGraph* tpirange = nullptr;
     file->GetObject("FitCurve", tpirange);
     //TGraph* tpirange = (TGraph*)file->Get("FitCurve");  
     //tpirange->SetDirectory(nullptr);  
     if (range > 2300.) {
		delete file;
     		tpirange->Clear();	
		delete tpirange;	
		return -9999.;

     }
     double tpiest = tpirange->Eval(range);
     //std::cout << "The Pion Range is " << range << " mm and the estimated Tpi is: " << tpiest << " MeV" << std::endl; 
     delete file;
     tpirange->Clear();
     delete tpirange;
     //file->Close();
     */
     double tpiest = 0.213206*range + 2.70387*sqrt(range);
     return tpiest; //Tpi in MeV

  }

  


  virtual std::vector<int> GetTrueFSPDGCodes() const {
     std::vector<int> pdgcodes =  GetVecInt("mc_FSPartPDG");;
     /*int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
      {
          int pdg = GetVecElem("mc_FSPartPDG", i);
          pdgcodes.push_back(pdg);
      }*/

      return pdgcodes;
  }       
  
  virtual int GetTrueNMichels() const {
     int ntruemichels = 0;
     int nmichels = GetNMichels();
     for (int i =0; i< nmichels; i++)
     {
       
      double datafrac =  GetVecElem("FittedMichel_michel_datafraction", i);
      int fitted = GetVecElem("FittedMichel_michel_fitPass", i);
      if (fitted == 0) continue;
      if (datafrac > 0.5) continue;
      ntruemichels++;

     }

     return ntruemichels;
  }

 virtual bool GetTrueIsFSPartInEvent(const int pdg) const {
     std::vector<int> FSparts = GetVecInt("mc_FSPartPDG"); //GetTrueFSPDGCodes();
     std::vector<int>::iterator it;
     it = std::find(FSparts.begin(), FSparts.end(), pdg);
     if (it != FSparts.end())
     {
	return true;
     }
     else return false;

 }
 

 /*
 virtual double GetTrueWexp() const {
    double  W = pow(M_p, 2.0) -  GetTrueQ2MeV() +
             2.0 * (M_p)*GetTrueEHad();

    W = W > 0 ? sqrt(W) : 0.0;
    return W/1000.;

  }

 virtual double GetWExp() const {
    double W = pow(M_p, 2.0) - GetQ2RecoMeV() +
             2.0 * (M_p)*GetEHad();
    W = W > 0 ? sqrt(W) : 0.0;
    return W/1000.;
  }

 */




 
 virtual double GetTrueIntVtxX() const {return GetVecElem("mc_vtx", 0);}
 virtual double GetTrueIntVtxY() const {return GetVecElem("mc_vtx", 1);}
 virtual double GetTrueIntVtxZ() const {return GetVecElem("mc_vtx", 2);}
 
 virtual bool IsInHexagon(double x, double y, double apothem) const {
   double lenOfSide = apothem * (2 / sqrt(3));
   double slope = (lenOfSide / 2.0) / apothem;
   double xp = fabs(x);
   double yp = fabs(y);

   if ((xp * xp + yp * yp) < apothem * apothem)
       return true;
   else if (xp <= apothem && yp * yp < lenOfSide / 2.0)
       return true;
   else if (xp <= apothem && yp < lenOfSide - xp * slope)
       return true;

   return false;
 }

 virtual bool IsInPlastic() const {
    if (!IsInHexagon(GetVecElem("mc_vtx", 0), GetVecElem("mc_vtx", 1), 1000.0)) return false;  // This is in the calorimeters
    double mc_vtx_z = GetVecElem("mc_vtx", 2);
    if (mc_vtx_z > 8467.0) return false;  // Ditto
    int mc_nuclei = GetInt("mc_targetZ");
    // In the carbon target?  The z is gotten from NukeBinningUtils
    if (fabs(mc_vtx_z - 4945.92) <=
          PlotUtils::TargetProp::ThicknessMC::Tgt3::C / 2 &&
      mc_nuclei == 6)
    return false;
    // Is it in water Targer?
    if (5200 < mc_vtx_z && mc_vtx_z < 5420 && (mc_nuclei == 8 || mc_nuclei == 1))
    return false;
    // Finally, do you have target material?  I'm going to say lead/iron isn't a
    //   // big consideration elsewhere in the detector
    if (mc_nuclei == 26 || mc_nuclei == 82) return false;
    return true; 
 }
 
 virtual double GetDiffractiveWeight() const {
    if (GetInt("mc_intType") != 4) return 1.;
    else if (!IsInPlastic() && !PlotUtils::TargetUtils::Get().InWaterTargetMC(
                            GetTrueIntVtxX(), GetTrueIntVtxY(),
                            GetTrueIntVtxZ(), GetInt("mc_targetZ"))) {
    	 return 1.;
    }
    else{
	 //std::cout << "Printing Diffractive weight for Event Type: " << GetInt("mc_intType") << ". And the weight is " << 1.4368 << std::endl;
	 return 1.4368;
    
    }
 }

 


 virtual int GetTrueNPionsinEvent() const {
     int npion = 0;
     
     int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
      {
          int pdg = GetVecElem("mc_FSPartPDG", i);
          if (pdg == 211) npion++;
      }
     return npion;

 }
 
 virtual int GetTrueNKaonsinEvent() const {
     int npart = 0;
     int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
      {
          int pdg = GetVecElem("mc_FSPartPDG", i);
          if (pdg == 321 ) npart++;
      }
      return npart;
 }


 virtual int GetTrueNNeutralKaonsinEvent() const {
     int npart = 0; 
     int pdgsize = GetInt("mc_nFSPart"); 
     for (int i = 0; i< pdgsize; i++)
      {
          int pdg = GetVecElem("mc_FSPartPDG", i);
          if (pdg == 311 ) npart++;
      }
      return npart; 
  }

 virtual int GetTrueNPi0inEvent() const {
     int npart = 0;
     int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
      {
          int pdg = GetVecElem("mc_FSPartPDG", i);
          if (pdg == 111) npart++;
      } 
      return npart;
 }

 virtual double GetTrueLowestEpiEvent() const{
    double tpi = 99999.;
    double Epi = 9999.;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec;
	    pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
		
	    double pionmomentum = pivec.P(); //TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
	    double pionmass = M_pi;//pivec.M();//TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE) {
			tpi = KE;

			Epi = energy;
	    }
        }	
     return Epi;	

 }

 virtual TVector3 GetTrueLowestPpiEvent() const{
    double tpi = 99999.;
    double Epi = 9999.;
    TVector3 ppi;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i)/1000.;
            double momentumy = GetVecElem("mc_FSPartPy", i)/1000.;
            double momentumz = GetVecElem("mc_FSPartPz", i)/1000.;
            double pionmomentum = TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
            double pionmass = M_pi; //TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE) {
                   tpi = KE;
		   ppi.SetX(momentumx);
		   ppi.SetY(momentumy);
		   ppi.SetZ(momentumz);			
            }
        }
     return ppi;

 }


  virtual double GetTrueLowestEpiEventGEV() const{
     
     return GetTrueLowestEpiEvent()/1000.; // in GeV
  }

 virtual double GetTrueLowestPpiEventGEV() const{
    double tpi = 99999.;
    double Ppi = 9999.;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec; 
	     pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
	    double pionmomentum = pivec.P();//TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
            double pionmass = M_pi; //pivec.M(); //TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE) {
                        tpi = KE;

                        Ppi = pionmomentum;
            }
        }
     return Ppi/1000.;

 }

 virtual double GetTrueThetapiEvent(int i) const{
      TVector3 pimomentumvec(GetVecElem("mc_FSPartPx", i), GetVecElem("mc_FSPartPz", i),GetVecElem("mc_FSPartPz", i));
      double angle_wrtb = thetaWRTBeam(pimomentumvec.X(), pimomentumvec.Y(), pimomentumvec.Z()); //rad
      return angle_wrtb;
 }

 virtual double GetTruePpizEvent(int i) const{
       double y = GetVecElem("mc_FSPartPy", i);
       double z = GetVecElem("mc_FSPartPz", i);
       double pzp = cos( MinervaUnits::numi_beam_angle_rad )*z + sin( MinervaUnits::numi_beam_angle_rad )*y;
       return pzp;
 }

 
 
 virtual double GetTruePpiyEvent(int i) const{
       double y = GetVecElem("mc_FSPartPy", i);
       double z = GetVecElem("mc_FSPartPz", i);
       double pyp = -1.0 *sin( MinervaUnits::numi_beam_angle_rad)*z + cos( MinervaUnits::numi_beam_angle_rad )*y;
       return pyp;
 }

 virtual double GetTruePpiEvent(int i) const{
	//int pdg = GetVecElem("mc_FSPartPDG", i);
        //double energy = GetVecElem("mc_FSPartE", i);
        double momentumx = GetVecElem("mc_FSPartPx", i);
        double momentumy = GetVecElem("mc_FSPartPy", i);
        double momentumz = GetVecElem("mc_FSPartPz", i);
        double pionmomentum = TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
        //double pionmass = TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
        //double KE = energy - pionmass;
	return pionmomentum;
 }

 virtual double GetTrueEpiEvent(int i) const{
        int pdg = GetVecElem("mc_FSPartPDG", i);
        double energy = GetVecElem("mc_FSPartE", i);
        //double momentumx = GetVecElem("mc_FSPartPx", i);
        //double momentumy = GetVecElem("mc_FSPartPy", i);
        //double momentumz = GetVecElem("mc_FSPartPz", i);
        //double pionmomentum = TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
        //double pionmass = TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
        //double KE = energy - pionmass;
        return energy;
 }

 virtual int GetTrueLowestKEPion() const{
    double tpi = 999999.;
    int idx = 9999;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (abs(pdg) != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec; //(momentumx, momentumy, momentumz, energy);
	     pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
	    double pionmomentum = pivec.P(); //TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
	    double pionmass = pivec.M();//TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE){
		tpi = KE;
	        idx = i;
	    }
        }
    return idx;
 }

 virtual int GetTrueEavMinusTpi() const{
     std::vector<int> pdgs = GetVecInt("mc_FSPartPDG");
     int count = std::count(pdgs.begin(), pdgs.end(), 211);
     double tpi = 9999.;
     if (count < 1) return -1.0;
     else  tpi = GetTrueLowestTpiEvent();
     double eavl =  GetTrueEAvail();
     if (tpi < 0 or tpi > eavl) return 0.0;
     else return (eavl - tpi);  
		
 }
 
 virtual int GetTrueEavMinusTpiGEV() const{
    std::vector<int> pdgs = GetVecInt("mc_FSPartPDG");
     int count = std::count(pdgs.begin(), pdgs.end(), 211);
     double tpi = 9999.;
     if (count < 1) return -1.0;
     else  tpi = GetTrueLowestTpiEventGEV();
     double eavl =  GetTrueEAvailGEV();
     if (tpi < 0 or tpi > eavl) return 0.0;
     else return (eavl - tpi);
 }

 virtual int GetTrueHighestKEPion() const{
    double tpi = -9999.;
    int idx = 9999;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (abs(pdg) != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec;//(momentumx, momentumy, momentumz, energy);
	     pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
	    double pionmomentum = pivec.P();//TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
            double pionmass = pivec.M();//TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - M_pi;
            if (tpi < KE){
                tpi = KE;
                idx = i;
            }
        }
    return idx;
 }

 virtual double GetTrueLowestTpiEvent() const {
    double tpi = 999999.;
    int pdgsize = GetInt("mc_nFSPart");
    std::vector<int> pdgs = GetVecInt("mc_FSPartPDG");
    int count = std::count(pdgs.begin(), pdgs.end(), 211);
    if (count < 1) return -1.0; 
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
	    double energy = GetVecElem("mc_FSPartE", i);
	    double momentumx = GetVecElem("mc_FSPartPx", i);
	    double momentumy = GetVecElem("mc_FSPartPy", i);
	    double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec; //(momentumx, momentumy, momentumz, energy);
             pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);	
	    double pionmomentum = pivec.P(); //TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
	    double pionmass = M_pi; //TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));  
	    double KE = energy - pionmass;
	    if (tpi > KE) tpi = KE;     
        }
    ///if(tpi < 99999.) std::cout << "Lowest Energy Primary Pion KE is " << tpi << std::endl;
    return tpi;
 } 

  virtual double GetTrueLowestTpiEventGEV() const {
	 double tpi = 999999.;
    int pdgsize = GetInt("mc_nFSPart");
    for (int i = 0; i< pdgsize; i++)
        {   
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i); 
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i); 
            TLorentzVector pivec;//(momentumx, momentumy, momentumz, energy);
	     pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
            double pionmomentum = pivec.P(); //TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
            double pionmass = M_pi; //pivec.M();//TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE) tpi = KE;
        }	
       return tpi/1000.; //GeV
  } 

  virtual int GetTrueLowestTpiIdx() const {
    double tpi = 999999.;
    int pdgsize = GetInt("mc_nFSPart");
    int idx = 9999;
    for (int i = 0; i< pdgsize; i++)
        {
            int pdg = GetVecElem("mc_FSPartPDG", i);
            if (pdg != 211) continue;
            double energy = GetVecElem("mc_FSPartE", i);
            double momentumx = GetVecElem("mc_FSPartPx", i);
            double momentumy = GetVecElem("mc_FSPartPy", i);
            double momentumz = GetVecElem("mc_FSPartPz", i);
            TLorentzVector pivec;//(momentumx, momentumy, momentumz, energy);
	    pivec.SetPxPyPzE(momentumx, momentumy, momentumz, energy);
	    double pionmomentum = pivec.P();//TMath::Sqrt(pow(momentumx, 2) + pow(momentumy,2)+pow(momentumz,2));
            double pionmass = M_pi;//pivec.M();//TMath::Sqrt(pow(energy, 2) - pow(pionmomentum, 2));
            double KE = energy - pionmass;
            if (tpi > KE) {
		tpi = KE;
		idx = i;
	    }
        }
       return idx; //returns index of lowest energy pion
  }
 
  virtual int GetTrueNPions() const{
      return GetInt("truth_FittedMichel_all_piontrajectory_pdg_sz");
  }

  virtual int GetTrueNPiPlus() const{
	int npiplus = 0;
	int npi = GetTrueNPions();
	for (int i = 0; i < npi; i++){
	    int pdg = GetVecElem("truth_FittedMichel_all_piontrajectory_pdg", i);
	    if (pdg == 211) npiplus++;
	
	}
	return npiplus;
  }

  virtual int GetPionParentID(int i) const {
     return GetVecElem("truth_FittedMichel_all_piontrajectory_ParentID", i);
  }

  virtual int GetPionPDG(int i) const{
     return GetVecElem("truth_FittedMichel_all_piontrajectory_pdg", i);
  }

  virtual double GetPionE(int i) const{
     return GetVecElem("truth_FittedMichel_all_piontrajectory_energy",i)/pow(10,3);
  }

  virtual double GetPionP(int i) const{
    return GetVecElem("truth_FittedMichel_all_piontrajectory_momentum", i)/pow(10,3);
  }

  virtual double GetPionMass(int i) const{
    double pionmass = sqrt(pow(GetPionE(i), 2) - pow(GetPionP(i), 2));
    return pionmass;
  }

  virtual double GetPionKE(int i) const{
    double pionKE = GetPionE(i) - GetPionMass(i);
  }

  virtual double thetaXWRTBeam(double x, double y, double z) const{

  double pzp = cos( MinervaUnits::numi_beam_angle_rad)*z + sin( MinervaUnits::numi_beam_angle_rad)*y;
  int sign = (x<0.) ? -1 : 1;
  double denom2 = pow(x,2) + pow(pzp,2);
  if( 0. == denom2 ) return -999.;
  else
    return sign * acos(pzp / sqrt(denom2));
  }

  


  virtual double thetaWRTBeam(double x, double y, double z) const{
      double pyp = -1.0 *sin( MinervaUnits::numi_beam_angle_rad)*z + cos( MinervaUnits::numi_beam_angle_rad )*y;
      double pzp = cos( MinervaUnits::numi_beam_angle_rad )*z + sin( MinervaUnits::numi_beam_angle_rad )*y;
      double denom2 = pow(x,2) + pow(pyp,2) + pow(pzp,2);
      if( 0. == denom2 ) return -9999.;
      else return acos(pzp / sqrt(denom2) );
  }

  virtual double phiWRTBeam(double x, double y, double z) const{
      double pyp = -1.0 *sin( MinervaUnits::numi_beam_angle_rad)*z + cos( MinervaUnits::numi_beam_angle_rad )*y;
      double phi = atan2(pyp, x);

  }
 

  virtual double GetTrueTpi() const {
     int nFSpi = GetTrueNPions();
     double pionKE = 9999.;
     for (int i = 0; i < nFSpi; i++){
         int pdg = GetVecElem("truth_FittedMichel_all_piontrajectory_pdg", i);
         int pitrackid = GetVecElem("truth_FittedMichel_all_piontrajectory_ParentID", i);

         double energy = GetVecElem("truth_FittedMichel_all_piontrajectory_energy",i);
         double p = GetVecElem("truth_FittedMichel_all_piontrajectory_momentum", i);
         double mass = sqrt(pow(energy,2) - pow(p, 2));
         double tpi = energy - mass;
         if (tpi <= pionKE) pionKE = tpi;
      }
       
      return pionKE;
   }
 virtual void PrintTrueArachneLink() const {
  int link_size = 200;
  char link[link_size];
  int run = GetInt("mc_run");
  int subrun = GetInt("mc_subrun");
  int gate = GetInt("mc_nthEvtInFile") + 1;
  int slice = GetVecElem("slice_numbers", 0);
  sprintf(link,
          "https://minerva05.fnal.gov/Arachne/"
          "arachne.html\?det=SIM_minerva&recoVer=v21r1p1&run=%d&subrun=%d&gate="
          "%d&slice=%d",
          run, subrun, gate, slice);
       	  std::cout << link << std::endl;
       	  std::cout << "Lepton E: " <<  GetElepTrueGeV() << " Run " << run << "/"<<subrun << "/" << gate << "/" << slice << std::endl;
	  std::cout << "Printing Available Energy " << NewEavail() << std::endl;
	  std::cout << "Print True Available Energy " << GetTrueEAvail() << std::endl; 
	  std::cout << "Muon P: " << GetMuonP() << std::endl;
	  std::cout << "Get Muon Pt: " << GetMuonPT() << std::endl;
	  std::cout << "Get Muon Pz: " << GetMuonPz() << std::endl;
	  std::cout << "Get Muon PT True " << GetMuonPTTrue() << std::endl;
 	  std::cout << "Get Muon E True " << GetElepTrueGeV() << std::endl;
	  std::cout << "Get True Lowest Epi in Event:" << GetTrueLowestEpiEvent() << std::endl;
	  std::cout << "Interaction Type: " << GetInt("mc_intType") << std::endl;   
 }

 virtual void PrintDataArachneLink() const {
  int link_size = 200;
  char link[link_size];
  int run = GetInt("ev_run");
  int subrun = GetInt("ev_subrun");
  int gate = GetInt("ev_gate");
  int slice = GetVecElem("slice_numbers", 0);
  sprintf(link,
          "https://minerva05.fnal.gov/Arachne/"
          "arachne.html\?det=MV&recoVer=v21r1p1&run=%d&subrun=%d&gate="
          "%d&slice=%d",
          run, subrun, gate, slice);
        std::cout << link << std::endl;
	std::cout << " Run " << run << "/"<<subrun << "/" << gate << "/" << slice << std::endl;
        
	

	//if(NewEavail() < 50. && GetMuonPT() < .20){
        //std::cout << link << std::endl;
       // std::cout << "Lepton E: " <<  GetMuonPT() << " Run " << run << "/"<<subrun << "/" << gate << "/" << slice << std::endl;
       //}

 }

 int GetFittedMichelsOnly() const
 {
   std::vector<int> nfitted = GetVecInt("FittedMichel_michel_fitPass");
   int count = std::count(nfitted.begin(), nfitted.end(), 1);
   //std::cout << "THERE ARE " << count << " N FITTED MICHELS IN EVENT " << std::endl;
   return count;
 }


 virtual int GetNonMuonClusters() const
 {
    std::vector<int> ismuclus = GetVecInt("cluster_isMuontrack");
    int count = std::count(ismuclus.begin(), ismuclus.end(), 0);
    //std::cout << "THERE ARE " << count << " N NON MUON CLUSTERS IN EVENT " << std::endl;
    return count;
 }

 virtual int GetNTrackerClusters() const{
    std::vector<int> nsubdet = GetVecInt("cluster_subdet");
    int count2 = std::count(nsubdet.begin(), nsubdet.end(), 2);
    return count2;
 } 

 virtual int GetNECALClusters() const{
    std::vector<int> nsubdet = GetVecInt("cluster_subdet");
    int count2 = std::count(nsubdet.begin(), nsubdet.end(), 3);
    return count2;
 }

 double GetNClusters() const
 {
  int nclusters = GetInt("cluster_view_sz");
  int nonmuclus = 0;
  //for (int i = 0; i < nclusters; i++){
  //      int ismuon = GetVecElemInt("cluster_isMuontrack", i);
  //      if (ismuon != 0) continue;
  //	nonmuclus++;

  //}
  double nclus = 1.0*nclusters;
  return nclus;//nonmuclus;
 
 }

 virtual double GetClusterEnergyTracker() const
 {
    double totenergy = GetDouble("cluster_trackerEsum"); //0.0;
    /*
    for (int i = 0; i < nclusters; i++){
	int ismuon = GetVecElemInt("cluster_isMuontrack", i);
	if (ismuon != 0) continue;
    	double time = GetVecElem("cluster_time", i); 
        double vtxtime = GetVertex().T(); //mus
    	double timediff = vtxtime - time;
    	//int ismuon = GetVecElemInt("cluster_isMuontrack", i);
    	double ecal = GetVecElem("cluster_ecalo", i);
	int subdet = GetVecElem("cluster_subdet", i);
	if (subdet != 2) continue; 
   	//if (ismuon !=0) continue; // check to make sure cluster is not on muon track, 0 is not muon, 1 is muon
        if (timediff < -20. || timediff > 30.) continue;
        totenergy += ecal;     
    }
    */
    return totenergy;
 }

 virtual double GetClusterEnergyECAL() const
 {
    double totenergy = GetDouble("cluster_ecalEsum");//0.0;
    /*
    for (int i = 0; i < nclusters; i++){
 	int ismuon = GetVecElemInt("cluster_isMuontrack", i);
	if (ismuon != 0) continue;
        double time = GetVecElem("cluster_time", i); //mus
        double vtxtime = GetVertex().T(); //mus
        double timediff = vtxtime - time;
        //int ismuon = GetVecElemInt("cluster_isMuontrack", i);
        double ecal = GetVecElem("cluster_ecalo", i);
        int subdet = GetVecElem("cluster_subdet", i);
        if (timediff < -20. || timediff > 30.) continue;
        if (subdet != 3) continue;
        //if (ismuon !=0) continue; // check to make sure cluster is not on muon track, 0 is not muon, 1 is muon
        totenergy += ecal;
    }
    */
    return totenergy;
 }

 virtual std::vector<double> GetTrackerECALMuFuzz() const
 {
   double trk_mufuzz = 0.0;
   double ecal_mufuzz = 0.0;
   int nfuzz = GetInt("muon_fuzz_per_plane_r80_planeIDs_sz");
   if (nfuzz == 0) return {0.0, 0.0};
   for (int i =0; i < nfuzz; i++){
        int planeID = GetVecElem("muon_fuzz_per_plane_r80_planeIDs", i);
        if (planeID < 1504968704 || planeID > 1709703168) continue;
        double fuzze = GetVecElem("muon_fuzz_per_plane_r80_energies", i);
        if (planeID > 1504968704 and planeID < 1560805376) trk_mufuzz += fuzze;
        else if (planeID > 1700003840 and planeID < 1709703168) ecal_mufuzz += fuzze;
    }  
   
   return {trk_mufuzz, ecal_mufuzz};

 }

 
 
 virtual double GetTrackerMuFuzz() const
 {
    double mufuzz = 0;
    int nfuzz = GetInt("muon_fuzz_per_plane_r80_planeIDs_sz");
    if (nfuzz == 0) return 0.0;
    for (int i =0; i < nfuzz; i++){
	int planeID = GetVecElem("muon_fuzz_per_plane_r80_planeIDs", i);
	double fuzze = GetVecElem("muon_fuzz_per_plane_r80_energies", i);
 	if (planeID < 1504968704 || planeID > 1560805376) continue; //sum fuzz for only in Tracker
	mufuzz += fuzze;
    }
    return mufuzz;
 } 

 virtual double GetECALMuFuzz() const
 {
    double mufuzz = 0;
    int nfuzz = GetInt("muon_fuzz_per_plane_r80_planeIDs_sz");
    if (nfuzz == 0) return 0.0;
    for (int i =0; i < nfuzz; i++){
        int planeID = GetVecElem("muon_fuzz_per_plane_r80_planeIDs", i);
        double fuzze = GetVecElem("muon_fuzz_per_plane_r80_energies", i);
        if (planeID < 1700003840 || planeID > 1709703168) continue; //sum fuzz for only in ECAL
        mufuzz += fuzze;
    }
    return mufuzz;
 }

 virtual double NewEavail() const
 {
    double recoiltracker =  GetDouble("blob_recoil_E_tracker") - GetTrackerECALMuFuzz()[0];  //GetTrackerMuFuzz(); 
    //double recoiltracker = GetClusterEnergyTracker() - GetTrackerECALMuFuzz()[0]; // GetTrackerMuFuzz();
    double recoilEcal = GetDouble("blob_recoil_E_ecal") - GetTrackerECALMuFuzz()[1]; //GetECALMuFuzz();
    //double recoilEcal = GetClusterEnergyECAL() - GetTrackerECALMuFuzz()[1]; //GetECALMuFuzz();
    const double Eavailable_scale = 1.17;
    double eavail = recoiltracker + recoilEcal;
    return eavail*Eavailable_scale;
 }

 virtual double NewEavailGEV() const
 {
    return NewEavail()/1000.;
 }

 virtual double NewRecoilE() const
 {

    double recoil = NewEavail() + M_pi;
    double newrecoil = recoil;
    //if (recoil > 75. and recoil < 300.) newrecoil = recoil - 75.;
    //else if(recoil > 300. and recoil < 600.) newrecoil = recoil - 25.;
    //else if(recoil > 600. and recoil < 1000.) newrecoil = recoil + 25.;
    //else if(recoil > 1000.) newrecoil = recoil + 175.;
    return  newrecoil;

 } 

 virtual double GetTrueEAvailGEV() const
 {
     double Eavail = 0.0;
     int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
     {
        int pdg = GetVecElem("mc_FSPartPDG", i);
        double energy = GetVecElem("mc_FSPartE", i);
 	if (pdg == 211 || pdg == -211) Eavail+= energy - 139.57; // subtracting pion mass to get Kinetic energy
        else if (pdg == 2212) Eavail += energy - 938.28; // proton
        else if (pdg == 111) Eavail += energy; // pi0
        else if (pdg == 22) Eavail += energy; // photons
        else if (pdg == 311) Eavail += energy - 497.611/2.0; // K0 ???? - Kaon rest mass / 2
        else if (abs(pdg) == 321) Eavail += energy - 497.611/2.0; // Kaon+ Kinetic Energy  divide by Kmass/2 
        else if (pdg == 221) Eavail += energy - 547.862; //Adding etas

     }
     
     return Eavail/1000.;//in GEV


 } 


 virtual double GetTrueEAvail() const
 {
     double Eavail = 0.0;
     int pdgsize = GetInt("mc_nFSPart");
     for (int i = 0; i< pdgsize; i++)
     {
        int pdg = GetVecElem("mc_FSPartPDG", i);  
        double energy = GetVecElem("mc_FSPartE", i); // hopefully this is in MeV
        /* 
        if(abs(pdg) == 211){ // charged pions (kinetic energy)
          double kinetic_energy = energy - M_pi;
          Eavail += kinetic_energy;
        } 
        else if (abs(pdg) == 311 or pdg == 321){
	  // Add Kaons
	  double KE = energy - 497.611/2.0;
	  Eavail += KE;
        }
        else if (pdg == 2212){ // protons (kinetic energy)
          double kinetic_energy = energy - M_p;
	  Eavail += kinetic_energy;
        }
        else if ((pdg == 111) or (abs(pdg) == 11) or (pdg == 22)){
	  // neutral pions, photons, electrons and positrons (total energy)
	  Eavail += energy;
      	} 
        else if (pdg > 1000000000){
          // do nothing for nucleons, assume negligible,
          Eavail += 0;
        }
        else if (pdg >= 2000){
 	 //Strange Baryons
         double kinetic_energy = energy - M_p;
	 Eavail += kinetic_energy;
        }
        else if (pdg <= -2000){
	 // antibaryons, primarily -2112 and -2212s
	 Eavail += energy + M_p;
	}
        else { // Get any other particle. strange mesons?
   	 Eavail += energy;
        }
        */	  
        if (pdg == 211 || pdg == -211) Eavail+= energy - 139.57; // subtracting pion mass to get Kinetic energy
        else if (pdg == 2212) Eavail += energy - 938.28; // proton
        else if (pdg == 111) Eavail += energy; // pi0
        else if (pdg == 22) Eavail += energy; // photons
        else if (pdg == 311) Eavail += energy - 497.611/2.0; // K0 ???? - Kaon rest mass / 2
        else if (abs(pdg) == 321) Eavail += energy - 497.611/2.0; // Kaon+ Kinetic Energy  divide by Kmass/2 
        else if (pdg == 221) Eavail += energy - 547.862; //Adding etas subtracting their mass (not adding eta prime yet tho)
	//else if (pdg == 3222) Eavail += energy - 1115.683; // mass of Lambda 
	//else if (pdg == 3122) Eavail += energy - 1189.37 ; // mass of Sigma
        	
        //else if (abs(pdg) !=  2112) Eavail += energy; //Adding anything else except neutrons.  
       
     }
     //std::cout << "True Eavail is " << Eavail << " --- Reco Eavail is " << GetEavail() << "New EAvail is " << GetNewEavail() << std::endl;
     return Eavail; // in MeV
 }


 


  //Still needed for some systematics to compile, but shouldn't be used for reweighting anymore.
  protected:
  #include "PlotUtils/WeightFunctions.h" // Get*Weight
};

#endif
