IP - interaction point - the origin (0,0,0) of the CS used in these measurements
Mirror_pos_on_beam_axis - point on the beam axis (x=0, y=0) with z=3500 used to define mirror Z position
Beam_axis - just a line along Z with some arbitrary length
Mirror_equator point - point on the horizontal plane of the mirror spherical surface; defined w.r.t the Mirror_pos_on_beam_axis; a little bit shifted towards IP along Z because it is not the exact point to be at 3500
Line.2 - horizontal point from Mirror_equator point with the length of the mirror radius
Plane.1 - lateral plane (YZ) of the mirror spherical surface
Mirror_profile - an arc build basing on CAD drawings and corresponding to what is in CbmRoot, which defines the size of the mirror in vertical direction; in this model it is a little bit extended to go down below the axis line (ZX plane) to enable better view of the intersections; in real life the (small) gap between the mirrors will cut some mirror acceptance; the top edge is precise
Mirror_center_point - a the middle point of the Mirror_profile arc; measured position of this point (before extending) corresponds to what is in CbmRoot 
Mirror_surface_rot_axis - a verical line used to build the mirror surface
Mirror_surface - spherical surface obtained by rotating the Mirror_profile around the Mirror_surface_rot_axis by the angles from CAD and corresponding to what is in CbmRoot
Camera_center - a point defining the origin of the camera; coordinates taken from CbmRoot (Rich_cyl_camera_fix.gdml file from Ievgenii 21.10.2016 = v17a version)
Line.5 - vertical (along Y) line with the length of the camera height (741mm)
Camera_rotation_axis - an axis parallel to the X axis throught the Camera_center; used to build the camera surface
Rotate.1 - the result of the rotation of Line.5 around Camera_rotation_axis by 18deg; used to build the camera surface
Line.7 - a line from the Camera_center to the axis of the camera cylinde (radial line) with the length of the camera cylinder radius
Translate.1 - the axis of the camera cylindrical surface obtainde by translating the Rotate.1 line along the Line.7
Camera_surface_boundaries - actual camera surface
Extrapol.1 - Camera_surface_boundaries extented by 100mm along camera axis direction in one direction
Camera_surface - Extrapol.1 extented by 100mm along camera axis direction in the other direction; the last two operations alow easier search for intersecions for better view
Rotate.2 - the beam line rotated by 25deg around X; used to define the acceptance
25deg_cone - result of the rotation of Rotate.2 around the beam axis; used to define the acceptance
Acceptance_cone - scaling of the 25deg_cone along X axis by 1.5
Line.250 - a vertical line connecting Mirror_pos_on_beam_axis and Mirror_center_point points


Generated.1 - a set points generated using spherical coordinate system equations to define the grid within acceptance

startPhi = 0#
startTheta = 0#
endPhi = 90#
endTheta = 25#
nPhi = 10
nTheta = 10
stepPhi = (endPhi - startPhi) / nPhi
stepTheta = (endTheta - startTheta) / nTheta

For iPhi = 0 To nPhi
	For iTheta = 0 To nTheta
		curPhi = (startPhi + iPhi * stepPhi)
		curTheta = (startTheta + iTheta * stepTheta)
		' Horizontal scaling
		curX = lineLength * Sin(curTheta * PI / 180#) * Cos(curPhi * PI / 180#) * 1.5
		curY = lineLength * Sin(curTheta * PI / 180#) * Sin(curPhi * PI / 180#)
		curZ = lineLength * Cos(curTheta * PI / 180#)
	Next iTheta
Next iPhi

Generated.2 - a set of lines (tracks) from the IP to each point of the grid in the Generated.1 set

Generated.3 - a set of intersections between each line from Generated.2 and the mirror surface

Generated.4 - 3000mm long lines, perpendicular the the mirror surface, from each intersecion from Generated.3. Luckily they all end up in the mirror spherical surface center

Generated.5 - lines obtained by 180deg rotation of tracks (Generated.2) around corresponding normals (Generated.4)

Generated.6 - intersection points of the reflected tracks (Generated.5) with the extended camera surface

Generated.8 - intersecions of the reflected tracks (Generated.5) with the shielding box
