#!/usr/bin/python

from ROOT import *
from ROOT import PlotUtils
import sys
import csv
import numpy

gROOT.SetBatch()

TH1.AddDirectory(False)
mcsig = TFile.Open(sys.argv[1])
datasig = TFile.Open(sys.argv[2])
mcside = TFile.Open(sys.argv[3])
dataside = TFile.Open(sys.argv[4])


var = "AvailableE"
axistitle = "Available Energy (MeV)"

mcsigh = mcsig.Get("AvailableE_data")
datasigh = datasig.Get("AvailableE_data")
mcsideh = mcside.Get("AvailableE_data")
datasideh = dataside.Get("AvailableE_data")






