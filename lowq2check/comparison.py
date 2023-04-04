#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
import ctypes

#ROOT.gInterpreter.Declare("void foo(double& d)")

#gROOT.SetBatch()



TH1.AddDirectory(False)

file1 = TFile.Open(sys.argv[1], "READONLY")
file2 = TFile.Open(sys.argv[2], "READONLY")

listoftunes = ["JOINT", "NU1PI", "NUNPI", "NUPI0", "NUBARPI0", "MENU1PI"]
gt = TFile.Open("WeightsShiftsComparison_cvfmsupdate_March282023.root", "RECREATE")

for name in listoftunes:

    tune1 = file1.Get(name+"_weights")#weight_shifts")
    print(tune1)
    #tune1.Draw("Ac*")
    #hist1 = tune1.GetHistogram()
    tune2 = file2.Get(name+"_weights")#weight_shifts")
    print(tune2)
    #tune2.Draw("Ac*")
    #hist2 = tune2.GetHistogram()

    npoints =tune1.GetN()
    print(npoints)
    xpoints = [0.]*npoints
    ypoints = [0.]*npoints
    ratio = TGraph(npoints)    
    for i in range(0, npoints):
        x1 = Double(-1.0)
        y1 = Double(-1.0)
        x2 = Double(-1.0)
 	y2 = Double(-1.0)
       
        tune1.GetPoint(i,x1,y1)
        #print(x1,y1)
        tune2.GetPoint(i,x2,y2)
        #print(x2,y2)
 
        xpoints[i] = x1
        ypoints[i] = y1/y2
        yratio = y1/y2
        ratio.SetPoint(i, x1, yratio)

    ratio.SetName(name+"_ratio")
    ratio.Draw("Ac*")
    ratio.SetMarkerSize(0.03)
    ratio.GetYaxis().SetRangeUser(0.5,1.5)
    ratio.GetXaxis().SetTitle("Q^{2} (GeV)")
    ratio.Write()


gt.Close()

     
    
