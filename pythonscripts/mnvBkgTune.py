#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
import csv

gROOT.SetBatch()

TH1.AddDirectory(False)
mcsig = TFile.Open(sys.argv[1])
datasig = TFile.Open(sys.argv[2])
mcside = TFile.Open(sys.argv[3])
dataside = TFile.Open(sys.argv[4])


var = "AvailableE"
axistitle = "Available Energy (MeV)"

mcsigsel = mcsig.Get("AvailableE_selected_signal_reco")
datasigh = datasig.Get("AvailableE_data")
mcsigside = mcside.Get("AvailableE_selected_signal_reco")
datasideh = dataside.Get("AvailableE_data")

mcsigreco = mcsig.Get("AvailableE_data")
mcsidereco = mcside.Get("AvailableE_data")

dataPOT = datasig.Get("POTUsed").GetVal()
mcPOT = mcsig.Get("POTUsed").GetVal()
dataPOTside = dataside.Get("POTUsed").GetVal()
mcPOTside = mcside.Get("POTUsed").GetVal()

scale = dataPOT/mcPOT
scaleside = dataPOTside/mcPOTside

selbkg1 = mcsig.Get("AvailableE_top_QE_like")
#selbkg2 = mcsig.Get("AvailableE_top_Other")
selbkg3 = mcsig.Get("AvailableE_top_Neutral_Pi_Only")

sidebkg1 = mcside.Get("AvailableE_top_QE_like")
#sidebkg2 = mcside.Get("AvailableE_top_Other")
sidebkg3 = mcside.Get("AvailableE_top_Neutral_Pi_Only")

selbkgsum = selbkg1.Clone()
#selbkgsum.Add(selbkg2.Clone())
selbkgsum.Add(selbkg3.Clone())

sidebkgsum = sidebkg1.Clone()
#sidebkgsum.Add(sidebkg2.Clone())
sidebkgsum.Add(sidebkg3.Clone())

mcsigsel.Scale(scale)
mcsigreco.Scale(scale)
selbkgsum.Scale(scale)
mcsigside.Scale(scaleside)
mcsidereco.Scale(scaleside)
sidebkgsum.Scale(scaleside)

print "Done Scaling Histots"

A = selbkgsum.Clone()
B = mcsigsel.Clone()
C = sidebkgsum.Clone()
D = mcsigside.Clone()

datatol = datasigh.Clone()
datasidetol = datasideh.Clone()

A.SetBinContent(0,1.0002)
B.SetBinContent(0,1.0003)
C.SetBinContent(0,1.0001)
D.SetBinContent(0,1.00002)

datatol.SetBinContent(0,1)
datasidetol.SetBinContent(0,1)

print "Done setting underflow content to some small number"

datatol.AddMissingErrorBandsAndFillWithCV(mcsig.Get(var + "_data"))
datasidetol.AddMissingErrorBandsAndFillWithCV(mcside.Get(var + "_data"))

AD = A.Clone()#*D.Clone()
#AD.Print()
AD.Multiply(A.Clone(),D.Clone())
BC = B.Clone()#*C.Clone()
BC.Multiply(B.Clone(),C.Clone())
#AD.Print()
diff = AD.Clone()
diff.Add(BC.Clone(), -1)
print "Done subtracting AD - BC"

Dinv = D.Clone()#/diff.Clone()
Dinv.Divide(Dinv.Clone(),diff.Clone())
Binv = B.Clone()
Binv.Scale(-1.)
Binv.Divide(Binv.Clone(), diff.Clone())
Cinv = C.Clone()
Cinv.Scale(-1.)
Cinv.Divide(Cinv.Clone(),diff.Clone())
Ainv = A.Clone()
Ainv.Divide(Ainv.Clone(), diff.Clone())
print "Done dividing to get inverse of each histo"

DinvDataSel = Dinv.Clone()
DinvDataSel.Multiply(datatol.Clone(), Dinv.Clone())
print "done multiple D with Dataseltol"
CinvDataSide = Cinv.Clone()
CinvDataSide.Multiply(datasidetol.Clone(), Cinv.Clone())

