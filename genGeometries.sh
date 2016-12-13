#!/bin/bash

rm -fv debugInfo.txt

FOLDERNAME=GeoFiles2
ORIGFILE=Rich_jan2016_misalign.gdml

rm -rf FOLDERNAME
mkdir FOLDERNAME

for TILE in 1_4 2_8
do

	for AXIS in x y
	do

		for MIS in 1 2 3 4 5
		do

			NEWFILE=${ORIGFILE%.*}_tile_${TILE}_axis_${AXIS}_mis_${MIS}mrad.gdml
			echo "Generating ${NEWFILE}"

			PATTERN="name=\"misalign_${AXIS}_${TILE}\" value=\"0\""
			SUBST="name=\"misalign_${AXIS}_${TILE}\" value=\"$MIS\""
			#echo "$PATTERN  --->  $SUBST"

			#PATTERN2=`echo $PATTERN | sed 's/\"/\\\"/g'`
			#SUBST2=`echo $SUBST | sed 's/\"/\\\"/g'`
			#echo "$PATTERN2  --->  $SUBST2"

			# Doesn't work
			#sed 's/$PATTERN2/$SUBST2/g' ${ORIGFILE} > ${NEWFILE}
			#sed 's/"$PATTERN2"/"$SUBST2"/g' ${ORIGFILE} > ${NEWFILE}
			#sed 's/\"$PATTERN2\"/\"$SUBST2\"/g' ${ORIGFILE} > ${NEWFILE}

			sed -e s/"$PATTERN"/"$SUBST"/g ${ORIGFILE} > ${FOLDERNAME}/${NEWFILE}

			OUTPUTROOTFILE=${NEWFILE%.*}.root

			root -l -b -q "Import_GDML_Export_ROOT.C(\"${FOLDERNAME}/$NEWFILE\", \"${FOLDERNAME}/$OUTPUTROOTFILE\")" >> debugInfo.txt 2>> debugInfo.txt

		done

		for (( j=0; j<2; j++ ))
		do
			for (( i=1; i<10; i++ ))
			do
				MIS=$j.$i

				NEWFILE=${ORIGFILE%.*}_tile_${TILE}_axis_${AXIS}_mis_${MIS}mrad.gdml
				echo "Generating ${NEWFILE}"

				PATTERN="name=\"misalign_${AXIS}_${TILE}\" value=\"0\""
				SUBST="name=\"misalign_${AXIS}_${TILE}\" value=\"$MIS\""

				sed -e s/"$PATTERN"/"$SUBST"/g ${ORIGFILE} > ${FOLDERNAME}/${NEWFILE}

				OUTPUTROOTFILE=${NEWFILE%.*}.root

				root -l -b -q "Import_GDML_Export_ROOT.C(\"${FOLDERNAME}/$NEWFILE\", \"${FOLDERNAME}/$OUTPUTROOTFILE\")" >> debugInfo.txt 2>> debugInfo.txt
			done
		done

	done

	for MIS in 1 2 3 4 5
	do
		BUFFER=${ORIGFILE%.*}_tile_axis_mis_buffer.gdml

		PATTERN="name=\"misalign_x_${TILE}\" value=\"0\""
		SUBST="name=\"misalign_x_${TILE}\" value=\"$MIS\""
		#echo "$PATTERN  --->  $SUBST"

		sed -e s/"$PATTERN"/"$SUBST"/g ${ORIGFILE} > ${FOLDERNAME}/${BUFFER}

		NEWFILE=${ORIGFILE%.*}_tile_${TILE}_axis_xy_mis_${MIS}mrad.gdml
		echo "Generating ${NEWFILE}"

		PATTERN2="name=\"misalign_y_${TILE}\" value=\"0\""
		SUBST2="name=\"misalign_y_${TILE}\" value=\"$MIS\""
		#echo "$PATTERN  --->  $SUBST"

		sed -e s/"$PATTERN2"/"$SUBST2"/g ${FOLDERNAME}/${BUFFER} > ${FOLDERNAME}/${NEWFILE}

		OUTPUTROOTFILE=${NEWFILE%.*}.root

		root -l -b -q "Import_GDML_Export_ROOT.C(\"${FOLDERNAME}/$NEWFILE\", \"${FOLDERNAME}/$OUTPUTROOTFILE\")" >> debugInfo.txt 2>> debugInfo.txt

	done

	for (( j=0; j<2; j++ ))
	do
		for (( i=1; i<10; i++ ))
		do
			MIS=$j.$i

			BUFFER=${ORIGFILE%.*}_tile_axis_mis_buffer.gdml

			PATTERN="name=\"misalign_x_${TILE}\" value=\"0\""
			SUBST="name=\"misalign_x_${TILE}\" value=\"$MIS\""
			#echo "$PATTERN  --->  $SUBST"

			sed -e s/"$PATTERN"/"$SUBST"/g ${ORIGFILE} > ${FOLDERNAME}/${BUFFER}

			NEWFILE=${ORIGFILE%.*}_tile_${TILE}_axis_xy_mis_${MIS}mrad.gdml
			echo "Generating ${NEWFILE}"

			PATTERN2="name=\"misalign_${AXIS}_${TILE}\" value=\"0\""
			SUBST2="name=\"misalign_${AXIS}_${TILE}\" value=\"$MIS\""

			sed -e s/"$PATTERN2"/"$SUBST2"/g ${FOLDERNAME}/${BUFFER} > ${FOLDERNAME}/${NEWFILE}

			OUTPUTROOTFILE=${NEWFILE%.*}.root

			root -l -b -q "Import_GDML_Export_ROOT.C(\"${FOLDERNAME}/$NEWFILE\", \"${FOLDERNAME}/$OUTPUTROOTFILE\")" >> debugInfo.txt 2>> debugInfo.txt
		done
	done

done

grep "Number of illegal overlaps" debugInfo.txt

echo "DONE!"
