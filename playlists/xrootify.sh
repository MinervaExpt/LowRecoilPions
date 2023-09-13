#!/bin/sh
# This is a cousin to samweb2xrootd. It takes a path that you could “ls” in
# /pnfs and puts it in a format that ROOT or ART can understand. Can accept
# multiple arguments.
while true
do
echo -n `readlink -f $1` | sed -e 's%/pnfs%root://fndcadoor.fnal.gov:1094//pnfs/fnal.gov/usr%'    #root://fndca1.fnal.gov:1094//pnfs/fnal.gov/usr%' 
echo -e ' '
shift
if [ x$1 == x ]; then break; fi
done
echo
