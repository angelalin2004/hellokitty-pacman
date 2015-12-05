#include "Flag.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Window.h"
#include "math.h"

Flag::Flag() : Drawable()
{
	debug = 1;
	p0 = Vector3(-10.0, -10.0, -10.0);
	p1 = Vector3(-5.0, -10.0, -30.0);
	p2 = Vector3(5.0, -10.0, -10.0);
	p3 = Vector3(10.0, -10.0, -30.0);
	p4 = Vector3(-10.0, -5.0, -10.0);
	p5 = Vector3(-5.0, -5.0, -30.0);
	p6 = Vector3(5.0, -5.0, -10.0);
	p7 = Vector3(10.0, -5.0, -30.0);
	p8 = Vector3(-10.0, 5.0, -10.0);
	p9 = Vector3(-5.0, 5.0, -30.0);
	p10 = Vector3(5.0, 5.0, -10.0);
	p11 = Vector3(10.0, 5.0, -30.0);
	p12 = Vector3(-10.0, 10.0, -10.0);
	p13 = Vector3(-5.0, 10.0, -30.0);
	p14 = Vector3(5.0, 10.0, -10.0);
	p15 = Vector3(10.0, 10.0, -30.0);
	t = 0.5;
	a = p0.scale(-1.0) + p1.scale(3.0) - p2.scale(3.0) + p3;
	b = p0.scale(3.0) - p1.scale(6.0) + p2.scale(3.0);
	c = p0.scale(-3.0) + p1.scale(3.0);
	d = p0;
	N = 100.0;
	delta = 0.00001;
	texture = new Texture("../Ucsd-logo.ppm");
	shader = new Shader("../shader.vert", "../shader.frag", true);
}

Flag::~Flag()
{
	//Delete any dynamically allocated memory/objects here
}


