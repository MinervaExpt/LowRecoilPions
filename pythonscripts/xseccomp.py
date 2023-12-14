#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
from ROOT import TStyle
import sys

gROOT.SetBatch()
gStyle.SetPalette(kBlueGreenYellow)
gStyle.SetOptStat(0)
TH1.AddDirectory(False)

file1 = "xsec_itr1/AvailableE_crossSection.root"
file3 = "xsec_itr4/AvailableE_crossSection.root"
file4 = "xsec_itr7/AvailableE_crossSection.root"
file5 = "xsec_itr10/AvailableE_crossSection.root"
file6 = "xsec_itr20/AvailableE_crossSection.root"
file2 = "xsec_itr3/AvailableE_crossSection.root"
files = [file1, file2, file3, file4, file5, file6]

overall = TCanvas("AvailableE CrossSection")

itrs = ["1 itr", "3 itr", "4 itr", "7 itr", "10 itr", "20 itr"]

f1 = TFile.Open(file1)
mcHist = f1.Get("simulatedCrossSection")
mcHist.SetLineColor(kRed)
i = 0
for path in files:
    f = TFile.Open(path)
    print f
    dataHist = f.Get("crossSection")
    dataHist.SetName(itrs[i])
    dataHist.SetTitle(itrs[i])
    dataHist.SetTitleSize(0)
    dataHist.SetMarkerStyle(8)
    dataHist.SetMarkerSize(.5)
    dataHist.GetXaxis().SetTitle("Available Energy (MeV)");
    dataHist.GetYaxis().SetTitle("#frac{d#sigma}{dE_{avail}} (cm^{-2} / nucleons / MeV)")
    dataHist.GetYaxis().SetTitleSize(0.34)
    dataHist.GetXaxis().SetTitleOffset(1.34)
    dataHist.SetMinimum(0.)
    dataHist.Draw("hist PLC PMC P x0 SAMES")
    i = i+1

mcHist.SetTitle("MC")     
mcHist.Draw("SAMES hist x0")

#legend = overall.BuildLegend(0.75, 0.40, 0.9, 0.9)

overall.Print("AvailableE_crossection_itrcomp.png")









