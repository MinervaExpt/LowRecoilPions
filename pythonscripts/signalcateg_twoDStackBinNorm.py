#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys 
import array as ar
from ROOT import gStyle
gStyle.SetHatchesSpacing(.10)

gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.
gStyle.SetLegendBorderSize(0)

var = sys.argv[4] #"NewEAvail_pTmubins" #"pTmu"
dataName = var + "_data" #"_data"

ratioMin = 0.0
ratioMax = 5.0

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)

dataFile = TFile.Open(sys.argv[2])
mcFile = TFile.Open(sys.argv[1])

sel = sys.argv[6]

mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

TH1.AddDirectory(False)

print("MC POT", mcPOT)
print("DATA POT", dataPOT)

#Organize the MC backgrounds into a stacked histogram.
#Also keep a sum of backgrounds that has full systematics
#information.
#mcStack = THStack()
signalHist = mcFile.Get(var + "_MC") #"_selected_signal_reco")
xbins = signalHist.GetNbinsX();
for i in range(1,xbins+1):
   print("bin center for bin " , i , signalHist.GetXaxis().GetBinLowEdge(i))

print(signalHist)
#signalHist.Scale(dataPOT/mcPOT)
mcSum = signalHist.Clone()
mcSum.Add(mcSum,-1)
nybins = signalHist.GetNbinsY()

