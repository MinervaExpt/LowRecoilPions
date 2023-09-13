import ROOT
import os,sys
from ROOT import PlotUtils
from ROOT import gStyle
#infile= ROOT.TFile("/minerva/app/users/vansari/cmtuser/Minerva_v22r1p1_NSF/Ana/NSFNukeCCInclusive/ana/make_hists/T2IRON/Hists_Migration_ML6A_wosys_t2_z26_AntiNu_v1_.root")
#infile= ROOT.TFile("Hists_Migration.root")
infile=ROOT.TFile("/pnfs/minerva/persistent/users/mmehmood/RHC_nosys_original/Hists_Migration_nosys_t99_z99_AntiNu_minervameCombinedPlaylists.root")

#infile= ROOT.TFile("/minerva/data/users/vansari/AllRootFiles/Hists_Migration_t5_z26_AntiNu_v1_.root")
canvas1 = ROOT.TCanvas() # have to declare canvas before calling mnvplotter :))
mnv = PlotUtils.MnvPlotter()

gStyle. SetPalette(1)
gStyle.SetOptTitle(0)

myvariable = sys.argv[1]

mc_hist = infile.Get("response2d_%s_migration"%myvariable)
#print(mc_hist.GetNbinsX())
#print(mc_hist.GetNbinsY())
#mc_hist.Draw("COLZ TEXTnn")
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
# ROW NORMALIZED
#mnv.AddHistoTitle("Fe Target 2", 0.035, 1)  
canvas1.Modified()
mnv.SetROOT6Palette(109)

keys = canvas1.GetListOfPrimitives();
for k in keys:
    if(k.ClassName().find("TH2D")!=-1): # to change axis titles
        if myvariable == "pZmu_pTmu":
                k.GetXaxis().SetTitle("Reconstructed Muon P_{||} per Muon P_{t} bins")
                k.GetYaxis().SetTitle("True Muon P_{||} per Muon P_{t} bins")
        if myvariable == "Enufine":
                k.GetXaxis().SetTitle("Reconstructed Enu Bins")
                k.GetYaxis().SetTitle("True Enu Bins")
        if myvariable == "x":
                k.GetXaxis().SetTitle("Reconstructed x_Bj Bins")
                k.GetYaxis().SetTitle("True x_Bj Bins")
        if myvariable == "xfine":
                k.GetXaxis().SetTitle("Reconstructed x_Bj Bins")
                k.GetYaxis().SetTitle("True x_Bj Bins")

        k.GetXaxis().CenterTitle()
        k.GetXaxis().SetTitleFont(42)
        k.GetXaxis().SetTitleSize(0.05)

        k.GetYaxis().SetTitleOffset(1)
        k.GetYaxis().CenterTitle()
        k.GetYaxis().SetTitleFont(42)
        k.GetYaxis().SetTitleSize(0.05)

        k.GetZaxis().SetTitle("Row Normalized Event Rate (%)")
        k.GetZaxis().CenterTitle()
        k.GetZaxis().SetRangeUser(0,100)
        k.GetZaxis().SetTitleFont(42)
        k.GetZaxis().SetTitleOffset(1.1)
        k.GetZaxis().SetTitleSize(0.045)
    
canvas1.SetLeftMargin(0.12)
canvas1.SetBottomMargin(0.12)
canvas1.SetRightMargin(0.16)
canvas1.Print("/minerva/app/users/mmehmood/cmtuser/Minerva_v22r1p1_MADNew/Ana/NSFNukeCCInclusive/ana/make_hists/MigrationMatrix_%s.png"%myvariable)

raw_input("Done")
