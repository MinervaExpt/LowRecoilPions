#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import array as ar
from ROOT import gStyle
gStyle.SetHatchesSpacing(.10)
import csv
gROOT.SetBatch() #Don't render histograms to a window.  Also gets filled areas correct.
gStyle.SetLegendBorderSize(0)

TH1.AddDirectory(False)

dataFile = TFile.Open(sys.argv[2])
mcFile = TFile.Open(sys.argv[1])

#print("MC POT", mcPOT)
#print("DATA POT", dataPOT)
var = "Bbin_Tpibins"

signalHist = mcFile.Get(var + "_MC")
dataHist = dataFile.Get(var + "_data")

#ratio = dataHist.Clone()

#ratio.Divide(ratio, signalHist)

#f = TFile.Open("Bbins_DATAMC_ratio.root", "RECREATE")


npibins = signalHist.GetNbinsY()+2

bkghists = []
npi = mcFile.Get(var + "_picat_Npip_Only")
npidis = mcFile.Get(var + "_picat_NPi_and_Mesons_DIS")
npires = mcFile.Get(var + "_picat_NPi_and_Mesons_RES")
onepi = mcFile.Get(var+"_picat_1pip_wo_p_or_n")
onepin = mcFile.Get(var+"_picat_1pip_w_LNeutron")
onepip = mcFile.Get(var+"_picat_1pip_w_LProton") 
coh = mcFile.Get(var+"_picat_COH_Pions")

qe = mcFile.Get(var+"_picat_QE_like")
pi0 = mcFile.Get(var+"_picat_Neutral_Pi_Only")
kaon = mcFile.Get(var+"_picat_Kaons_Only")
other = mcFile.Get(var+"_picat_Other")


