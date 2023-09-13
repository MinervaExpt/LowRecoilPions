#!/bin/bash

#Usage: runTransWarp.sh runEventLoopMC.root runEventLoopData.root warped.root

VARIABLE=$3 #AvailableE
MIGRATION_FILE=$1
DATA_FILE=$2
TRUE_HIST=${VARIABLE}_efficiency_numerator
WARPED_FILE=$2
RECO_HIST=${VARIABLE}_signal_reco
OUTFILE_NAME=$(basename $3_$2)

TransWarpExtraction --output_file Warping_$OUTFILE_NAME --data $RECO_HIST --data_file $WARPED_FILE --data_truth $TRUE_HIST --data_truth_file $WARPED_FILE --migration ${VARIABLE}_migration --migration_file $MIGRATION_FILE --reco $RECO_HIST --reco_file $MIGRATION_FILE --truth $TRUE_HIST --truth_file $MIGRATION_FILE --num_iter 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,40,50,60,70,80,90,100 --num_uni 100 -C 2 -c 10000 --num_dim 2 -V true #--exclude_bins "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 19, 20, 21, 22, 23, 24, 35, 36, 47, 48, 59, 60, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83" 

#"0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 23, 24, 35, 36, 47, 48, 59, 60, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83" 
#"0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 19, 20, 21, 22, 23, 24, 35, 36, 47, 48, 59, 60, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83"
# "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 23, 24, 35, 36, 47, 48, 59, 60, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83"
#TODO: change num_uni to 100 once you actually run with full flux systematics
