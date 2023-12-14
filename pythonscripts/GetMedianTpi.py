from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import array as ar

ROOT.TH1.AddDirectory(ROOT.kFALSE)

mcfile = TFile.Open(sys.argv[1]) #TFile.Open("Feb112022_noreweight_distcuts_q3bin6_MC.root")#"Nov18_noreweight_q3bin1_MC.root")

tDhist = mcfile.Get("Pion_Range_vs_Pion_KE_MC")
tDhist.SetDirectory(0)
tDhisto = tDhist.GetCVHistoWithStatError()
nxbins = tDhisto.GetNbinsX()
nybins = tDhisto.GetNbinsY()
print("There are ", nxbins, " NXBins (PionKE) and ", nybins, " NYBins (Range)")
for i in range (1, nxbins):
	tpihist_bini = tDhisto.ProjectionX("e",i)
	ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	prob = ar.array('d',[0.5])
	q = ar.array("d", [0.0])
        y = tpihist_bini.GetQuantiles(1, q, prob)
	#print ybincenter, q[0]

gt = TFile.Open("ProjectionGgraph.root", "RECREATE")

rangebins = ar.array("d", [0]*nybins)
tpibins = ar.array("d", [0]*nybins)
tpierrs = ar.array("d", [0]*nybins)
rangeerrs = ar.array("d", [0]*nybins)

#This is the bit that get Median Value of Tpi for each Range bin 
for i in range(1,nybins):
	x = ar.array("d", [0]*nxbins)
	y = ar.array("d", [0]*nxbins)
	sumy = 0;
	for j in range(1, nxbins):
		x[j] = tDhisto.GetXaxis().GetBinCenter(j)
		y[j] = tDhisto.GetBinContent(j, i)
		sumy+=tDhisto.GetBinContent(j, i)
	graphi = ROOT.TGraph(len(x), x, y)
	graphi.SetName("Rangebin_"+str(i))
	graphi.Write()
	median = ROOT.TMath.Median(nxbins, x, y)
	meany = ROOT.TMath.RMS(nxbins, x, y)
	div = ROOT.TMath.Sqrt(sumy)
	if (i == 0): tpierrs[i] = 0;
	else: tpierrs[i] = meany/div
		
	ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	print ybincenter, median
        rangebins[i] = ybincenter
        tpibins[i] = median
	
gt.Close()
rangebins = rangebins[1:-1]
tpibins = tpibins[1:-1]
f = TFile.Open("TpiRangeFit_MultiFitTest.root", "RECREATE")
g = ROOT.TGraph(len(rangebins),rangebins, tpibins) 
g.SetName("TpiEst")
g.GetXaxis().SetTitle("Pion Range (mm)")
g.GetYaxis().SetTitle("Pion KE (MeV)")
g.Draw("AC*")
g.Write()
ge = ROOT.TGraphErrors(len(rangebins),rangebins, tpibins, rangeerrs, tpierrs)
ge.SetName("TpiEst_Errs")
ge.GetXaxis().SetTitle("Pion Range (mm)")
ge.GetYaxis().SetTitle("Pion KE (MeV)")
ge.Draw("ALP")
ge.Write()
overall = TCanvas("Fit")
func = TF1('func', '[0]*x + [1]*sqrt(x)', 0., 350.)
func2 = TF1('func2', 'gaus',350., 3000.)
func3 = TF1('func3', '[0]*x + [1]*sqrt(x)+gaus(3)', 0.0, 3000.)
#func = TF1('func', 'landau', 0.0, 3000.)
gefit = ge.Clone()
gefit.SetName("TpiEst_Errs_Fit")
fit = gefit.Fit('func', 'R')
gefit.Fit('func2','R+')

par = ar.array('d',[]*5)

func.GetParameters(par[2])
func2.GetParameters(par[3])

func3.SetParameters(par)
fit = gefit.Fit(func3, 'R+')

rangepoints = ar.array("d", [0]*2400)
tpipoints = ar.array("d", [0]*2400)
for q in range(0, 2400):
    rangepoints[q] = float(q);
    tpipoints[q] = func.Eval(q)

gefit.Draw("ALP")
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
	