BinvDataSel = datatol.Clone()
BinvDataSel.Multiply(datatol.Clone(), Binv.Clone())
AinvDataSide = datasidetol.Clone()
AinvDataSide.Multiply(datasidetol.Clone(), Ainv.Clone())

print "Done Multiplying the relevant quanitites"

scalefactorsbkg = DinvDataSel.Clone()
scalefactorsbkg.Add(CinvDataSide.Clone())
print "bkg scale factors added"

scalefactorssig = AinvDataSide.Clone()
scalefactorssig.Add(BinvDataSel.Clone())
print "Done Summing the final scale factor thingies"
scalefactorsbkg.GetXaxis().SetTitle("Available Energy (MeV)")
scalefactorsbkg.GetYaxis().SetTitle("Background Scale Factors")
scalefactorssig.GetXaxis().SetTitle("Available Energy (MeV)")
scalefactorssig.GetYaxis().SetTitle("Signal Scale Factors")

#scalefactorssig.GetYaxis().SetRangeUser(0.0, 2.0)
#scalefactorsbkg.GetYaxis().SetRangeUser(0.0, 2.0)


overall = TCanvas(var+"_ScaleFactors")

scalefactorsbkg.Draw("HIST")
overall.Print(var+"_Bkg_ScaleFactors.png")

scalefactorssig.Draw("HIST")
overall.Print(var+"_Sig_ScaleFactors.png")
 

nbins = datatol.GetNbinsX()+2

scalefactorbkg = datatol.GetCVHistoWithError().Clone()
scalefactorbkg.Add(scalefactorbkg,-1)
scalefactorsig = datatol.GetCVHistoWithError().Clone()
scalefactorsig.Add(scalefactorsig, -1)

for i in range(1, nbins):
    bkgsel = selbkgsum.GetBinContent(i)
    sigsel = mcsigsel.GetBinContent(i)
    bkgside = sidebkgsum.GetBinContent(i)
    sigside = mcsigside.GetBinContent(i)
    dataselec = datasigh.GetBinContent(i)
    datasideb = datasideh.GetBinContent(i)

    datatol = TVectorD(2)
    datatol[0] = dataselec
    datatol[1] = datasideb
    
    matA = TMatrixD(2,2)
    matA[0,0] = bkgsel
    matA[0,1] = sigsel
    matA[1,0] = bkgside
    matA[1,1] = sigside


    #datatol.Print()
    #matA.Print()

    lu = TDecompLU(matA)
    lu.Solve(datatol)
    #datatol.Print()

    #scalefactorbkg[i] = datatol[0]
    #scalefactorsig[i] = datatol[1]
    scalefactorbkg.SetBinContent(i, datatol[0])
    scalefactorsig.SetBinContent(i, datatol[1])
    print(scalefactorsig[i], scalefactorbkg[i])


 
scalefactorbkg.GetXaxis().SetTitle("Available Energy (MeV)")
scalefactorbkg.GetYaxis().SetTitle("Background Scale Factors")
scalefactorsig.GetXaxis().SetTitle("Available Energy (MeV)")
scalefactorsig.GetYaxis().SetTitle("Signal Scale Factors")
scalefactorbkg.SetName("BkgScaleFactors")
scalefactorsig.SetName("SigScaleFactors")
scalefactorsig.GetYaxis().SetRangeUser(0.0, 2.)
scalefactorbkg.GetYaxis().SetRangeUser(0.0, 2.)
scalefactorbkg.Draw("HIST")
overall.Print(var+"_bin_Bkg_ScaleFactors.png")

scalefactorsig.Draw("HIST")
overall.Print(var+"_bin_Sig_ScaleFactors.png")


fout = TFile.Open("BkgSigScaleFactors.root", "RECREATE")
scalefactorsig.Write()
scalefactorbkg.Write()
fout.Close()

