#include "bits/stdc++.h"
using namespace std;

double area(double x_1, double y_1, double x_2, double y_2, double x_3, double y_3) {
	return abs((x_1 * (y_2 - y_3) + x_2 * (y_3 - y_1) + x_3 * (y_1 - y_2)) / 2.0);
}

bool isInside(double x_1, double y_1, double x_2, double y_2, double x_3, double y_3, double x, double y) {
	// A FUNCTION TO CHECK WHETHER POINT P(x, y) LIES INSIDE THE TRIANGLE FORMED BY A(x_1, y_1), B(x_2, y_2) AND C(x_3, y_3)

	// CALCULATE AREA OF TRIANGLE ABC
	double A = area(x_1, y_1, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PBC
	double A1 = area(x, y, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAC
	double A2 = area(x_1, y_1, x, y, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAB
	double A3 = area(x_1, y_1, x_2, y_2, x, y);

	// CHECK IF SUM OF A1, A2 AND A3 IS SAME AS A
	return A == A1 + A2 + A3;
}

bool check(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x, int y) {
	vector<double>X1{(double)x_1 - 0.5, (double)x_1 + 0.5};
	vector<double>Y1{(double)y_1 - 0.5, (double)y_1 + 0.5};
	vector<double>X2{(double)x_2 - 0.5, (double)x_2 + 0.5};
	vector<double>Y2{(double)y_2 - 0.5, (double)y_2 + 0.5};
	vector<double>X3{(double)x_3 - 0.5, (double)x_3 + 0.5};
	vector<double>Y3{(double)y_3 - 0.5, (double)y_3 + 0.5};

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

tuple<double, double, double, double> equationOfPlane(
    double x_1, double y_1, double z_1,
    double x_2, double y_2, double z_2,
    double x_3, double y_3, double z_3) {

	double a1 = x_2 - x_1;
	double b1 = y_2 - y_1;
	double c1 = z_2 - z_1;
	double a2 = x_3 - x_1;
	double b2 = y_3 - y_1;
	double c2 = z_3 - z_1;

	double a = b1 * c2 - b2 * c1;
	double b = a2 * c1 - a1 * c2;
	double c = a1 * b2 - b1 * a2;
	double d = - a * x_1 - b * y_1 - c * z_1;

	// ax+by+cz+d IS THE EQUATION OF THE PLANE
	return make_tuple(a, b, c, d);
}

void setObj(vector<tuple<int, int, int>>& voxelsCenters) {
	string toBeSavedIntoObj;
	for (auto &[x, y, z] : voxelsCenters) {
		vector<tuple<int, int, int>> verticesOfVoxels{
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
		vector<tuple<int, int, int, int>>wallsOfVoxels{
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

int x_1, y_1, z_1, x_2, y_2, z_2, x_3, y_3, z_3;


void xIsUnknown(double a, double b, double c, double d) {
	// EQUATION OF PLANE OF YZ PLANE IS b j' + c k' + d = 0
	// THE TRIANGLE PROJECTED ON YZ PLANE HAS VERTICES
	// (y_1, z_1), (y_2, z_2), (y_3, z_3)

	int lowY = min({y_1, y_2, y_3}), highY = max({y_1, y_2, y_3});
	int lowZ = min({z_1, z_2, z_3}), highZ = max({z_1, z_2, z_3});

	vector<tuple<int, int, int>>voxelsCenters;

	for (int y = lowY; y <= highY; y++) {
		for (int z = lowZ; z <= highZ; z++) {
			if (check(y_1, z_1, y_2, z_2, y_3, z_3, y, z)) {
				// THIS CREATES A PIXEL AT (0, y, z)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON YZ PLANE X IS 0

				voxelsCenters.emplace_back(tuple<int, int, int>(0, y, z));
			}
		}

	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON X
		double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (b * (double)y + c * (double)z + d)) / a;
		double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (b * (double)y + c * (double)z + d)) / a;
		// assert(upperLimit - lowerLimit <= 1.0);
		x = (int)upperLimit;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
}




void yIsUnknown(double a, double b, double c, double d) {
	int lowX = min({x_1, x_2, x_3}), highX = max({x_1, x_2, x_3});
	int lowZ = min({z_1, z_2, z_3}), highZ = max({z_1, z_2, z_3});

	vector<tuple<int, int, int>>voxelsCenters;

	for (int x = lowX; x <= highX; x++) {
		for (int z = lowZ; z <= highZ; z++) {
			if (check(x_1, z_1, x_2, z_2, x_3, z_3, x, z)) {
				// THIS CREATES A PIXEL AT (x, 0, z)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON XZ PLANE Y IS 0

				voxelsCenters.emplace_back(tuple<int, int, int>(x, 0, z));
			}
		}

	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON Y
		double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (double)x + c * (double)z + d)) / b;
		double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (double)x + c * (double)z + d)) / b;
		// assert(upperLimit - lowerLimit <= 1.0);
		y = (int)upperLimit;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
}

void zIsUnknown(double a, double b, double c, double d) {
	int lowX = min({x_1, x_2, x_3}), highX = max({x_1, x_2, x_3});
	int lowY = min({y_1, y_2, y_3}), highY = max({y_1, y_2, y_3});

	vector<tuple<int, int, int>>voxelsCenters;

	for (int x = lowX; x <= highX; x++) {
		for (int y = lowY; y <= highY; y++) {
			if (check(x_1, y_1, x_2, y_2, x_3, y_3, x, y)) {
				// THIS CREATES A PIXEL AT (x, y, 0)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON XY PLANE Z IS 0

				voxelsCenters.emplace_back(tuple<int, int, int>(x, y, 0));
			}
		}
	}

	for (auto &[x, y, z] : voxelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON Z
		double upperLimit = ((max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (double)x + b * (double)y + d)) / c;
		double lowerLimit = (- (max({abs(a), abs(b), abs(c)}) / 2.0) - (a * (double)x + b * (double)y + d)) / c;
		// assert(upperLimit - lowerLimit <= 1.0);
		z = (int)upperLimit;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM
	setObj(voxelsCenters);
}

void printNumberOfVoxels() {
	string line;
	ifstream file("a.obj");
	int count = - 1;
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

int main() {
	cout << "Please enter the vertices of the triangle\nx1 y1 z1\nx2 y2 z2\nx3 y3 z3\n";
	cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2 >> x_3 >> y_3 >> z_3;

	cout << "Printing meshlab object to a.obj\n";
	// INPUT THE 3D TRIANGLE
	// A(x_1, y_1, z_1), B(x_2, y_2, z_2), C(x_3, y_3, z_3)

	double a, b, c, d;
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
	printNumberOfVoxels();
	return 0;
}