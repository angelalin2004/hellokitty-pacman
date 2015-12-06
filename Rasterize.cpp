#include "Rasterize.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>

#include "Vector3.h"
#include "Vector4.h"
#include "Cube.h"

#define deleteVector(__type__, __vect__) do {\
                                   std::vector<__type__>::iterator iter; \
                                   std::vector<__type__>::iterator end; \
                                   iter = __vect__->begin();\
                                   end = __vect__->end();\
                                   while(iter != end) delete (*(iter++));\
                                   delete __vect__;\
                               } while(false)

int width = 512, height = 512;
float* pixels = new float[width * height * 3];
float* zbuffer = new float[width * height];

float colors[126];

bool printed;
bool printed2;

Vector4 a, b, c;
float colorR, colorG, colorB;
Vector3 c0, c1, c2, c33, c4, c5, c6, c7;
int ax, ay, bx, by, cx, cy, minx, maxx, miny, maxy;
Vector3 va, vb, vc, v;
float Aa, Ab, Ac, A;
float alpha, beta, gamma, z;
Vector3 a3, b3, c3, colorP;
int draw_type;

Rasterize::Rasterize()
{
	printed = false;
	printed2 = false;

	c0 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0 );
	c1 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c2 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c33 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c4 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c5 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c6 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);
	c7 = Vector3((rand() % 100) / 100.0, (rand() % 100) / 100.0, (rand() % 100) / 100.0);

	for (int i = 0; i < (sizeof colors / sizeof colors[0]); i++) {
		colors[i] = (rand() % 100) / 100.0;
	}

}

void Rasterize::resizePixelBuffer(int w, int h) {
	pixels = new float[w * h * 3];
	width = w;
	height = h;
}

void Rasterize::draw(int draw_item, Camera camera, House house) {

	draw_type = draw_item;
	clearBuffers();
	if (level == 1)
		rasterizeVertex(draw_item, camera, house);
	else
		rasterizeTriangle(draw_item, camera, house);
	glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);
	glFlush();

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();
}

// Draw a point into the frame buffer
void Rasterize::drawPoint(int x, int y, float r, float g, float b)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return;
	int offset = y*width * 3 + x * 3;
	if (offset + 2 >= width*height * 3)
		return;
	pixels[offset] = r;
	pixels[offset + 1] = g;
	pixels[offset + 2] = b;
}

void Rasterize::rasterizeVertex(int draw_item, Camera camera, House house) {
	Vector4 p;
	int x;
	int y;
	// Put your main rasterization loop here
	// It should go over the data model and call rasterizeTriangle for every triangle in it
	int counter = 0;
	if (draw_item == 0) {
		float halfSize = ((Cube *)(toDraw))->halfSize;
		float one, two, three;
		for (int i = 0; i < 8; i++) {
			one = two = three = 1.0;
			if (i % 2 != 0)
				three = -1.0;
			if (i == 2 || i == 3 || i == 6 || i == 7)
				two = -1.0;
			if (i > 3)
				one = -1.0;
			p = Vector4(halfSize*one, halfSize*two, halfSize*three);
			p = P * camera.getInverseMatrix() * toDraw->toWorld * p;
			p = p.dehomogenize();
			p = D * p;
			x = (int)(p[0]);
			y = (int)(p[1]);
			if ((x >= 0 && x <= width) && (y >= 0 && y <= height)) {
				drawPoint(x, y, 1.0, 1.0, 1.0);
			}
		}		
	}
	if (draw_item == 1) {
		for (int i = 0; i < (sizeof House::vertices / sizeof House::vertices[0])/3; i++) {
			p = Vector4(House::vertices[i*3], House::vertices[i*3+1], House::vertices[i*3+2]);
			p = P * camera.getInverseMatrix() * toDraw->toWorld * p;
			p = p.dehomogenize();
			p = D * p;
			x = (int)(p[0]);
			y = (int)(p[1]);
			if ((x >= 0 && x <= width) && (y >= 0 && y <= height)) {
				drawPoint(x, y, 1.0, 1.0, 1.0);
			}
		}
	}
	if (draw_item == 2) {
		for (int i = 0; i < ((OBJObject *)(toDraw))->vertices->size(); i++) {
			p = (*(((OBJObject *)(toDraw))->vertices->at(i))).toVector4(1.0);
			p = P * camera.getInverseMatrix() * toDraw->toWorld * p;
			p = p.dehomogenize();
			p = D * p;
			x = (int)(p[0]);
			y = (int)(p[1]);
			if ((x >= 0 && x <= width) && (y >= 0 && y <= height)) {
				drawPoint(x, y, 1.0, 1.0, 1.0);
			}
		}
	}
	
	// example: draw diagonal line:
	/*
	for (int i = 0; i<min(width, height); ++i)
		drawPoint(i, i, 1.0, 0.0, 0.0);
	*/
}

