#!/bin/bash

OUTPUTFILE=table.txt

rm -fv $OUTPUTFILE

for TILE in 1_4 2_8
do

	for AXIS in x y xy
	do

		for (( j=0; j<2; j++ ))
		do

			for (( i=1; i<10; i++ ))
			do

				#echo -e "$TILE \t $AXIS \t $j.$i"

				N1=`head -n2 Tile_${TILE}/${AXIS}_${j}.${i}mrad/track_ring_distances_Track_Ring_${j}.${i}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`
				N2=`head -n3 Tile_${TILE}/${AXIS}_${j}.${i}mrad/track_ring_distances_Track_Ring_${j}.${i}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`
				N3=`head -n4 Tile_${TILE}/${AXIS}_${j}.${i}mrad/track_ring_distances_Track_Ring_${j}.${i}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`

				echo -e "$TILE \t $AXIS \t $j.$i \t $N1 \t $N2 \t $N3" >> $OUTPUTFILE 

			done

		done

		for (( j=1; j<6; j++ ))
		do

			#echo -e "$TILE \t $AXIS \t $j"

			N1=`head -n2 Tile_${TILE}/${AXIS}_${j}mrad/track_ring_distances_Track_Ring_${j}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`
			N2=`head -n3 Tile_${TILE}/${AXIS}_${j}mrad/track_ring_distances_Track_Ring_${j}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`
			N3=`head -n4 Tile_${TILE}/${AXIS}_${j}mrad/track_ring_distances_Track_Ring_${j}_${AXIS}_${TILE}.txt | tail -n1 | cut -d" " -f13`

			echo -e "$TILE \t $AXIS \t $j \t $N1 \t $N2 \t $N3" >> $OUTPUTFILE

		done

	done

done
