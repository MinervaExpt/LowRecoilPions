from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import array as ar
from ROOT import gStyle
gStyle.SetHatchesSpacing(.10)
gStyle.SetOptStat(0)
ROOT.TH1.AddDirectory(ROOT.kFALSE)
gROOT.SetBatch()
mcfile = TFile.Open(sys.argv[1]) #TFile.Open("Feb112022_noreweight_distcuts_q3bin6_MC.root")#"Nov18_noreweight_q3bin1_MC.root")

tDhist = mcfile.Get("Pion_Range_vs_Pion_KE_signal")#"Pion_Range_vs_Pion_KE_MC")
tDhist.SetDirectory(0)
tDhisto = tDhist#.GetBinNormalizedCopy() #GetCVHistoWithStatError()
nxbins = tDhisto.GetNbinsX()+2
nybins = tDhisto.GetNbinsY()+2
print("There are ", nxbins, " NXBins (PionKE) and ", nybins, " NYBins (Range)")
for i in range (1, nxbins):
	tpihist_bini = tDhisto.ProjectionX("e",i)
	ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	prob = ar.array('d',[0.5])
	q = ar.array("d", [0.0])
        y = tpihist_bini.GetQuantiles(1, q, prob)
	#print ybincenter, q[0]

gt = TFile.Open("ProjectiontestGgraph.root", "RECREATE")

rangebins = ar.array("d", [0]*nybins)
tpibins = ar.array("d", [0]*nybins)
tpierrs = ar.array("d", [0]*nybins)
rangeerrs = ar.array("d", [0]*nybins)

#This is the bit that get Median Value of Tpi for each Range bin 
for i in range(1,nybins):
	x = ar.array("d", [0]*nxbins)
	y = ar.array("d", [0]*nxbins)
	xbincenter = tDhisto.GetYaxis().GetBinCenter(i)
	tpihistd = tDhisto.ProjectionX("e", i, i)
        tpihist = tpihistd.GetBinNormalizedCopy()
	ntpibins = tpihist.GetXaxis().GetNbins()
	itpimax = tpihist.GetMaximum()
	itpibin = tpihist.GetMaximumBin()
	itpimaxval = tpihist.GetBinCenter(itpibin)
	ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	meany = tpihist.GetRMS();
	div = ROOT.TMath.Sqrt(tpihist.Integral())
	
	#print ("the histo RMS is ", meany, "sum of this range bin is ", div, "the error is ", meany/div ) 
	#if (ybincenter > 1000 ): 
		#tpierrs[i] = 0;
		#rangebins[i] = ybincenter
                #tpibins[i] = itpimaxval
	if (ybincenter < 0.06): 
	 		
		ybincenter = 0.0
		itpimaxval = 0.0
	#if (ybincenter < 1000):
	   
        rangebins[i] = ybincenter
	tpibins[i] = itpimaxval

	#tpierrs[i] = meany/div

	print("Range bin center: " , ybincenter , " Tpi max val bin center :", itpimaxval)
	
	sumy = 0;
        for j in range(1, nxbins):
		x[j] = tDhist.GetXaxis().GetBinCenter(j)
		y[j] = tDhist.GetBinContent(j, i)
		sumy+=tDhist.GetBinContent(j, i)
	#graphi = ROOT.TGraph(len(x), x, y)

	overall = TCanvas("Range Slice for Bin " + str(i))
	graphi = tpihist
	graphi.SetName("Rangebin_"+str(i))
	graphi.GetXaxis().SetTitle("True Pion KE (MeV) in Range bin"+str(i)+" With bincenter " + str(xbincenter)+" mm")
	#graphi.ShowPeaks()
	pt = TPaveText(.05,.1,.95,.8);
	pt.AddText("Tpi Peak at "+str(itpimaxval)+" MeV")
	graphi.SetMarkerStyle(20)
	graphi.SetMarkerColor(kBlack)
	graphi.SetMarkerSize(0.5)
	graphi.GetYaxis().SetRangeUser(0, itpimax*1.13)
	#overall.SetLogx()
	graphi.Draw()
	graphi.Draw("HIST SAMES L")
	
	myline=TLine(itpimaxval,0,itpimaxval,itpimax*1.13)
	
	myline.SetLineColor(kRed)
	myline.Draw("sames")	
	#pt.Draw("sames")
	
	overall.Update()
	overall.Print("TpiNormRangebin_"+str(i)+".png")
	graphi.Write()
	#median = ROOT.TMath.Median(nxbins, x, y)
	#meany2 = ROOT.TMath.RMS(nxbins, x, y)
	#div2 = ROOT.TMath.Sqrt(sumy)
	#print("sum of this range bin by manual calc is ", ROOT.TMath.Sqrt(sumy))
	meany2 =ROOT.TMath.RMS(nxbins, x, y)
	div2 = ROOT.TMath.Sqrt(sumy);
	#print("the RMS manually is ", meany2, "sum of this range bin by manual calc is ", ROOT.TMath.Sqrt(sumy), "the error is ", meany2/div2)
	#geterrorhist = tpihist.GetStatError()
	
	if (i == 0): tpierrs[i] = 0;
	else:
		if (div2 != 0): tpierrs[i] = meany2/div2
		
	#ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	#print ybincenter, median
        #rangebins[i] = ybincenter
        #tpibins[i] = median
	
