#ifndef CSE167_Flag_h
#define CSE167_Flag_h

#include "Shader.h"
#include "Drawable.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Texture.h"


class Flag : public Drawable
{

public:

	Texture * texture;
	Shader * shader;

	//Vector3 * p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9, *p10, *p11, *p12, *p13, *p14, *p15;
	Vector3 p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15;
	float t, t2, B0, B1, B2, B3;
	Vector3 a, b, c, d;
	Vector3 q0, q1, q2, q3;
	Vector3 r0, r1, r2, r3;
	Vector3 u0, u1, v0;
	Vector3 tanu, tanv, normal;
	Vector3 arr [101][101];
	Vector3 normals[101][101];
	Vector3 top[100];
	Vector3 bottom[100];
	Vector3 left[100];
	Vector3 right[100];
	Vector3 horiz4[4][101];
	Vector3 vert4[4][101];
	float N;
	float delta;
	int debug;

	Flag();
	virtual ~Flag(void);

	virtual void draw(DrawData&);
	virtual void update(UpdateData&);

	Vector3 Bez(float, Vector3, Vector3, Vector3, Vector3);
	void setABCD(Vector3, Vector3, Vector3, Vector3);
};

#endif