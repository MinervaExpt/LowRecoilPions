#!/bin/sh

#This code is to split the playlist.txt files into four files for quicker grid processing. 

while true
do
FileName=$2
x=$(wc -l < "$1")

split -l ${x} -d --additional-suffix=.txt  $FileName $1

done 

