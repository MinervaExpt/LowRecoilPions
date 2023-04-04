# LowRecoilPions
Mehreen's Low Recoil Pion Inclusive Analysis (updated MAT framework)

Analysis Code is mainly in LowRecoilPions directory. twoDEventLoop.cpp is the one mainly used to output for event level histos and studiesEventLoop.cpp is for michel level. 

For MATMINERVA: follow directions in https://github.com/MinervaExpt/MAT-MINERvA


## Installation for MAT (Installs MAT and UnfoldUtils too)
```
#If you're on a MINERvA GPVM hosted by Fermilab, you need to set up newer versions of ROOT and CMake first
source /cvmfs/minerva.opensciencegrid.org/minerva/hep_hpc_products/setups
setup root v6_10_04d -q e14:prof
setup cmake v3_7_1
kinit #Fermilab Kerberos ticket for getting flux and reweight files
unset SSH_ASKPASS  # this will stop it putting up an xwindow to ask for your token for https git access.

#Have you set up a github personal access token yet?  If you don't know what I'm talking about:
#First, follow these instructions: https://docs.github.com/en/authentication/keeping-your-account-and-data-secure/creating-a-personal-access-token
#When you get to setting permissions, tick only the "repo" box.
#Next, write down the key it generated.
#Then, run this before proceeding: git config --global credential.helper store #Saves your access token as a plaintext file in your home directory
#Later, when you're asked for a username and a password, use your github username and your access key.  This will save the access key forever.

git clone https://github.com/MinervaExpt/MAT-MINERvA.git #Use a github personal access token for checkout
#git clone git@github.com:MinervaExpt/MAT-MINERvA.git #Use an ssh key instead of a github personal access token
mkdir -p opt/build && cd opt/build
cmake ../../MAT-MINERvA/bootstrap -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release

make install #-j 4
```

You will need to also re-build the LowRecoilPions area as well as GENIEXSecExtract using same method. 

For GENIEXSecExtract:

In your working area opt directory:
1. Make a build dir in optdir for this package
``mkdir buildGENIEXSecExtract ``
2. Enter build dir
``cd buildGENIEXSecExtract ``
3. Run cmake
``cmake ../../GENIEXSecExtract -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release ``
4. Install the build
``make install``


This builds the GENIE extraction package to be used by your analysis. 

For LowRecoilPions:

In your working area opt directory:
1. Make a build dir in optdir for this package
``mkdir buildLowRecoilPions ``
2. Enter build dir
``cd buildLowRecoilPions ``
3. Run cmake
``cmake ../../LowRecoilPions -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release ``
4. Install the build
``make install``

This will create setup files for your different tools/packages in the `opt/bin` area.
