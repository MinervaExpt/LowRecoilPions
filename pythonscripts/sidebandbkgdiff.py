#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys

gROOT.SetBatch()

var = "QE_bkg" #"pi0_bkg" #"permichel_pirange" #"pion_angle" #"permichel_pirange"
bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3
ratioMin = 0.0
ratioMax = 2.0


TH1.AddDirectory(False)
file1 = sys.argv[1]
file2 = sys.argv[2]

wFile = TFile.Open(file1)  #TFile.Open("../PmuPionReweight/Sep052022_Pmucut_noreweight_pTbin6_MC.root")

nwFile = TFile.Open(file2)#TFile.Open("Sep052022_Pmucut_noreweight_pTbin6_MC.root")

nwHist = nwFile.Get(var)
wHist = wFile.Get(var)

ratio = wHist.Clone()
ratio.Divide(ratio, nwHist.Clone())#.GetCVHistoWithError().Clone())

for whichBin in range(1, ratio.GetXaxis().GetNbins()+1):
  lowbinedge = ratio.GetBinLowEdge(whichBin)
  highbinedge = ratio.GetBinLowEdge(whichBin+1) 
  print lowbinedge, highbinedge, ratio.GetBinContent(whichBin)
mcRatio = wHist.GetTotalError(False, True, False) #The second "true" makes this fractional error
for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
  mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
  mcRatio.SetBinContent(whichBin, 1)

overall = TCanvas("Reweight Shape for " + var)

ratio.SetTitle("")
ratio.SetLineColor(kBlack)
ratio.SetLineWidth(lineSize)
#ratio.SetTitleSize(0)

ratio.GetYaxis().SetTitle(var + " Selection/Sideband")
#ratio.GetYaxis().SetLabelSize(labelSize)
#ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(1.2)
ratio.GetYaxis().SetNdivisions(505)

#ratio.GetXaxis().SetTitleSize(0.16)
ratio.GetXaxis().SetTitleOffset(0.9)
#ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
ratio.Draw()

mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
mcRatio.Draw("E2 SAME")

straightLine = mcRatio.Clone()
straightLine.SetFillStyle(0)
straightLine.Draw("HIST SAME")

plotter = PlotUtils.MnvPlotter()
overall.Print(var + "_bkg_ratio.png")