for i in range(1, npibins):
        print("Now Fitting Tpi Bin " , str(i) )
	npihist = npi.ProjectionX("e",i,i)
	npidishist = npidis.ProjectionX("e",i,i)
	npireshist = npires.ProjectionX("e",i,i)
	onepihist = onepi.ProjectionX("e",i,i)
	onepinhist = onepin.ProjectionX("e",i,i)
	onepiphist = onepip.ProjectionX("e",i,i)
	cohhist = coh.ProjectionX("e",i,i)
	qehist = qe.ProjectionX("e",i,i)
	pi0hist = pi0.ProjectionX("e",i,i)
	kaonhist = kaon.ProjectionX("e",i,i)
	otherhist = other.ProjectionX("e",i,i)
		
	sighisto = signalHist.ProjectionX("e",i,i)

	datahisto = dataHist.ProjectionX("e",i,i)
	


	otherhistos = onepiphist.Clone()
	otherhistos.Add(kaonhist, 1)
	otherhistos.Add(qehist, 1)
	otherhistos.Add(pi0hist,1)
	otherhistos.Add(npidishist,1)
	otherhistos.Add(npireshist,1)
	otherhistos.Add(npihist, 1)
	otherhistos.Add(otherhist,1)
	otherhistos.Add(onepi, 1)

	nxbins = sighisto.GetNbinsX()
	
	if (i == 1):
	    prefixcsv=var+"_"+str(i)
	    with open(prefixcsv+'_othershisto.csv', 'w') as csvfile:
		fieldnames = ['tbin', 'bincenter', 'bincontent']
		writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
		writer.writeheader()
		print("Printing bin, bin center, bincontent for Other Histos")
            	for j in range(1,nxbins):
                	#print j, otherhistos.GetBinCenter(j), otherhistos.GetBinContent(j)
			writer.writerow({'tbin':j, 'bincenter': otherhistos.GetBinCenter(j), 'bincontent':otherhistos.GetBinContent(j)})
	
	    print("Printing bin, bin center, bincontent for Other Histos")
	    for j in range(1,nxbins):
		print j, otherhistos.GetBinCenter(j), otherhistos.GetBinContent(j)

	    print("\n Printing bin, bin center, bincontent for Coh Pions")

	    with open(prefixcsv+'_cohhisto.csv', 'w') as csvfile:
                fieldnames = ['tbin', 'bincenter', 'bincontent']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
                print("Printing bin, bin center, bincontent for Other Histos")
		for j in range(1,nxbins):
			writer.writerow({'tbin':j, 'bincenter': cohhist.GetBinCenter(j), 'bincontent':cohhist.GetBinContent(j)})

	    for j in range(1,nxbins):
		 print j, cohhist.GetBinCenter(j), cohhist.GetBinContent(j)
	    
	    print("\n Printing bin, bin center, bincontent for 1 pi+ w. Neutron Pions")

	
	    with open(prefixcsv+'_onepiwNeutron.csv', 'w') as csvfile:
                fieldnames = ['tbin', 'bincenter', 'bincontent']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
		for j in range(1,nxbins):	
			writer.writerow({'tbin':j, 'bincenter': onepinhist.GetBinCenter(j), 'bincontent':onepinhist.GetBinContent(j)})		


	    for j in range(1,nxbins):
                 print j, onepinhist.GetBinCenter(j), onepinhist.GetBinContent(j)

	    print("\n Printing bin, bin center, bincontent for Data Histo")

	    with open(prefixcsv+'_Data.csv', 'w') as csvfile:
                fieldnames = ['tbin', 'bincenter', 'bincontent']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
                for j in range(1,nxbins):
			writer.writerow({'tbin':j, 'bincenter': datahisto.GetBinCenter(j), 'bincontent':datahisto.GetBinContent(j)}) 
		
            for j in range(1,nxbins):
                 print j, datahisto.GetBinCenter(j), datahisto.GetBinContent(j)

	    print("\n Printing bin, bin center, bincontent for MC Histo")
		
	    with open(prefixcsv+'_MC.csv', 'w') as csvfile:
                fieldnames = ['tbin', 'bincenter', 'bincontent']
                writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                writer.writeheader()
                for j in range(1,nxbins):
                        writer.writerow({'tbin':j, 'bincenter': sighisto.GetBinCenter(j), 'bincontent':sighisto.GetBinContent(j)}) 

            for j in range(1,nxbins):
                 print j, sighisto.GetBinCenter(j), sighisto.GetBinContent(j)

	sighisto.Add(npihist, -1)
	datahisto.Add(npihist,-1)
	sighisto.Add(npidishist,-1)
	datahisto.Add(npidishist,-1)
	sighisto.Add(npireshist,-1)
	datahisto.Add(npireshist,-1)
	sighisto.Add(onepihist, -1)
	datahisto.Add(onepihist,-1)
	sighisto.Add(cohhist, -1)
	datahisto.Add(cohhist,-1)
	sighisto.Add(onepiphist, -1)
	datahisto.Add(onepiphist, -1)
	sighisto.Add(qehist, -1)
	datahisto.Add(qehist,-1)
	sighisto.Add(pi0hist, -1)
	datahisto.Add(pi0hist,-1)
	sighisto.Add(kaonhist,-1)
	datahisto.Add(kaonhist,-1)
	sighisto.Add(otherhist,-1)
	datahisto.Add(otherhist,-1)	


	#f.cd()
	
	#ratio = datahisto.Clone()
	#ratio.Divide(ratio, sighisto)
	#ratio.GetXaxis().SetRangeUser(0.001, 1.0)
	#ratio.GetYaxis().SetRangeUser(0.0, 10)	
	#ratio.SetName("DataMCRatio_Bbins_TpiBin"+str(i))
	#ratio.Write()
	#func = TF1('func','[0]+[1]*x', 0.001, 1.0)
	#func2 = TF1('func2', '([0]-x)/x + [1]*x*x + [2]', 0.00, 1.0)
	#func3 = TF1('func3', '[0]+[1]*TMath::Power((x-[2]),[3])', 0.00, 1.0)
	#fithist = ratio.Clone()	
	#fithist.SetName("Fit_Tpibin"+str(i))
	#fithist.GetXaxis().SetTitle("|t| (GeV/c)^{2}")
	#fithist.GetYaxis().SetTitle("Scale Factor")
	#fit = fithist.Fit('func3',"RS")
	
	#chiperndf = fit.Chi2()/fit.Ndf()

	#print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
	#print "Chi Squared Per Degree of Freedom: ", chiperndf
	
	#fithist.Draw("ALP")
	#fithist.Write()
	#fit.Write()
	#ratio.Write()

#f.Close()

	

	
