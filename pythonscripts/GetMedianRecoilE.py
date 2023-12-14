from ROOT import *
from ROOT import PlotUtils
import ROOT
import os
import sys
import array as ar

ROOT.TH1.AddDirectory(ROOT.kFALSE)

mcfile =  TFile.Open("runEventLoopMC_July292022_EavailComp_pTbin6_noreweight.root")#runEventLoopMC_July262022_EavailComp_q3bin6_reweight.root")#runEventLoopMC_June232022_EavailComp_q3bin6_reweight.root ")#runEventLoopMC_June262022_EavailComp_q3bin6_reweight.root")#runEventLoopMC_June262022_EavailComp_ptbin6_reweight.root")#runEventLoopMC_June262022_EavailComp_pTbin6_reweight.root")#Feb112022_noreweight_distcuts_q3bin6_MC.root")#"Nov18_noreweight_q3bin1_MC.root")
gt = TFile.Open("ProjectionGraph_RecoilEvq0_RBins.root", "RECREATE")
tDhist = mcfile.Get("NewRecoilE_NewRecoilEbins_recoMinusTrueRBins")#EavailCorr_EavailCorry_recoMinusTrueTBins")#ERecoilCorrvq0_ERecoilCorrvq0y_recoMinusTrueRBins")#EAvailable_Eavaily_recoMinusTrueRBins")#ERecoilvq0_ERecoilvq0y_recoMinusTrueRBins")
tDhist.SetDirectory(0)
tDhisto = tDhist.GetCVHistoWithStatError()
nxbins = tDhisto.GetNbinsX()
nybins = tDhisto.GetNbinsY()
print nxbins, nybins

print "Trying to Find peaks in 2D histo"

s = ROOT.TSpectrum2(nxbins, 1)  
nfound = s.Search(tDhisto,2,"colz,noMarkov",0.5)       
xpeaks = s.GetPositionX()
ypeaks = s.GetPositionY()
for i in range(0,nfound):
	print "Residual Peak ", i, " at Recoil Bin ", ypeaks[i] , " is ", xpeaks[i]

recoilbins = ar.array("d", [0]*nybins)
resmeanbins = ar.array("d", [0]*nybins)
resmedbins = ar.array("d", [0]*nybins)
resrmsbins = ar.array("d", [0]*nybins)
peakbins = ar.array("d", [0]*nybins)
ybinnin = [0.0, 150., 200., 250., 300., 400., 500., 600., 800., 1000., 1200., 1400.,]
for i in range (0, nybins):
	tpihist_bini = tDhisto.ProjectionX("e",i,i)
	ybincenter = tpihist_bini.GetYaxis().GetBinCenter(i)
	tpihist_bini.SetName("RecoilERecoBin"+str(i))
	tpihist_bini.GetXaxis().SetTitle("reco - true (MeV)")
	tpihist_bini.GetYaxis().SetTitle("N Events")
        tpihist_bini.GetXaxis().SetRangeUser(-3000.,1000.)
	tpihist_bini.SetFillColor(kRed)
	tpihist_bini.Draw("h")
        tpihist_bini.Write()		
        mean = tpihist_bini.GetMean()
	rms = tpihist_bini.GetRMS()
        resmeanbins[i] = mean
	resrmsbins[i] = rms
	recoilbins[i] = ybincenter
        s = ROOT.TSpectrum(nxbins, 1)
        nfound = s.Search(tpihist_bini,1,"noMarkov",0.85) 
        xpeaks = s.GetPositionX()
        ypeaks = s.GetPositionY()
       
 	for j in range(0,nfound):
        	#print "Residual Peak for ybin  ", i, " at Res Bin ", xpeaks[j] , " is ", ypeaks[j]
		print i, ", ", ypeaks[j], "," , xpeaks[j], "," , rms, "," , mean
        
        #s = ROOT.TSpectrum2(5)
	#nfound = s.Search(tpihist_bini,2,"",0.10)
	

	#func1 = TF1('func1', '[0] + x*[1] + 1/(TMath::Sqrt(2*TMath::Pi())*[2]) * [3] * TMath::Exp(-((x - [4])/(TMath::Sqrt(2.)*[2]))*((x - [4])/(TMath::Sqrt(2.)*[2])))')
        #func = TF1('func', '[0]*x + [1]*sqrt(x)', 0., 1200.)
	#gethistfit = tpihist_bini.Clone()
        #gethistfit.SetName("RecoilEres_fitbin"+str(i))
	#fit = gethistfit.Fit('func1', 'S')
	#gethistfit.Draw("ALP")
	#chiperndf = fit.Chi2()/fit.Ndf()
	#print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
	#print "Chi Squared Per Degree of Freedom: ", chiperndf
	#gethistfit.Write()
	#print ybincenter,",", q[0]


