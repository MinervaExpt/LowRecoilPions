#!/bin/sh

#This code is to split the playlist.txt files into four files for quicker grid processing. 

round() {
    printf "%.${2:-0}f" "${x}"
}

# do some math, bc style
math() {
    echo "$*" | bc -l
}

file=$1
FileName=$(echo "${file%.txt}")
x=$(wc -l < "$1")
echo "File ${file} has ${x} lines"
y=4
nlines=$(expr $x / $y)
cp ${file} ${FileName}
if [ $((${x} % ${y})) -eq 0 ]; then

   echo "File will be divide with ${nlines} each"
   split -l ${nlines} -d  $FileName $FileName	

else
   modu=$(($x % $y))
   echo "This many files are left over $modu"
   newnlines=$(( $nlines+$modu ))
   echo "File will be divide with ${newnlines} each"
   split -l ${newnlines} -d  $FileName $FileName   
fi	
#mv ${FileName} ${file}
file1=00
file2=01
file3=02
file4=03
filename1="${FileName}${file1}"
filename2="${FileName}${file2}"
filename3="${FileName}${file3}"
filename4="${FileName}${file4}"
newname1="${FileName}_${file1}.txt"
newname2="${FileName}_${file2}.txt"
newname3="${FileName}_${file3}.txt"
newname4="${FileName}_${file4}.txt"
mv ${filename1} ${newname1} 
mv ${filename2} ${newname2}
mv ${filename3} ${newname3}
mv ${filename4} ${newname4}

#echo "File will be divide with ${nlines} each"
#split -l ${x} -d  $file $1


