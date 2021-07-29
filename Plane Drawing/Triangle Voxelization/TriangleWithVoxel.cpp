#include "bits/stdc++.h"
using namespace std;

long double area(long double x_1, long double y_1, long double x_2, long double y_2, long double x_3, long double y_3) {
	return abs((x_1 * (y_2 - y_3) + x_2 * (y_3 - y_1) + x_3 * (y_1 - y_2)) / 2.0);
}

bool isInside(long double x_1, long double y_1, long double x_2, long double y_2, long double x_3, long double y_3, long double x, long double y) {
	// A FUNCTION TO CHECK WHETHER POINT P(x, y) LIES INSIDE THE TRIANGLE FORMED BY A(x_1, y_1), B(x_2, y_2) AND C(x_3, y_3)

	// CALCULATE AREA OF TRIANGLE ABC
	long double A = area(x_1, y_1, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PBC
	long double A1 = area(x, y, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAC
	long double A2 = area(x_1, y_1, x, y, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAB
	long double A3 = area(x_1, y_1, x_2, y_2, x, y);

	// CHECK IF SUM OF A1, A2 AND A3 IS SAME AS A
	return A == A1 + A2 + A3;
}

bool check(long long x_1, long long y_1, long long x_2, long long y_2, long long x_3, long long y_3, long long x, long long y) {
	vector<long double>X1{(long double)x_1 - 0.49999999, (long double)x_1 + 0.49999999};
	vector<long double>Y1{(long double)y_1 - 0.49999999, (long double)y_1 + 0.49999999};
	vector<long double>X2{(long double)x_2 - 0.49999999, (long double)x_2 + 0.49999999};
	vector<long double>Y2{(long double)y_2 - 0.49999999, (long double)y_2 + 0.49999999};
	vector<long double>X3{(long double)x_3 - 0.49999999, (long double)x_3 + 0.49999999};
	vector<long double>Y3{(long double)y_3 - 0.49999999, (long double)y_3 + 0.49999999};

	// IF THE PIXEL IS WITHIN 0.5 OF ANY BOUNDARY IT IS A PART OF THE PIXELIZATION
	for (auto x_1 : X1)
		for (auto x_2 : X2)
			for (auto x_3 : X3)
				for (auto y_1 : Y1)
					for (auto y_2 : Y2)
						for (auto y_3 : Y3)
							if (isInside(x_1, y_1, x_2, y_2, x_3, y_3, x, y))
								return 1;
	return 0;
}

tuple<long double, long double, long double, long double> equationOfPlane(
    long double x_1, long double y_1, long double z_1,
    long double x_2, long double y_2, long double z_2,
    long double x_3, long double y_3, long double z_3) {

	long double a1 = x_2 - x_1;
	long double b1 = y_2 - y_1;
	long double c1 = z_2 - z_1;
	long double a2 = x_3 - x_1;
	long double b2 = y_3 - y_1;
	long double c2 = z_3 - z_1;

	long double a = b1 * c2 - b2 * c1;
	long double b = a2 * c1 - a1 * c2;
	long double c = a1 * b2 - b1 * a2;
	long double d = - a * x_1 - b * y_1 - c * z_1;

	// ax+by+cz+d IS THE EQUATION OF THE PLANE
	return make_tuple(a, b, c, d);
}

void cleanse(vector<tuple<long long, long long, long long>>& voxelsCenters) {
	vector<tuple<long long, long long, long long>> cleanesdVoxelCenters;
	for (auto &[x, y, z] : voxelsCenters) {
		if (x == LLONG_MIN or y == LLONG_MIN or z == LLONG_MIN) {
			continue;
		}
		cleanesdVoxelCenters.emplace_back(x, y, z);
	}
	voxelsCenters = cleanesdVoxelCenters;
}

void setObj(vector<tuple<long long, long long, long long>>& voxelsCenters) {
	// REMOVE WRONG VOXEL CENTERS
	cleanse(voxelsCenters);
	string toBeSavedIntoObj;
	for (auto &[x, y, z] : voxelsCenters) {
		vector<tuple<long long, long long, long long>> verticesOfVoxels{
			{0, 0, 0},
			{1, 0, 0},
			{1, 1, 0},
			{0, 1, 0},
			{0, 0, 1},
			{1, 0, 1},
			{1, 1, 1},
			{0, 1, 1}
		};
		for (auto &[xx, yy, zz] : verticesOfVoxels) {
			xx += x, yy += y, zz += z;
		}

		// WALLS ARE CREATED BY ORDERING POINTS PROPERLY
		vector<tuple<long long, long long, long long, long long>>wallsOfVoxels{
			{1, 4, 3, 2},
			{1, 2, 6, 5},
			{5, 6, 7, 8},
			{4, 8, 7, 3},
			{1, 5, 8, 4},
			{2, 3, 7, 6}
		};

		// JUST CONSIDERING THE VERTEX INDICES IN THE ORDER -8 TO -1, INSTEAD OF 1 TO 8.
		for (auto &[a, b, c, d] : wallsOfVoxels) {
			a -= 9, b -= 9, c -= 9, d -= 9;
		}

		for (auto &[xx, yy, zz] : verticesOfVoxels) {
			toBeSavedIntoObj += "v " + to_string(xx) + " " + to_string(yy) + " " + to_string(zz) + "\n";
		}

		for (auto &[a, b, c, d] : wallsOfVoxels) {
			toBeSavedIntoObj += "f " + to_string(a) + " " + to_string(b) + " " + to_string(c) + " " + to_string(d) + "\n";
		}
	}
	ofstream ObjFileStream("a.obj");
	ObjFileStream << toBeSavedIntoObj;
	ObjFileStream.close();
}

void printNumberOfVoxels(vector<tuple<long long, long long, long long>>voxelsCenters) {
	cout << "Total number of voxels generated : " << (long long)voxelsCenters.size() << "\n";
}

long long x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3;

void xIsUnknown(long double a, long double b, long double c, long double d) {
	// EQUATION OF PLANE OF YZ PLANE IS b j' + c k' + d = 0
	// THE TRIANGLE PROJECTED ON YZ PLANE HAS VERTICES
	// (y_1, z_1), (y_2, z_2), (y_3, z_3)

	long long lowY = min({y_1, y_2, y_3}), highY = max({y_1, y_2, y_3});
	long long lowZ = min({z_1, z_2, z_3}), highZ = max({z_1, z_2, z_3});

	vector<tuple<long long, long long, long long>>voxelsCenters;

	for (long long y = lowY; y <= highY; y++) {
		for (long long z = lowZ; z <= highZ; z++) {
			if (check(y_1, z_1, y_2, z_2, y_3, z_3, y, z)) {
				// THIS CREATES A PIXEL AT (0, y, z)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON YZ PLANE X IS 0

				voxelsCenters.emplace_back(tuple<long long, long long, long long>(0, y, z));
			}
		}

	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON X
		long double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (b * (long double)y + c * (long double)z + d)) / a;
		long double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (b * (long double)y + c * (long double)z + d)) / a;
		if (upperLimit - lowerLimit <= 1.0)
			x = (long long)upperLimit;
		else
			x = LLONG_MIN;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
	printNumberOfVoxels(voxelsCenters);
}

void yIsUnknown(long double a, long double b, long double c, long double d) {
	long long lowX = min({x_1, x_2, x_3}), highX = max({x_1, x_2, x_3});
	long long lowZ = min({z_1, z_2, z_3}), highZ = max({z_1, z_2, z_3});

	vector<tuple<long long, long long, long long>>voxelsCenters;

	for (long long x = lowX; x <= highX; x++) {
		for (long long z = lowZ; z <= highZ; z++) {
			if (check(x_1, z_1, x_2, z_2, x_3, z_3, x, z)) {
				// THIS CREATES A PIXEL AT (x, 0, z)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON XZ PLANE Y IS 0

				voxelsCenters.emplace_back(tuple<long long, long long, long long>(x, 0, z));
			}
		}

	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON Y
		long double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (long double)x + c * (long double)z + d)) / b;
		long double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (long double)x + c * (long double)z + d)) / b;
		if (upperLimit - lowerLimit <= 1.0)
			y = (long long)upperLimit;
		else
			y = LLONG_MIN;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
	printNumberOfVoxels(voxelsCenters);
}

