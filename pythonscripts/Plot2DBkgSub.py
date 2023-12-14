import ROOT
import os,sys
from ROOT import PlotUtils
from ROOT import gStyle
from ROOT import gROOT
gROOT.SetBatch()

mcfile = ROOT.TFile(sys.argv[1])
datafile = ROOT.TFile(sys.argv[2])

canvas1 = ROOT.TCanvas()
mnv = PlotUtils.MnvPlotter()

gStyle. SetPalette(1)
gStyle.SetOptTitle(0)

name = sys.argv[3]

shouldscale = int(sys.argv[5])

mc_hist = mcfile.Get(name+"_MC")
data_hist = datafile.Get(name+"_data")
bkg1 = mcfile.Get(name+"_by_BKG_Label_Bkg_Wrong_Sign")
bkg2 = mcfile.Get(name+"_by_BKG_Label_NC_Bkg")
bkg3 = mcfile.Get(name+"_by_BKG_Label_Other")

bkgtot = bkg1.Clone()
bkgtot.Add(bkg2)
bkgtot.Add(bkg3)

mcPOT = mcfile.Get("POTUsed").GetVal()
dataPOT = datafile.Get("POTUsed").GetVal()
mc_hist.Add(bkgtot,-1)

scale = dataPOT/mcPOT

if (shouldscale > 1):
	bkgtot.Scale(scale)
	mc_hist.Scale(scale)
else:
        print("Not Scaling this because MC already scaled to dataPOT")        

databkgsub = data_hist.Clone()
databkgsub.Add(bkgtot, -1)

mnv.ApplyStyle(6)


ratio = databkgsub.Clone()
ratio.Divide(databkgsub, mc_hist)
k = ratio.Clone()
k.GetXaxis().SetTitle("Reconstructed Available Energy (MeV)")
k.GetYaxis().SetTitle("True p_{T#mu} (GeV/c)")
k.GetXaxis().SetTitleSize(0.05)
k.GetYaxis().SetTitleSize(0.05)
k.GetYaxis().SetTitleOffset(1.2)
k.GetZaxis().SetRangeUser(0.0, 1.0)
k.GetZaxis().SetTitle("Efficiency")
mnv.SetROOT6Palette(109)
#k.Draw("colz")
#k.Draw("text sames")

#mnv.DrawNormalizedMigrationHistogram(mc_hist, True, False, True, True)
#'''
#void MnvPlotter::DrawNormalizedMigrationHistogram(
#        const TH2D* h_migration,
#        const bool drawAsMatrix,
#        const bool coarseContours, /* = false */
#        const bool includeFlows, /* = true */
#        const bool noText /* = false */
#        )
#{
#'''
#canvas1.Modified()
#mnv.SetROOT6Palette(109)

#keys = canvas1.GetListOfPrimitives();
#for k in keys:
#    if(k.ClassName().find("TH2D")!=-1): # to change axis titles
#          k.GetXaxis().SetTitle("Reconstructed Available Energy (MeV)")
#	  k.GetYaxis().SetTitle("True p_{T#mu} (GeV/c)")
#	  k.GetXaxis().SetTitleSize(0.05)
#	  k.GetYaxis().SetTitleSize(0.05)
#	  k.GetYaxis().SetTitleOffset(1.2)
#	  k.GetZaxis().SetRangeUser(0.0, 1.0)
#          k.GetZaxis().SetTitle("Efficiency")
canvas1.SetLeftMargin(0.10)
canvas1.SetBottomMargin(0.12)
canvas1.SetRightMargin(0.16)
#canvas1.Print("Efficiency_test.png")

bini = int(sys.argv[4])
ki = k.ProjectionX("eff",bini, bini)
ki.GetXaxis().SetTitle("Reconstructed Available Energy (MeV)")
ki.GetYaxis().SetTitle("Efficiency")
ki.GetYaxis().SetRangeUser(0.0, 0.5)
ki.SetLineColor(ROOT.kBlack)
#ki.Draw("HIST")
databini = databkgsub.ProjectionX("data", bini, bini)
mcbini = mc_hist.ProjectionX("mc", bini, bini)
mnv.DrawDataMCRatio(databini, mcbini,1.0, True, True, 0.0, 2.5)
canvas1.Print("BkgSubtracted_DataMCRatio_"+ name +"_bin"+str(bini)+".png")

databini = databkgsub.ProjectionX("data", bini, bini)
mcbini = mc_hist.ProjectionX("mc", bini, bini)
bkgtoti = bkgtot.ProjectionX("bkg", bini, bini)
mnv.DrawDataMCWithErrorBand(databini.GetBinNormalizedCopy(), mcbini.GetBinNormalizedCopy(), 1.0, "TR", False,bkgtoti)
mnv.AddChi2Label(databini, mcbini)
canvas1.Print("BkgSubtracted_DataSignalReco_"+ name +"_bin"+str(bini)+".png")