// Clear frame buffer
void Rasterize::clearBuffers()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	for (int i = 0; i<width*height; ++i)
	{
		pixels[i * 3] = clearColor[0];
		pixels[i * 3 + 1] = clearColor[1];
		pixels[i * 3 + 2] = clearColor[2];
		zbuffer[i] = 1.0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Rasterize::rasterizeTriangle(int draw_item, Camera camera, House house)
{
	// Add code to rasterize a triangle
	if (draw_item == 0) {
		float halfSize = ((Cube *)(toDraw))->size / 2.0;
		//p = Vector4(halfSize*one, halfSize*two, halfSize*three);

		// Draw front face:
		a = Vector4(halfSize, halfSize, halfSize);
		b = Vector4(halfSize, halfSize, (-1.0)*halfSize);
		c = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		a3 = c0;
		b3 = c1;
		c3 = c2;
		barycentric(camera, 0);
		a = Vector4(halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, halfSize, (-1.0)*halfSize);
		c = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		a3 = c3;
		b3 = c1;
		c3 = c2;
		barycentric(camera, 1);

		// Draw left side:
		a = Vector4(halfSize, halfSize, halfSize);
		b = Vector4((-1.0)*halfSize, halfSize, halfSize);
		c = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		a3 = c0;
		b3 = c4;
		c3 = c2;
		barycentric(camera, 2);
		a = Vector4((-1.0)*halfSize, (-1.0)*halfSize, halfSize);
		b = Vector4((-1.0)*halfSize, halfSize, halfSize);
		c = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		a3 = c6;
		b3 = c4;
		c3 = c2;
		barycentric(camera, 3);

		// Draw right side:
		a = Vector4(halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, halfSize, (-1.0)*halfSize);
		c = Vector4((-1.0)*halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		a3 = c3;
		b3 = c1;
		c3 = c7;
		barycentric(camera, 4);
		a = Vector4((-1.0)*halfSize, halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, halfSize, (-1.0)*halfSize);
		c = Vector4((-1.0)*halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		a3 = c5;
		b3 = c1;
		c3 = c7;
		barycentric(camera, 5);

		// Draw back face:
		a = Vector4((-1.0)*halfSize, halfSize, (-1.0)*halfSize);
		b = Vector4((-1.0)*halfSize, (-1.0)*halfSize, halfSize);
		c = Vector4((-1.0)*halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		a3 = c5;
		b3 = c6;
		c3 = c7;
		barycentric(camera, 6);
		a = Vector4((-1.0)*halfSize, halfSize, (-1.0)*halfSize);
		b = Vector4((-1.0)*halfSize, (-1.0)*halfSize, halfSize);
		c = Vector4((-1.0)*halfSize, halfSize, halfSize);
		a3 = c5;
		b3 = c6;
		c3 = c4;
		barycentric(camera, 7);

		// Draw top side:
		a = Vector4((-1.0)*halfSize, halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, halfSize, halfSize);
		c = Vector4((-1.0)*halfSize, halfSize, halfSize);
		a3 = c5;
		b3 = c0;
		c3 = c4;
		barycentric(camera, 8);
		a = Vector4((-1.0)*halfSize, halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, halfSize, halfSize);
		c = Vector4(halfSize, halfSize, (-1.0)*halfSize);
		a3 = c5;
		b3 = c0;
		c3 = c1;
		barycentric(camera, 9);

		// Draw bottom side:
		a = Vector4((-1.0)*halfSize, (-1.0)*halfSize, halfSize);
		b = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		c = Vector4((-1.0)*halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		a3 = c6;
		b3 = c2;
		c3 = c7;
		barycentric(camera, 10);
		a = Vector4(halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		b = Vector4(halfSize, (-1.0)*halfSize, halfSize);
		c = Vector4((-1.0)*halfSize, (-1.0)*halfSize, (-1.0)*halfSize);
		a3 = c3;
		b3 = c2;
		c3 = c7;
		barycentric(camera, 11);

	}
	if (draw_item == 1) {
		for (int i = 0; i < (sizeof House::indices / sizeof House::indices[0])/3; i++) {
			a = Vector4(House::vertices[House::indices[i*3] * 3], House::vertices[House::indices[i*3] * 3 + 1], House::vertices[House::indices[i*3] * 3 + 2]);
			b = Vector4(House::vertices[House::indices[i * 3+1] * 3], House::vertices[House::indices[i * 3 +1] * 3 + 1], House::vertices[House::indices[i * 3+1] * 3 + 2]);
			c = Vector4(House::vertices[House::indices[i * 3+2] * 3], House::vertices[House::indices[i * 3 +2] * 3 + 1], House::vertices[House::indices[i * 3 +2] * 3 + 2]);
			a3 = Vector3(colors[House::indices[i * 3] * 3], colors[House::indices[i * 3] * 3 + 1], colors[House::indices[i * 3] * 3 + 2]);
			b3 = Vector3(colors[House::indices[i * 3 + 1] * 3], colors[House::indices[i * 3 + 1] * 3 + 1], colors[House::indices[i * 3 + 1] * 3 + 2]);
			c3 = Vector3(colors[House::indices[i * 3 + 2] * 3], colors[House::indices[i * 3 + 2] * 3 + 1], colors[House::indices[i * 3 + 2] * 3 + 2]);
			barycentric(camera, i);
		}
	}
	if (draw_item == 2) {
		for (int i = 0; i < ((OBJObject *)(toDraw))->faces->size(); i++) {

			int n = ((OBJObject *)(toDraw))->faces->at(i)->vertexIndices[0];
			a = (*(((OBJObject *)(toDraw))->vertices->at(n))).toVector4(1.0);
			n = ((OBJObject *)(toDraw))->faces->at(i)->vertexIndices[1];
			b = (*(((OBJObject *)(toDraw))->vertices->at(n))).toVector4(1.0);
			n = ((OBJObject *)(toDraw))->faces->at(i)->vertexIndices[2];
			c = (*(((OBJObject *)(toDraw))->vertices->at(n))).toVector4(1.0);

			barycentric(camera, i);
		}
	}
}

/* returns true if there's nothing else to do, false if we have to do per pixel coloring */
bool Rasterize::barycentric(Camera camera, int i) {
	srand(i + 2);
	colorR = (rand() % 100) / 100.0;
	colorG = (rand() % 100) / 100.0;
	colorB = (rand() % 100) / 100.0;

	a = P * camera.getInverseMatrix() * toDraw->toWorld * a;
	a = a.dehomogenize();
	a = D * a;
	ax = (int)(a[0]);
	ay = (int)(a[1]);
	b = P * camera.getInverseMatrix() * toDraw->toWorld * b;
	b = b.dehomogenize();
	b = D * b;
	bx = (int)(b[0]);
	by = (int)(b[1]);
	c = P * camera.getInverseMatrix() * toDraw->toWorld * c;
	c = c.dehomogenize();
	c = D * c;
	cx = (int)(c[0]);
	cy = (int)(c[1]);
	minx = min(min(ax, bx), cx)-1;
	if (minx < 0) minx = 0;
	miny = min(min(ay, by), cy)-1;
	if (miny < 0) miny = 0;
	
		
	maxx = max(max(ax, bx), cx)+1;
	if (maxx > width) maxx = width-1;
	maxy = max(max(ay, by), cy)+1;
	if (maxy > height) maxy = height-1;

	// draw bounding boxes
	if (!(minx >= width || miny >= height)) {
		if (debug && (level == 2 || level == 3)) {
			for (int j = minx; j < maxx; j++) {
				if (miny - 1 >= 0)
					drawPoint(j, miny - 1, 0.937, 0.506, 0.608);
				if (maxy + 1 < height)
					drawPoint(j, maxy + 1, 0.937, 0.506, 0.608);
			}

			for (int k = miny; k < maxy; k++) {
				if (minx - 1 >= 0)
					drawPoint(minx - 1, k, 0.937, 0.506, 0.608);
				if (minx + 1 < width)
					drawPoint(maxx + 1, k, 0.937, 0.506, 0.608);
			}
		}
	}
	Vector3 p;
	for (int j = minx; j < maxx; j++) {
		for (int k = miny; k < maxy; k++) {
			p = Vector3((float)j, (float)k, 0.0);
			va = b.toVector3().subtract(p).cross(c.toVector3().subtract(p));
			vb = c.toVector3().subtract(p).cross(a.toVector3().subtract(p));
			vc = a.toVector3().subtract(p).cross(b.toVector3().subtract(p));
			v = c.toVector3().subtract(b.toVector3()).cross(a.toVector3().subtract(b.toVector3()));
			Aa = va.magnitude() / 2.0;
			Ab = vb.magnitude() / 2.0;
			Ac = vc.magnitude() / 2.0;
			A = v.magnitude() / 2.0;
			alpha = va.dot(v) / (va.magnitude()*v.magnitude()) * (Aa / A);
			beta = vb.dot(v) / (vb.magnitude()*v.magnitude()) * (Ab / A);
			gamma = vc.dot(v) / (vc.magnitude()*v.magnitude()) * (Ac / A);
			z = a[2] * alpha + b[2] * beta + c[2] * gamma;
			
			if ((alpha >= 0.0 && alpha <= 1.0) && (beta >= 0.0 && beta <= 1.0) && (gamma >= 0.0 && gamma <= 1.0)) {
				if (level == 2)
					drawPoint(j, k, colorR, colorG, colorB);
				else if (z <= zbuffer[j*width + k] && z > 0.0) {
					if (level == 3) {
						drawPoint(j, k, colorR, colorG, colorB);
					}
					else if (level == 4) {
						if (draw_type == 0 || draw_type == 1) {
							colorP = a3.multiply(alpha).add(b3.multiply(beta)).add(c3.multiply(gamma));
							drawPoint(j, k, colorP[0], colorP[1], colorP[2]);
						}
						if (draw_type == 2) {
							int n = ((OBJObject *)(toDraw))->faces->at(i)->normalIndices[0];
							a3 = (*(((OBJObject *)(toDraw))->normals->at(n))).normalize().multiply(1.0 / 2.0).add(Vector3(0.5, 0.5, 0.5));
							n = ((OBJObject *)(toDraw))->faces->at(i)->normalIndices[1];
							b3 = (*(((OBJObject *)(toDraw))->normals->at(n))).normalize().multiply(1.0 / 2.0).add(Vector3(0.5, 0.5, 0.5));
							n = ((OBJObject *)(toDraw))->faces->at(i)->normalIndices[2];
							c3 = (*(((OBJObject *)(toDraw))->normals->at(n))).normalize().multiply(1.0 / 2.0).add(Vector3(0.5, 0.5, 0.5));
							colorP = a3.multiply(alpha).add(b3.multiply(beta)).add(c3.multiply(gamma));
							drawPoint(j, k, colorP[0], colorP[1], colorP[2]);
						}
					}
					zbuffer[j*width + k] = z;
				}
			}
			else {
				
			}
			
		}
	}
	
	return true;
}

void Rasterize::resizeZBuffer(int w, int h) {
	zbuffer = new float[w*h];
	width = w;
	height = h;
}

/*
void Rasterize::perPixel(Vector3 a, Vector3 b, Vector3 c) {
	Vector3 colorP = a.multiply(alpha).add(b.multiply(beta)).add(c.multiply(gamma));
	colorP.multiply(0.5).add(Vector3(0.5, 0.5, 0.5));
	drawPoint(perPixelX, perPixelY, colorP[0], colorP[1], colorP[2]);
}
*/