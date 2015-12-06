#include "Vector3.h"

class FrustumG {

private:

	enum {
		TOP = 0, BOTTOM, LEFT,
		RIGHT, NEARP, FARP
	};

public:

	static enum { OUTSIDE, INTERSECT, INSIDE };
	Plane pl[6];
	Vector3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
	float nearD, farD, ratio, angle, tang;
	float nw, nh, fw, fh;

	FrustumG::FrustumG();
	FrustumG::~FrustumG();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(Vector3 &p, Vector3 &l, Vector3 &u);
	int pointInFrustum(Vector3 &p);
	int sphereInFrustum(Vector3 &p, float raio);
	int boxInFrustum(AABox &b);
};