#ifndef MNV_NUKEHISTOGRAMUTILS_h
#define MNV_NUKEHISTOGRAMUTILS_h 1

#include "NukeBinningUtils.h"
#include "NukeFunctionUtils.h"
#include "NukeCCPionCuts.h"

#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/Hist2DWrapper.h"

#include "ROOTinclude.h"

class SidebandType1Pi;
class CVUniverse;
//class SidebandType1Pi;
#ifndef __CINT__
using UniverseMap        = std::map<std::string, std::vector<CVUniverse*>>; // convenience
using SidebandType1PiMap = std::map<std::string, std::vector<SidebandType1Pi*>>;
#endif

template <typename T>
struct VtxHists
{
  std::vector< T > vtx_x;
  std::vector< T > vtx_y;
  std::vector< T > vtx_z;

  std::vector< T > per_c;
  std::vector< T > per_x;
  std::vector< T > per_u;
  std::vector< T > per_d;

  std::vector< T > per_c_sb;
  std::vector< T > per_x_sb;
  std::vector< T > per_u_sb;
  std::vector< T > per_d_sb;
};

template <typename T>
struct EffHist
{
  T numerator;
  T denominator;  
};

namespace PlotUtils
{
  class ChainWrapper;
  class MnvH1D;
  class MnvH2D;
}

//namespace MinervaUnfold
//{
//  class MnvResponse;
//}

class NukeHistogramUtils {
  public:

    //enum HistoTypes{ kData = 0,
    //                 kMC   = 1
    //};
  
    //! Default Constructor
    NukeHistogramUtils( int targets, int nuclei, int plastic, bool bWaterTarget = false, bool bUseScintNTR = false, bool bUseDaisy = false ) :
      m_targetsFlag(targets),
      m_nucleiFlag( nuclei),
      m_plasticFlag(plastic),
      m_useWaterTarget(bWaterTarget),
      m_classification(""), 
      m_SetMnvUni(true),
      m_MCBkgdOnly(false),
      m_IncludeNucleusOutside(false),
      m_bkgd_fit_enum(3)
      {
        histo_cuts = new NukeCCPionCuts( targets, nuclei, plastic, bWaterTarget, bUseScintNTR, bUseDaisy );
      };//,
    
    //! singleton gettor
    //static NukeHistogramUtils& Get();
  
    void setMCBkgdOnly( bool MCBkgdOnly ) { m_MCBkgdOnly = MCBkgdOnly; }
    void setIncludeNucleusOutside( bool IncludeNucleusOutside ) { m_IncludeNucleusOutside = IncludeNucleusOutside; }

    void setAllClassify(string classify) { m_classification = classify; }
    
    string getAllClassify() { return m_classification; }

    //! get the base names of the requested histograms to book
    std::vector< string > getHistosNames( string title, string tar_spec = "" );
   
    //! get the number of requested histograms to book 
    int getNHistos( string title, string tar_spec = "" );
  
    // !get sidebands related to the systematic bands 
    SidebandType1PiMap getSideband1PiMap( UniverseMap syst_bands, NukeCCPionCuts* cuts, bool useVertexZ = true ); 

    // !get systematic errors by bands 
    UniverseMap getSystematicUniversesMap(PlotUtils::ChainWrapper* chw, 
                                          std::string flux_playlist,
                                          bool is_truth = false,
                                          bool do_full_systematics = false,
                                          int  iSBWeight  = 9999,
                                          bool bNeutronCV = true, 
                                          bool bElastics  = true);
    //! get systematic universe
    std::vector<CVUniverse*> getSystematicUniverses( PlotUtils::ChainWrapper* chw ); 
    
    //! book PlotUtils::MnvH1D histograms from the file
    void bookHistos( TFile* file, PlotUtils::MnvH1D** h, string var_name, string title );

    //! book PlotUtils::MnvH1D histograms from the file
    void bookHistos( TFile* file, PlotUtils::MnvH1D** h, int nhistos, string var_name );

    //! book new PlotUtils::MnvH1D histograms
    void bookHistos( PlotUtils::MnvH1D** h, string var_name, string title, int nbins, double min, double max );
    
    //! book new PlotUtils::MnvH1D histograms
    void bookHistos( PlotUtils::MnvH1D** h, string var_name, string title, nuke_axis_binning xbins );
    
    //! book new PlotUtils::MnvH1D histograms
    void bookHistos( PlotUtils::MnvH1D** h, int nhistos, string var_name, string title, nuke_axis_binning xbins );

    //! book new PlotUtils::MnvH1D histograms
    void bookHistos( PlotUtils::MnvH1D** h, string var_name, std::vector<string> names, string title, nuke_axis_binning xbins );

