#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
from ROOT import TColor
import sys
from ROOT import gStyle

#gStyle.SetHatchesSpacing(.25)
#gStyle->SetHatchesLineWidth()
gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.
gStyle.SetLegendBorderSize(0)
var = sys.argv[1] #"pTmu" #"AvailableE" #"NewEAvail" #"pTmu"
dataName = var + "_data"

ratioMin = 0.0
ratioMax = 1.5

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)
dataFile = TFile.Open(sys.argv[3])#"runEventLoopData.root")#runEventLoopData_Sep022022_pmu_reweight.root")
mcFile = TFile.Open(sys.argv[2])#"runEventLoopMC.root")



#mcPOT = mcFile.Get("POTUsed").GetVal()
#dataPOT = dataFile.Get("POTUsed").GetVal()
#scale = dataPOT/mcPOT
#print("Data POT is ", dataPOT)
#print("MC POT is ", mcPOT)
#print(" dAta/MC scale is ", scale )
#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
mcStack = THStack()
signalHist = mcFile.Get(var + "_data")#"_selected_signal_reco")
print(signalHist)
#signalHist.Scale(dataPOT/mcPOT)
#signalHist.GetXaxis().SetRangeUser(0.0,3000.)
mcSum = signalHist.Clone()
mcSum.Add(mcSum, -1)
hists = [None]*11
bkghists = []
for key in mcFile.GetListOfKeys():
  name = str(key.GetName())
  #if name.find(var+"_background") > -1: bkghists.append(key.ReadObj())
  if name.find("_"+var) > -1: continue
  if name.find("s"+var) > -1: continue
  if name.find(var+"_background") > -1: bkghists.append(key.ReadObj())
  if name.find("picat") > -1 and name.find(var+"_picat_") > -1:
    hist = key.ReadObj()
    print "Name of Hist: ", hist.GetName()
    #hist.Scale(dataPOT/mcPOT)
    newname = hist.GetName()
    
    if newname.find("_picat_Kaons_Only") > -1:
    			hist.SetName("Kaons Only")
                        hist.SetLineColor(1321)
                        hist.SetFillColor(1321)
                        hists[0] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    if newname.find("_picat_1pip_w_LProton") > -1:
        		hist.SetTitle("1 #pi^{+} w Proton")
                        hist.SetLineColor(1319)
                        hist.SetFillColor(1319)
		        hists[10] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    if newname.find("_picat_1pip_w_LNeutron") > -1:
                        hist.SetTitle("1 #pi^{+} w Neutron")
			lightcolor = kGreen - 5#1319+100 # TColor.GetColorDark(1319)
                        hist.SetLineColor(lightcolor)
                        hist.SetFillColor(lightcolor)
   			#hist.SetFillStyle(3144)
                        hists[9] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_picat_1pip_wo_p_or_n") > -1:
                        hist.SetTitle("1 #pi^{+} w/o p or n")
                        lightcolor = kGreen - 8#TColor.GetColorLight(1319)
                        hist.SetLineColor(lightcolor)
                        hist.SetFillColor(lightcolor)
                        hists[8] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()    


    if newname.find("_picat_Npip_Only") > -1:
                        hist.SetTitle("> 1 #pi^{+}, no other mesons")
                        hist.SetLineColor(1323)
                        hist.SetFillColor(1323)
                        hists[7] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    
    #if newname.find("_picat_Npip_w_LNeutron") > -1:
    #                    hist.SetTitle("N#pi^{+} w Neutron")
    #		lightcolor = kRed -5 #TColor.GetColorDark(1323)
    #                    hist.SetLineColor(lightcolor)
    #                    hist.SetFillColor(lightcolor)
    #                    hists[8] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    #if newname.find("_picat_Npip_wo_p_or_n") > -1:
    #                    hist.SetTitle("N#pi^{+} w/o p or n")
    #                    lightcolor = kRed -8 #TColor.GetColorLight(1323)
    #                    hist.SetLineColor(lightcolor)
    #                    hist.SetFillColor(lightcolor)
    #                    hists[7] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue

    if newname.find("_picat_NPi_and_Mesons_RES") > -1:
                        hist.SetTitle("N #pi^{+} and Mesons (RES)")
                        hist.SetLineColor(kBlue-10)
                        hist.SetFillColor(kBlue-10)
			#hist.SetFillStyle(3144)
			hists[6] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    if newname.find("_picat_NPi_and_Mesons_DIS") > -1:
                        hist.SetTitle("N #pi^{+} and Mesons (DIS)")
                        hist.SetLineColor(kBlue-6)
                        hist.SetFillColor(kBlue-6)
                        hists[5] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue

    if newname.find("_picat_Other") > -1:
                        hist.SetLineColor(1322)
                        hist.SetFillColor(1322)
			hists[2] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_picat_Neutral_Pi_Only") > -1:
                        hist.SetTitle("Neutral Pions Only")
			hist.SetLineColor(1320)
                        hist.SetFillColor(1320)
			hists[4] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_picat_QE_like") > -1:
			hist.SetLineColor(1318)
                        hist.SetFillColor(1318)
			hists[3] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    if newname.find("_picat_COH_Pions") > -1:
                        hist.SetTitle("Coherent Pions")
			hist.SetLineColor(1317)
                        hist.SetFillColor(1317)
			hist2 = hist.Clone()
			#hist2.Scale(10.)
			hists[1] = hist2.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
    #mcStack.Add(hist.GetCVHistoWithError().Clone())
    #hist.GetXaxis().SetRangeUser(0.0, 3000.)
    histnorm = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
    #histnorm.GetXaxis().SetRangeUser(0.0,3000.)
    #histnorm.Scale(5) #scaling backgrounds to show shape
    mcStack.Add(histnorm)
    mcSum.Add(hist)
