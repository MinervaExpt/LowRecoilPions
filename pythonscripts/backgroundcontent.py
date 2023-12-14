#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
#import TStyle

gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.
gStyle.SetOptStat(0)
var = sys.argv[1] #"pTmu" #"AvailableE" #"NewEAvail" #"pTmu"
dataName = var + "_data"

ratioMin = 0.0
ratioMax = 2.0

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)
dataFile = TFile.Open(sys.argv[4])#"runEventLoopData.root")#runEventLoopData_Sep022022_pmu_reweight.root")
mcFile = TFile.Open(sys.argv[3])#"runEventLoopMC.root")



mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
mcStack = THStack()
signalHist = mcFile.Get(var + "_data")#"_selected_signal_reco")
signalHist.Scale(dataPOT/mcPOT)
#signalHist.GetXaxis().SetRangeUser(0.0,3000.)
mcSum = signalHist.Clone()
mcSum.Add(mcSum, -1)
hists = [None]*7
for key in mcFile.GetListOfKeys():
  name = str(key.GetName())
  if name.find("_"+var) > -1: continue
  if name.find("top") > -1 and name.find(var+"_t") > -1:
    hist = key.ReadObj()
    print "Name of Hist: ", hist.GetName()
    hist.Scale(dataPOT/mcPOT)
    newname = hist.GetName()
    
    if newname.find("_top_Kaons_Only") > -1:
    			hist.SetName("Kaons Only")
                        hist.SetLineColor(1321)
                        hist.SetFillColor(1321)
                        continue
    if newname.find("_NPi_Plus_Only") > -1:
        		hist.SetTitle("N #pi^{+} Only")
                        hist.SetLineColor(1319)
                        hist.SetFillColor(1319)
		        continue
    if newname.find("_NPi_and_Other_Mesons") > -1:
                        hist.SetTitle("N #pi^{+} and Mesons")
                        hist.SetLineColor(1323)
                        hist.SetFillColor(1323)
			continue
    if newname.find("_top_Other") > -1:
                        hist.SetLineColor(1322)
                        hist.SetFillColor(1322)
                        hists[0] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_Neutral_Pi") > -1:
                        hist.SetLineColor(1320)
                        hist.SetFillColor(1320)
			hists[1] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_QE_like") > -1:
			hist.SetLineColor(1318)
                        hist.SetFillColor(1318)
			hists[2] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_COH_Pions") > -1:
                        hist.SetLineColor(1317)
                        hist.SetFillColor(1317)
			continue
    #mcStack.Add(hist.GetCVHistoWithError().Clone())
    #hist.GetXaxis().SetRangeUser(0.0, 3000.)
    histnorm = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    #histnorm.GetXaxis().SetRangeUser(0.0,3000.)
    #histnorm.Scale(5) #scaling backgrounds to show shape
    mcStack.Add(hist)#norm)
    mcSum.Add(hist)
#mcStack.Add(signalHist.GetCVHistoWithError().Clone())

#Apply a different color for each MC category
mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
nextColor = 0

output = TFile.Open("bkgratio.root", "RECREATE")
ratioqe = mcSum.Clone()
ratioqe.GetYaxis().SetRangeUser(0.0, 1.0)
ratioqe.Add(mcSum, -1)
ratiopi0 = ratioqe.Clone()
ratiopi0.Add(ratioqe, -1)
bkgall = TCanvas("BackgroundFrac for " + var)
for hist in mcStack.GetHists():
    name = hist.GetName()
    if name.find("_top_Neutral_Pi") > -1:
       ratiopi0.Divide(hist, mcSum.Clone())
    if name.find("_top_QE_like") > -1:
       ratioqe.Divide(hist, mcSum.Clone())

ratioqe.SetName("QE_bkg")
ratioqe.GetXaxis().SetTitle(sys.argv[2])#"Available Energy")
ratioqe.GetYaxis().SetTitle("Fraction QE in background")
ratioqe.GetYaxis().SetRangeUser(0.0, 1.0)
ratioqe.Draw("HIST")
ratioqe.Write()
bkgall.Print(var + "_background_Fraction_QE.png")
#bkgall.Write() #Print(var + "_background_Fraction_QE.root")

