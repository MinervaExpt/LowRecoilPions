#!/bin/bash

dateTAG="submitted_`date +%Y_%m_%d`" #_%H_%M`"

#playArray=("5A" "6A" "6B" "6C" "6D" "6E" "6F" "6G" "6H" "6I" "6J")
declare -a playArray=("1A" "1B" "1C" "1D" "1E" "1F" "1L" "1M" "1N" "1O" "1P")
#declare -a playArray=("1E")
#declare -a playArray=("1B" "1C" "1D" "1E" "1L" "1M" "1O" "1P")
tarfile=sultana_Mnv431_noPionWeight_Apr_18_2023.tgz #this one has the Pion Angle bug fix
#sultana_Mnv431_PionWeight_Apr_17_2023.tgz
#sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz
#playlistmc=mctest.txt
#playlistdata=datatest.txt

for val in ${playArray[@]};
do
	playlistmc=p3_x_me${val}_central_mc.txt
	playlistdata=p3_x_me${val}_central_data.txt

	name=MnvTunev431_noPionWeight_Selection_EventLoop
	nameside=MnvTunev431_noPionWeight_Sideband_EventLoop
	namestudy=MnvTunev431_noPionWeight_Selection_StudiesLoop
	playlistname=all_me${val}_actuallynoPionweight
        
        #echo ${val}
	echo ${playlistname}        

        jobsub_submit -G minerva --onsite --memory=10GB --disk=10GB --expected-lifetime=35h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${playlistname}_${dateTAG}_${name}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/${tarfile} --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh ${playlistdata} ${playlistmc}
	jobsub_submit -G minerva --onsite --memory=10GB --disk=10GB --expected-lifetime=35h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${playlistname}_${dateTAG}_${nameside}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/${tarfile} --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_liteSide.sh ${playlistdata} ${playlistmc}
	jobsub_submit -G minerva --onsite --memory=10GB --disk=10GB --expected-lifetime=35h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/AllPlaylistTest/${playlistname}_${dateTAG}_${namestudy}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/${tarfile} --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/studyLoop_lite.sh ${playlistdata} ${playlistmc}
done

#playlistmc=p3_x_me1A_central_mc.txt
#playlistdata=p3_x_me1A_central_data.txt

#name=MnvTunev431_noPionWeight_Selection_EventLoop
#nameside=MnvTunev431_noPionWeight_Sideband_EventLoop
#namestudy=MnvTunev431_noPionWeight_Selection_StudiesLoop
#playlistname=_all_me1A

############ TEST LITE ####################

#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/${name}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_lite.sh ${playlistdata} ${playlistmc}
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/${nameside}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/runEventLoop_liteSide.sh ${playlistdata} ${playlistmc}
#jobsub_submit -G minerva --onsite --memory=6GB --disk=6GB --expected-lifetime=30h -N 1 -d OUT /pnfs/minerva/scratch/users/sultana/GridJobs/${namestudy}_${dateTAG}_${playlistname}_fullSystematics -e IFDH_CP_MAX_RETRIES=1 --tar-file-name dropbox:///minerva/data/users/sultana/tarballs/sultana_Mnv431_NoPionWeight_Apr_11_2023.tgz --skip-check rcds file:///minerva/app/users/sultana/cmtuser/WorkingArea/LowRecoilPions/LowRecoilPions/studyLoop_lite.sh ${playlistdata} ${playlistmc}
