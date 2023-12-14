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

TH1.AddDirectory(False)
dataFile = TFile.Open(sys.argv[2])
mcFile = TFile.Open(sys.argv[1])

mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

signalHist = mcFile.Get(var + "_MC")
mcSum = signalHist.Clone()
mcSum.Add(mcSum,-1)
nybins = signalHist.GetNbinsY()

ratioplots = []
ratioMin = 0.0
ratioMax = 4.0

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3
import copy
#i = int(sys.argv[3])
#if i == int(sys.argv[3]):
if int(sys.argv[3]) > 0:
       	if int(sys.argv[3]) == 1: scale = dataPOT/mcPOT
	if int(sys.argv[3]) == 2: scale = 1.0
	mci = copy.copy(signalHist)#.Clone()#.ProjectionX("mc",i,i)
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
			newname = hist0.GetName()
			hist = hist0#.ProjectionX("e",i,i)
                        if newname.find("_picat_Kaons_Only") > -1:
                           hist.SetName("Kaons Only")
                           hist.SetLineColor(1321)
                           hist.SetFillColor(1321)
                           hists[0] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
                        if newname.find("_picat_1pip_w_LProton") > -1:
                           hist.SetTitle("1 #pi^{+} w Proton")
                           hist.SetLineColor(1319)
                           hist.SetFillColor(1319)
                           hists[10] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
                        if newname.find("_picat_1pip_w_LNeutron") > -1:
                           hist.SetTitle("1 #pi^{+} w Neutron")
                           lightcolor = kGreen - 5#1319+100 # TColor.GetColorDark(1319)
                           hist.SetLineColor(lightcolor)
                           hist.SetFillColor(lightcolor)
                           hist.SetFillStyle(3144)
                           hists[9] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
                        if newname.find("_picat_1pip_wo_p_or_n") > -1:
                           hist.SetTitle("1 #pi^{+} w/o p or n")
                           lightcolor = kGreen - 8#TColor.GetColorLight(1319)
                           hist.SetLineColor(lightcolor)
                           hist.SetFillColor(lightcolor)
                           hists[8] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
                        if newname.find("_picat_Npip_Only") > -1:
                           hist.SetTitle("N #pi^{+}, no other mesons")
                           hist.SetLineColor(1323)
                           hist.SetFillColor(1323)
			   hists[7] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
                        if newname.find("_picat_NPi_and_Mesons_RES") > -1:
                           hist.SetTitle("N #pi^{+} and Mesons (RES)")
                           hist.SetLineColor(kBlue-10)
                           hist.SetFillColor(kBlue-10)
                           hist.SetFillStyle(3144)
                           hists[6] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
                        if newname.find("_picat_NPi_and_Mesons_DIS") > -1:
                           hist.SetTitle("N #pi^{+} and Mesons (DIS)")
                           hist.SetLineColor(kBlue-6)
                           hist.SetFillColor(kBlue-6)
                           hists[5] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue
                        if newname.find("_picat_Other") > -1:
                           hist.SetLineColor(1322)
                           hist.SetFillColor(1322)
                           hists[2] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
                        if newname.find("_picat_Neutral_Pi_Only") > -1:
			   hist.SetTitle("Neutral Pions")
                           hist.SetLineColor(1320)
                           hist.SetFillColor(1320)
                           hists[4] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
                        if newname.find("_picat_QE_like") > -1:
                           hist.SetLineColor(1318)
                           hist.SetFillColor(1318)
                           hists[3] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()
                        if newname.find("_picat_COH_Pions") > -1:
                           hist.SetTitle("Coherent Pions")
                           hist.SetLineColor(1317)
                           hist.SetFillColor(1317)
                           hists[1] = hist#.GetBinNormalizedCopy().GetCVHistoWithError().Clone()#continue

                        histi = hist.Clone()#.ProjectionX("e",i,i)
                        histi.SetName(name)
			mcSum.Add(hist0)
                        mci.Add(hist0)


	print(mci)
	dataHist2D = dataFile.Get(dataName)
        dataHist2D.AddMissingErrorBandsAndFillWithCV(mcFile.Get(var + "_data"))
        dataHist = dataHist2D#.ProjectionX("e", i,i)
        #dataWithStatErrors = dataHist.GetCVHistoWithError().Clone()
        #dataHistnorm = dataHist.GetBinNormalizedCopy().GetCVHistoWithError().Clone()

		   
	databkgsub = dataHist2D
	#mci.Scale(scale)
	for bkg in bkghists:
		#bkgi = bkg#.ProjectionX("e", i,i)
                #bkg.Scale(scale)
		#mci.Scale(scale)
	        databkgsub.Add(bkg,-1)
                mci.Add(bkg, -1)
		print("Background Subtracted: ", bkg.GetName())
	for oth in [1,5,6,7,8,10]:	
		sigoth = hists[oth]
       		#sigoth.Scale(scale)
		databkgsub.Add(sigoth,-1)
                mci.Add(sigoth, -1)
		print("Subtracing ", sigoth.GetName())


	#histtodraw = hists[9]
	#histtodraw.Scale(scale)	
	plotter = PlotUtils.MnvPlotter()
	ratio0 = databkgsub.Clone()
	#mciproj = mci.ProjectionX("",1,1)
	#databkgsubproj = databkgsub.ProjectionX("",1,1)
	
	ratio0.Divide(ratio0,mci)
	#ratio1 = databkgsub.Clone()
	#ratio1.Divide(ratio1, histtodraw)

	mcRatio = mci.GetTotalError(False, True, False)
	for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
                mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
                mcRatio.SetBinContent(whichBin, 1)

	for i in range(1,9):
	     overall = TCanvas("Data/MC for " + var)
             top = TPad("Overlay", "Overlay", 0, bottomFraction, 1, 1)
             bottom = TPad("Ratio", "Ratio", 0, 0, 1, bottomFraction + margin)
	     top.Draw()
             bottom.Draw()
	     top.cd()
	     print("Now projecting MC histo")
	     projimc = mci.ProjectionX("h",i,i)
	     print("Now projecting Data histo")
	     projidata = databkgsub.ProjectionX("h", i,i)
	     projimc.SetMaximum(projidata.GetMaximum()*1.8)
	     projimc.GetYaxis().SetTitle("N Events")
	     projimc.Draw("HIST")
	     

	     norderedStack = THStack()
	     norderedStack.Add(hists[0].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[3].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[2].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[4].ProjectionX("e", i,i))
	     # The following are signal histos
             norderedStack.Add(hists[1].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[5].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[6].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[7].ProjectionX("e", i,i))
	     norderedStack.Add(hists[8].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[9].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
             norderedStack.Add(hists[10].ProjectionX("e", i,i))#.ProjectionX("e", i,i))
		

	     norderedStack.SetMinimum(0)
             norderedStack.SetMaximum(projidata.GetMaximum()*1.5) #dataHistnorm.GetMaximum()*1.8) #Make room for annotations above the plot
             norderedStack.Draw("HIST")
             norderedStack.GetHistogram().GetYaxis().SetTitle("N Events")
             norderedStack.GetYaxis().SetTitleOffset(0.7)
             norderedStack.Draw("HIST")
	     projidata.SetLineColor(kBlack)
             projidata.SetLineWidth(lineSize)
             projidata.SetMarkerStyle(20) #Resizeable closed circle
             projidata.SetMarkerColor(kBlack)
             projidata.SetMarkerSize(0.7)
             projidata.SetTitle("Data")
             projidata.GetYaxis().SetTitle("entries")
             projidata.Draw("SAMES")
	     bottom.cd()
             bottom.SetTopMargin(0)
             bottom.SetBottomMargin(0.3) 
	     print("Now projecting mctotal for bin ", i)
	     mciproj = mci.ProjectionX("",i,i)#mci.GetNbinsY()+1)
	     print("Now projecting ratio") 
	     ratio = ratio0.ProjectionX("",i,i)#ratio.GetNbinsY()+1)
	     print("Done Projecting")
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
	     ratioplots.append(ratio) 
	     #mcRatio = mci.ProjectionX("",i,i)
	     mcRatio = mciproj.GetTotalError(False, True, False)
             for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
                mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
                mcRatio.SetBinContent(whichBin, 1)
	     mcRatioi = mcRatio #.ProjectionX("", i,i)
	     mcRatioi.SetLineColor(kRed)
             mcRatioi.SetLineWidth(lineSize)
             mcRatioi.SetFillColorAlpha(kPink + 1, 0.4)
             mcRatioi.Draw("E2 SAME")

	     straightLine = mcRatio.Clone()
             straightLine.SetFillStyle(0)
             straightLine.Draw("HIST SAME")
	     ratio.Draw("SAME")

	     overall.Print(var + "_bin"+str(i) +"_1pi_w_Neutron_only" +"_DataMCRatio.png")

nbin= signalHist.GetNbinsX()
ratiox = ar.array("d", [0]*8)
ratioy = ar.array("d", [0]*8)
xerrs = ar.array("d", [0]*8)
yerrs = ar.array("d", [0]*8)
#ratiox[0] = 0.0
#ratioy[0] = ratio.GetBinContent(1)
print(nbin)
count = 0
for ratio in ratioplots:
	xval = signalHist.GetYaxis().GetBinCenter(count+1)
        yval = ratio.GetBinContent(1)
	yerror = ratio.GetBinError(1)
	ratiox[count] = xval
	ratioy[count] = yval
	yerrs[count] = yerror
	count += 1

f = TFile.Open("EavMinusTpivspTmu_ScaleFactors_exp_neutronpionly_Firstxbinonly.root", "RECREATE")
g = ROOT.TGraph(len(ratiox),ratiox, ratioy)
g.SetName("ScaleFactorsVspTmu")
g.GetXaxis().SetTitle("pTmu (GeV/c)")
g.GetYaxis().SetTitle("Scale Factor for (Available E - T_{#pi^{+}}) < 75 MeV")
g.Draw("AC*")
g.Write()
ge = ROOT.TGraphErrors(len(ratiox),ratiox, ratioy, xerrs, yerrs)
ge.SetName("Ratio_with_Errs")
ge.GetXaxis().SetTitle("pTmu (GeV/c)")
ge.GetYaxis().SetTitle("Scale Factor for (Available E - T_{#pi^{+}}) < 75 MeV")
ge.Draw("ALP")
ge.Write()
overall = TCanvas("Fit")
#func = TF1('func', '[0] + [1]*x + [2]*x*x', 0., 1.6)
#func = TF1('func', ' 1/(x+[0])', 0.0, 1.6)
func = TF1('func', '[0]*exp(-sqrt(x)*[1])', 0.0, 1.6)
gefit = ge.Clone()
gefit.SetName("pTmuScaleFactors_Errs_Fit")
fit = gefit.Fit('func', 'ROB S')

gefit.Draw("ALP")
overall.Print("fitResult_exp_neutronpionly.root")
overall.Print("fitResult_exp_neutronpionly.png")
chiperndf = fit.Chi2()/fit.Ndf()

print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
print "Chi Squared Per Degree of Freedom: ", chiperndf
gefit.Write()



f.Close()
