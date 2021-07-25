Triangle voxelization:  

INPUT: A(x1, y1, z1), B(x2, y2, z2), C(x3, y3, z3).  
OUTPUT: Voxelized triangle (obj file)

Steps:
1) Find the equation of the plane of the input triangle ABC.
2) Project ABC on the three principal planes and identify the projected
triangle with the maximum area (Hint: compute the normals).
3) Let the traingle with the maximum area be A'B'C'.
4) There exists a bijection between the pixels of A'B'C' with the voxels of ABC.
5) Find all the pixels belonging to A'B'C'.
6) The coordinates of each pixel of A'B'C' represent the two out of the three
coordinates of each voxel of ABC.
7) Solve the equation -max{ |a|, |b|, |c| }/2 < ax+by+cz+d <= max{ |a|, |b|, |c| }/2
to find the third coordinate (Why? Already explained in the meeting).
8) Store and view the triangle in a.obj.

Commands after Running Code:
1) meshlab a.obj

Demonstration:  
  
![Triangle voxelization Demonstration](https://github.com/AbhJ/Line-Drawing-Brassenham/blob/master/Plane%20Drawing/Triangle%20Voxelization/demoTriangleVoxelisation.gif?raw=true)