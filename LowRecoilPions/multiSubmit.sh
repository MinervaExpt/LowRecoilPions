#!/bin/bash

dateTAG="submitted_`date +%Y_%m_%d_%H_%M`"





#mkdir /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1-PionReweight_${dateTAG}_gridtest 

#jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=6GB --disk=15GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_studiesEventLoop_${dateTAG}_gridtest -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan242023_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/studiesEventLoop_grid.sh data_me1A_xrootd_plist.txt mc_me1A_xrootd_plist.txt

#Sideband Loop

#jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=6GB --disk=15GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_twoDSideband_${dateTAG}_gridtest -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan232023_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoopSide_job.sh data_me1A_xrootd_plist.txt mc1_me1A_xrootd_plist.txt

#Selection Loop

#jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=4GB --disk=15GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_twoDSelection_${dateTAG}_gridtest -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan232023_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_job.sh data_me1A_xrootd_plist.txt mc1_me1A_xrootd_plist.txt



#Submitting jobs for Pion Reweight

#sultana_Jan252023_PionReweight_LowRecoilPions.tgz

#jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=4GB --disk=6GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_SideEventLoop_PiReweight_${dateTAG}_gridtest -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan252023_PionReweight_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/studiesEventLoop_grid.sh data_me1A_xrootd_plist.txt mc_me1A_xrootd_plist.txt

#Sideband Loop

#jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=4GB --disk=6GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_twoDSideband_noreweight_fullMC_nosystematics_${dateTAG}_gridtest/ -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan262023_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoopSide_job.sh data_me1A_xrootd_plist.txt mc_me1A_xrootd_plist.txt

#Selection Loop

jobsub_submit --group minerva -role=Analysis --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --memory=6GB --disk=5GB --expected-lifetime=24h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/MnvTunev4-3-1_twoDSelection_fullMC_nosystematics_PiReweight_${dateTAG}_gridtest/ -e IFDH_CP_MAX_RETRIES=1 -f /pnfs/minerva/resilient/tarballs/sultana_Jan262023_PionReweight_LowRecoilPions.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_job.sh datatest.txt mctest.txt
#data_me1A_xrootd_plist.txt mc_me1A_xrootd_plist.txt
