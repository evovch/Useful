#!/bin/bash

#DIR=/mnt/ForVirtualDrives/Grzegorz/He8
DIR=He8
OUTDIR=output/

rm -f out.txt err.txt table.txt

for FILE in $DIR/he8_*.root
do
	echo ${FILE}

	FILENAME=$(basename "$FILE")
	#echo ${FILENAME}

	OUTFILENAME=out_${FILENAME}

	#echo ${OUTFILENAME}

	#echo ${OUTDIR}${OUTFILENAME}

	root -l -b -q "drawTest.C(\"${FILE}\", \"${OUTDIR}${OUTFILENAME}\")" >> out.txt 2>> err.txt

	# Check that root finished successfully....
done

grep entries out.txt > table.txt