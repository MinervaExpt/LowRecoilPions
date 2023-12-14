#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import os, sys
gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.

var = "permichel_pirange" #"NewEAvail" #"pTmu"
dataName = var + "_data"

ratioMin = 0.0
ratioMax = 1.5

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)
dataFile = TFile.Open(sys.argv[2]) #"studiesData.root")

mcFile = TFile.Open(sys.argv[1])#"studiesMC.root") 


mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()
print "mcPOT: ", mcPOT
print "dataPOT: ", dataPOT
print "Scale: ", dataPOT/mcPOT 
#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
mcStack = THStack()
signalHist = mcFile.Get(var + "_MC")#"_selected_signal_reco")
print signalHist
#signalHist.Scale(dataPOT/mcPOT)
mcSum = signalHist.Clone()
mcSum.Add(mcSum, -1)
hists = [None]*7
for key in mcFile.GetListOfKeys():
  name = str(key.GetName())
  if name.find("top") > -1 and name.find(var+"_t") > -1:
    hist = key.ReadObj()
    print "Name of Hist: ", hist.GetName()
    #hist.Scale(dataPOT/mcPOT)
    #hist.GetXaxis().SetRangeUser(0.0, 3000.)
    newname = hist.GetName()
    if newname.find("_top_Kaons_Only") > -1:
    			hist.SetName("Kaons Only")
                        hist.SetLineColor(1321)
                        hist.SetFillColor(1321)
                        hists[0] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_NPi_Plus_Only") > -1:
        		hist.SetTitle("N #pi^{+} Only")
                        hist.SetLineColor(1319)
                        hist.SetFillColor(1319)
			hists[6] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_NPi_and_Other_Mesons") > -1:
                        hist.SetTitle("N #pi^{+} and Mesons")
                        hist.SetLineColor(1323)
                        hist.SetFillColor(1323)
			hists[5] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_Other") > -1:
                        hist.SetLineColor(1322)
                        hist.SetFillColor(1322)
			hists[2] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_Neutral_Pi") > -1:
                        hist.SetLineColor(1320)
                        hist.SetFillColor(1320)
			hists[4] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_QE_like") > -1:
			hist.SetLineColor(1318)
                        hist.SetFillColor(1318)
			hists[3] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_top_COH_Pions") > -1:
                        hist.SetLineColor(1317)
                        hist.SetFillColor(1317)
                        hists[1] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    #hist.GetXaxis().SetLimits(0.1, 2400.)
    mcSum.Add(hist)
    mcStack.Add(hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone())
    
#mcStack.Add(signalHist.GetCVHistoWithError().Clone())

#Apply a different color for each MC category
mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
nextColor = 0

orderedStack = THStack()
orderedStack.Add(hists[0])
orderedStack.Add(hists[1])
orderedStack.Add(hists[2])
orderedStack.Add(hists[3])
orderedStack.Add(hists[4])
orderedStack.Add(hists[5])
orderedStack.Add(hists[6])

#for hist in hists: #mcStack.GetHists():
   

#  hist.SetLineColor(mcColors[nextColor])
#  hist.SetFillColor(mcColors[nextColor])
#  nextColor = nextColor + 1


dataHist = dataFile.Get(dataName)
dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_MC"))#"_selected_signal_reco"))
dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
#Create a TCanvas on which to draw plots and split it into 2 panels
overall = TCanvas("Data/MC for " + var)
top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
overall.SetLogx()
#Thou shalt Draw() new TPads lest they be blank!
top.SetLogx()
top.Draw()

#top.SetLogy()
bottom.SetLogx()
bottom.Draw()

top.cd()
mcTotal = mcSum #mcStack.GetStack().Last()
mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

