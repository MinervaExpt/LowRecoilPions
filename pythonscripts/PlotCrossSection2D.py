#!/usr/bin/python
from ROOT import *
from ROOT import PlotUtils
import sys
gROOT.SetBatch()

var = sys.argv[1]

ratioMin = 0.5
ratioMax = 3.0

bottomFraction = 0.2
margin = 0.078 #Tuned by hand
labelSize = 0.15
lineSize = 3

TH1.AddDirectory(False)

xfile = TFile.Open(var+"_crossSection.root")

mcFile =  TFile.Open(sys.argv[3])
dataFile = TFile.Open(sys.argv[4])

mcPOT = mcFile.Get("POTUsed").GetVal()
dataPOT = dataFile.Get("POTUsed").GetVal()

overall = TCanvas("Data/MC for " + var)
top = TPad("Overlay", "Overlay", 0, bottomFraction+margin, 1.0, 1.00)
bottom = TPad("Ratio", "Ratio", 0, 0.0, 1.0, bottomFraction + margin)

#top.SetLogx()
#bottom.SetLogx()
overall.SetCanvasSize(1600, 1300)
overall.SetWindowSize(600,500)
top.Draw()
bottom.Draw()
xsec = xfile.Get("crossSection")
sim = xfile.Get("simulatedCrossSection")

npipTwoD = xfile.Get("NpiplusOnlyxsec")
onepiTwoD = xfile.Get("OnepiNoPNxsec")
onepiNTwoD = xfile.Get("OnepiplusNeutronxsec")
onepiPTwoD = xfile.Get("OnepiplusProtonxsec")
npiresTwoD = xfile.Get("NpiplusRESxsec")
npidisTwoD = xfile.Get("NpiplusDISxsec")
cohTwoD = xfile.Get("COHxsec")

i = int(sys.argv[2])
xsecHist = xsec.ProjectionX("e",i,i) #xfile.Get("crossSection")
simHist = sim.ProjectionX("e",i,i)#xfile.Get("simulatedCrossSection")

npip = npipTwoD.ProjectionX("e",i,i)
npip.SetLineColor(kPink+5)
npip.SetFillColorAlpha(kPink+5, 0.35)
onepi = onepiTwoD.ProjectionX("e",i,i)
onepi.SetLineColor(kGreen - 8)
onepi.SetFillColorAlpha(kGreen - 8, 0.35)
onepiN = onepiNTwoD.ProjectionX("e",i,i)
onepiN.SetLineColor(kGreen -5)
onepiN.SetFillColorAlpha(kGreen -5, 0.35)
onepiP = onepiPTwoD.ProjectionX("e",i,i)
onepiP.SetLineColor(kGreen + 3)
onepiP.SetFillColorAlpha(kGreen + 3, 0.35)
npires = npiresTwoD.ProjectionX("e",i,i)
npires.SetLineColor(kBlue-10)
npires.SetFillColorAlpha(kBlue-10, 0.35)
npidis = npidisTwoD.ProjectionX("e",i,i)
npidis.SetLineColor(kBlue-6)
npidis.SetFillColorAlpha(kBlue-6, 0.35)
coh = cohTwoD.ProjectionX("e",i,i)
coh.SetLineColor(TColor.GetColor("#f6c700"))
coh.SetFillColorAlpha(TColor.GetColor("#f6c700"), 0.35)
#coh.SetFillColorAlpha(1317, 0.35)

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
xsecHist.GetXaxis().SetTitle(sys.argv[5])
xsecHist.SetMinimum(0)
xsecHist.SetMaximum(simHist.GetMaximum()*1.2)
simHist.GetXaxis().SetTitle(sys.argv[5])
xsecHist.GetXaxis().SetTitle(sys.argv[5])
xsecHist.Draw();
simHist.Draw("SAMES,hx0")




#legend = top.BuildLegend(0.1,0.7,0.48,0.9)#0.20, 0.30, .95, 0.99)
#legend.SetLegendTextSize(0.)

errorband = simHist.Clone()
errorband.SetFillColor(kRed)
errorband.SetFillColorAlpha(kPink + 1, 0.4)
errorband.SetLineWidth(lineSize)
errorband.Draw("E2 SAME")

xsecHist.Draw("SAMES");
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
#overall.Print(var + "_CrossSection_DATAMC.png")
bottom.cd()
#plotter.AddPOTNormBox(dataPOT, mcPOT, .19, .87)
plotter.DrawDataMCRatio(xsecHist, simHist,1.0, True, True, 0.0, 3.0)
bottom.ResizePad()
overall.Print("out_"+var + "_CrossSection_bin"+str(sys.argv[2])+"_DataMCRatio.png")