void Flag::draw(DrawData& data)
{
	glDisable(GL_CULL_FACE);
	//Apply the material properties
	//From here forward anything drawn will be drawn with this material
	//material.apply();

	//Set the OpenGL Matrix mode to ModelView (used when drawing geometry)
	glMatrixMode(GL_MODELVIEW);

	//Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	//Make cube!
	//Note: The glBegin, and glEnd should always be as close to the glNormal/glVertex calls as possible
	//These are special calls that 'freeze' many internal states of OpenGL.
	//Once the glBegin state is active many of the calls made to OpenGL (like glMultMatrixf) will be IGNORED!
	//As a good habit, only call glBegin just before you need to draw, and call end just after you finish

	setABCD(p0, p1, p2, p3);
	for (int j = 0; j <= N; j++) {
		horiz4[0][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p4, p5, p6, p7);
	for (int j = 0; j <= N; j++) {
		horiz4[1][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p8, p9, p10, p11);
	for (int j = 0; j <= N; j++) {
		horiz4[2][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p12, p13, p14, p15);
	for (int j = 0; j <= N; j++) {
		horiz4[1][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}

	setABCD(p0, p4, p8, p12);
	for (int j = 0; j <= N; j++) {
		vert4[0][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p1, p5, p9, p13);
	for (int j = 0; j <= N; j++) {
		vert4[1][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p2, p6, p10, p14);
	for (int j = 0; j <= N; j++) {
		vert4[2][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}
	setABCD(p3, p7, p11, p15);
	for (int j = 0; j <= N; j++) {
		vert4[3][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;
	}

	/* calculate points */
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			setABCD(vert4[0][i], vert4[1][i], vert4[2][i], vert4[3][i]);
			arr[i][j] = a.scale(pow(j / N, 3.0)) + b.scale(pow(j / N, 2.0)) + c.scale(j / N) + d;				
		}
	}

	/* calculate normals */
	for (int j = 0; j <= N; j++) {
		t = j / N;

		q0 = Bez(t, p0, p1, p2, p3);
		q1 = Bez(t, p4, p5, p6, p7);
		q2 = Bez(t, p8, p9, p10, p11);
		q3 = Bez(t, p12, p13, p14, p15);
		if (j == N) {
			r0 = Bez(t + delta, p0, p1, p2, p3);
			r1 = Bez(t + delta, p4, p5, p6, p7);
			r2 = Bez(t + delta, p8, p9, p10, p11);
			r3 = Bez(t + delta, p12, p13, p14, p15);
		}
		else {
			r0 = Bez(t + delta, p0, p1, p2, p3);
			r1 = Bez(t + delta, p4, p5, p6, p7);
			r2 = Bez(t + delta, p8, p9, p10, p11);
			r3 = Bez(t + delta, p12, p13, p14, p15);
		}
		for (int k = 0; k <= N; k++) {
			t2 = k / N;

			if (k == N) {
				u0 = Bez(t2 - delta, q0, q1, q2, q3);
				u1 = Bez(t2, q0, q1, q2, q3);
				v0 = Bez(t2, r0, r1, r2, r3);
			}
			else {
				u0 = Bez(t2, q0, q1, q2, q3);
				u1 = Bez(t2 + delta, q0, q1, q2, q3);
				v0 = Bez(t2 + delta, r0, r1, r2, r3);
			}
			tanu = (u1 - u0).normalize();
			tanv = (v0 - u0).normalize();
			normal = tanu.cross(tanv);

			normals[k][j] = normal;
			
			
				//normal.print("normal");
				//Vector3 n = Vector3(normal[0] / 2.0 + 0.5, normal[1] / 2.0 + 0.5, normal[2] / 2.0 + 0.5);
				//n.print("normal");
			
		}
	}




	float default[4] = { 1.0, 1.0, 1.0, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)default);

	// now draw each quad
	
	texture->bind();
	shader->bind();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			
			glBegin(GL_QUADS);
			glNormal3f(normals[i][j][0], normals[i][j][1], normals[i][j][2]);
			glTexCoord2d(j / N, 1.0 - i / N), glVertex3f(arr[i][j][0], arr[i][j][1], arr[i][j][2]);
			glNormal3f(normals[i][j+1][0], normals[i][j+1][1], normals[i][j+1][2]);
			glTexCoord2d((j + 1) / N, 1.0 - i / N), glVertex3f(arr[i][j + 1][0], arr[i][j + 1][1], arr[i][j + 1][2]);
			glNormal3f(normals[i+1][j + 1][0], normals[i+1][j + 1][1], normals[i+1][j + 1][2]);
			glTexCoord2d((j + 1) / N, 1.0 - (i + 1) / N), glVertex3f(arr[i + 1][j + 1][0], arr[i + 1][j + 1][1], arr[i + 1][j + 1][2]);
			glNormal3f(normals[i+1][j][0], normals[i+1][j][1], normals[i+1][j][2]);
			glTexCoord2d(j / N, 1.0 - (i + 1) / N), glVertex3f(arr[i + 1][j][0], arr[i + 1][j][1], arr[i + 1][j][2]);
			glEnd();
		}
	}
	shader->unbind();
	texture->unbind();
	
	

	/*
	B0 = (-1.0)*pow(t, 3.0) + 3 * pow(t, 2.0) - 3 * t + 1;
	B1 = 3 * pow(t, 3.0) - 6 * pow(t, 2.0) + 3 * t;
	B2 = (-1.0) * 3 * pow(t, 3.0) + 3 * pow(t, 2.0);
	B3 = pow(t, 3.0);
	std::cout << B0 << " " << B1 << " " << B2 << " " << B3 << std::endl;
	q0 = Bez(t, p0, p1, p2, p3);
	q1 = Bez(t, p4, p5, p6, p7);
	q2 = Bez(t, p8, p9, p10, p11);
	q3 = Bez(t, p12, p13, p14, p15);
	
	Vector3 x = Bez(t, q0, q1, q2, q3);
	*/

	//Pop the save state off the matrix stack
	//This will undo the multiply we did earlier
	glPopMatrix();
	glEnable(GL_CULL_FACE);
}


void Flag::update(UpdateData& data)
{
	//
}

Vector3 Flag::Bez(float f, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) {
	// u should be 0.5
	B0 = (-1.0)*pow(f, 3.0) + 3.0 * pow(f, 2.0) - 3.0 * f + 1;
	B1 = 3.0 * pow(f, 3.0) - 6.0 * pow(f, 2.0) + 3.0 * f;
	B2 = (-1.0) * 3 * pow(f, 3.0) + 3.0 * pow(f, 2.0);
	B3 = pow(f, 3.0);
	return p0.scale(B0) + p1.scale(B1) + p2.scale(B2) + p3.scale(B3);
	/*
	
	
	
	*/
	
}

void Flag::setABCD(Vector3 v0, Vector3 v1, Vector3 v2, Vector3 v3) {
	a = v0.scale(-1.0) + v1.scale(3.0) - v2.scale(3.0) + v3;
	b = v0.scale(3.0) - v1.scale(6.0) + v2.scale(3.0);
	c = v0.scale(-3.0) + v1.scale(3.0);
	d = v0;
}