rangebins = ar.array("d", [0]*nybins)
tpibins = ar.array("d", [0]*nybins)
tpierrs = ar.array("d", [0]*nybins)
rangeerrs = ar.array("d", [0]*nybins)
meanbins = ar.array("d", [0]*nybins)
print "Getting Mean Bin"
	#median = ROOT.TMath.Median(nxbins, x, y)
	#meany = ROOT.TMath.RMS(nxbins, x, y)
	#div = ROOT.TMath.Sqrt(sumy)
	#if (i == 0): tpierrs[i] = 0;
	#else: tpierrs[i] = meany/div
        
		
	#ybincenter = tDhisto.GetYaxis().GetBinCenter(i)
	#print ybincenter,",", median
        #rangebins[i] = ybincenter
        #tpibins[i] = median
	#meanbins[i] = meany
	
gt.Close()
#rangebins = rangebins[1:-1]
#tpibins = tpibins[1:-1]
#meanbins = meanbins[1:-1]
#f = TFile.Open("June9_Mediangraph.root", "RECREATE")

#meangraph = ROOT.TGraph(len(rangebins),rangebins, meanbins)
#meangraph.SetName("RecoulEvMeandiff")
#meangraph.GetXaxis().SetTitle("Reconstructed Recoil E (MeV)")
#meangraph.GetYaxis().SetTitle("Mean reco - true (MeV)")
#meangraph.Draw("AC*")
#meangraph.Write()


#g = ROOT.TGraph(len(rangebins),rangebins, tpibins) 
#g.SetName("TpiEst")
#g.GetXaxis().SetTitle("Reconstructed Recoil E (MeV)")
#g.GetYaxis().SetTitle("Median reco - true (MeV)")
#g.Draw("AC*")
#g.Write()


#ge = ROOT.TGraphErrors(len(rangebins),rangebins, tpibins, rangeerrs, tpierrs)
#ge.SetName("RecoilEEst_Errs")
#ge.GetXaxis().SetTitle("Reconstructred Recoil E (MeV)")
#ge.GetYaxis().SetTitle("Additive Correction To Apply (MeV)")
#ge.Draw("ALP")
#ge.Write()

#func = TF1('func', '[0]*x + [1]*sqrt(x)', 0., 1200.)
#gefit = ge.Clone()
#gefit.SetName("TpiEst_Errs_Fit")
#fit = gefit.Fit('func', 'S')
#gefit.Draw("ALP")
#chiperndf = fit.Chi2()/fit.Ndf()
#print "Chi Squared: " ,  fit.Chi2(), " NdF: ", fit.Ndf()
#print "Chi Squared Per Degree of Freedom: ", chiperndf

#gefit.Write()

#f.Close()
        #int n = tpihist_bini.GetXaxis().GetNbins()  
        #x(n);
        #bincenter = tpihist_bini.GetXaxis().GetCenter( &x[0] );
        #y = h1.GetArray(); 
       # exclude underflow/overflows from bin content array y
        #tpimedian = TMath::Median(n, &x[0], &y[1]); 
	





