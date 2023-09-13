#!/bin/bash

#dateTAG="submitted_`date +%Y_%m_%d_%H_%M`"
dateTAG="submitted_`date +%Y_%m_%d`"

playArray=("5A" "6A" "6B" "6C" "6D" "6E" "6F" "6G" "6H" "6I" "6J")
playArray=("1A", "1B", "1C", "1D", "1E", "1F", "1L", "1M", "1N", "1O", "1P")
#playlistmc=mctest.txt
#playlistdata=datatest.txt

playlistmc=p3_x_me1E_central_mc.txt
playlistdata=p3_x_me1E_central_data.txt

name=MnvTunev431_noPionWeight_Selection_EventLoop
nameside=MnvTunev431_noPionWeight_Sideband_EventLoop
namestudy=MnvTunev431_noPionWeight_Selection_StudiesLoop
playlistname=_all_me1E

############ TEST LITE ####################

jobsub_submit -G minerva --onsite --memory=8GB --disk=8GB --expected-lifetime=40h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${name}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh ${playlistdata} ${playlistmc}

jobsub_submit -G minerva --onsite --memory=8GB --disk=8GB --expected-lifetime=40h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${nameside}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_liteSide.sh ${playlistdata} ${playlistmc}

jobsub_submit -G minerva --onsite --memory=8GB --disk=8GB --expected-lifetime=40h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${namestudy}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/studyLoop_lite.sh ${playlistdata} ${playlistmc}
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_${dateTAG}_Sideband_MichCVCut_fullSystematics_allME1A -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431_MichelCutCVOnly.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoopSide_lite.sh data_me1A_xrootd_plist.txt mc2_me1A_xrootd_plist.txt
#
#
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=28h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_PionReweight_${dateTAG}_testjob_fullSystematics_allME1A -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431PionReweight_MehreenCuts.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh data_me1A_xrootd_plist.txt mc2_me1A_xrootd_plist.txt
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=28h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/MnvTunev431_PionReweight_BkgTune_${dateTAG}_testjob_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_MnvTunev431PionReweightBkgTune_MehreenCuts.tgz file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh data_me1A_xtest.txt mc_me1A_xtest.txt 
