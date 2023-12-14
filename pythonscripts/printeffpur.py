from ROOT import *
import sys,os,time
filename = sys.argv[1]
var = sys.argv[2]
from PlotUtils import *


mc = TFile.Open(filename, "READONLY")

purnum = mc.Get(var+"_selected_signal_reco")
purdenom = mc.Get(var+"_data")

nbins = purnum.GetNbinsX()+2
purnumtot = purnum.Integral(1,nbins-1)
purdenomtot = purdenom.Integral(1,nbins-1)
print("Total Purity in "+var+" is: ", purnumtot/purdenomtot)

effnum = mc.Get(var+"_efficiency_numerator")
effdenom = mc.Get(var+"_efficiency_denominator")
effnumtot = effnum.Integral(1,nbins-1)
effdenomtot= effdenom.Integral(1,nbins-1)

print("Total Efficiency in "+var+" is: ", effnumtot/effdenomtot)

