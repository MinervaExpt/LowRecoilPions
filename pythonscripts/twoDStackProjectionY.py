#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys 

gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.

var = sys.argv[4] #"NewEAvail_pTmubins" #"pTmu"
dataName = var + "_DATA" #"_data"

ratioMin = 0.0
ratioMax = 3.5

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)
dataFile = TFile.Open(sys.argv[2])
mcFile = TFile.Open(sys.argv[1])


mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

print("Data MC POT Ratio is ", dataPOT/mcPOT)

#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
#mcStack = THStack()
signalHist = mcFile.Get(var + "_MC")#"_selected_signal_reco")
signalHist.Scale(dataPOT/mcPOT)
mcSum = signalHist.Clone()
mcSum.Add(mcSum,-1)
nybins = signalHist.GetNbinsY()

i = int(sys.argv[3])
if i == int(sys.argv[3]):
	#signalHist = mcFile.Get(var + "_data")
	#signalHist.Scale(dataPOT/mcPOT)
	mci = signalHist.ProjectionX("mc",i,i)
	mci.Add(mci,-1)
	mcStack = THStack()
	hists = [None]*7
        for key in mcFile.GetListOfKeys():
  		name = str(key.GetName())
		if (name.find("GENIE") > -1): continue
  		if name.find(var+"_") > -1 and name.find("_DATA") < 0 and name.find("_MC") < 0 and name.find("_bkg") < 0:
    			hist = key.ReadObj()
    			print "Name of Hist: ", name
    			hist.Scale(dataPOT/mcPOT)
    			newname = hist.GetName()
			#if newname.find("_DATA") > -1 or : continue
			if newname.find("_Kaons_Only") > -1:
                        	hist.SetLineColor(1321)
                        	hist.SetFillColor(1321)
    				hists[0] = hist.GetCVHistoWithError().Clone()
			if newname.find("_NPi_Plus_Only") > -1:
                        	hist.SetTitle("N #pi^{+} Only")
                        	hist.SetLineColor(1319)
                        	hist.SetFillColor(1319)
				hists[6] = hist.GetCVHistoWithError().Clone()
    			if newname.find("_NPi_and_Other_Mesons") > -1:
                        	hist.SetTitle("N #pi^{+} and Mesons")
                        	hist.SetLineColor(1323)
                        	hist.SetFillColor(1323)
				hists[5] = hist.GetCVHistoWithError().Clone()
			if newname.find("_Other") > -1:
			 	hist.SetLineColor(1322)
                        	hist.SetFillColor(1322)
				hists[2] = hist.GetCVHistoWithError().Clone()
    			if newname.find("_Neutral_Pi") > -1:
				hist.SetLineColor(1320)
                        	hist.SetFillColor(1320)
				hists[4] = hist.GetCVHistoWithError().Clone()
    			if newname.find("_QE_like") > -1:
				hist.SetLineColor(1318)
                        	hist.SetFillColor(1318)
				hists[3] = hist.GetCVHistoWithError().Clone()
    			if newname.find("_COH_Pions") > -1:
                        	hist.SetLineColor(1317)
                        	hist.SetFillColor(1317)
				hists[1] = hist.GetCVHistoWithError().Clone()
			histi = hist.ProjectionX("e",i,i)
			histi.SetName(name)
			
			#hist2D = histi.GetCVHistoWithError().Clone()
			mcStack.Add(histi.GetCVHistoWithError().Clone())	
    			mci.Add(histi)

        #mcStack.Add(signalHist.GetCVHistoWithError().Clone())
	#Apply a different color for each MC category
	mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
	print "Size of Color Palette ", mcColors.size()
	nextColor = -1
	colororder = [0, 1, 3, 2, 4, 5]	

	dataHist2D = dataFile.Get(dataName)
	dataHist = dataHist2D.ProjectionX("e", i,i)
	dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
	dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
	dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_DATA"))#"_selected_signal_reco"))
	
	orderedStack = THStack()
	orderedStack.Add(hists[0].ProjectionX("e", i,i))
	orderedStack.Add(hists[1].ProjectionX("e", i,i))
	orderedStack.Add(hists[2].ProjectionX("e", i,i))
	orderedStack.Add(hists[3].ProjectionX("e", i,i))
	orderedStack.Add(hists[4].ProjectionX("e", i,i))
	orderedStack.Add(hists[5].ProjectionX("e", i,i))
	orderedStack.Add(hists[6].ProjectionX("e", i,i))


	#Create a TCanvas on which to draw plots and split it into 2 panels
	overall = TCanvas("Data/MC for " + var)
	top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
	bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
	#Thou shalt Draw() new TPads lest they be blank!
	#overall.SetLogx()
        #top.SetLogy()
	#top.SetLogx()
        top.Draw()
	#bottom.SetLogx()
        
	bottom.Draw()

	top.cd()
	mcTot = mci #signalHist.ProjectionX("e",i,i)#GetCVHistoWithError().Clone(); #mcStack.GetStack().Last()
	mcTotal = mcTot.GetCVHistoWithError().Clone();
	mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

	mcStack.SetMinimum(1)
	mcStack.SetMaximum(mcTotal.GetMaximum() * 4) #Make room for annotations above the plot
	mcStack.Draw("HIST")


        orderedStack.SetMinimum(1)
	orderedStack.SetMaximum(700)#dataWithStatErrors.GetMaximum()*1.5) #Make room for annotations above the plot
	orderedStack.Draw("HIST")
        orderedStack.GetHistogram().GetYaxis().SetTitle("N Events")
        orderedStack.Draw("HIST")
	dataWithStatErrors.SetLineColor(kBlack)
	dataWithStatErrors.SetLineWidth(lineSize)
	dataWithStatErrors.SetMarkerStyle(20) #Resizeable closed circle
	dataWithStatErrors.SetMarkerColor(kBlack)
	dataWithStatErrors.SetMarkerSize(0.7)
	dataWithStatErrors.SetTitle("Data")
	dataWithStatErrors.Draw("SAME")

	#dataHistnorm.SetLineColor(kBlack)
        #dataHistnorm.SetLineWidth(lineSize)
        #dataHistnorm.SetMarkerStyle(20) #Resizeable closed circle
        #dataHistnorm.SetMarkerColor(kBlack)
        #dataHistnorm.SetMarkerSize(0.7)
        #dataHistnorm.SetTitle("Data")
        #dataHistnorm.GetYaxis().SetTitle("entries")
	#dataHistnorm.Draw("SAME")

	#legend = top.BuildLegend(0.5, 0.1, 0.9, 0.9) #TODO: Explain legend position.  ROOT 6 might even make an intelligent decision about where to place it.

	#Data/MC ratio panel
	bottom.cd()
	bottom.SetTopMargin(0)
	bottom.SetBottomMargin(0.3)

	ratio0 = dataWithStatErrors.Clone() #dataHist2D.ProjectionX("e", i,i) #dataWithStatErrors.Clone()#dataHist.Clone()
	mcTotalWithSys = mci
	mcTotalWithSys.SetName("mcTotal")
	ratio0.Divide(ratio0, mcTotalWithSys)
	ratio = ratio0.Clone() #ratio0.Divide(ratio0.GetCVHistoWithError(), mcTotalWithSys)

	#print("Getting Ratio Bin Content ", ratio.GetBinContent(i,3))
	#ratio.Draw()
