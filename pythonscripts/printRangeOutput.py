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
tpiout = open('tpi.csv', 'arw')
rangebincenter = open('rangebincenter.csv','arw')
bkgout = open('rangebkg.csv', 'arw')
writertpi = csv.writer(tpiout)
writerrangebincenter = csv.writer(rangebincenter)
writerbkg = csv.writer(bkgout)
tpibincenter = open('tpibincenter.csv', 'arw')
writertpibincenter = csv.writer(tpibincenter)

mcf = TFile.Open(mcfile)

mighist = mcf.Get("Pion_Range_vs_Pion_KE_signal")

nxbins = mighist.GetNbinsX() + 2
nybins = mighist.GetNbinsY() + 2
print "N range bins: ", nybins
print "N tpi bins: ", nxbins
print "N migration matrix entries ", mighist.GetIntegral()


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

migoutbkg = open('migrationbkg.csv','w')
migbinoutbkg = open('migrationbinbkg.csv','w')
mighistbkg = mcf.Get("Pion_Range_vs_Pion_KE_bkg")
writermigbkg = csv.writer(migoutbkg)
for y in range(0, nybins):
   row = [0]*nxbins
   bini = []
   for x in range(0, nxbins):
         row[x] = '{0:.2f}'.format(mighistbkg.GetBinContent(x, y))
         bini.append(x)
         bini.append(y)
   writermigbkg.writerow(row)


rangemc = open('rangeMC.csv','arw')
rangehist = mcf.Get("permichel_pirange_MC")
nbins = rangehist.GetNbinsX() + 2
bkghist = mcf.Get("permichel_pirange_bkg")

print "N range bins from histo: ", nbins

writerrangemc = csv.writer(rangemc)


print "N Range MC bins: ", nbins
for i in range(0,nbins):
    bincon = ['{0:.2f}'.format(rangehist.GetBinContent(i))]
    writerrangemc.writerow(bincon)

for c in range(0,nbins):
    bincon = ['{0:.2f}'.format(bkghist.GetBinContent(c))]
    writerbkg.writerow(bincon)


dataf = TFile.Open(datafile)
dataPOT = dataf.Get("POTUsed").GetVal()
scale = dataPOT/mcPOT
print "DATA/MC POT scale is : ", scale
rangedata = open('rangeData.csv','arw')
rangedhist = dataf.Get("permichel_pirange_data")
nbinsd = rangedhist.GetNbinsX()+2
print "N Range Data bins: ", nbinsd
writerrangedata= csv.writer(rangedata)
tpihist = mcf.Get("permichel_tpi_signal")
pibins = tpihist.GetNbinsX()+2
for j in range(0, nbinsd):
     bincon = [rangedhist.GetBinContent(j)]
     writerrangedata.writerow(bincon)
     bincent = [rangedhist.GetBinCenter(j)]
     writerrangebincenter.writerow(bincent)
     print bincon

pibins = tpihist.GetNbinsX()+2
for z in range(0, pibins):
    tpibin = [tpihist.GetBinContent(z)]
    writertpi.writerow(tpibin)

#for w in range(0, nybins):
    #rangebin = [mighist.GetYaxis().GetBinWidth(w)]
    #writerrange.writerow(rangebin)

print("======================= \n")
for m in range(0, pibins):
    bincenter = [tpihist.GetBinCenter(m)]
    lowbinedge = tpihist.GetBinLowEdge(m)
    print(lowbinedge)
    writertpibincenter.writerow(bincenter)
