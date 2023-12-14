#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
import csv
gROOT.SetBatch()

TH1.AddDirectory(False)
mc = TFile.Open(sys.argv[1])
rangehist = mc.Get("permichel_pirange_MC")
tpihist = mc.Get("permichel_tpi_MC")
rangehist = tpihist
can = TCanvas(rangehist.GetName())
plotter = PlotUtils.MnvPlotter()
plotter.ApplyStyle(PlotUtils.kCCQENuStyle);
plotter.axis_maximum = 0.75;

plotter.DrawErrorSummary(rangehist);
varnam = rangehist.GetName()
can.Print(varnam+"_uncertaintySummary.png")

plotter.DrawErrorSummary(rangehist, "TR", True, True, 1e-5, False, "Other");
can.Print(varnam+"_otherUncertainties.png")

plotter.DrawErrorSummary(rangehist, "TR", True, True, 0.00001, False, "Cross Section Models", True, "");
can.Print(varnam+"_CrossSectionModels_uncertainty.png")

plotter.DrawErrorSummary(rangehist, "TR", True, True, 0.00001, False, "Low Recoil Fits", True, "");
can.Print(varnam+"_LowRecoilFits_uncertainty.png")

plotter.DrawErrorSummary(rangehist,"TR", True, True, 0.00001, False, "Low_Recoil_2p2h_Tune", True, "")
can.Print(varnam+"_2p2hTune_uncertainty.png")


print rangehist.GetVertErrorBand("Low_Recoil_2p2h_Tune").GetNHists()

rangehist.GetVertErrorBand("Low_Recoil_2p2h_Tune").GetHist(0).Draw("HIST");
rangehist.GetVertErrorBand("Low_Recoil_2p2h_Tune").GetHist(1).Draw("HIST SAMES");
rangehist.GetVertErrorBand("Low_Recoil_2p2h_Tune").GetHist(2).Draw("HIST SAMES");
can.Print(varnam+"_2p2h_Universes.png")
