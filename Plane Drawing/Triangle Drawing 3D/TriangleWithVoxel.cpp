#include "bits/stdc++.h"
using namespace std;

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

	return tuple<float, float, float, float>(a, b, c, d);
}

void xIsUnknown() {

}

void yIsUnknown() {

}

void zIsUnknown() {

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
		xIsUnknown();
	else if (b == max({a, b, c}))
		yIsUnknown();
	else if (c == max({a, b, c}))
		zIsUnknown();
	else
		cerr << "Error";
	return 0;
}