#TODO: I need GetCVHistoWithError() from mcRatio, but THStack doesn't keep a MnvH1D.  I have to Add() the histograms myself.

#Now fill mcRatio with 1 for bin content and fractional error
	mcRatio = mci.GetTotalError(False, True, False) #The second "true" makes this fractional error
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
        ratio.GetXaxis().SetTitle(sys.argv[5])
	ratio.SetMinimum(ratioMin)
	ratio.SetMaximum(ratioMax)
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

	ratio.Draw("SAME")
	#Title for the whole plot
	top.cd()
	title = TPaveText(0.3, 0.91, 0.7, 1.0, "nbNDC") #no border and use Normalized Device Coordinates to place it
	title.SetFillStyle(0)
	title.SetLineColor(kWhite)
	#title.AddText("CC Inclusive Event Rate")
	#title.Draw()

	plotter = PlotUtils.MnvPlotter()
	plotter.WritePreliminary(0.3, 0.82, 5e-2, True)
	#ratio.Print(var+"_bin"+str(i)+"_ratio.png")
	#Make a PNG file of this canvas
	overall.Print(var + "_bin"+str(i) +"_nolog_DataMCRatio.png")
        
        ratiocan = TCanvas("Ratio_ " + var)
	ratiocan.cd()
        #ratiocan.SetLogx()
	ratio.GetYaxis().SetTitle("Data / MC")
        ratio.GetYaxis().SetLabelSize(0.06)#labelSize)
        ratio.GetYaxis().SetTitleSize(0.03)
        ratio.GetYaxis().SetTitleOffset(1.3)
        ratio.GetYaxis().SetNdivisions(505)
	ratio.GetXaxis().SetTitleSize(0.06)
        ratio.GetXaxis().SetTitleOffset(1.1)
        ratio.GetXaxis().SetLabelSize(0.02) #labelSize)
        ratio.GetXaxis().SetTitle(sys.argv[5])
        ratio.SetMinimum(ratioMin)
        ratio.SetMaximum(ratioMax)
        ratio.Draw()

        ratio.Draw()
	mcRatio.Draw("E2 SAME")
	straightLine.Draw("HIST SAME")
	ratio.Draw("SAME")
	ratiocan.Print(var +"_RATIO" + "_bin"+str(i)+"_logx.png")
        
