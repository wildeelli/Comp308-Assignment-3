#version 120

//attribute vec4 gl_Color;

varying vec3 N;
varying vec3 v;
attribute vec2 LTexCoord;
varying vec2 texCoord;
void main(void)  
{     
   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   texCoord = LTexCoord;  
}