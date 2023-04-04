#!/bin/bash

#From merged tuples make text playlists
#printenv
DATE="20230119"
PNFS_TOPDIR="/pnfs/minerva/persistent/users/sultana/"
ZUB_TOPDIR="/pnfs/minerva/persistent/users/zdar/"
BLUE_TOPDIR="/minerva/data/users/bmesserl/"
INV="v22r1p1"
echo $PNFS_TOPDIR
indir_format_str="${ZUB_TOPDIR}/Merged_%s_ana_%s_DualVertex_p3/%s"
outfile_format_str="$PNFS_TOPDIR/PlaylistFiles/%s_%s_plist.txt"
outfile_xrootd_format_str="$PNFS_TOPDIR/PlaylistFiles/%s_%s_xrootd_plist.txt"

#declare -a playlists=("ME1A" "ME1B" "ME1C" "ME1D" "ME1E" "ME1F" "ME1G" "ME1L" "ME1M" "ME1N" "ME1O" "ME1P")
declare -a playlists=("me1A") # "me1B" "me1M" "me1H")
declare -a datamc=("data" "mc")

for playlist in "${playlists[@]}"; do
  for dm in "${datamc[@]}"; do
      indir=$(printf "$indir_format_str" $dm $playlist)
      echo $indir
      outfile=$(printf "$outfile_format_str" $dm $playlist)
      outfile_xrootd=$(printf "$outfile_xrootd_format_str" $dm $playlist)
      echo "$outfile_xrootd"
      if [ ! -f $outfile ]; then
        find ${indir} -name "*.root" -printf "%p\n" | sort >  ${outfile}
      
      fi
      if [ ! -f $outfile_xrootd ]; then
        
        sed 's|^/pnfs/minerva/\(.*\)\.root|root://fndca1.fnal.gov:1094///pnfs/fnal.gov/usr/minerva/\1\.root|g' $outfile > ${outfile_xrootd}
         
      fi
  done # datamc loop
done # playlist loop