ratiopi0.SetName("pi0_bkg")
ratiopi0.GetXaxis().SetTitle(sys.argv[2])#Available Energy")
ratiopi0.GetYaxis().SetTitle("Fraction pi0 in background")
ratiopi0.GetYaxis().SetRangeUser(0.0, 1.0)
ratiopi0.Draw("HIST")
ratiopi0.Write()
bkgall.Print(var + "_background_Fraction_pi0.png")

dataHist = dataFile.Get(dataName)
#dataHist.GetXaxis().SetRangeUser(0.0,3000.)
dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))#"_selected_signal_reco"))
dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
#dataHistnorm.GetXaxis().SetRangeUser(0.0, 3000.)
#Create a TCanvas on which to draw plots and split it into 2 panels
overall = TCanvas("Data/MC for " + var)
#top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
#bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
#Thou shalt Draw() new TPads lest they be blank!
overall.SetLogy()
#bottom.SetLogx()
#overall.SetLogx()
#top.Draw()
#top.SetLogx()
#bottom.SetLogx()
#bottom.Draw()

orderedStack = THStack()
orderedStack.Add(hists[0])
orderedStack.Add(hists[1])
orderedStack.Add(hists[2])

#top.cd()
mcTotal = mcSum #mcStack.GetStack().Last()
#mcTotal.GetXaxis().SetRangeUser(0.0, 3000.)
mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

mcStack.SetMinimum(1)
mcStack.SetMaximum(mcSum.GetMaximum()*1.6) #Make room for annotations above the plot
#mcStack.GetHistogram().GetXaxis().SetRangeUser(0, 2000)
#mcStack.GetXaxis().SetRangeUser(0.0, 3000.)
mcStack.Draw("HIST")
mcStack.GetHistogram().GetYaxis().SetRangeUser(0, mcSum.GetMaximum()*1.6)
mcStack.GetHistogram().GetXaxis().SetTitle(sys.argv[2])
mcStack.Draw("HIST")


orderedStack.SetMinimum(1)
orderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.6) #Make room for annotations above the plot
orderedStack.Draw("HIST")
#dataWithStatErrors.SetLineColor(kBlack)
#dataWithStatErrors.SetLineWidth(lineSize)
#dataWithStatErrors.SetMarkerStyle(20) #Resizeable closed circle
#dataWithStatErrors.SetMarkerColor(kBlack)
#dataWithStatErrors.SetMarkerSize(0.7)
#dataWithStatErrors.SetTitle("Data")
#dataWithStatErrors.Draw("SAME")

dataHistnorm.SetLineColor(kBlack)
dataHistnorm.SetLineWidth(lineSize)
dataHistnorm.SetMarkerStyle(20) #Resizeable closed circle
dataHistnorm.SetMarkerColor(kBlack)
dataHistnorm.SetMarkerSize(0.7)
dataHistnorm.SetTitle("Data")
dataHistnorm.GetYaxis().SetTitle("N Events")
#dataHistnorm.GetXaxis().SetRangeUser(0.0, 2000.)
#dataHistnorm.Draw("SAME")

legend = overall.BuildLegend(0.75, 0.6, .95, 0.9) #TODO: Explain legend position.  ROOT 6 might even make an intelligent decision about where to place it.

#Data/MC ratio panel
#bottom.cd()
#bottom.SetTopMargin(0)
#bottom.SetBottomMargin(0.3)

ratio = dataHist.Clone()
mcTotalWithSys = mcSum
ratio.Divide(ratio, mcTotalWithSys)
#ratio.GetXaxis().SetRangeUser(0.0,3000.)
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
ratio.GetXaxis().SetTitle(sys.argv[2])
ratio.GetXaxis().SetTitleSize(0.16)
ratio.GetXaxis().SetTitleOffset(0.9)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
#ratio.GetXaxis().SetRangeUser(0.0, 2000.)
#ratio.Draw()

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
#top.cd()
title = TPaveText(0.3, -0.91, 0.7, 1.0, "nbNDC") #no border and use Normalized Device Coordinates to place it
title.SetFillStyle(0)
title.SetLineColor(0)#kWhite)
#title.AddText(sys.argv[2])
#title.Draw()

plotter = PlotUtils.MnvPlotter()
plotter.WritePreliminary(0.3, 0.82, 5e-2, True)

#Make a PNG file of this canvas
overall.Print(var + "_baackgroundscaled_DataMCRatio.png")
