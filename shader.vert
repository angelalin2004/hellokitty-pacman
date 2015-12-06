#version 120
// This preprocessor call should only fail if your OpenGL version
// is older than 3.0, which was released in August 2008.

// "varying" provides an interface between the vertex and fragment shaders.
// Anything that is "varying" is shared between the two.
varying vec4 position;
varying vec3 normal;

// Oh look, this looks just like C
void main(){
	
   // This transforms the normal into world coordinates
   // gl_NormalMatrix is the transpose of the inverse of the modelViewMatrix
   // gl_Normal is whatever you last passed into glNormal*
   //normal = normalize(gl_NormalMatrix *gl_Normal);
   normal = normalize(gl_Normal);
   normal = vec3(normal.x/2.0+0.5, normal.y/2.0+0.5, normal.z/2.0+0.5);
   
   // This transforms the vertex into world coordinates
   // gl_ModelViewMatrix is whatever matrix that's on top of the GL_MODELVIEW stack
   position = gl_ModelViewMatrix * gl_Vertex;


   // Determine the projection of the vertex.
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   
   gl_TexCoord[0] = gl_MultiTexCoord0;

}