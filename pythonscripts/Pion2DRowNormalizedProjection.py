from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import time
ROOT.TH1.AddDirectory(ROOT.kFALSE)
gROOT.SetBatch()

mcfile = TFile.Open(sys.argv[1]) #"Sep052022_Pmucut_noreweight_pTbin6_MC.root")

hfile = TFile.Open("PionRangeColumnNormalizedSignal.root","RECREATE", "EventRowNorm")
names = ["Pion_Range_vs_Pion_KE"]#"ptpi_true_vs_ptpi_reco","pzpi_true_vs_pzpi_reco", "ptsum_true_vs_ptsum_reco","epsub_true_vs_epsub_reco", "t_true_vs_t_reco", "true_piangle_vs_reco_piangle" ]

for name in names:
    print ("Now norming ", name)
    namenew = str(name)+str("_signal")
    mnv = mcfile.Get(namenew)
    #mnv.SetDirectory(0)
    tmp = mnv.Clone()#GetCVHistoWithError().Clone()
    tmp2 = tmp.Clone()
    nbinsy = tmp.GetNbinsY()+2
    nbinsx = tmp.GetNbinsX()+2
    #tmp.ClearUnderflowAndOverflow(); 
    #tmp2.ClearUnderflowAndOverflow();
    for i in range(1,nbinsy-1):
	if (i == 0): continue
	if (i > tmp.GetNbinsY()): continue
	norm = 0.
        #tpihist_bini = tmp.GetProjectionX("e", i+1)
	#tpihist_bini.SetName(name+"Bin"+str(i))
        #tpihist_bini.GetXaxis().SetTitle("reco - true (MeV)")
        #tpihist_bini.GetYaxis().SetTitle("N Events")
        #tpihist_bini.Draw()
        #tpihist_bini.Write()
        for j in range(1,nbinsx-1):
		norm += tmp.GetBinContent(j,i)
  	if(abs(norm) != 0):
	   for x in range(1,nbinsx-1):
		fraction = 1.*tmp.GetBinContent(x,i)/norm
		tmp2.SetBinContent(x,i,fraction)

   
    for i in range(1,nbinsy-1):
        norm = 0.
        #tpihist_bini = tmp.GetProjectionX("e", i+1)
        #tpihist_bini.SetName(name+"Bin"+str(i))
        #tpihist_bini.GetXaxis().SetTitle("reco - true (MeV)")
        #tpihist_bini.GetYaxis().SetTitle("N Events")
        #tpihist_bini.Draw()
        #tpihist_bini.Write()
        for j in range(1,nbinsx-1):
                norm += tmp2.GetBinContent(j,i)


        print("Printing bin content sum of column ", i, "  in the normalized matrix", norm)

    

    tmp2.GetXaxis().SetTitle("Pion Kinetic Energy (MeV)")
    tmp2.GetYaxis().SetTitle("Pion Range (mm)")
    tmp2.SetName(namenew+"_norm")
    tmp2.Draw("colz")
    tmp2.Write()
    #tmp2.Print(name+"_ColumnNormalized.png")
    tmp.GetXaxis().SetTitle("true")
    tmp.GetYaxis().SetTitle("reco")
    tmp.SetName(name+"_event")
    tmp.Draw("colz")
    tmp.Write() 
    #tmp.Print(name+"_EventRate.png")
 
hfile.Close()
     		


