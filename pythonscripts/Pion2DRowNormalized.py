from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import time
ROOT.TH1.AddDirectory(ROOT.kFALSE)
gROOT.SetBatch()

mcfile = TFile.Open(sys.argv[1]) #"Sep052022_Pmucut_noreweight_pTbin6_MC.root")

hfile = TFile.Open("TpiPionRowNormalizedEventVar.root","RECREATE", "EventRowNorm")
names = ["Tpi"] #Pion_Range_vs_Pion_KE"]#,"epsub_true_vs_epsub_reco", "t_true_vs_t_reco", "true_piangle_vs_reco_piangle" ]

for name in names:
    print ("Now norming ", name)
    namenew = str(name)+str("_migration")
    mnv = mcfile.Get(namenew)
    #mnv.SetDirectory(0)
    tmp = mnv.Clone()#GetCVHistoWithError().Clone()
    tmp2 = tmp.Clone()
    nbinsy = tmp.GetNbinsY()+2
    nbinsx = tmp.GetNbinsX()+2
    for i in range(0,nbinsy):
	norm = 0.
        #tpihist_bini = tmp.GetProjectionX("e", i+1)
	#tpihist_bini.SetName(name+"Bin"+str(i))
        #tpihist_bini.GetXaxis().SetTitle("reco - true (MeV)")
        #tpihist_bini.GetYaxis().SetTitle("N Events")
        #tpihist_bini.Draw()
        #tpihist_bini.Write()
        for j in range(0,nbinsx):
		norm += tmp.GetBinContent(j,i)
  	if(abs(norm) != 0):
		for x in range(0,nbinsx):
			fraction = 1.*tmp.GetBinContent(x,i)/norm
		        tmp2.SetBinContent(x,i,fraction)

         
    tmp2.GetXaxis().SetTitle("reco")
    tmp2.GetYaxis().SetTitle("true")
    tmp2.Draw("colz")
    tmp2.Write()
    tmp2.Print(name+"_RowNormalized.png")
    tmp.GetXaxis().SetTitle("reco")
    tmp.GetYaxis().SetTitle("true")
    tmp.SetName(name+"_event")
    tmp.Draw("colz")
    tmp.Write() 
    tmp.Print(name+"_EventRate.png")
    for x in range(1, nbinsy):
	overall = TCanvas("True Tpi Slice for Bin " + str(x))
	trueslice = tmp.ProjectionX("e",x,x)
	trueslice.SetName("Truebin"+str(x))
	tpibinmax = trueslice.GetMaximum()
	tpimaxbin = trueslice.GetMaximumBin()
	tpimaxcenter = trueslice.GetBinCenter(tpimaxbin)
	tpibincenter = tmp.GetYaxis().GetBinCenter(x)
	trueslice.GetXaxis().SetTitle("Reco T_{#pi^{+}} (GeV) For True Bin "+str(x)+" Slice Centered at "+str(tpibincenter)+" GeV")
	trueslice.GetYaxis().SetTitle("N Events")
	trueslice.SetMarkerStyle(20)
	trueslice.SetMarkerColor(kBlack)
 	trueslice.SetMarkerSize(0.5)
	trueslice.GetYaxis().SetRangeUser(0, tpibinmax*1.13)
	trueslice.Draw()
	#overall.Print("TrueTpibin_"+str(x)+".png")
	trueslice.Write()
	bin1 = trueslice.FindFirstBinAbove(tpibinmax/2)
	bin2 = trueslice.FindLastBinAbove(tpibinmax/2)
	fwhm = trueslice.GetBinCenter(bin2) - trueslice.GetBinCenter(bin1)
	disttofalloff = trueslice.GetBinCenter(bin2) - tpimaxcenter
	lowedge = trueslice.GetBinLowEdge(tpimaxbin)
	width = trueslice.GetBinWidth(tpimaxbin)
	binHedge = lowedge + width
	print(format(tpibincenter,'.3f'), format(tpimaxcenter,'.3f'), format(lowedge,'.3f'), format(binHedge,'.3f')) #, disttofalloff)
	
	#print("True Tpi bin center ", tpibincenter , " Reco Tpi Max Bin Center ", tpimaxcenter)
hfile.Close()
     		


