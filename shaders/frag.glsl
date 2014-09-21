#version 120

uniform sampler2D tex;
uniform int hasTex;

varying vec3 N;
varying vec3 v;

const float cos_outer_cone_angle = 0.8;

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
   // The second light, a dual cone spot light - courtesy of http://www.ozone3d.net/tutorials/glsl_lighting_phong_p3.php
   L = normalize(gl_LightSource[1].position.xyz - v);
   vec3 D = normalize(gl_LightSource[1].spotDirection);
   
   float cos_cur_angle = dot(-L, D);
   float cos_inner_cone_angle = gl_LightSource[1].spotCosCutoff;
   
   float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
   
   float spot = 0.0;
   spot = clamp((cos_cur_angle - cos_outer_cone_angle) / cos_inner_minus_outer_angle, 0.0, 1.0);
   
   float lambert = max(dot(N,L), 0.0);
   if (lambert > 0.0){
      total += gl_LightSource[1].diffuse * gl_FrontMaterial.diffuse * lambert * spot;
	  R = normalize(-reflect(L,N)); 
	  
	  float spec = pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess);
	  total += gl_LightSource[1].specular * gl_FrontMaterial.specular * spec * spot;
   }

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