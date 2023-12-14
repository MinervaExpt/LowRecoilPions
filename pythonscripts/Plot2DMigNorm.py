import ROOT
import os,sys
from ROOT import PlotUtils
from ROOT import gStyle

infile = ROOT.TFile(sys.argv[1])

canvas1 = ROOT.TCanvas()
mnv = PlotUtils.MnvPlotter()

gStyle. SetPalette(1)
gStyle.SetOptTitle(0)

name = sys.argv[2]

mc_hist = infile.Get(name+"_migration")
mnv.ApplyStyle(6)
mnv.DrawNormalizedMigrationHistogram(mc_hist, True, False, True, True)
'''
void MnvPlotter::DrawNormalizedMigrationHistogram(
        const TH2D* h_migration,
        const bool drawAsMatrix,
        const bool coarseContours, /* = false */
        const bool includeFlows, /* = true */
        const bool noText /* = false */
        )
{
'''
canvas1.Modified()
mnv.SetROOT6Palette(109)

keys = canvas1.GetListOfPrimitives();
for k in keys:
    if(k.ClassName().find("TH2D")!=-1): # to change axis titles
          k.GetXaxis().SetTitle("Reconstructed Available Energy per p_{T#mu} bin")
	  k.GetYaxis().SetTitle("True Available Energy per p_{T#mu} bin")
	  k.GetXaxis().SetTitleSize(0.05)
	  k.GetYaxis().SetTitleSize(0.05)
	  k.GetYaxis().SetTitleOffset(1.2)
canvas1.SetLeftMargin(0.12)
canvas1.SetBottomMargin(0.12)
canvas1.SetRightMargin(0.16)
canvas1.Print("migrationtest.png")