#Background subtracted and signal reco comparison:
#overall.Clear()
overall.Clear()
pad = TPad("Overlay", "Overlay", 0.05, .05, 1.0, 1.00)
overall.SetCanvasSize(600, 400)
#pad.SetLogx()
pad.Draw()
pad.cd()
simHist.SetMaximum(xsecHist.GetMaximum()*1.3)
plotter.axis_maximum_group=xsecHist.GetMaximum()*1.1
plotter.DrawDataMCWithErrorBand(xsecHist, simHist, 1.0, "TR", False)
xsecHist.Draw()
npip.SetTitle("N > 1 #pi^{+} Only")
#npip.SetLineWidth(3)
#npip.Draw("HIST C,SAMES")
onepi.SetTitle("1 #pi^{+} w/o p or n")
#onepi.SetLineWidth(3)
#onepi.Draw("HIST C,SAMES")
onepiN.SetTitle("1 #pi^{+} w Neutron")
#onepiN.SetLineWidth(3)
#onepiN.Draw("HIST C,SAMES")
onepiP.SetTitle("1 #pi^{+} w Proton")
#onepiP.SetLineWidth(3)
#onepiP.Draw("HIST C,SAMES")
npires.SetTitle("1+ #pi^{+} and Mesons (RES)")
#npires.SetLineWidth(3)
#npires.Draw("HIST C,SAMES")
npidis.SetTitle("1+ #pi^{+} and Mesons (DIS)")
#npidis.SetLineWidth(3)
#npidis.Draw("HIST C,SAMES")
coh.SetTitle("Coherent Pions")
#coh.SetLineWidth(3)
#coh.Draw("HIST C,SAMES")

overall.SetFillStyle(4000)
pad.SetFillStyle(4000)

norderedStack = THStack()
norderedStack.Add(coh)
norderedStack.Add(npidis)
norderedStack.Add(npires)
norderedStack.Add(npip)
norderedStack.Add(onepi)
norderedStack.Add(onepiN)
norderedStack.Add(onepiP)
plotter.DrawDataMCWithErrorBand(xsecHist, simHist, 1.0, "TR", False)
norderedStack.Draw("HIST")
norderedStack.GetHistogram().SetMaximum(simHist.GetMaximum() * 1.6)
norderedStack.GetHistogram().SetFillStyle(4000)
#xsecHist.Draw("HIST SAMES")
plotter.DrawDataMCWithErrorBand(xsecHist, simHist, 1.0, "TR", False)
norderedStack.Draw("HIST SAMES")

#plotter.DrawDataStackedMCWithErrorBand(xsecHist, norderedStack, 1.0, "TR", "Data", 2, 1, 0)
legend = TLegend(0.15, 0.71, 0.93, 0.9)
legend.SetTextFont(62)
legend.SetTextSizePixels(5)
legend.SetFillStyle(0)
legend.AddEntry(onepi)
legend.AddEntry(onepiN)
legend.AddEntry(onepiP)
legend.AddEntry(npip)
legend.AddEntry(npires)
legend.AddEntry(npidis)
legend.AddEntry(coh)
legend.SetBorderSize(0)
legend.Draw("SAMES")
#overall.SetLogx()
#pad.SetLogx()
pad.Update()
xsecHist.Draw("SAMES")
pad.Draw()

overall.Print("out_"+var +"_bin" + str(i) + "_MCProcess_crossection.png")

plotter.DrawDataMCWithErrorBand(xsecHist, simHist, 1.0, "TR", False)
overall.Print("out_"+var +"_bin" + str(i) + "_just_crossection.png")

#top.cd()
bkg = xfile.Get("backgroundSubtracted")
bkg.SetName("backgroundSubtracted")
sig =  mcFile.Get(var+"_signal_reco")
#sig.Scale(dataPOT/mcPOT)
bkgtot = xfile.Get("background")
bkgsub = bkg.ProjectionX("e",i,i) #xfile.Get("backgroundSubtracted")
sigreco = sig.ProjectionX("e",i,i)#xfile.Get("Signalreco")
bkgtoti = bkgtot.ProjectionX("e", i,i)
bkgtoti.SetName("Background")
bkgsub.GetXaxis().SetTitle(sys.argv[5])
bkgsub.GetYaxis().SetTitle("N Events")
bkgsub.GetXaxis().SetTitleOffset(1.2);
bkgsub.GetYaxis().SetTitleSize(.035);
bkgsub.GetXaxis().SetTitleSize(0.03)
bkgsub.GetYaxis().SetTitleOffset(0.9);
sigreco.GetXaxis().SetTitle(sys.argv[5])
sigreco.GetYaxis().SetTitle("N Events")
sigreco.GetXaxis().SetTitleSize(0.03)
sigreco.GetXaxis().SetTitleOffset(1.2);
sigreco.GetXaxis().SetTitleSize(.035);
sigreco.GetYaxis().SetTitleOffset(0.9);
sigreco.SetMaximum(bkgsub.GetMaximum()*1.2)
bkgsub.SetMaximum(bkgsub.GetMaximum()*1.2)
sigreco.GetXaxis().SetTitle(sys.argv[5])
#top.ResizePad()
plotter.DrawDataMCWithErrorBand(bkgsub.GetBinNormalizedCopy(), sigreco.GetBinNormalizedCopy(), 1.0, "TR", False)#,bkgtoti.GetBinNormalizedCopy())
#plotter.AddChi2Label(bkgsub, sigreco)
overall.Print("out_"+var +"_bin" + str(i) + "_bkgSubtractedReco.png")
#bottom.cd()