void zIsUnknown(long double a, long double b, long double c, long double d) {
	long long lowX = min({x_1, x_2, x_3}), highX = max({x_1, x_2, x_3});
	long long lowY = min({y_1, y_2, y_3}), highY = max({y_1, y_2, y_3});

	vector<tuple<long long, long long, long long>>voxelsCenters;

	for (long long x = lowX; x <= highX; x++) {
		for (long long y = lowY; y <= highY; y++) {
			if (check(x_1, y_1, x_2, y_2, x_3, y_3, x, y)) {
				// THIS CREATES A PIXEL AT (x, y, 0)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON XY PLANE Z IS 0

				voxelsCenters.emplace_back(tuple<long long, long long, long long>(x, y, 0));
			}
		}
	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON Z
		long double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (long double)x + b * (long double)y + d)) / c;
		long double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (long double)x + b * (long double)y + d)) / c;
		if (upperLimit - lowerLimit <= 1.0)
			z = (long long)upperLimit;
		else
			z = LLONG_MIN;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
	printNumberOfVoxels(voxelsCenters);
}

void printNumberOfVoxelsFromFileSize() {
	// THIS FUNCTION IS NOW NOT BEING USED
	// DUE TO SLOW EXECUTION TIME

	string line;
	ifstream file("a.obj");
	long long count = - 1;
	if (file.is_open()) {
		while (!file.eof()) {
			getline(file, line);
			count++;
		}
		file.close();
	}
	assert(count % 14 == 0);
	cout << "Total number of voxels generated : " << count / 14 << "\n";
}