i = int(sys.argv[3])
if i == int(sys.argv[3]):
	#signalHist = mcFile.Get(var + "_data")
	mci = signalHist.ProjectionX("mc",i,i)
	mci.Add(mci,-1)
	mcStack = THStack()
	hists = [None]*11
	bkghists = []
        for key in mcFile.GetListOfKeys():
  		name = str(key.GetName())
		if (name.find("GENIE") > -1): continue
	 	if (name.find("_top_") > -1): continue
		if name.find("s"+var) > -1: continue
		if name.find(var+"_by_BKG_Label") > -1: bkghists.append(key.ReadObj())
  		if name.find(var+"_") > -1 and name.find("_DATA") < 0 and name.find("_MC") < 0 and name.find("_bkg") < 0:
    		    if name.find("picat") > -1 and name.find(var+"_picat_") > -1:
			hist0 = key.ReadObj()
    			print "Name of Hist: ", name
    			#hist0.Scale(dataPOT/mcPOT)
    			newname = hist0.GetName()
			#if newname.find("_DATA") > -1 or : continue
			hist = hist0.ProjectionX("e",i,i)
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
                           hist.SetFillStyle(3144)
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
			if newname.find("_picat_NPi_and_Mesons_RES") > -1:
                           hist.SetTitle("N #pi^{+} and Mesons (RES)")
                           hist.SetLineColor(kBlue-10)
                           hist.SetFillColor(kBlue-10)
			   hist.SetFillStyle(3144)
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
                           hist.SetTitle("Neutral Pions")
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
                           hists[1] = hist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue

			histi = hist.Clone()#.ProjectionX("e",i,i)
			histi.SetName(name)
			mcSum.Add(hist0)		
			#hist2D = histi.GetCVHistoWithError().Clone()
			mcStack.Add(histi.GetCVHistoWithError().Clone())	
    			mci.Add(histi)

        #mcStack.Add(signalHist.GetCVHistoWithError().Clone())
	#Apply a different color for each MC category
	mcColors = MnvColors.GetColors(MnvColors.kOkabeItoDarkPalette)
	print "Size of Color Palette ", mcColors.size()
	nextColor = -1
	colororder = [0, 1, 3, 2, 4, 5]	
		#print "Setting Color", mcColors[nextColor], " for ", hist.GetName()
		#nextColor = nextColor + 1

	dataHist2D = dataFile.Get(dataName)
	dataHist2D.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))
	dataHist = dataHist2D.ProjectionX("e", i,i) 
	dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
	dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
        #dataHistnorm = dataHistnorm0.ProjectionX("e", i,i)
	#dataHist.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))#"_selected_signal_reco"))
	
	orderedStack = THStack()
	orderedStack.Add(hists[0])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[3])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[2])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[4])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[1])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[5])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[6])#.ProjectionX("e", i,i))
	orderedStack.Add(hists[7])#.ProjectionX("e", i,i))
        #orderedStack.Add(hists[1]) #COH 
	orderedStack.Add(hists[8])#.ProjectionX("e", i,i))
        orderedStack.Add(hists[9])#.ProjectionX("e", i,i))
        orderedStack.Add(hists[10])#.ProjectionX("e", i,i))

	#Create a TCanvas on which to draw plots and split it into 2 panels
	overall = TCanvas("Data/MC for " + var)
	top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
	bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
	#Thou shalt Draw() new TPads lest they be blank!
	#overall.SetLogx()
        #top.SetLogy()
        

        if (int(sys.argv[7]) == 1):
    		overall.SetLogx()
    		top.SetLogx()
    		top.Draw()
    		bottom.SetLogx()
    		bottom.Draw()
	else: 
		if (int(sys.argv[7]) == 0):
    			top.Draw()
    			bottom.Draw()


	top.cd()
	mcTot = mci #signalHist.ProjectionX("e",i,i)#GetCVHistoWithError().Clone(); #mcStack.GetStack().Last()
	mcTotal = mcTot.GetCVHistoWithError().Clone();
	#mcTotal.Draw("E2") #Draw the error envelope only.  Not used here except to force the THStack to calculate its sum.

	#mcStack.SetMinimum(1)
	#mcStack.SetMaximum(mcTotal.GetMaximum() * 4) #Make room for annotations above the plot
	#mcStack.Draw("HIST")


        orderedStack.SetMinimum(0)
	orderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.8) #1.5) #Make room for annotations above the plot
	orderedStack.Draw("HIST")
	orderedStack.GetHistogram().GetYaxis().SetTitle("N Events / " + sys.argv[8])
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
        dataHistnorm.GetYaxis().SetTitle("entries")
	dataHistnorm.Draw("SAMES")

	#legend = top.BuildLegend(0.75, 0.4, .95, 0.9)
	#legend = top.BuildLegend(0.5, 0.1, 0.9, 0.9) #TODO: Explain legend position.  ROOT 6 might even make an intelligent decision about where to place it.
    
        if (int(sys.argv[7]) == 1):
        	#legend = top.BuildLegend(0.10, 0.5, 0.38, 0.9) #for log scale hists that do not have many events at 0
	        #legend = top.BuildLegend(0.12, 0.5, 0.93, 0.9)
		legend = top.BuildLegend(0.65, 0.5, 0.93, 0.9) #TODO: keep at .65 for first number for RHS legend 
	else:
        	legend = top.BuildLegend(0.55, 0.58, 0.93, 0.9)

	legend.SetTextFont(62)
	legend.SetTextSizePixels(5)
	legend.SetFillStyle(0)
	plotter = PlotUtils.MnvPlotter()
	plotter.WritePreliminary(0.26, 0.966, .05, True)


	#Data/MC ratio panel
	bottom.cd()
	bottom.SetTopMargin(0)
	bottom.SetBottomMargin(0.3)

	ratio0 = dataHist.GetCVHistoWithError().Clone() #2D.ProjectionX("e", i,i) #dataWithStatErrors.Clone()#dataHist.Clone()
	mcTotalWithSys = mci.GetCVHistoWithError().Clone()
	mcTotalWithSys.SetName("mcTotal")
	ratio0.Divide(ratio0, mcTotalWithSys)
	ratio = ratio0.Clone()
	ratioMax = 3.0 #ratio.GetMaximum()*1.10
	ratio.SetBinContent(0, 1.0)
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

	#plotter = PlotUtils.MnvPlotter()
	#plotter.WritePreliminary(0.3, 0.82, 5e-2, True)
	#ratio.Print(var+"_bin"+str(i)+"_ratio.png")
	#Make a PNG file of this canvas
	overall.Print(var + "_bin"+str(i) +"_"+ sel +"_DataMCRatio.png")

        #overall2 = TCanvas("ratio can for " + var)
	#overall2.cd()
	#ratio.Draw()
	#mcRatio.Draw("E2 SAME")
	#straightLine.Draw("HIST SAME")
	#ratio.Draw("SAMES")
	#overall2.SetGrid()
 	#overall2.Print(var + "_bin"+str(i) +"_"+ sel +"_RATIOONLY.png")

        top.cd()
	nbinsx = ratio.GetNbinsX()
	nbin = nbinsx
	ratiox = ar.array("d", [0]*nbin)
        ratioy = ar.array("d", [0]*nbin)
	xerrs = ar.array("d", [0]*nbin)
        yerrs = ar.array("d", [0]*nbin)
        print nbinsx
	ratiox[0] = 0.0
        ratioy[0] = 1.0	
        for j in range(1, nbin):
	      xval = ratio.GetBinCenter(j)
	      yval = ratio.GetBinContent(j)
	      if (j > 4):
		 ratiox[j] = xval
		 ratioy[j] = 1.0
	      else:
	      	ratiox[j] = xval
	      	ratioy[j] = yval
	      print(ratiox[j],ratioy[j])
	#ratiox[10] = 1.0
	#ratioy[10] = 1.0
	print ratiox, ratioy
	
	gt = TFile.Open("RatioFit_line_forcedto1_bin"+str(i)+".root", "RECREATE") 
        g = ROOT.TGraph(nbinsx, ratiox, ratioy)
	g.SetName("bin"+str(i)+"_Ratio")
	g.GetXaxis().SetTitle("Available Energy (MeV)")
	g.GetYaxis().SetTitle("DATA/MC Ratio")
        gefit = g.Clone()
	g.Draw("AC*")
	g.Write()

	func = TF1('func', '[0] + [1]*x', 0.0, .270)
	#func = TF1('func', 'gaus', 0.025, 0.300)
	func2 = TF1('func2', '[0]*1', 0.300, 1.2)

	func3 = TF1('func3', 'func+func2', 0.0, 1.2)

	#Define the parameter array for the total function
	#par = ar.array("d", [0]*3)
	#gefit = g.Clone()
	gefit.SetName("RatioFit_bin"+str(i))
	fit = gefit.Fit(func,'R+S')
	chiperndf = fit.Chi2()/fit.Ndf()
        print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
        print "Chi Squared Per Degree of Freedom: ", chiperndf
	fit = gefit.Fit(func2,'R+S')
	
	#par = func.GetParameters()
	#print par
	#par2 = func2.GetParameters()
	#parm = [par[0], par[1], par[2], 1.0]
	#print parm
	#func3.SetParameters(par[0], par[1],par[2], par2[0])
 	
	#fit =  gefit.Fit(func3,'R+S')	

	chiperndf = fit.Chi2()/fit.Ndf()
	print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
	print "Chi Squared Per Degree of Freedom: ", chiperndf
	gefit.Write()
	
	gt.Close()
	databkgsub = dataHist #dataWithStatErrors.Clone()  #dataHist.Clone()
	for bkg in bkghists:
		#bkg.Scale(dataPOT/mcPOT)
		bkgi = bkg.ProjectionX("e", i,i)
		databkgsub.Add(bkgi,-1)
		mci.Add(bkgi, -1)
		print("Background Subtracted: ", bkg.GetName())

        norderedStack = THStack()
	norderedStack.Add(hists[1])#.ProjectionX("e", i,i))
        norderedStack.Add(hists[5])#.ProjectionX("e", i,i))
        norderedStack.Add(hists[6])#.ProjectionX("e", i,i))
        norderedStack.Add(hists[7])#.ProjectionX("e", i,i))
        #norderedStack.Add(hists[1])
	norderedStack.Add(hists[8])#.ProjectionX("e", i,i))
        norderedStack.Add(hists[9])#.ProjectionX("e", i,i))
        norderedStack.Add(hists[10])#.ProjectionX("e", i,i))

        datahistnorm0 = databkgsub.GetBinNormalizedCopy().Clone()
	#datahistnorm = datahistnorm0.Clone() #.ProjectionX("e",i,i) #GetBinNormalizedCopy().Clone()
	datahistnorm0.SetLineColor(kBlack)
	datahistnorm0.SetLineWidth(lineSize)
	datahistnorm0.SetMarkerStyle(20) #Resizeable closed circle
	datahistnorm0.SetMarkerColor(kBlack)
	datahistnorm0.SetMarkerSize(0.7)
	datahistnorm0.SetTitle("Data")
	datahistnorm0.GetYaxis().SetTitle("N Events")
	datahistnorm0.GetYaxis().SetTitleOffset(0.7)
	norderedStack.SetMinimum(0)
	norderedStack.SetMaximum(dataHistnorm.GetMaximum()*1.5) #dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
	norderedStack.Draw("HIST")
	#norderedStack.GetHistogram().GetYaxis().SetTitle("N Events")
        norderedStack.GetHistogram().GetYaxis().SetTitle("N Events / " + sys.argv[8])
	norderedStack.GetYaxis().SetTitleOffset(0.7)
	norderedStack.Draw("HIST")
	datahistnorm0.Draw("SAMES")

	#legend = top.BuildLegend(0.75, 0.4, .95, 0.9)
	
	bottom.cd()
	ratio1 = databkgsub.Clone()
        ratio1.Divide(ratio1, mci.Clone())
	#ratio1 = ratiotot.Clone() #ProjectionX("e",i,i)
	ratio1.SetTitle("")
        ratio1.SetLineColor(kBlack)
        ratio1.SetLineWidth(lineSize)
        ratio1.SetTitleSize(0)

        ratio1.GetYaxis().SetTitle("Data / MC")
        ratio1.GetYaxis().SetLabelSize(labelSize)
        ratio1.GetYaxis().SetTitleSize(0.16)
        ratio1.GetYaxis().SetTitleOffset(0.2)
        ratio1.GetYaxis().SetNdivisions(505) #5 minor divisions between 5 major divisions.  I'm trying to match a specific paper here.

        ratio1.GetXaxis().SetTitleSize(0.16)
        ratio1.GetXaxis().SetTitleOffset(0.9)
        ratio1.GetXaxis().SetLabelSize(labelSize)
        ratio1.GetXaxis().SetTitle(sys.argv[5])
        ratio1.SetMinimum(ratioMin)
        ratio1.SetMaximum(ratioMax)
        ratio1.Draw()

        #Error envelope for the MC
        mcRatio.SetLineColor(kRed)
        mcRatio.SetLineWidth(lineSize)
        mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
        mcRatio.Draw("E2 SAME")

        #Draw a flat line at 1 for ratio of MC to itself
        straightLine = mcRatio.Clone()
        straightLine.SetFillStyle(0)
        straightLine.Draw("HIST SAME")

        ratio1.Draw("SAMES")

	top.cd()

	#plotter.AddChi2Label(databkgsub,mci)
	if (int(sys.argv[7]) == 1):
                #legend = top.BuildLegend(0.10, 0.5, 0.38, 0.9) #for log scale hists that do not have many events at 0
                #legend = top.BuildLegend(0.12, 0.5, 0.93, 0.9)
                legend = top.BuildLegend(0.55, 0.5, 0.93, 0.9) #TODO: keep at .65 for first number for RHS legend 
        else:
                legend = top.BuildLegend(0.55, 0.58, 0.93, 0.9)

	legend.SetTextFont(62)
        legend.SetTextSizePixels(5)
        legend.SetFillStyle(0)
	#plotter.AddChi2Label(databkgsub,mci)	
	plotter.WritePreliminary(0.26, 0.966, .05, True)
	overall.Print(var + "_bkgsub_bin"+str(i) +"_"+ sel +"_DataMCRatio.png")
        #overall.Print(var + "_bkgsub_bin"+str(i) +"_"+ sel +"_DataMCRatio.root")
                                                                                            
