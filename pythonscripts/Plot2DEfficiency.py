import ROOT
import os,sys
from ROOT import PlotUtils
from ROOT import gStyle
from ROOT import gROOT
gROOT.SetBatch()

infile = ROOT.TFile(sys.argv[1])

canvas1 = ROOT.TCanvas()
mnv = PlotUtils.MnvPlotter()

gStyle. SetPalette(1)
gStyle.SetOptTitle(0)

name = sys.argv[2]

num_hist = infile.Get(name+"_efficiency_numerator")
denom_hist = infile.Get(name+"_efficiency_denominator")
mnv.ApplyStyle(6)
ratio = num_hist.Clone()
ratio.Divide(num_hist, denom_hist)
k = ratio.Clone()
k.GetXaxis().SetTitle("Reconstructed Available Energy (MeV)")
k.GetYaxis().SetTitle("True p_{T#mu} (GeV/c)")
k.GetXaxis().SetTitleSize(0.05)
k.GetYaxis().SetTitleSize(0.05)
k.GetYaxis().SetTitleOffset(1.2)
k.GetZaxis().SetRangeUser(0.0, 1.0)
k.GetZaxis().SetTitle("Efficiency")
mnv.SetROOT6Palette(109)
k.Draw("colz")
k.Draw("text sames")
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

bini = int(sys.argv[3])
ki = k.ProjectionX("eff",bini, bini)
ki.GetXaxis().SetTitle("Reconstructed Available Energy (MeV)")
ki.GetYaxis().SetTitle("Efficiency")
ki.GetYaxis().SetRangeUser(0.0, 0.5)
ki.SetLineColor(ROOT.kBlack)
ki.Draw("HIST")
canvas1.Print("Efficiency_"+ name +"_bin"+str(bini)+".png")



