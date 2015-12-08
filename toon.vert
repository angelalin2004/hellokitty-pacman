#version 120
// This preprocessor call should only fail if your OpenGL version
// is older than 3.0, which was released in August 2008.

// "varying" provides an interface between the vertex and fragment shaders.
// Anything that is "varying" is shared between the two.
uniform vec3 lightDir;
varying float intensity;

void main()
{
	intensity = dot(lightDir,gl_Normal);
	gl_Position = ftransform();
}