#mcStack.Add(signalHist.GetCVHistoWithError().Clone())

#Apply a different color for each MC category
mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
nextColor = 0

#for hist in mcStack.GetHists():
#  hist.SetLineColor(mcColors[nextColor])
#  hist.SetFillColor(mcColors[nextColor])
#  nextColor = nextColor + 1

orderedStack = THStack()
orderedStack.Add(hists[0])
orderedStack.Add(hists[3])
orderedStack.Add(hists[2])
orderedStack.Add(hists[4])
orderedStack.Add(hists[1]) #COH category should be above the backgrounds
orderedStack.Add(hists[5])
orderedStack.Add(hists[6])
orderedStack.Add(hists[7])
#orderedStack.Add(hists[1]) #COH category should be above the backgrounds and after the single pion category
orderedStack.Add(hists[8])
orderedStack.Add(hists[9])
orderedStack.Add(hists[10])

#dont need these
#orderedStack.Add(hists[11])
#orderedStack.Add(hists[12])

datahist = dataFile.Get(dataName)
dataHist = datahist.Clone()
#dataHist.GetXaxis().SetRangeUser(0.0,3000.)
dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))#"_selected_signal_reco"))
dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
#dataHistnorm.GetXaxis().SetRangeUser(0.0, 3000.)
#Create a TCanvas on which to draw plots and split it into 2 panels

overall = TCanvas("Data/MC for " + var)

top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)


#Thou shalt Draw() new TPads lest they be blank!
#top.SetLogy()
#bottom.SetLogx()
if (int(sys.argv[6]) == 1):
    overall.SetLogx()
    top.SetLogx()
    bottom.SetLogx()
    top.Draw()
    bottom.SetLogx()
    bottom.Draw()
else:
    top.Draw()
    bottom.Draw()


top.cd()
#top.SetLogx()
mcTotal = mcSum #mcStack.GetStack().Last()
#mcTotal.GetXaxis().SetRangeUser(0.0, 3000.)
mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

mcStack.SetMinimum(1)
mcStack.SetMaximum(dataHistnorm.GetMaximum()*1.5) #Make room for annotations above the plot
#mcStack.GetHistogram().GetYaxis().SetRangeUser(0, 5000)
#mcStack.GetXaxis().SetRangeUser(0.0, 3000.)
#mcStack.Draw("HIST")
#mcStack.GetHistogram().GetYaxis().SetRangeUser(0.1, 4000.)

