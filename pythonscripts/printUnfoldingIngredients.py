#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
import csv
gROOT.SetBatch()

TH1.AddDirectory(False)
mcfile = sys.argv[1]
datafile = sys.argv[2]

migout = open('migration.csv','arw')
migbinout = open('migrationbin.csv','arw')
writermig = csv.writer(migout)
writermigbin = csv.writer(migbinout)
tpibincenter = open('tpibincenter.csv', 'arw')
rangebincenter = open('rangebincenter.csv','arw')
writertpi = csv.writer(tpibincenter)
writerrange = csv.writer(rangebincenter)
mcf = TFile.Open(mcfile)

mighist = mcf.Get("Pion_Range_vs_Pion_KE_MC")

nxbins = mighist.GetNbinsX() + 2
nybins = mighist.GetNbinsY() + 2
print "N range bins: ", nybins
print "N tpi bins: ", nxbins

mcPOT = mcf.Get("POTUsed").GetVal()


for y in range(0, nybins):
   row = [0]*nxbins
   bini = []
   for x in range(0, nxbins):
         row[x] = '{0:.2f}'.format(mighist.GetBinContent(x, y)) 
         bini.append(x) 
         bini.append(y)
   writermig.writerow(row)
   writermigbin.writerow(bini)

rangemc = open('rangeMC.csv','arw')
rangehist = mcf.Get("permichel_pirange_MC")
nbins = rangehist.GetNbinsX() + 2
writerrangemc = csv.writer(rangemc)
print "N Range MC bins: ", nbins
for i in range(0,nbins):
    bincon = [rangehist.GetBinContent(i)]
    writerrangemc.writerow(bincon)
    
dataf = TFile.Open(datafile)
dataPOT = dataf.Get("POTUsed").GetVal()
scale = dataPOT/mcPOT
print "DATA/MC POT scale is : ", scale
rangedata = open('rangeData.csv','arw')
rangedhist = dataf.Get("permichel_pirange_data")
nbinsd = rangedhist.GetNbinsX()+2
print "N Range Data bins: ", nbinsd
writerrangedata= csv.writer(rangedata)
for j in range(0, nbinsd):
     bincon = [rangedhist.GetBinContent(j)]
     writerrangedata.writerow(bincon)
     print bincon
for z in range(0, nxbins):
    tpibin = [mighist.GetXaxis().GetBinCenter(z)]
    writertpi.writerow(tpibin)

for w in range(0, nybins):
    rangebin = [mighist.GetYaxis().GetBinCenter(w)]
    writerrange.writerow(rangebin)
