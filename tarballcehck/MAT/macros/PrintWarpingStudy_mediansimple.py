import ROOT
from ROOT import PlotUtils
import sys

chi2SummaryDir = "Chi2_Iteration_Dists"
chi2SummaryName = "h_chi2_modelData_trueData_iter_chi2"
medianHistName = "h_median_chi2_modelData_trueData_iter_chi2"
meanChi2ProfileName = "m_avg_chi2_modelData_trueData_iter_chi2_truncated"

lineWidth = 3
yNDF = 12
iterChosen = 3

can = ROOT.TCanvas("chi2")

for fileName in sys.argv[1:]:
  myFile = ROOT.TFile.Open(fileName)

  univName = fileName[fileName.find("merged") + len("merged") + 1:fileName.find(".root")]  
 
  spread = myFile.Get(chi2SummaryDir).Get(chi2SummaryName)
  spread.GetYaxis().SetRangeUser(0.0, 500.)
  spread.SetTitle("Universe: " + univName)
  spread.SetTitleOffset(0.75, "X")
  spread.SetTitleOffset(0.65, "Y")
  spread.Draw("colz")

  profile = myFile.Get(chi2SummaryDir).Get(meanChi2ProfileName)
  profile.SetTitle("Mean Chi2")
  profile.SetLineWidth(lineWidth)
  profile.SetLineColor(ROOT.kBlue)
  profile.SetMarkerStyle(0)
  profile.Draw("SAME")

  median = myFile.Get(chi2SummaryDir).Get(medianHistName)
  median.SetTitle("Median Chi2")
  median.SetLineWidth(lineWidth)
  median.SetLineColor(ROOT.kBlack)
  median.Draw("HIST SAME")


  ndfLine = ROOT.TLine(1, yNDF, spread.GetXaxis().GetXmax(), yNDF)
  ndfLine.SetLineWidth(lineWidth)
  ndfLine.SetLineStyle(ROOT.kDashed)
  ndfLine.Draw()

  doubleNDFLine = ROOT.TLine(1, 2*yNDF, spread.GetXaxis().GetXmax(), 2*yNDF)
  doubleNDFLine.SetLineColor(ROOT.kRed)
  doubleNDFLine.SetLineWidth(lineWidth)
  doubleNDFLine.SetLineStyle(ROOT.kDashed)
  doubleNDFLine.Draw()


  iterLine = ROOT.TLine(iterChosen + 0.5, 0, iterChosen + 0.5, spread.GetYaxis().GetXmax())
  iterLine.SetLineWidth(lineWidth)
  iterLine.SetLineStyle(ROOT.kDotted)
  iterLine.Draw()

  leg = ROOT.TLegend(0.6, 0.6, 0.9, 0.9)
  leg.AddEntry(profile)
  leg.AddEntry(median)
  leg.AddEntry(ndfLine, "Number of Bins", "l")
  leg.AddEntry(doubleNDFLine, "2x Number of Bins", "l")
  leg.AddEntry(iterLine, str(iterChosen) + " iterations", "l")
  leg.Draw()

  can.Print("WarpingStudy_Printed.root")
  can.Print(fileName + ".png")
