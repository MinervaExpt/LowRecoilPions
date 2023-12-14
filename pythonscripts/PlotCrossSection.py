#!/usr/bin/python
from ROOT import *
from ROOT import PlotUtils
import sys
gROOT.SetBatch()

var = sys.argv[1]

ratioMin = 0.5
ratioMax = 1.5

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)

xfile = TFile.Open(var+"_crossSection.root")

mcFile =  TFile.Open(sys.argv[2])
dataFile = TFile.Open(sys.argv[3])

#potFile = TFile.Open("../totalPOT.root")

mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

overall = TCanvas("Data/MC for " + var)
top = TPad("Overlay", "Overlay", 0, bottomFraction+margin, 1.0, 1.00)
bottom = TPad("Ratio", "Ratio", 0, 0.0, 1.0, bottomFraction + margin)

top.SetLogx()
bottom.SetLogx()
overall.SetCanvasSize(1600, 1300)
overall.SetWindowSize(600,500)
top.Draw()
bottom.Draw()
xsecHist = xfile.Get("crossSection")
simHist = xfile.Get("simulatedCrossSection")

top.cd()

simHist.SetLineColor(kRed)
xsecHist.SetName("DATA")
xsecHist.SetTitle("DATA")
simHist.SetName("MC")
simHist.SetTitle("MC")
xsecHist.GetYaxis().SetTitle("CrossSection/nucleon");
xsecHist.GetXaxis().SetTitleOffset(1.2);
xsecHist.GetYaxis().SetTitleSize(.035);
xsecHist.SetLineColor(kBlack)
xsecHist.GetXaxis().SetTitle(sys.argv[4])
xsecHist.SetMinimum(0)
xsecHist.SetMaximum(simHist.GetMaximum()*1.2)
simHist.GetXaxis().SetTitle(sys.argv[4])
xsecHist.Draw();
simHist.Draw("SAMES,hx0")

#legend = top.BuildLegend(0.1,0.7,0.48,0.9)#0.20, 0.30, .95, 0.99)
#legend.SetLegendTextSize(0.)

errorband = simHist.Clone()
errorband.SetFillColor(kRed)
errorband.SetFillColorAlpha(kPink + 1, 0.4)
errorband.SetLineWidth(lineSize)
errorband.Draw("E2 SAME")
#bottom.cd()


ratio = xsecHist.Clone()
mcRatio = simHist.GetTotalError(False, True, False)
for whichBin in range(1, mcRatio.GetXaxis().GetNbins()+1):
  mcRatio.SetBinError(whichBin, max(mcRatio.GetBinContent(whichBin), 1e-9))
  mcRatio.SetBinContent(whichBin, 1)

ratio = xsecHist.Clone()
ratio.Divide(ratio, simHist)

ratio.SetLineColor(kBlack)
ratio.SetLineWidth(lineSize)
ratio.SetTitleSize(0)

ratio.GetYaxis().SetTitle("Data / MC")
ratio.GetYaxis().SetLabelSize(labelSize)
ratio.GetYaxis().SetTitleSize(0.16)
ratio.GetYaxis().SetTitleOffset(0.2)
ratio.GetYaxis().SetNdivisions(505) #5 minor divisions between 5 major divisions.  I'm trying to match a specific paper here.
ratio.GetXaxis().SetTitle(sys.argv[4])
ratio.GetXaxis().SetTitleSize(0.12)
ratio.GetXaxis().SetTitleOffset(0.9)
ratio.GetXaxis().SetLabelSize(labelSize)

ratio.SetMinimum(ratioMin)
ratio.SetMaximum(ratioMax)

#ratio.Draw()

mcRatio.SetLineColor(kRed)
mcRatio.SetLineWidth(lineSize)
mcRatio.SetFillColorAlpha(kPink + 1, 0.4)
#mcRatio.Draw("E2 SAME")

straightLine = mcRatio.Clone()
straightLine.SetFillStyle(0)
#straightLine.Draw("HIST SAME")

#top.cd()

dPOT = TPaveText(0.05, 0.20, 0.9, 0.20, "nbNDC")
dPOT.SetFillStyle(0)
dPOT.IsTransparent() 
dPOT.SetLineColor(kWhite)
dPOT.SetTextSize(0.04)
dPOT.AddText("Data")
#dPOT.AddText("Data POT: "+str('{0:.2e}'.format(dataPOT)))
#dPOT.Draw("SAMES")
#dPOT.AddText("\n")
mPOT = TPaveText(0.05, 0.20, 0.96, 0.20, "nbNDC")
mPOT.SetFillStyle(0)
mPOT.IsTransparent()
mPOT.SetLineColor(kWhite)
mPOT.SetTextSize(0.04)
#mPOT.AddText("MC POT: "+ str('{0:.2e}'.format(mcPOT)))
mPOT.AddText("MC")
(mPOT.GetListOfLines().Last()).SetTextColor(kRed);
#mPOT.Draw("SAMES")

plotter = PlotUtils.MnvPlotter()
#plotter.AddChi2Label(xsecHist,simHist)
#plotter.AddPOTNormBox(dataPOT, mcPOT, .19, .87)
top.ResizePad()
#plotter.DrawDataMCWithErrorBand(
#overall.Print(var + "_CrossSection_DATAMC.png")
bottom.cd()
#plotter.AddPOTNormBox(dataPOT, mcPOT, .19, .87)
plotter.DrawDataMCRatio(xsecHist, simHist,1.0, True, True, 0.5, 2.0)
bottom.ResizePad()
overall.Print(var + "_CrossSection_logx15_DataMCRatio.png")

