from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import time
ROOT.TH1.AddDirectory(ROOT.kFALSE)
gROOT.SetBatch()

dataFile = TFile.Open("runEventLoopData.root")
mcfile = TFile.Open("runEventLoopMC.root")
hfile = TFile.Open("RowNormalizedEventVar.root","RECREATE", "EventRowNorm")
names = ["AvailableE_pTmubins","Tpi", "Tpi_pTmubins","AvailableE","pTmu","pzmu"]

for name in names:
    print ("Now norming ", name)
    namenew = str(name)+str("_migration")
    mnv = mcfile.Get(namenew).Clone()
    mnv.SetDirectory(0)
    tmp = mnv.GetCVHistoWithError().Clone()
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
 
hfile.Close()
     		


