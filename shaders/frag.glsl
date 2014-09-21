#version 120

uniform sampler2D tex;
uniform int hasTex;

varying vec3 N;
varying vec3 v;

void main (void)  
{  
   // direction vector from surface to light
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);
   // eye?
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   // surface reflection
   vec3 R = normalize(-reflect(L,N));
   float dist = length(L);
   
   vec4 total = vec4(0, 0, 0, 0);
 
   //use the ambient value of light0 as the ambient light:  
   vec4 Iamb = gl_FrontLightProduct[0].ambient * gl_FrontMaterial.ambient;

   // The first light, a point light
   //calculate Diffuse Term:  
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
   Idiff = clamp(Idiff, 0.0, 1.0);
   
   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   Ispec = clamp(Ispec, 0.0, 1.0); 
   
   total = Iamb + Idiff + Ispec;
   // The second light, a spot light
   L = normalize(gl_LightSource[1].position.xyz - v);
   R = normalize(-reflect(L,N));
   float angle = max(dot( normalize(gl_LightSource[1].spotDirection), -L), 0);
   
   float attenuation = 1.0 * (gl_LightSource[1].constantAttenuation +
                    gl_LightSource[1].linearAttenuation * dist +
                    gl_LightSource[1].quadraticAttenuation * dist * dist);
					// float attenuation = 1.0;
    if (angle < cos(radians(gl_LightSource[1].spotCutoff))){
      attenuation = 0.0;
   } else {
      attenuation = attenuation * pow(angle, gl_LightSource[1].spotExponent );
	  // attenuation = 1.0;
   }
   // total += attenuation * gl_FrontLightProduct[1].diffuse * max(dot(N,L), 0.0);
   // total += attenuation * gl_FrontLightProduct[1].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
    // total = vec4(angle, angle, angle, 1.0);
   total += attenuation * gl_FrontMaterial.diffuse * gl_LightSource[1].diffuse * max(0.0, dot(N, L));
   total += attenuation * gl_FrontMaterial.specular * pow(max(dot(N,L),0.0),0.3*gl_FrontMaterial.shininess);
   // if (angle < cos(radians(gl_LightSource[1].spotCutoff))) total.y = 1;
   
   // The third light, a directional light
   
   // write Total Color:  
   if (hasTex == 1){ // if there is a texture, merge it 
      //gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * ( gl_FrontLightModelProduct.sceneColor + Iamb + texture2D(tex, gl_TexCoord[0].st) *Idiff + Ispec);
	  gl_FragColor = texture2D(tex, gl_TexCoord[0].st) * ( gl_FrontLightModelProduct.sceneColor + total);
   } else {
      gl_FragColor = gl_FrontLightModelProduct.sceneColor + total;
   }
   // gl_FragColor = total;
}