mcStack.SetMinimum(1)
mcStack.SetMaximum(dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
#mcStack.Draw("HIST")
#histstack = mcStack.GetHistogram()
#histstack.GetXaxis().SetRangeUser(, 2400.)
#mcStack.Draw("HIST")
#mcStack.GetHistogram().GetXaxis().SetRangeUser(0.0,3000.)
#mcStack.Draw("HIST")
#dataWithStatErrors.SetLineColor(kBlack)
#3dataWithStatErrors.SetLineWidth(lineSize)
#dataWithStatErrors.SetMarkerStyle(20) #Resizeable closed circle
#dataWithStatErrors.SetMarkerColor(kBlack)
#dataWithStatErrors.SetMarkerSize(0.7)
#dataWithStatErrors.SetTitle("Data")
#dataWithStatErrors.Draw("SAME")

orderedStack.SetMinimum(1)
orderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
orderedStack.Draw("HIST")
histstack = orderedStack.GetHistogram()
#histstack.GetXaxis().SetRangeUser(500., 3000.)
orderedStack.Draw("HIST")


dataHistnorm.SetLineColor(kBlack)
dataHistnorm.SetLineWidth(lineSize)
dataHistnorm.SetMarkerStyle(20) #Resizeable closed circle
dataHistnorm.SetMarkerColor(kBlack)
dataHistnorm.SetMarkerSize(0.7)
dataHistnorm.SetTitle("Data")
#dataHistnorm.GetXaxis().SetRangeUser(0.0,3000.)
dataHistnorm.Draw("SAME")


#legend = top.BuildLegend(0.75, 0.4, 0.9, 0.9) #TODO: Explain legend position.  ROOT 6 might even make an intelligent decision about where to place it.

#Data/MC ratio panel
bottom.cd()
bottom.SetTopMargin(0)
bottom.SetBottomMargin(0.3)

ratio = dataWithStatErrors.Clone()#dataHist.Clone()
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
#ratio.GetXaxis().SetLimits(500, 3000.)
ratio.GetYaxis().SetTitle("Data / MC")
ratio.GetYaxis().SetLabelSize(0.09)
ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(0.2)
ratio.GetYaxis().SetNdivisions(505) #5 minor divisions between 5 major divisions.  I'm trying to match a specific paper here.

ratio.GetXaxis().SetTitleSize(0.16)
ratio.GetXaxis().SetTitleOffset(0.9)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
ratio.GetXaxis().SetTitle("Reconstructed Pion Range (mm)")
#ratio.GetXaxis().SetRangeUser(500.,3000.)
#ratio.GetYaxis().SetLog()

ratio.Draw()
#bottom.SetLogx()
#Error envelope for the MC
mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
#mcRatio.GetXaxis().SetRangeUser(500,3000.)
#mcRatio.GetXaxis().SetLimits(500, 3000.)
mcRatio.Draw("E2 SAME")

#Draw a flat line at 1 for ratio of MC to itself
straightLine = mcRatio.Clone()
straightLine.SetFillStyle(0)
straightLine.Draw("HIST SAME")

#Title for the whole plot
top.cd()
title = TPaveText(0.3, 0.91, 0.7, 1.0, "nbNDC") #no border and use Normalized Device Coordinates to place it
title.SetFillStyle(0)
title.SetLineColor(kWhite)
title.AddText(var)
#title.Draw()

plotter = PlotUtils.MnvPlotter()
plotter.WritePreliminary(0.3, 0.82, 5e-2, True)
plotter.AddChi2Label(dataHist,signalHist)

overall.SetLogx()
#Make a PNG file of this canvas
overall.Print(var + "_All_DataMCRatio_logx_chi2.png")


migcan = TCanvas("Migration for " + var)
migration = mcFile.Get("Pion_Range_vs_Pion_KE_MC")
#migration.Scale(dataPOT/mcPOT)
migration.GetXaxis().SetTitle("Pion Kinetic Energy (MeV)")
migration.GetYaxis().SetTitle("Reconstructed Pion Range (mm)") 
migration.Draw("colz")
migcan.Print("range_vs_tpi_mig_AllMC.png")

bkg = mcFile.Get("permichel_pirange_bkg")
#bkg.Scale(dataPOT/mcPOT)
databkg = dataHist.Clone()
databkg.Add(bkg, -1)
databkg.GetXaxis().SetTitle("Reconstructed Pion Range (mm)")
databkg.GetYaxis().SetTitle("N Events")
databkg.SetLineColor(kBlack)
databkg.Draw("HIST")
bkg.SetFillStyle(3315)
bkg.Draw("HIST Sames")
migcan.Print("pirange_alldata_HIST_bkgsub.png")
