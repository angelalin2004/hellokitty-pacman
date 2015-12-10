#version 120
// Once again, this preprocessor will only fail if your OpenGL version
// is horribly outdated.

// The fragment shader will get the values of these varying qualifiers
// from the vertex shader
varying float intensity;

// Another sample fragment shader is provided in the Spring 2015 midterm 2.
// You'd definitely want to take a look at that to know how to modify the
// colors based off of the positions, lighting, and normals. Plus you need
// to review for midterm 2 anyways, so why not start soon?

varying vec3 normal;

void main()
{
	float intensity;
	vec4 color;
	vec3 n = normalize(normal);
	intensity = dot(vec3(gl_LightSource[0].position),n);

	if (intensity > 0.95)
		color = vec4(1.0,0.5,1.0,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.6,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.4,1.0);
	else
		color = vec4(0.2,0.1,0.2,1.0);
	gl_FragColor = color;

}