    //! book new PlotUtils::MnvH2d histograms
    void bookHistos( PlotUtils::MnvH2D** h, string var_name, string title, nuke_axis_binning xbins, nuke_axis_binning ybins );

    void bookResponses( PlotUtils::MnvH2D** h, string var_name, string title, nuke_axis_binning xbins, nuke_axis_binning ybins );

    //! easy way to simplify setup.  Only works on nukebins for right now
    std::vector< PlotUtils::MnvH1D** > setupHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, nuke_axis_binning bins);
    std::vector< PlotUtils::MnvH1D** > setupHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, std::vector<nuke_axis_binning> bins);
    //! easy way to simplify setup.  Only works on nukebins for right now
    std::vector< PlotUtils::MnvH2D** > setupHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, nuke_axis_binning bins1, nuke_axis_binning bins2);
    std::vector< PlotUtils::MnvH2D** > setupHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, std::vector<nuke_axis_binning> bins1, std::vector<nuke_axis_binning> bins2);

    std::vector< PlotUtils::MnvH2D** > setupResponses(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, nuke_axis_binning bins1, nuke_axis_binning bins2);

    //! book efficiency histogram using the EffHist struct
    std::vector< EffHist< PlotUtils::MnvH1D** > > setupEffHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, nuke_axis_binning bins);
    std::vector< EffHist< PlotUtils::MnvH1D** > > setupEffHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, std::vector<nuke_axis_binning> bins);
    std::vector< EffHist< PlotUtils::MnvH2D** > > setupEffHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, nuke_axis_binning bins1, nuke_axis_binning bins2);
    std::vector< EffHist< PlotUtils::MnvH2D** > > setupEffHists(int nTypes, int nHistograms, string name, std::vector<string> tnames, string title, std::vector<nuke_axis_binning> bins1, std::vector<nuke_axis_binning> bins2);

    //! Get the HistWrapper from the list of histograms...
    //! NOTE:  BY setting the default to getting all histograms, we're solving/pushing a problem with fill1DHistwrappers down the road
    std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > >   getHistWrapper( std::vector< PlotUtils::MnvH1D** > h, std::vector<CVUniverse*> &sys_uni, bool bAllHists = true );
    std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > getHistWrapper( std::vector< PlotUtils::MnvH2D** > h, std::vector<CVUniverse*> &sys_uni, bool bAllHists = true );

    std::vector< std::vector< PlotUtils::HistWrapper<CVUniverse>* > >   getHistWrapper( std::vector< PlotUtils::MnvH1D** > h, std::map< std::string,std::vector<CVUniverse*> > &sys_band, bool bAllHists = true );
    std::vector< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > getHistWrapper( std::vector< PlotUtils::MnvH2D** > h, std::map< std::string,std::vector<CVUniverse*> > &sys_band, bool bAllHists = true );

    std::vector< EffHist< std::vector< PlotUtils::HistWrapper<CVUniverse>* > > >   getEffHistWrapper( std::vector< EffHist< PlotUtils::MnvH1D** > > h, 
                                                                                                      UniverseMap &mc_sys_band, 
                                                                                                      UniverseMap &truth_sys_band, 
                                                                                                      //std::map< std::string,std::vector<CVUniverse*> > &sys_band, 
                                                                                                      bool bAllHists = false );
    std::vector< EffHist< std::vector< PlotUtils::Hist2DWrapper<CVUniverse>* > > >   getEffHistWrapper( std::vector< EffHist< PlotUtils::MnvH2D** > > h, 
                                                                                                        UniverseMap &mc_sys_band, 
                                                                                                        UniverseMap &truth_sys_band, 
                                                                                                        //std::map< std::string,std::vector<CVUniverse*> > &sys_band,
                                                                                                        bool bAllHists = false );

    //! get the merged files input files
    std::vector<TString> getGridInputFiles( std::string input_dir, int process, std::string &run_str, bool bGridTest = false );

    //! add single hists to the list
    void addHistToList(PlotUtils::MnvH1D* h) {  m_histList.Add(h); }
    void addHistToList(PlotUtils::MnvH2D* h) {  m_histList.Add(h); }

    //! easy way to save hists, if the hists are setup using setupHists
    void writeHists(TString outname, bool bMCBkgd = false );

    //!sync all HistWrappers
    void syncHistWrappers();

    //! make a histograms of POT.  Put POT in each bin
    void makePOTHists( TChain *meta, std::string datamc, std::string modifier = "" );

    //! add the PlotUtils::MnvH1D systematic error bands
    void addErrorBands( PlotUtils::MnvH1D** h, int nhistos );

    //! add the PlotUtils::MnvH2D systematic error bands
    void addErrorBands( PlotUtils::MnvH2D** h, int nhistos );

    //! used to debug cuts
    void debugCuts( std::map<string, bool>& Cuts ); 

    PlotUtils::MnvH1D* subtractBkgdFromData( PlotUtils::MnvH1D* h_data, PlotUtils::MnvH1D* h_bkgd, double pot_scale );
    PlotUtils::MnvH2D* subtractBkgdFromData( PlotUtils::MnvH2D* h_data, PlotUtils::MnvH2D* h_bkgd, double pot_scale );

    //! fill PlotUtils::MnvH1D** histogram with index
    void fillHisto( PlotUtils::MnvH1D** h, double var, double wgt=1, int index=0);

    //! fill PlotUtils::MnvH1D** data histogram (index=0)
    void fillDataHisto( PlotUtils::MnvH1D** h, double var, double wgt=1);
   
    //! fill PlotUtils::MnvH2D** histogram with index
    void fillHisto( PlotUtils::MnvH2D** h, double var1, double var2, double wgt=1, int index=0);

    //! fill PlotUtils::MnvH2D** data histogram (index=0)
    void fillDataHisto( PlotUtils::MnvH2D** h, double var1, double var2, double wgt=1);
   
    //! fill PlotUtils::MnvH1D** with cuts 
    void fill1DHistos( PlotUtils::MnvH1D** h, string classification, map< string, bool> &Cuts, double var, double wgt = 1.0 );

    //! fill PlotUtils::MnvH2D** with cuts 
    void fill2DHistos( PlotUtils::MnvH2D** h, string classification, map< string, bool> &Cuts, double var1, double var2, double wgt = 1.0 );

    ////! fill a vector of HistWrappers with cuts 
    void fill1DHistwrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, string classification, map< string, bool> &Cuts, double var, double wgt = 1.0 );

    ////! fill a vector of HistWrappers with cuts 
    void fill2DHistwrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, string classification, map< string, bool> &Cuts, double var1, double var2, double wgt = 1.0 );

    //! fill PlotUtils::MnvH1D** with cuts 
    void fillNukeCutsHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0);     

    //! fill MC/BKGD  HistWrappers
    void fillMCHistWrappers( std::vector< PlotUtils::HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var, double wgt, double wgt_bkgd, bool bSignalOnePi = true );
    void fillMCHistWrappers( std::vector< PlotUtils::Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var1, double var2, double wgt, double wgt_bkgd, bool bSignalOnePi = true );

    void fillResponseWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, PlotUtils::MnvH2D** h, CVUniverse* cv, int index, double reco_var1, double reco_var2, double true_var1, double true_var2, double wgt, double wgt_bkgd, bool bSignalOnePi = true );

    void fillHistWrappersAndClassifyHistos( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, int iProng = -1, bool bSignalOnePi = true );

    //! fill Classify HistWrappers
    void fillClassifyHistWrappers( std::vector< HistWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var, double wgt, double wgt_bkgd, int iProng = -1, bool bSignalOnePi = true );
    void fillClassifyHistWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, int index, double var1, double var2, double wgt, double wgt_bkgd, int iProng = -1, bool bSignalOnePi = true );

    ////! fill data vertex position hists 
    void fillDataVtxHistos( VtxHists<MnvH1D**> vtx_hists, int index, int_vtx_info int_vtx, int plastic );    
 
    ////! fill MC vertex position HistWrappers 
    void fillMCVtxHistWrappers( VtxHists< std::vector< HistWrapper<CVUniverse>* > > vtx_hws, CVUniverse* cv, int index, int_vtx_info int_vtx, double wgt, double wgt_bkgd, bool bSignalOnePi, int plastic );    
 
    //! fill Hist2DWrappers
    //void fillHist2DWrappers( std::vector< Hist2DWrapper< CVUniverse >* > &hws, CVUniverse* cv, double var1, double var2, double wgt );
    
    //! fill with inputted classifications
    void fillClassifyHistos( PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, 
                               PlotUtils::ChainWrapper* chw, int ievent, int iProng = -1, bool bSignalOnePi = true, int MichelPDG=0, bool fill_outside_fid_histos = false );
    
    void fillClassifyHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, double wgt, double wgt_bkgd, 
                               PlotUtils::ChainWrapper* chw, int ievent, int iProng = -1, bool bSignalOnePi = true, int MichelPDG=0,bool fill_outside_fid_histos = false );


    void fillClassifyCVHistos( PlotUtils::MnvH1D** h, int index, double var, double wgt, double wgt_bkgd, CVUniverse* cv, int iProng = -1, bool bSignalOnePi = true );
    void fillClassifyCVHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, double wgt, double wgt_bkgd, CVUniverse* cv, int iProng = -1, bool bSignalOnePi = true );

    //! fill HistWrappers with channel cuts
    void fillEventChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse* cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    //! fill HistWrappers with channel cuts
    void fillEventChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse* cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill PlotUtils::MnvH1D** with channel cuts
    void fillEventChannelHistos( PlotUtils::MnvH1D** h, int index, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    void fillEventChannelHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill PlotUtils::MnvH1D** with channel cuts
    void fillEventChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    void fillEventChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    
    //! fill HistWrappers with channel cuts (similar to ben) 
    void fillPiChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    void fillPiChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill PlotUtils::MnvH1D** with channel cuts (similar to ben)
    void fillPiChannelHistos( PlotUtils::MnvH1D** h, int index, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    void fillPiChannelHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill PlotUtils::MnvH1D** with channel cuts (similar to ben)
    void fillPiChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    void fillPiChannelHistos( PlotUtils::MnvH2D** h, int index, double var1, double var2, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    
    void fillPhaseSpaceHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);
    
    void fillBackgroundHistos( PlotUtils::MnvH1D** h, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi=true);
    
    //! fill HistWrappers with targets/plastic signal, W, and other bkgd
    void fillTarPlasticHistos( std::vector< HistWrapper< CVUniverse >* > &hws, int index, double var, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill HistWrappers with targets/plastic signal, W, and other bkgd
    void fillTarPlasticHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, int index, double var1, double var2, CVUniverse *cv, double wgt = 1.0, 
                                  double wgt_bkgd=1.0, bool bSignalOnePi =true);

    //! fill HistWrappers with W categories 
    void fillWHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int index,  double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi =true  );
    void fillWHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int index,  double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi =true  );

    //! fill PlotUtils::MnvH1D** with W categories
    void fillWHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper* chw, int ievent, int index, double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi =true  );
    void fillWHistos( PlotUtils::MnvH1D** h, double var, CVUniverse *cv, int index,  double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi =true  );

    void fillWHistos( PlotUtils::MnvH2D** h, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, int index, double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi = true );
    void fillWHistos( PlotUtils::MnvH2D** h, double var1, double var2, CVUniverse *cv, int index, double wgt = 1.0, double wgt_bkgd = 1.0, bool bSignalOnePi = true  );

    //! fill HistWrappers with production channel cuts 
    void fillProductionChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);
    void fillProductionChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);

    //! fill PlotUtils::MnvH1D** with production channel cuts
    void fillProductionChannelHistos( PlotUtils::MnvH1D** h, double var, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, double wgt_bkgd=1.0);
    void fillProductionChannelHistos( PlotUtils::MnvH1D** h, double var, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);

    //! fill PlotUtils::MnvH2D** with production channel cuts
    void fillProductionChannelHistos( PlotUtils::MnvH2D** h, double var1, double var2, PlotUtils::ChainWrapper* chw, int ievent, double wgt = 1.0, double wgt_bkgd=1.0);
    void fillProductionChannelHistos( PlotUtils::MnvH2D** h, double var1, double var2, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);

    //! fill HistWrappers with nucleon channel cuts 
    void fillNucleonChannelHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);
    void fillNucleonChannelHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int ivec, double wgt = 1.0, double wgt_bkgd=1.0);

    //! fill HistWrappers with number of piplus
    void fillNPiPlusHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int ivec, double wgt = 1.0  );
    void fillNPiPlusHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int ivec, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with number of piplus
    void fillNPiPlusHistos( PlotUtils::MnvH1D** h, double var, std::vector<int>& FS_PDG, double wgt = 1.0  );
    void fillNPiPlusHistos( PlotUtils::MnvH2D** h, double var1, double var2, std::vector<int>& FS_PDG, double wgt = 1.0  );

    //! fill HistWrappers with number of piplus 
    void fillNPiZeroHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse *cv, int ivec, double wgt = 1.0  );
    void fillNPiZeroHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse *cv, int ivec, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with number of piplus
    void fillNPiZeroHistos( PlotUtils::MnvH1D** h, double var, std::vector<int>& FS_PDG, double wgt = 1.0  );
    void fillNPiZeroHistos( PlotUtils::MnvH2D** h, double var1, double var2, std::vector<int>& FS_PDG, double wgt = 1.0  );

    //! fill HistWrapper with particle cuts
    void fillParticleHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt = 1.0  );
    void fillParticleHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with particle cuts
    void fillParticleHistos( PlotUtils::MnvH1D** h, double var, int pdg, double true_tpi, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with particle cuts
    void fillParticleHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, double true_tpi, double wgt = 1.0  );

    //! fill HistWrapper with particle cuts
    void fillTwoPartHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt = 1.0  );
    void fillTwoPartHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with particle cuts
    void fillTwoPartHistos( PlotUtils::MnvH1D** h, double var, int pdg, double true_tpi, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D** with particle cuts
    void fillTwoPartHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, double true_tpi, double wgt = 1.0  );

    //! fill HistWrapper mc histograms based on the plastic background 
    void fillPlasContHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, double wgt = 1.0, double wgt_bkgd = 1.0 );
    void fillPlasContHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, double wgt = 1.0, double wgt_bkgd = 1.0 );

    //! fill PlotUtils::MnvH1D mc histograms based on the plastic background
    void fillPlasContHistos( PlotUtils::MnvH1D** h, double var, true_info mc, double wgt = 1.0, double wgt_bkgd = 1.0 );

    //! fill PlotUtils::MnvH1D mc histograms based on the plastic background
    void fillPlasContHistos( PlotUtils::MnvH2D** h, double var1, double var2, true_info mc, double wgt = 1.0, double wgt_bkgd = 1.0 );

    //! fill HistWrapper mc histograms based on the nucleus 
    void fillNucleiHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, double wgt = 1.0, double wgt_bkgd = 1.0, 
                              bool fill_outside_fid_histos = false);

    void fillNucleiHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, double wgt = 1.0, double wgt_bkgd = 1.0, 
                              bool fill_outside_fid_histos = false);

    //! fill PlotUtils::MnvH1D mc histograms based on the nucleus
    void fillNucleiHistos( PlotUtils::MnvH1D** h, double var, true_info mc, double wgt = 1.0, double wgt_bkgd = 1.0, 
                              bool fill_outside_fid_histos = false);

    //! fill PlotUtils::MnvH1D mc histograms based on the nucleus
    void fillNucleiHistos( PlotUtils::MnvH2D** h, double var1, double var2, true_info mc, double wgt = 1.0, double wgt_bkgd = 1.0, bool fill_outside_fid_histos = false );

    //! fill HistWrapper mc histograms based on the pdg and topology 
    void fillTopologyHistos( std::vector< HistWrapper< CVUniverse >* > &hws, double var, CVUniverse* cv, int iProng, double wgt = 1.0  );
    void fillTopologyHistos( std::vector< Hist2DWrapper< CVUniverse >* > &hws, double var1, double var2, CVUniverse* cv, int iProng, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D mc histograms based on the pdg and topology
    void fillTopologyHistos( PlotUtils::MnvH1D** h, double var, int pdg, int destructCode, double true_tpi, double wgt = 1.0  );
    
    //! fill PlotUtils::MnvH1D mc histograms based on the pdg and topology
    void fillTopologyHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, int destructCode, double true_tpi, double wgt = 1.0  );
    
    //! fill PlotUtils::MnvH1D mc histograms based on the pdg and topology
    void fillCliffTopologyHistos( PlotUtils::MnvH1D** h, double var, int pdg, int destructCode, double wgt = 1.0  );
    
    //! fill PlotUtils::MnvH1D mc histograms based on the pdg and topology
    void fillCliffTopologyHistos( PlotUtils::MnvH2D** h, double var1, double var2, int pdg, int destructCode, double wgt = 1.0  );

    //! fill PlotUtils::MnvH1D mc histograms based on whether there is a michel in the vertex or at the endpoint
    void fillNewMichelHistos( PlotUtils::MnvH1D** h, double var, michel_info& michel, int iprong, double wgt = 1.0 );

    //! fill PlotUtils::MnvH1D mc histograms based on whether there is a michel in the vertex or at the endpoint
    void fillNewMichelHistos( PlotUtils::MnvH2D** h, double var1, double var2, michel_info& michel, int iprong, double wgt = 1.0);

    void setBkgdFitType( std::string bkgd_fit_type );

    int m_targetsFlag;
    int m_nucleiFlag;
    int m_plasticFlag; 
    bool m_useWaterTarget;
    NukeCCPionCuts* histo_cuts;

  private:
    string m_classification;
    bool m_SetMnvUni;
    bool m_MCBkgdOnly;
    bool m_IncludeNucleusOutside;
    int m_bkgd_fit_enum;
    TObjArray m_histList;   
    std::vector< PlotUtils::HistWrapper< CVUniverse >* > m_hwList;
    std::vector< Hist2DWrapper< CVUniverse >* > m_hw2DList;
};   
#endif