gt.Close()
#print(rangebins, tpibins)
rangebins0 = rangebins
tpibins0 = tpibins
rangebins = rangebins[1:-1]
tpibins = tpibins[1:-1]

print("Range bin lowest val ", rangebins[0], " Tpi bins lowest val ", tpibins[0])

f = TFile.Open("ExcludeBigBins_ErrTpiRangeFit_allbinTest.root", "RECREATE")
g = ROOT.TGraph(len(rangebins0),rangebins0, tpibins0) 
g.SetName("TpiEst")
g.GetXaxis().SetTitle("Pion Range (mm)")
g.GetYaxis().SetTitle("Pion KE (MeV)")
g.Draw("AC*")
g.Write()
rangeerrs0 = rangeerrs
tpierrs0 = tpierrs
rangeerrs = rangeerrs[1:-1]
tpierrs = tpierrs[1:-1]
print("len(rangebins) ", len(rangebins), "len(rangeerrs) ", len(rangeerrs))
ge = ROOT.TGraphErrors(len(rangebins),rangebins, tpibins, rangeerrs, tpierrs)
ge.SetName("TpiEst_Errs")
ge.GetXaxis().SetTitle("Pion Range (mm)")
ge.GetYaxis().SetTitle("Pion KE (MeV)")
ge.Draw("APL")
ge.Write()
overall = TCanvas("Fit")
func = TF1('func', '[0]*x + [1]*sqrt(x)', 0., 1200.)
#func2 = TF1('func2', 'gaus',350., 3000.)
#func3 = TF1('func3', '[0]*x + [1]*sqrt(x)+gaus(3)', 0.0, 3000.)
#func = TF1('func', 'landau', 0.0, 3000.)
gefit = ge.Clone()
gefit.SetName("TpiEst_Errs_Fit")
fit = gefit.Fit('func', 'S')
#gefit.Fit('func2','R+')

#par = ar.array('d',[]*2)

#func.GetParameters(par[1])
#func2.GetParameters(par[3])

#func3.SetParameters(par)
#fit = gefit.Fit(func3, 'R+')

rangepoints = ar.array("d", [0]*2400)
tpipoints = ar.array("d", [0]*2400)
for q in range(0, 2400):
    rangepoints[q] = float(q);
    tpipoints[q] = func.Eval(q)


overall.Print("fitResult.root")
overall.Print("fitResult.png")
chiperndf = fit.Chi2()/fit.Ndf()

print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
print "Chi Squared Per Degree of Freedom: ", chiperndf
gefit.Write()
#fitline = gefit.GetCurve()
#fitline.Draw()

curve = ROOT.TGraph(len(rangepoints),rangepoints, tpipoints)
#fit.Scan(curve, 0.0, 2400.)
curve.SetName("FitCurve")
curve.GetXaxis().SetTitle("Pion Range (mm)")
curve.GetYaxis().SetTitle("Pion KE (MeV)")
curve.Draw("AC*")
curve.Write()


f.Close()
        #int n = tpihist_bini.GetXaxis().GetNbins()  
        #x(n);
        #bincenter = tpihist_bini.GetXaxis().GetCenter( &x[0] );
        #y = h1.GetArray(); 
       # exclude underflow/overflows from bin content array y
        #tpimedian = TMath::Median(n, &x[0], &y[1]); 
	





