#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils

gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.

var = "LowTpi" #"NewEAvail" #"pTmu"
dataName = var + "_data"

ratioMin = 0.0
ratioMax = 2.0

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)
dataFile = TFile.Open("runEventLoopData.root")#_Sep022022_pmu_reweight.root")
mcFile = TFile.Open("runEventLoopMC.root")#_Sep022022_pmu_reweight.root")



mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
mcStack = THStack()
signalHist = mcFile.Get(var + "_data")#"_selected_signal_reco")
signalHist.Scale(dataPOT/mcPOT)
mcSum = signalHist.Clone()
mcSum.Add(mcSum, -1)
for key in mcFile.GetListOfKeys():
  name = str(key.GetName())
  if name.find("top") > -1 and name.find(var+"_t") > -1:
    hist = key.ReadObj()
    print "Name of Hist: ", hist.GetName()
    hist.Scale(dataPOT/mcPOT)
    newname = hist.GetName()
    if newname.find("_top_Kaons_Only") > -1:
    			hist.SetName("Kaons Only")
                        hist.SetLineColor(1321)
                        hist.SetFillColor(1321)
    if newname.find("_NPi_Plus_Only") > -1:
        		hist.SetTitle("N #pi^{+} Only")
                        hist.SetLineColor(1319)
                        hist.SetFillColor(1319)
    if newname.find("_NPi_and_Other_Mesons") > -1:
                        hist.SetTitle("N #pi^{+} and Mesons")
                        hist.SetLineColor(1323)
                        hist.SetFillColor(1323)
    if newname.find("_top_Other") > -1:
                        hist.SetLineColor(1322)
                        hist.SetFillColor(1322)
    if newname.find("_top_Neutral_Pi") > -1:
                        hist.SetLineColor(1320)
                        hist.SetFillColor(1320)
    if newname.find("_top_QE_like") > -1:
			hist.SetLineColor(1318)
                        hist.SetFillColor(1318)
    if newname.find("_top_COH_Pions") > -1:
                        hist.SetLineColor(1317)
                        hist.SetFillColor(1317)
    mcStack.Add(hist.GetCVHistoWithError().Clone())
    mcSum.Add(hist)
#mcStack.Add(signalHist.GetCVHistoWithError().Clone())

#Apply a different color for each MC category
mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
nextColor = 0

#for hist in mcStack.GetHists():
#  hist.SetLineColor(mcColors[nextColor])
#  hist.SetFillColor(mcColors[nextColor])
#  nextColor = nextColor + 1


dataHist = dataFile.Get(dataName)
dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))#"_selected_signal_reco"))

#Create a TCanvas on which to draw plots and split it into 2 panels
overall = TCanvas("Data/MC for " + var)
top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
#Thou shalt Draw() new TPads lest they be blank!
top.Draw()
#top.SetLogy()
bottom.Draw()

top.cd()
mcTotal = mcSum #mcStack.GetStack().Last()
mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

mcStack.SetMinimum(1)
mcStack.SetMaximum(mcTotal.GetMaximum() * 4) #Make room for annotations above the plot
mcStack.Draw("HIST")

dataWithStatErrors.SetLineColor(kBlack)
dataWithStatErrors.SetLineWidth(lineSize)
dataWithStatErrors.SetMarkerStyle(20) #Resizeable closed circle
dataWithStatErrors.SetMarkerColor(kBlack)
dataWithStatErrors.SetMarkerSize(0.7)
dataWithStatErrors.SetTitle("Data")
#dataWithStatErrors.Draw("SAME")

legend = top.BuildLegend(0.5, 0.4, 0.9, 0.9) #TODO: Explain legend position.  ROOT 6 might even make an intelligent decision about where to place it.

#Data/MC ratio panel
bottom.cd()
bottom.SetTopMargin(0)
bottom.SetBottomMargin(0.3)

ratio = dataHist.Clone()
mcTotalWithSys = mcSum
ratio.Divide(ratio, mcTotalWithSys)

#TODO: I need GetCVHistoWithError() from mcRatio, but THStack doesn't keep a MnvH1D.  I have to Add() the histograms myself.

#Now fill mcRatio with 1 for bin content and fractional error
mcRatio = mcTotalWithSys.GetTotalError(False, True, False) #The second "true" makes this fractional error
for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
  mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
  mcRatio.SetBinContent(whichBin, 1)

ratio.SetTitle("")
ratio.SetLineColor(kBlack)
ratio.SetLineWidth(lineSize)
ratio.SetTitleSize(0)

ratio.GetYaxis().SetTitle("Data / MC")
ratio.GetYaxis().SetLabelSize(labelSize)
ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(0.2)
ratio.GetYaxis().SetNdivisions(505) #5 minor divisions between 5 major divisions.  I'm trying to match a specific paper here.

ratio.GetXaxis().SetTitleSize(0.16)
ratio.GetXaxis().SetTitleOffset(0.9)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
ratio.Draw()

#Error envelope for the MC
mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
#mcRatio.Draw("E2 SAME")

#Draw a flat line at 1 for ratio of MC to itself
straightLine = mcRatio.Clone()
straightLine.SetFillStyle(0)
#straightLine.Draw("HIST SAME")

#Title for the whole plot
top.cd()
title = TPaveText(0.3, 0.91, 0.7, 1.0, "nbNDC") #no border and use Normalized Device Coordinates to place it
title.SetFillStyle(0)
title.SetLineColor(kWhite)
title.AddText(var)
#title.Draw()

plotter = PlotUtils.MnvPlotter()
plotter.WritePreliminary(0.3, 0.82, 5e-2, True)

#Make a PNG file of this canvas
overall.Print(var + "DataMCRatio.png")

#tPi efficiency 

newcan = TCanvas("Efficiency For " + var) 
newcan.cd()
num = mcFile.Get(var + "_efficiency_numerator")
denom = mcFile.Get(var + "_efficiency_denominator")

eff = num.Clone()
eff.Divide(num,denom)
eff.SetTitle("")
eff.SetLineColor(kBlack)
eff.SetLineWidth(lineSize)
eff.SetTitleSize(0)
eff.GetYaxis().SetTitle("Efficiency")
eff.GetXaxis().SetTitle("Lowest Pion Kinetic Energy (MeV)")
eff.GetYaxis().SetTitleSize(0.06)
eff.GetYaxis().SetTitleOffset(1.2)
eff.GetYaxis().SetNdivisions(505)
eff.GetXaxis().SetTitleSize(0.06)
eff.GetXaxis().SetTitleOffset(0.9)
eff.GetYaxis().SetRangeUser(0.0, 1.0)
eff.Draw()

newcan.Print(var+"_Efficiency.png")
