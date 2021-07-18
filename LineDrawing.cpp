#include <bits/stdc++.h>
using namespace std;
FILE *fp;
int x, y, px, py, dx, dy, y_las, x_las, g;
void upd(int &zz) {
	if (dx < 0 and dy < 0)
		zz = zz + 1;
	else if (dx > 0 and dy > 0)
		zz = zz + 1;
	else
		zz = zz - 1;
}
void pr() {
	// the box
	fprintf(fp, "<rect x=\"%d\" y =\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255, 255, 0);stroke-width:%d;stroke:rgb(0,0,255)\" />\n",
	        // corner of cell
	        x * g, y * g,
	        // width
	        g,
	        // height
	        g,
	        // border width
	        max(g / 16, 2)
	       );

	// the circle
	fprintf(fp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"rgb(0,0,255)\" stroke-width=\"%d\" fill = \"blue\"/>\n",
	        // center
	        x * g + (g >> 1), y * g + (g >> 1),
	        // radius
	        max(g / 8, 4),
	        // border width
	        max(g / 16, 2)
	       );
}
int main() {
	int x1, x2, y1, y2;
	cout << "Enter start co-ordinates (spaced)" << ": ";
	cin >> x1 >> y1;
	cout << "Enter end co-ordinates (spaced)" << ": ";
	fp = fopen("ex.svg", "w");
	cin >> x2 >> y2;
	int l = max({0, -x1, -x2});
	int r = max({0, -y1, -y2});
	x1 += l, x2 += l;
	y1 += r, y2 += r;
	fprintf(fp, "<?xml version=\"1.0\" standalone=\"no\" ?>\n");
	fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n\n");
	dx = x2 - x1,
	dy = y2 - y1,
	x = x1,
	y = y1,
	px = 2 * abs(dy) - abs(dx);
	py = 2 * abs(dx) - abs(dy);
	g = min(100, 1600 / max({1, abs(dx) + 1, abs(dy) + 1}));
	string s = "<svg width=\"" + to_string(max(2 * g * abs(x2 - x1) + 4 * g * max(x1, x2))) + "px\" height=\"" + to_string(max(2 * g * abs(y2 - y1) + 4 * g * max(y1, y2))) + "px\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";
	fprintf(fp, s.c_str());
	if (abs(dx) < abs(dy)) {
		if (dy >= 0)
			x = x1, y = y1, y_las = y2;
		else
			x = x2, y = y2, y_las = y1;
		pr();
		while (y != y_las) {
			y = y + 1;
			if (py <= 0)
				py = py + 2 * abs(dx);
			else {
				upd(x);
				py = py + 2 * (abs(dx) - abs(dy));
			}
			pr();
		}
	}
	else {
		if (dx >= 0)
			x = x1, y = y1, x_las = x2;
		else
			x = x2, y = y2, x_las = x1;
		pr();
		while (x != x_las) {
			x = x + 1;
			if (px < 0)
				px = px + 2 * abs(dy);
			else {
				upd(y);
				px = px + 2 * (abs(dy) - abs(dx));
			}
			pr();
		}
	}
	fprintf(fp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(0,0,255);stroke-width:%d\" />\n", x1 * g + (g >> 1), y1 * g + (g >> 1), x2 * g + (g >> 1), y2 * g + (g >> 1), max(g / 16, 2));
	fprintf(fp, "</svg>\n");
	fclose(fp);
	return 0;
}