long long dis(long long x, long long y, long long z, long long X, long long Y, long long Z) {
	return (x - X) * (x - X) + (y - Y) * (y - Y) + (z - Z) * (z - Z);
}

bool collinear() {
	vector<long long> distances_of_sub_segments_squared{
		dis(x_1, y_1, z_1, x_2, y_2, z_2),
		dis(x_2, y_2, z_2, x_3, y_3, z_3),
		dis(x_3, y_3, z_3, x_1, y_1, z_1),
	};
	sort(distances_of_sub_segments_squared.begin(), distances_of_sub_segments_squared.end());
	return sqrt((long double)distances_of_sub_segments_squared[2]) == sqrt((long double)distances_of_sub_segments_squared[0]) + sqrt((long double)distances_of_sub_segments_squared[1]);
}

int main() {
	cout << "Please enter the vertices of the triangle\nx1 y1 z1\nx2 y2 z2\nx3 y3 z3\n";
	cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2 >> x_3 >> y_3 >> z_3;

	// CHECK COLLINEARITY
	if (collinear()) {
		cout << "The input points are collinear\nNo triangle formation is possible\n";
		return 0;
	}

	cout << "Printing meshlab object to a.obj\n";
	// INPUT THE 3D TRIANGLE
	// A(x_1, y_1, z_1), B(x_2, y_2, z_2), C(x_3, y_3, z_3)

	long double a, b, c, d;
	tie(a, b, c, d) = equationOfPlane(x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3);

	// NORMAL VECTOR IS a i' + b j' + c k' + d = 0
	if (a == max({a, b, c}))
		xIsUnknown(a, b, c, d);
	else if (b == max({a, b, c}))
		yIsUnknown(a, b, c, d);
	else if (c == max({a, b, c}))
		zIsUnknown(a, b, c, d);
	else
		cerr << "Error";
	return 0;
}