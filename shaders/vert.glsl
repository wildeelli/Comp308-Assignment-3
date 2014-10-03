#version 120

varying vec3 t;
varying vec3 b;

varying vec3 N;
varying vec3 v;
void main(void)  
{     
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   t = normalize(gl_NormalMatrix * gl_MultiTexCoord1.xyz);
   b = normalize(cross(N, t));
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   gl_TexCoord[0] = gl_MultiTexCoord0;  
}