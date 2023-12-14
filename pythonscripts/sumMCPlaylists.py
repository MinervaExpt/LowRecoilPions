import sys,os,time
from ROOT import *
from PlotUtils import *

playlists = ["1B", "1C", "1D","1E", "1L", "1M", "1N", "1O", "1P"]
#playlists = ["1A", "1B", "1C","1E", "1F", "1L", "1M", "1O", "1N"]
#Template directory name to use for each playlist path
template = sys.argv[1]

potdata ={}
potmc = {}
potcorr = {}
potmctot = {}
count = 0
mcpottot = 0
datapottot = 0
fiducialNucleons = 0
for play in playlists:
  count += 1
  #filename = template.replace("5A",play)
  pathname = template.replace("1A",play)
  mcfilename = pathname+sys.argv[2] #"studiesMC.root" #"/runEventLoopMC.root"
  datafilename = pathname+sys.argv[3] #"studiesData.root" #"/runEventLoopData.root"
         
  mcfilepath  = os.path.dirname(mcfilename)
  datafilepath = os.path.dirname(datafilename)
  newfilename = os.path.join(mcfilepath,"fluxpotScaled_"+os.path.basename(mcfilename))
  print ("newfilename",newfilename)
  f_out = TFile.Open(newfilename,"RECREATE")
  mcf_root = TFile.Open(mcfilename,'READONLY')
  dataf_root = TFile.Open(datafilename,'READONLY')
  mc_POT = mcf_root.Get("POTUsed").GetVal()
  data_POT = dataf_root.Get("POTUsed").GetVal()
  mcpottot += mc_POT
  datapottot += data_POT
  print("Added data POT So far ", datapottot)
  print("Added MC POT so far ", mcpottot)
  #mcpottot+= mc_POT
   #mc_POT.Print("ALL")
  #potdata[play] = data_POT  #h_POT.GetBinContent(1)
  #potmctot[play] = h_POT.GetBinContent(2)
  #potmc[play] = mc_POT #h_POT.GetBinContent(3)
  pot_corr = data_POT/mc_POT
  #potcorr[play] = potdata[play]/potmc[play]
  #h_POT.SetBinContent(2,h_POT.GetBinContent(2)*potcorr[play])
  #h_POT.SetBinContent(3,h_POT.GetBinContent(3)*potcorr[play])
  print ("file ", mcfilename, " had ",data_POT, mc_POT," POT and Scale is ", pot_corr )
  keys = {}
  keylist = mcf_root.GetListOfKeys();
  fluxlist = []
  for k in keylist:
    name = k.GetName()
    keys[name]=mcf_root.Get(name)
    
    if "nucleon" in name or "POT" in name: #or ("h__" not in name and "h2D" not in name):
      f_out.cd()
      #keys[name].Write()
      #keys[name].Print("ALL")
      #auto newkey = keys[name].SetName(play+name) 
      #print("Writing TParameter ", name)
      keys[name].Write(name)
      fiducialNucleons = keys[name].GetVal()
      #keys[name].Print("ALL")
    else:
      if "flux" in name:
      	#f_out.cd()
      	title = k.GetTitle()
        #print("Writing FluxFileName ", name, " title ", title)
      	#setname = title+"_"+name
      	#keys[name].Scale(data_POT)
	f_out.cd()
        keys[name].Write(name+"_"+play)
        #keys[name].SetName(name+"_"+play)
        fluxlist.append(k)
      else: 
        keys[name].Scale(data_POT/mc_POT)
        f_out.cd()
        keys[name].Write(name)
        #keys[name].Print()
    mcf_root.cd()
  mcf_root.Close()
  f_out.Close()
pot_out = TFile.Open("totalPOT.root","RECREATE")
dataPOT = TParameter(double)("dataPOTUsed", datapottot)
mcPOT = TParameter(double)("mcPOTUsed", mcpottot)
pot_out.cd()
print("DATA POT TOTAL IS: ", dataPOT.GetVal())
print("MC POT total is: ", mcPOT.GetVal())
fiduc = TParameter(double)("fiducialNucleons", fiducialNucleons)
dataPOT.Write()
mcPOT.Write()
fiduc.Write()
dataPOT.Print("ALL")
mcPOT.Print("ALL")

#pot_out.Close()
