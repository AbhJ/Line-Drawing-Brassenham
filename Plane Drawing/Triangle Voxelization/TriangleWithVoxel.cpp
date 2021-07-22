#include "bits/stdc++.h"
using namespace std;

float area(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3) {
	return abs((x_1 * (y_2 - y_3) + x_2 * (y_3 - y_1) + x_3 * (y_1 - y_2)) / 2.0);
}

bool isInside(float x_1, float y_1, float x_2, float y_2, float x_3, float y_3, float x, float y) {
	// A FUNCTION TO CHECK WHETHER POINT P(x, y) LIES INSIDE THE TRIANGLE FORMED BY A(x_1, y_1), B(x_2, y_2) AND C(x_3, y_3)

	// CALCULATE AREA OF TRIANGLE ABC
	float A = area(x_1, y_1, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PBC
	float A1 = area(x, y, x_2, y_2, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAC
	float A2 = area(x_1, y_1, x, y, x_3, y_3);

	// CALCULATE AREA OF TRIANGLE PAB
	float A3 = area(x_1, y_1, x_2, y_2, x, y);

	// CHECK IF SUM OF A1, A2 AND A3 IS SAME AS A
	return A == A1 + A2 + A3;
}

bool check(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x, int y) {
	vector<float>X1{(float)x_1 - 0.5, (float)x_1 + 0.5};
	vector<float>Y1{(float)y_1 - 0.5, (float)y_1 + 0.5};
	vector<float>X2{(float)x_2 - 0.5, (float)x_2 + 0.5};
	vector<float>Y2{(float)y_2 - 0.5, (float)y_2 + 0.5};
	vector<float>X3{(float)x_3 - 0.5, (float)x_3 + 0.5};
	vector<float>Y3{(float)y_3 - 0.5, (float)y_3 + 0.5};

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

tuple<float, float, float, float> equationOfPlane(
    float x_1, float y_1, float z_1,
    float x_2, float y_2, float z_2,
    float x_3, float y_3, float z_3) {

	float a1 = x_2 - x_1;
	float b1 = y_2 - y_1;
	float c1 = z_2 - z_1;
	float a2 = x_3 - x_1;
	float b2 = y_3 - y_1;
	float c2 = z_3 - z_1;

	float a = b1 * c2 - b2 * c1;
	float b = a2 * c1 - a1 * c2;
	float c = a1 * b2 - b1 * a2;
	float d = - a * x_1 - b * y_1 - c * z_1;

	// ax+by+cz+d IS THE EQUATION OF THE PLANE
	return make_tuple(a, b, c, d);
}

void setObj(vector<tuple<int, int, int>>& voxelsCenters) {
	string toBeSavedIntoObj;
	for (auto &[x, y, z] : voxelsCenters) {
		vector<tuple<int, int, int>> verticesOfVoxels{
			{0, 0, 0},
			{10, 0, 0},
			{10, 10, 0},
			{0, 10, 0},
			{0, 0, 10},
			{10, 0, 10},
			{10, 10, 10},
			{0, 10, 10}
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


void xIsUnknown(float a, float b, float c, float d) {
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
		float upperLimit = ((max(abs(a), abs(b), abs(c)) / 2.0) - (b * (float)y + c * (float)z + d)) / a;
		float lowerLimit = (- (max(abs(a), abs(b), abs(c)) / 2.0) - (b * (float)y + c * (float)z + d)) / a;
		assert(upperLimit - lowerLimit <= 1.0);
		x = (int)upperLimit;
	}

	// PLOTTING THE VOXELS TO CUBES SURROUNDING THEM

	setObj(voxelsCenters);
}




void yIsUnknown(float a, float b, float c, float d) {

}

void zIsUnknown(float a, float b, float c, float d) {

}

int main() {
	cin >> x_1 >> y_1 >> z_1 >> x_2 >> y_2 >> z_2 >> x_3 >> y_3 >> z_3;

	// INPUT THE 3D TRIANGLE
	// A(x_1, y_1, z_1), B(x_2, y_2, z_2), C(x_3, y_3, z_3)

	float a, b, c, d;
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