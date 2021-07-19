#include "bits/stdc++.h"
using namespace std;

FILE *fp;
int x, y, px, py, dx, dy, y_las, x_las, g;

int get_int(float x) {
	float y = (int)x;
	if (x - y >= 0.5)return (int)y + 1;
	return (int)y;
}

void upd(int &val) {
	if (dx < 0 and dy < 0)
		val = val + 1;
	else if (dx > 0 and dy > 0)
		val = val + 1;
	else
		val = val - 1;
}


void printLine(int x1, int y1, int x2, int y2) {
	// THIS PRINTS THE LINE JOINING THE END POINTS

	fprintf(fp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" style=\"stroke:rgb(0,0,255);stroke-width:%d\" />\n",
	        // STARTING POINT
	        x1 * g + (g >> 1), y1 * g + (g >> 1),
	        // ENDING POINT
	        x2 * g + (g >> 1), y2 * g + (g >> 1),
	        // LINE WIDTH
	        max(g / 16, 2)
	       );
}

void pr() {
	// THIS TAKES INPUT THE POINTS AND PLOTS THEM ON THE SVG

	// THE BOX
	fprintf(fp, "<rect x=\"%d\" y =\"%d\" width=\"%d\" height=\"%d\" style=\"fill:rgb(255, 255, 0);stroke-width:%d;stroke:rgb(0,0,255)\" />\n",
	        // CORNER OF CELL
	        x * g, y * g,
	        // WIDTH
	        g,
	        // HEIGHT
	        g,
	        // BORDER WIDTH
	        max(g / 16, 2)
	       );

	// THE CIRCLE
	fprintf(fp, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"rgb(0,0,255)\" stroke-width=\"%d\" fill = \"blue\"/>\n",
	        // CENTER
	        x * g + (g >> 1), y * g + (g >> 1),
	        // RADIUS
	        max(g / 8, 4),
	        // BORDER WIDTH
	        max(g / 16, 2)
	       );

}

void createSVG() {
	fprintf(fp, "<?xml version=\"1.0\" standalone=\"no\" ?>\n");
	fprintf(fp, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n\n");
	fprintf(fp, ((string)"<svg width=\"100%\" height=\"100%\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n").c_str());
}

void closeSVG() {
	fprintf(fp, "</svg>\n");
	fclose(fp);
}

void printPoints(int x1, int y1, int x2, int y2) {
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
}

int main() {
	// INPUT AND SET PARAMETERS

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
	dx = x2 - x1, dy = y2 - y1;
	x = x1, y = y1;
	px = 2 * abs(dy) - abs(dx);
	py = 2 * abs(dx) - abs(dy);
	g = min(16, (1 << 30) / max({1, abs(dx) + 1, abs(dy) + 1}));
	if (dx > 1e3 or dy > 1e3)
		g = 16;

	// DRAW THE LINE WITH BOXED POINTS

	createSVG();
	printPoints(x1, y1, x2, y2);
	printLine(x1, y1, x2, y2);
	closeSVG();

	return 0;
}