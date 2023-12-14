#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys

gROOT.SetBatch()

var = sys.argv[3] #"permichel_pirange" #"pion_angle" #"permichel_pirange"
bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3
ratioMin = 0.0
ratioMax = 2.0



TH1.AddDirectory(False)
file1 = sys.argv[1]
file2 = sys.argv[2]
#file3 = sys.argv[3]
wFile = TFile.Open(file1)  #TFile.Open("../PmuPionReweight/Sep052022_Pmucut_noreweight_pTbin6_MC.root")

nwFile = TFile.Open(file2)#TFile.Open("Sep052022_Pmucut_noreweight_pTbin6_MC.root")
#qFile = TFile.Open(file3) 
nwHist = nwFile.Get(var+"_efficiency_numerator")#"GENIE_"+ var + "_RES")#"_efficiency_numerator")
wHist = wFile.Get(var+"_efficiency_numerator")#"GENIE_"+var+ "_RES")#"_efficiency_numerator")
print(nwHist.GetNbinsX())
print(wHist.GetNbinsX())

pot1 = nwFile.Get("POTUsed").GetVal()
pot2 = wFile.Get("POTUsed").GetVal()

scale = pot1/pot2
print("The scale is ", scale)
wHist.Scale(scale)

wHist.SetLineColor(kRed)
nwHist.SetLineColor(kBlue)
wHist.SetStats(0)
nwHist.SetStats(0)
verall = TCanvas("Reweight Shape for " + var)
#nwHist.Draw("")
wHist.Draw("L")
nwHist.Draw("sames,L")
verall.Print(var + "_PotNorm_Dist_" + sys.argv[4] + ".png")



ratio = wHist.Clone()
ratio.Divide(ratio, nwHist.Clone())#.GetCVHistoWithError().Clone())

mcRatio = wHist.GetTotalError(False, True, False) #The second "true" makes this fractional error
for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
  mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
  mcRatio.SetBinContent(whichBin, 1)

overall = TCanvas("Reweight Shape for " + var)

ratio.SetTitle("")
ratio.SetLineColor(kBlack)
ratio.SetLineWidth(lineSize)
#ratio.SetTitleSize(0)

ratio.GetYaxis().SetTitle(sys.argv[5])
#ratio.GetYaxis().SetLabelSize(labelSize)
#ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(1.2)
ratio.GetYaxis().SetNdivisions(505)

#ratio.GetXaxis().SetTitleSize(0.16)
ratio.GetXaxis().SetTitleOffset(0.9)
#ratio.GetXaxis().SetLabelSize(labelSize)
#weights = qFile.Get("MENU1PI_weights")
#weights.SetLineColor(kBlue)
#weights.SetMarkerColor(kBlue)
#weights.Draw("ALP")

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
#ratio.GetXaxis().SetRangeUser(0.0, 1.2)
ratio.GetXaxis().SetTitle(var)
ratio.Draw("SAMES")

mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
mcRatio.Draw("E2 SAME")

straightLine = mcRatio.Clone()
#straightLine.GetXaxis().SetRangeUser(0.0, 1.2)
straightLine.SetFillStyle(0)
straightLine.Draw("HIST SAME")


#weights = qFile.Get("MENU1PI_weights")
#weights.SetLineColor(kBlue)
#weights.SetMarkerColor(kBlue)
#weights.Draw("SAMES")

#overall.SetLogx()
plotter = PlotUtils.MnvPlotter()
overall.Print(var + "_" + sys.argv[4] + ".png")

