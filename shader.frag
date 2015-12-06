#version 120
// Once again, this preprocessor will only fail if your OpenGL version
// is horribly outdated.

// The fragment shader will get the values of these varying qualifiers
// from the vertex shader
varying vec4 position;
varying vec3 normal;
uniform sampler2D textureSampler;
varying vec2 st;
varying vec4 textureColor;

// Another sample fragment shader is provided in the Spring 2015 midterm 2.
// You'd definitely want to take a look at that to know how to modify the
// colors based off of the positions, lighting, and normals. Plus you need
// to review for midterm 2 anyways, so why not start soon?
void main(){
	

	
   vec2 st = gl_TexCoord[0].st;
   vec4 textureColor = texture2D(textureSampler, st); 
   if (textureColor.x < 0.5) {
     gl_FragColor = vec4(textureColor.rgb, 0.0);
   }
   else {
	 gl_FragColor = vec4(normal,1.0);
   }
}

/*
	// Variables used for calculations declared here
   vec3 lightDirection;
   vec3 finalColor = vec3(0.0, 0.0, 0.9);

   // Loop through the 8 lights supported by OpenGL.
   // Now you know what the light IDs are used for!
   for (int i = 0; i < 8; i++){
      // Calculate the light direction
      lightDirection = gl_LightSource[i].position.xyz - position.xyz;

      // Determine if the light is in front of the surface.
      // If it is, I want to highlight it yellow
      if(dot(normal, lightDirection) >= 0.0)
         finalColor = vec3(0.9, 0.9, 0.0);
   }

   // Return the color for this fragment. It will be blue if no light
   // shines on it, and yellow if it receives light. GO TRITONS!
   //gl_FragColor = vec4(finalColor, 1.0);
*/