#mcStack.Draw("HIST")

orderedStack.SetMinimum(0)
orderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
orderedStack.Draw("HIST")
#orderedStack.GetHistogram().GetXaxis().SetRangeUser(8, 2400.)
orderedStack.GetHistogram().GetYaxis().SetTitle("N Events / " + sys.argv[7])
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
dataHistnorm.GetYaxis().SetTitle("N Events / " + sys.argv[7])
#dataHistnorm.GetXaxis().SetRangeUser(0.0, 2000.)
dataHistnorm.Draw("SAME")

#legend = top.BuildLegend(0.75, 0.4, .95, 0.9) #TODO: Explain legend position. second number is how much Y is left at the bottom of the top plot. Want this to be bigger, if you want legend to be shorter.  ROOT 6 might even make an intelligent decision about where to place it.
if (int(sys.argv[6]) == 1):
	legend = top.BuildLegend(0.16, 0.27, 0.38, 0.9) #for log scale hists that do not have many events at 0
else:
	legend = top.BuildLegend(0.52, 0.25, 0.93, 0.9)

legend.SetTextFont(62)
legend.SetTextSizePixels(5)
legend.SetFillStyle(0)
plotter = PlotUtils.MnvPlotter()
plotter.WritePreliminary(0.26, 0.976, .05, True)
#plotter.WritePreliminary(0.26, 0.97, .03, True)


#Data/MC ratio panel
bottom.cd()
bottom.SetTopMargin(0)
bottom.SetBottomMargin(0.3)

#dataHist.SetMinimum(1.0)
#dataHist.GetXaxis().SetRangeUser(1.0, 1000.)
#mcSum.GetXaxis().SetRangeUser(1.0, 1000.)
ratio = dataHist.Clone()
mcTotalWithSys = mcSum.Clone()
ratio.Divide(ratio, mcTotalWithSys)
#ratio.GetXaxis().SetRangeUser(1.0,1000.)
#TODO: I need GetCVHistoWithError() from mcRatio, but THStack doesn't keep a MnvH1D.  I have to Add() the histograms myself.

#Now fill mcRatio with 1 for bin content and fractional error
mcRatio = mcTotalWithSys.GetTotalError(False, True, False) #The second "true" makes this fractional error
for whichBin in range(0, mcRatio.GetXaxis().GetNbins()+1):
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
ratio.GetXaxis().SetTitle(sys.argv[5])
ratio.GetXaxis().SetTitleSize(0.12)
ratio.GetXaxis().SetTitleOffset(1.05)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)
#ratio.GetXaxis().SetRangeUser(8, 2400.)
#orderedStack.Draw("HIST")
ratio.SetBinContent(0, 0.00001)
ratio.Draw()

#Error envelope for the MC
mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
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
#title.AddText(var)
#title.Draw()

#plotter = PlotUtils.MnvPlotter()
plotter.AddChi2Label(dataHist,signalHist)
#plotter.WritePreliminary(0.3, 0.82, 5e-2, True)

#Make a PNG file of this canvas
#overall.SetLogx()
#overall.Update()
overall.Print(var + "_" + sys.argv[4] + "_newcat_DataMCRatio.png")

databkgsub = dataHist.Clone()
for hist in bkghists:
    #hist.Scale(dataPOT/mcPOT)
    databkgsub.Add(hist, -1)
    mcSum.Add(hist,-1)

norderedStack = THStack()
#norderedStack.Add(hists[0]) #kaons not included
#norderedStack.Add(hists[2]) #other not included in signal definition
norderedStack.Add(hists[1]) #COH category should be above the backgrounds
norderedStack.Add(hists[5])
norderedStack.Add(hists[6])
norderedStack.Add(hists[7])
norderedStack.Add(hists[8])
norderedStack.Add(hists[9])
norderedStack.Add(hists[10])
#norderedStack.Add(hists[11])
#norderedStack.Add(hists[12])

#dataHistnorm.clear()