bkgsub.GetXaxis().SetTitle(sys.argv[5])
sigreco.GetXaxis().SetTitle(sys.argv[5])
plotter.DrawDataMCRatio(bkgsub, sigreco,1.0, True, True, 0.0, 3.0)
#bottom.ResizePad()
overall.Print("out_"+var +"_bin" + str(i) + "_bkgSubtractedReco_Ratio.png")

unf =  xfile.Get("unfolded")
strue = mcFile.Get(var+"_efficiency_numerator")
#strue.Scale(dataPOT/mcPOT)
unfold = unf.ProjectionX("e",i,i) #xfile.Get("unfolded")
sigtrue = strue.ProjectionX("e",i,i)#xfile.Get(var+"_efficiency_numerator")
unfold.GetXaxis().SetTitle(sys.argv[5])
unfold.GetYaxis().SetTitle("N Events")
sigtrue.GetXaxis().SetTitle(sys.argv[5])
sigtrue.GetYaxis().SetTitle("N Events")
unfold.GetXaxis().SetTitleSize(0.03)
sigtrue.GetXaxis().SetTitleSize(0.03)
#plotter.AddChi2Label(unfold,sigtrue)
unfold.GetXaxis().SetTitleOffset(1.2);
unfold.GetXaxis().SetTitleSize(.035);
unfold.GetYaxis().SetTitleOffset(0.9);
sigtrue.GetYaxis().SetTitleOffset(0.9);
unfold.SetMaximum(bkgsub.GetMaximum()*1.2)
sigtrue.SetMaximum(bkgsub.GetMaximum()*1.2)
unfold.GetXaxis().SetTitle(sys.argv[5])
plotter.DrawDataMCWithErrorBand(unfold.GetBinNormalizedCopy(), sigtrue.GetBinNormalizedCopy(),  1.0, "TR", False)
#plotter.AddChi2Label(unfold,sigtrue)
overall.Print("out_"+var +"_bin" + str(i) + "_unfoldedtrue.png")
#bottom.cd()
unfold.GetXaxis().SetTitle(sys.argv[5])
sigtrue.GetXaxis().SetTitle(sys.argv[5])

plotter.DrawDataMCRatio(unfold, sigtrue,True, True, 0.0, 3.0)
overall.Print("out_"+var+"_bin" + str(i) + "_unfoldedtrue_Ratio.png")

effcorr = xfile.Get("EfficiencyCorrected")
simdenom = strue = mcFile.Get(var+"_efficiency_denominator")
effcorri = effcorr.ProjectionX("e",i,i)
simdenomi = simdenom.ProjectionX("e",i,i)

effcorri.GetXaxis().SetTitle(sys.argv[5])
effcorri.GetYaxis().SetTitle("N Events")
simdenomi.GetXaxis().SetTitle(sys.argv[5])
simdenomi.GetYaxis().SetTitle("N Events")
effcorri.GetXaxis().SetTitleSize(0.03)
simdenomi.GetXaxis().SetTitleSize(0.03)
effcorri.GetXaxis().SetTitleOffset(1.2);
effcorri.GetXaxis().SetTitleSize(.035);
effcorri.GetYaxis().SetTitleOffset(0.9);
effcorri.GetXaxis().SetTitleOffset(1.2);
effcorri.GetXaxis().SetTitleSize(.035);
effcorri.GetYaxis().SetTitleOffset(0.9);
simdenomi.GetYaxis().SetTitleOffset(0.9);
effcorri.SetMaximum(bkgsub.GetMaximum()*1.2)
simdenomi.SetMaximum(bkgsub.GetMaximum()*1.2)
plotter.DrawDataMCWithErrorBand(effcorri.GetBinNormalizedCopy(), simdenomi.GetBinNormalizedCopy(),  1.0, "TR", False)
overall.Print("out_"+var +"_bin" + str(i) + "_effcorrec_effdenom.png")


effcan = TCanvas("Efficiency for " + var)
#effcan.SetLogx()
effi = xfile.Get("Efficiency")
eff = effi.ProjectionX("e",i,i)
eff.GetYaxis().SetRangeUser(0.0, 1.0)
eff.GetXaxis().SetTitle(sys.argv[5])
eff.GetYaxis().SetTitle("Efficiency")
plotter.DrawMCWithErrorBand(eff)
effcan.Print("out_"+var +"_bin" + str(i) + "_efficiencydist.png")


effcan = TCanvas("Purity for " + var)
pur =  xfile.Get("Purity")
eff = pur.ProjectionX("e",i,i)
eff.GetYaxis().SetRangeUser(0.0, 1.0)
eff.GetXaxis().SetTitle(sys.argv[5])
eff.GetYaxis().SetTitle("Purity")
plotter.DrawMCWithErrorBand(eff)
effcan.Print("out_"+var+"_bin" + str(i) + "_Puritydist.png")
