#include "bits/stdc++.h"
using namespace std;

float area(float x1, float y1, float x2, float y2, float x3, float y3) {
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y) {
	// A FUNCTION TO CHECK WHETHER POINT P(x, y) LIES INSIDE THE TRIANGLE FORMED BY A(x1, y1), B(x2, y2) AND C(x3, y3)

	// CALCULATE AREA OF TRIANGLE ABC
	float A = area(x1, y1, x2, y2, x3, y3);

	// CALCULATE AREA OF TRIANGLE PBC
	float A1 = area(x, y, x2, y2, x3, y3);

	// CALCULATE AREA OF TRIANGLE PAC
	float A2 = area(x1, y1, x, y, x3, y3);

	// CALCULATE AREA OF TRIANGLE PAB
	float A3 = area(x1, y1, x2, y2, x, y);

	// CHECK IF SUM OF A1, A2 AND A3 IS SAME AS A
	return A == A1 + A2 + A3;
}

bool check(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y) {
	vector<float>X1{(float)x1 - 0.5, (float)x1 + 0.5};
	vector<float>Y1{(float)y1 - 0.5, (float)y1 + 0.5};
	vector<float>X2{(float)x2 - 0.5, (float)x2 + 0.5};
	vector<float>Y2{(float)y2 - 0.5, (float)y2 + 0.5};
	vector<float>X3{(float)x3 - 0.5, (float)x3 + 0.5};
	vector<float>Y3{(float)y3 - 0.5, (float)y3 + 0.5};

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
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3) {

	float a1 = x2 - x1;
	float b1 = y2 - y1;
	float c1 = z2 - z1;
	float a2 = x3 - x1;
	float b2 = y3 - y1;
	float c2 = z3 - z1;

	float a = b1 * c2 - b2 * c1;
	float b = a2 * c1 - a1 * c2;
	float c = a1 * b2 - b1 * a2;
	float d = - a * x1 - b * y1 - c * z1;

	// ax+by+cz+d IS THE EQUATION OF THE PLANE
	return make_tuple(a, b, c, d);
}

void xIsUnknown(float a, float b, float c, float d) {
	// EQUATION OF PLANE OF YZ PLANE IS b j' + c k' + d = 0
	// THE TRIANGLE PROJECTED ON YZ PLANE HAS VERTICES
	// (y1, z1), (y2, z2), (y3, z3)

	int lowY = min({y1, y2, y3}), highY = max({y1, y2, y3});
	int lowZ = min({z1, z2, z3}), highZ = max({z1, z2, z3});

	vector<pair<int, int>>pixelsCenters;

	for (int y = lowY; y <= highY; y++) {
		for (int z = lowZ; z <= highZ; z++) {
			if (check(y1, z1, y2, z2, y3, z3, y, z)) {
				// THIS CREATES A PIXEL AT (0, y, z)
				// SINCE THERE IS A BIJECTION WE HAVE ONE PIXEL PER VOXEL HERE
				// ALSO, SINCE PROJECTION ON YZ PLANE X IS 0

				pixelsCenters.emplace_back(y, z);
			}
		}
	}

	vector<tuple<int, int, int>>voxelsCenters;

	for (auto &[y, z] : pixelsCenters) {
		// FINDING UPPER AND LOWER LIMITS ON X
		float upperLimit = ((max(abs(a), abs(b), abs(c)) / 2.0) - (b * (float)y + c * (float)z + d)) / a;
		float lowerLimit = (- (max(abs(a), abs(b), abs(c)) / 2.0) - (b * (float)y + c * (float)z + d)) / a;
		for (int x = lowerLimit + 1; x <= upperLimit; x++) {
			voxelsCenters.emplace_back(x, y, z);
		}
	}

}

void yIsUnknown(float a, float b, float c, float d) {

}

void zIsUnknown(float a, float b, float c, float d) {

}

int main() {
	int x1, y1, z1, x2, y2, z2, x3, y3, z3;
	cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;

	// INPUT THE 3D TRIANGLE
	// A(x1, y1, z1), B(x2, y2, z2), C(x3, y3, z3)

	float a, b, c, d;
	tie(a, b, c, d) = equationOfPlane(x1, y1, z1, x2, y2, z2, x3, y3, z3);

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