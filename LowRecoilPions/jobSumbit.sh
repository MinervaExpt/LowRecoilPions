#!/bin/bash

dateTAG="submitted_`date +%Y_%m_%d_%H_%M`"

playArray=("5A" "6A" "6B" "6C" "6D" "6E" "6F" "6G" "6H" "6I" "6J")


############ TEST LITE ####################
jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_${dateTAG}_Selection_MichCVCut_TEST_fullSystematics_allME1A -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh p3_xnew_me1A_data.txt p3_xnew_me1A_mc.txt
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_${dateTAG}_Sideband_MichCVCut_fullSystematics_allME1A -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431_MichelCutCVOnly.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoopSide_lite.sh data_me1A_xrootd_plist.txt mc2_me1A_xrootd_plist.txt
#
#
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=28h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_PionReweight_${dateTAG}_testjob_fullSystematics_allME1A -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431PionReweight_MehreenCuts.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh data_me1A_xrootd_plist.txt mc2_me1A_xrootd_plist.txt
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=28h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_PionReweight_BkgTune_${dateTAG}_testjob_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431PionReweightBkgTune_MehreenCuts.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh data_me1A_xtest.txt mc_me1A_xtest.txt 
