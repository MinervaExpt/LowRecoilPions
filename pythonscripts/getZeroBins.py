#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys

gROOT.SetBatch()

var = sys.argv[1]
TH1.AddDirectory(False)
file1 = sys.argv[2]
wFile = TFile.Open(file1)
wHist = wFile.Get(var+"_migration")

nxbins = wHist.GetNbinsX()+2
nybins = wHist.GetNbinsY()+2
#integral = float(wHist.GetIntegral())
#scale = 1.0/integral
#wHist.Scale(scale)
zerobins = [0]

for i in range(1,nxbins-1):
	for j in range(1,nybins-1):
                content = wHist.GetBinContent(i, j)
		if (content < 50):
		   bcx = wHist.GetXaxis().GetBinCenter(i)
		   bcy = wHist.GetYaxis().GetBinCenter(j)
		   a = wHist.FindBin(bcx,bcy)	
	     	   zerobins.append(a)

res = list(set(zerobins))

print(res)