#Background subtracted and signal reco comparison:
#overall.Clear()
overall.Clear()
pad = TPad("Overlay", "Overlay", 0.05, .05, 1.0, 1.00)
overall.SetCanvasSize(600, 400)
#pad.SetLogx()
pad.Draw()
pad.cd()
pad.SetLogx()
plotter.DrawDataMCWithErrorBand(xsecHist, simHist, 1.0, "TR")
overall.Print(var + "_crossSection_plotter.png")

#top.cd()
bkgsub = xfile.Get("backgroundSubtracted")
sigreco = xfile.Get("Signalreco")
bkgsub.GetXaxis().SetTitle(sys.argv[4])
bkgsub.GetYaxis().SetTitle("N Events")
bkgsub.GetXaxis().SetTitleOffset(1.2);
bkgsub.GetYaxis().SetTitleSize(.035);
bkgsub.GetXaxis().SetTitleSize(0.03)
bkgsub.GetYaxis().SetTitleOffset(0.9);
sigreco.GetXaxis().SetTitle(sys.argv[4])
sigreco.GetYaxis().SetTitle("N Events")
sigreco.GetXaxis().SetTitleSize(0.03)
sigreco.GetXaxis().SetTitleOffset(1.2);
sigreco.GetXaxis().SetTitleSize(.035);
sigreco.GetYaxis().SetTitleOffset(0.9);
sigreco.SetMaximum(600)#bkgsub.GetMaximum()*1.2)
bkgsub.SetMaximum(600)#(bkgsub.GetMaximum()*1.2)

#top.ResizePad()
plotter.axis_maximum = 600
plotter.DrawDataMCWithErrorBand(bkgsub.GetBinNormalizedCopy(), sigreco.GetBinNormalizedCopy(), 1.0, "TR")
#plotter.AddChi2Label(bkgsub, sigreco)

overall.Print(var + "_bkgSubtractedReco.png")
#bottom.cd()

bkgsub.GetXaxis().SetTitle(sys.argv[4])
sigreco.GetXaxis().SetTitle(sys.argv[4])
plotter.DrawDataMCRatio(bkgsub, sigreco,1.0, True, True, 0.5, 1.5)
#bottom.ResizePad()
overall.Print(var + "_bkgSubtractedReco_Ratio.png")


unfold = xfile.Get("unfolded")
sigtrue = xfile.Get("trueSignal") #var+"_efficiency_numerator")
unfold.GetXaxis().SetTitle(sys.argv[4])
unfold.GetYaxis().SetTitle("N Events")
sigtrue.GetXaxis().SetTitle(sys.argv[4])
sigtrue.GetYaxis().SetTitle("N Events")
unfold.GetXaxis().SetTitleSize(0.03)
sigtrue.GetXaxis().SetTitleSize(0.03)
plotter.AddChi2Label(unfold,sigtrue)
unfold.GetXaxis().SetTitleOffset(1.2);
unfold.GetXaxis().SetTitleSize(.035);
unfold.GetYaxis().SetTitleOffset(0.9);
sigtrue.GetYaxis().SetTitleOffset(0.9);
unfold.SetMaximum(600)#unfold.GetMaximum()*1.2)
sigtrue.SetMaximum(600)#unfold.GetMaximum()*1.2)
plotter.DrawDataMCWithErrorBand(unfold.GetBinNormalizedCopy(), sigtrue.GetBinNormalizedCopy(), 1.0, "TR")
plotter.AddChi2Label(unfold,sigtrue)
overall.Print(var + "_unfoldedtrue.png")
#bottom.cd()

unfold.GetXaxis().SetTitle(sys.argv[4])
sigtrue.GetXaxis().SetTitle(sys.argv[4])

plotter.DrawDataMCRatio(unfold, sigtrue,True, True, 0.5, 2.0)
overall.Print(var + "_unfoldedtrue_Ratio.png")


#effcan = TCanvas("Efficiency for " + var)
simEv = xfile.Get("SimEventRate")
effCorr = xfile.Get("efficiencyCorrected")
effCorr.SetMaximum(600)
effCorr.GetXaxis().SetTitle(sys.argv[4])
plotter.DrawDataMCWithErrorBand(effCorr.GetBinNormalizedCopy(), simEv.GetBinNormalizedCopy(), 1.0, "TR")
overall.Print(var + "_efficiecyCorrected_plotter.png")

effcan = TCanvas("Efficiency for " + var)
eff = xfile.Get("Efficiency")
eff.GetYaxis().SetRangeUser(0.0, 1.0)
eff.GetXaxis().SetTitle(sys.argv[4])
eff.GetYaxis().SetTitle("Efficiency")
plotter.DrawMCWithErrorBand(eff)
effcan.Print(var + "_efficiencydist.png")

effcan = TCanvas("Purity for " + var)
eff = xfile.Get("Purity")
eff.GetYaxis().SetRangeUser(0.0, 1.0)
eff.GetXaxis().SetTitle(sys.argv[4])
eff.GetYaxis().SetTitle("Purity")
plotter.DrawMCWithErrorBand(eff)
effcan.Print(var + "_Puritydist.png")