datahistnorm = databkgsub.GetBinNormalizedCopy().Clone()
datahistnorm.SetLineColor(kBlack)
datahistnorm.SetLineWidth(lineSize)
datahistnorm.SetMarkerStyle(20) #Resizeable closed circle
datahistnorm.SetMarkerColor(kBlack)
datahistnorm.SetMarkerSize(0.7)
datahistnorm.SetTitle("Data")
datahistnorm.GetYaxis().SetTitle("N Events / " + sys.argv[7])

norderedStack.SetMinimum(0)
norderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
norderedStack.Draw("HIST")
#norderedStack.GetHistogram().GetXaxis().SetRangeUser(1.0, 1000.)
norderedStack.GetHistogram().GetYaxis().SetTitle("N Events /  " + sys.argv[7])
norderedStack.GetHistogram().GetXaxis().SetTitleOffset(1.15)
norderedStack.GetHistogram().GetYaxis().SetTitleOffset(0.7)
#norderedStack.GetHistogram().GetYaxis().SetTitleSize(0.16)
norderedStack.Draw("HIST")
datahistnorm.Draw("SAMES")

if (int(sys.argv[6]) == 1):
        legend = top.BuildLegend(0.16, 0.27, 0.38, 0.9) #for log scale hists that do not have many events at 0
else:
        legend = top.BuildLegend(0.42, 0.40, 0.93, 0.9)

legend.SetTextFont(62)
legend.SetTextSizePixels(5)
legend.SetFillStyle(0)
plotter = PlotUtils.MnvPlotter()
plotter.WritePreliminary(0.26, 0.976, .05, True)

bottom.cd()
ratio = databkgsub.Clone()
mcTotalWithSys = mcSum.Clone()
ratio.Divide(ratio, mcTotalWithSys)
#ratio.GetXaxis().SetRangeUser(1.0,1000.)
ratio.SetTitle("")
ratio.SetLineColor(kBlack)
ratio.SetLineWidth(lineSize)
ratio.SetTitleSize(0)

ratio.GetYaxis().SetTitle("Data / MC")
ratio.GetYaxis().SetLabelSize(labelSize)
ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(0.2)
ratio.GetYaxis().SetNdivisions(505) #5 minor divisions between 5 major divisions.  I'm trying to match a specific paper here.
ratio.GetXaxis().SetTitle(sys.argv[5])
ratio.GetXaxis().SetTitleSize(0.12)
ratio.GetXaxis().SetTitleOffset(1.02)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)

#datahistnorm.SetMinimum(ratioMin)
#datahistnorm.SetMaximum(ratioMax)
#datahistnorm.Draw("")
ratio.Draw("")

mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
mcRatio.Draw("E2 SAME")

#Draw a flat line at 1 for ratio of MC to itself
straightLine = mcRatio.Clone()
straightLine.SetFillStyle(0)
straightLine.Draw("HIST SAME")

top.cd()

plotter.AddChi2Label(databkgsub,mcSum)
overall.Print(var + "_" + sys.argv[4] + "_bkgsubtracted_newcat_DataMCRatio.png")


newcan = TCanvas("Stacked Histo For " + var)
if (int(sys.argv[6]) == 1):
    newcan.SetLogx()
orderedStack.Draw("HIST")
orderedStack.GetHistogram().GetXaxis().SetTitle(sys.argv[5])
orderedStack.GetHistogram().GetXaxis().SetTitleOffset(1.15)
dataHistnorm.Draw("SAME")
legend = newcan.BuildLegend(0.16, 0.62, 0.93, 0.9) # ("WHERE TO START IN X (from left)", "HOW LOW DOES THE LEGEND GO IN Y", :"SOMETHING ABOUT CORNER higher the number, the more it is to the right", "SOMETHING ABOUT CORNER")" For Available Energy hists (lots of events near 0)
#legend = newcan.BuildLegend(0.16, 0.27, 0.38, 0.9) #for log scale hists that do not have many events at 0
legend.SetTextFont(62)
legend.SetTextSizePixels(5)
legend.SetFillStyle(0)
plotter.WritePreliminary(0.26, 0.97, .03, True)
newcan.Print(var + "_noratio_" + sys.argv[4] + ".png")
