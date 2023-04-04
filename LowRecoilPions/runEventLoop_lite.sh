#!/bin/bash

export OUTDIR=`pwd`/$CONDOR_DIR_OUT

printenv
set -x #start bash debugging at this point
echo Start  `date`
echo Site:${GLIDEIN_ResourceName}
echo "the worker node is " `hostname` "OS: "  `uname -a`
echo "the user id is " `whoami`
echo "the output of id is " `id`
echo "------ CHECK THE VOMS PROXY and the x509"
echo $X509_USER_PROXY
echo "OUTPUT DIRECTORY"
echo $CONDOR_DIR_OUT
#voms-proxy-info --all
set +x #stop bash debugging at this point

umask 002 #set the read/write permission of files created to be 775

cd $_CONDOR_SCRATCH_DIR

echo "pwd is " `pwd`

#Output location defined by use of $CONDOR_DIR_OUT defined by -d OUT /pnfs/minerva/scratch/users/dlast/

if [ -z ${GRID_USER} ]; then
GRID_USER=`basename $X509_USER_PROXY | cut -d "_" -f 2`
fi

echo "GRID_USER = `echo $GRID_USER`"

OUTTAG=`date +%m_%d_%Y_%H_%M`_${CLUSTER}.${PROCESS}

echo "Here is the your environment in this job: " > job_output_${OUTTAG}.log 2> job_output_${OUTTAG}.err #Creates file for logging information, note that this is stdout only
env >> job_output_${OUTTAG}.log 2>> job_output_${OUTTAG}.err #since file already exists use ">>" instead of ">"

#/usr/bin/time -v -o tarTime.txt tar -xzvf $CONDOR_DIR_INPUT/dlast_*.tgz

if [ $? -ne 0 ]; then
        echo "Failed to unpack input tarball"
        echo "Are you sure this file exists?:"
        echo "$INPUT_TAR_FILE"
        exit 71
fi


cd `dirname $INPUT_TAR_FILE`/opt

echo "pwd is " `pwd`
#ls 
export MINERVA_PREFIX=`pwd`
#echo "Going to LowRecoils for the setup scripts"
#cd LowRecoilPions/
#ls
echo "pwd is " `pwd`
echo "Now setting up setup scripts"
export INSTALL_DIR=${MINERVA_PREFIX}
source ../LowRecoilPions/setup_env.sh
source ../LowRecoilPions/setup_utils.sh
cd ../LowRecoilPions/
echo "pwd is " `pwd`
export LOWRECPREFIX=`pwd`
export LOWREC_PREFIX=${LOWRECPREFIX}

echo "now going to opt/bin/"
cd ../opt/bin
echo "pwd is " `pwd`
export SOURCE_PREFIX=`pwd`
source setup.sh
export INSTALL_DIR=${MINERVA_PREFIX}
export MPARAMFILESROOT=${INSTALL_DIR}/etc/MParamFiles
export MATFLUXANDWEIGHTFILES=${INSTALL_DIR}/etc/MATFluxAndReweightFiles
export MPARAMFILES=${MPARAMFILESROOT}/data

source setup_MAT.sh
source setup_MAT-MINERvA.sh
source setup_UnfoldUtils.sh
source setupROOT6OnGPVMs.sh
source setup_MAT_IncPions.sh

source setup_weights.sh

cd ../../LowRecoilPions/

echo "pwd is " `pwd`
export LOWRECPREFIX=`pwd`
export LOWREC_PREFIX=${LOWRECPREFIX}

#source setup_env.sh

echo "Done Setting up env. Now Setting up Utils. pwd is " `pwd`
#ls 
#source setup_utils.sh
echo "Moving to CONDOR SCRATCH DIR $_CONDOR_SCRATCH_DIR"
cd $_CONDOR_SCRATCH_DIR

echo "pwd is " `pwd`

#Arguments Reminder
#1: playlist data
#2: playlist MC
#3: skip syst? anything non-zero means yes.
#4: MnvTune: 1 or 2 only options currently (10/27/2021)
#5: "Tracker" or "Targets" for FV definition
#6: Necessary TgtNum for single target selection
#7: Switch for "doVtxOnly" only relevant for the 2D target memory limits.
#8: optional naming of the files
#9: optional switch to turn off neutron cuts when passed a 0.


echo "With playlists: " `$1` " and " `$2`
/usr/bin/time -v -o runEventLoopTime.txt twoDEventLoop $LOWREC_PREFIX/playlists/$1 $LOWREC_PREFIX/playlists/$2 > Dump_runEventLoop.txt

echo "Moving files to output Dir " `$CONDOR_DIR_OUT`

mv *.txt $OUTDIR
mv *.log *.err $OUTDIR
mv *.root $OUTDIR
