/*
 * main.cpp
 *
 *  Created on: 14/09/2014
 *      Author: Elliot Wilde
 */

//#define GL_ARB_debug_output
#include <stdio.h>
#include <stdlib.h>
#include <gl/glew.h>
#include <gl/glut.h>
#include <gl/freeglut.h>
#include <ctime>
#include "define.h"
#include "G308_Geometry.h"
#include "materials.hpp"
#include "loadShader.h"
#include <string>
#include "Log.hpp"

GLuint g_mainWnd;
GLuint g_nWinWidth = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

void G308_Reshape(int w, int h);
void G308_display();
void G308_SetCamera();
void G308_SetLight();
void G308_Idle();

void G308_LoadFiles();

struct gl_error{

};

G308_Geometry* table = NULL;
G308_Geometry* sphere = NULL;
G308_Geometry* cube = NULL;
G308_Geometry* teapot = NULL;
G308_Geometry* torus = NULL;
G308_Geometry* bunny = NULL;
//G308_Geometry* envmap = NULL;
//G308_Geometry* normap = NULL;

GLuint shaderID;

GLUquadric* q;

clock_t lastframe;

using namespace std;
using namespace gecom;

void G308_Idle(){
	//	glutPostRedisplay();
}

void APIENTRY callbackDebugGL(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar *message,
		void *userParam
){
	// enum documentation:
	// https://www.opengl.org/sdk/docs/man4/html/glDebugMessageControl.xhtml
	// message source within GL -> log source
	string log_source = "GL";
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		log_source = "GL:API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		log_source = "GL:Window";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		log_source = "GL:Shader";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		log_source = "GL:ThirdParty";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		log_source = "GL:App";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		log_source = "GL:Other";
		break;
	default:
		break;
	}
	// piecewise construct log message
	auto logs = log(log_source);
	bool exceptional = false;
	// message type -> log type
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		logs.error();
		logs << "Error";
		exceptional = true;
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		logs.warning();
		logs << "Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		logs.warning();
		logs << "Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		logs.warning();
		logs << "Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		logs.warning();
		logs << "Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		logs << "Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		logs << "Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		logs << "Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		logs << "Other";
		break;
	}
	// severity -> log verbosity
	switch (severity) {
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		logs % 3;
		break;
	case GL_DEBUG_SEVERITY_LOW:
		logs % 2;
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		logs % 1;
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		logs % 0;
		break;
	}
	// actual message. id = as returned by glGetError()
	ostringstream oss;
	oss << " [" << id << "] : " << message;
	logs << oss.str();
#ifndef GECOM_GL_NO_EXCEPTIONS
	if (exceptional) {
		throw gl_error(); //oss.str());
	}
#endif
}

int main(int argc, char** argv){
	clock_t start = clock();
	glutInit(&argc, argv);

	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitContextFlags(GLUT_DEBUG);
	glutInitWindowSize(g_nWinWidth, g_nWinHeight);
	g_mainWnd = glutCreateWindow("COMP308 Assignment 3");

	glutDisplayFunc(G308_display);
	glutReshapeFunc(G308_Reshape);
	glutIdleFunc(G308_Idle);
	//	glutMouseFunc(mouseFunc);
	//	glDebugMessageCallback

	glewInit();

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	if (glewIsExtensionSupported("GL_ARB_debug_output")){
		printf("GL Debug support found\n");
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallbackARB(callbackDebugGL, NULL);
	}

	cout << glGetString(GL_VERSION) << endl;

	G308_SetCamera();
	G308_SetLight();

	G308_LoadFiles();

	printf("\nFile loading took %.2f seconds.\n", 1.f/((float)(clock()-start)/CLOCKS_PER_SEC));

	glutMainLoop();

	delete table;
	delete sphere;
	delete cube;
	delete teapot;
	delete torus;
	delete bunny;

	return 0;
}

void G308_display(){
	char* title = new char[64];
	sprintf(title, "Comp308 Assignment 3 - fps: %.2f", 1./(float(clock() - lastframe )/CLOCKS_PER_SEC));
	lastframe = clock();
	glutSetWindowTitle(title);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("%s\n", gluErrorString(err));
		//		exit(0);
	}

	if (shaderID) {
		glUseProgram(shaderID);
		glUniform1i(glGetUniformLocation(shaderID, "tex"), 0);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 0);
		glUniform1i(glGetUniformLocation(shaderID, "norm"), 1);
		glUniform1i(glGetUniformLocation(shaderID, "hasNorm"), 0);
	}

//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 9001);

	G308_SetCamera();
	glColor3f(.8, .8, .2);

	glPushMatrix();
	// do arcball for camera control?

	//	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	if (table) {
		// wood with wood.jpg as texture map
		glPushMatrix();
		//		glRotatef(180, 1,0,0);
		//		glColor3f(1,1,1);
		//		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_wood_ambient);
		//		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_wood_specular);
		//		glMaterialfv(GL_FRONT, GL_SHININESS, mat_wood_shininess);
		//		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_wood_diffuse);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 1);
		table->RenderGeometry();
		glPopMatrix();
	}
	if (sphere) {
		// bronze metal
		glPushMatrix();
		glTranslatef(-4, 2, 4);
		glColor3f(205./255., 127./255., 50./255.);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_bronze_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_bronze_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_bronze_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_bronze_diffuse);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 0);
		sphere->RenderGeometry();
		glPopMatrix();
	}
	if (cube && true) {
		// brick block with brick.jpg as the texture map

		glPushMatrix();
		glScalef(1.2, 1.2, 1.2);
		glColor3f(1,1,1);

		glTranslatef(4.5, 2.5, -4.5);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 1);
		glUniform1i(glGetUniformLocation(shaderID, "hasNorm"), 1);
		cube->RenderGeometry();
		glPopMatrix();
		glUniform1i(glGetUniformLocation(shaderID, "hasNorm"), 0);
	}


	if (teapot) {
		// bluish metal (lead?)
		glPushMatrix();
		glTranslatef(-4, .5, -4);
		glColor3f(.8, .8, .91);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_psilver_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_psilver_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_psilver_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_psilver_diffuse);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 0);
		teapot->RenderGeometry();
		glPopMatrix();
	}
	if (torus) {
		// red plastic
		glPushMatrix();
		glTranslatef(5, 1, 6);
		glColor3f(.9, .05, .01);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_red_plastic_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_red_plastic_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_red_plastic_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_red_plastic_diffuse);
				glUniform1i(glGetUniformLocation(shaderID, "hasNorm"), 1);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 0);
		torus->RenderGeometry();
		glPopMatrix();
				glUniform1i(glGetUniformLocation(shaderID, "hasNorm"), 0);
	}
	if (bunny) {
		// white bone china
		glPushMatrix();
		glTranslatef(1, .5, 2);
		glColor3f(.9, .9, .95);
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_white_plastic_ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_white_plastic_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_white_plastic_shininess);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_white_plastic_diffuse);
		glUniform1i(glGetUniformLocation(shaderID, "hasTex"), 0);
		bunny->RenderGeometry();
		glPopMatrix();
	}

	glPushMatrix();

	glTranslatef(l0_position[0], l0_position[1], l0_position[2]);
	glColor3f(1, 1, 1);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, l0_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, l0_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, l0_ambient);

	gluSphere(q, .2, 256, 256);
	gluQuadricNormals(q, GLU_SMOOTH);
	gluQuadricOrientation(q, GLU_INSIDE);
	glShadeModel(GL_FLAT);

	glBegin( GL_TRIANGLES );
	glNormal3f(0,-1,0);
	glVertex3f(0,0,0);
	glVertex3f(0, 0.66,-0.33);
	glVertex3f(0, 0.66,0.33);
	glNormal3f(0,10,0);
	glVertex3f(0,0,0);
	glVertex3f(0,-0.66,-0.33);
	glVertex3f(0,-0.66,0.33);
	glEnd();
	glBegin( GL_LINES );
	glVertex3f(0,0,0);
	glVertex3f(0,-5,0);
	glEnd();


	glPopMatrix();

	glPopMatrix();

	// and ends here
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

void G308_Reshape(int w, int h){
	if (w>0) g_nWinWidth = w;
	if (h>0) g_nWinHeight = h;
	glViewport(0, 0, g_nWinWidth, g_nWinHeight);
	G308_SetCamera();
}

void G308_SetCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(G308_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, G308_ZNEAR_3D, G308_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(30.0, 20.0, 45.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//	gluLookAt(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void G308_LoadFiles(){

	shaderID = LoadShaders("shaders/vert.glsl", "shaders/frag.glsl");
	printf("\n");

	table = new G308_Geometry;
	table->ReadOBJ("assets/Table.obj");
	table->ReadTexture("assets/wood.jpg", TEXTURE, 3);
	table->CreateGLPolyGeometry(0);
	table->CreateGLWireGeometry();
	//	table->toggleMode();

	sphere = new G308_Geometry;
	sphere->ReadOBJ("assets/Sphere.obj");
	sphere->CreateGLPolyGeometry(0);

	cube = new G308_Geometry;
	cube->ReadOBJ("assets/Box.obj");
	cube->ReadTexture("assets/brick.jpg", TEXTURE, 3);
	cube->ReadTexture("assets/brick-normal.jpg", NORMAL, 3);
	cube->CreateGLPolyGeometry(shaderID);
	cube->CreateGLWireGeometry();
	//	cube->toggleMode();


	teapot = new G308_Geometry;
	teapot->ReadOBJ("assets/Teapot.obj");
	teapot->CreateGLPolyGeometry(0);

	torus = new G308_Geometry;
	torus->ReadOBJ("assets/Torus.obj");
		torus->ReadTexture("assets/normal.jpg", NORMAL, 1);
	torus->CreateGLPolyGeometry(shaderID);

	bunny = new G308_Geometry;
	bunny->ReadOBJ("assets/Bunny.obj");
	bunny->CreateGLPolyGeometry(0);

	q = gluNewQuadric();


}

void G308_SetLight(){
	//	float direction[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	//	float diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	//	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	// setup the global light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mat_zero);
	// setup the ambient light

	//	GLfloat l0_position[] = {  -6.0f, 10.0f, 7.0f, 1.0f  };

	glLightfv(GL_LIGHT0, GL_POSITION, l0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0_diffintensity);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l0_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_zero);

	glEnable(GL_LIGHT0);
	//	glDisable(GL_LIGHT0);

	// setup the spot light

	glLightfv(GL_LIGHT1, GL_POSITION, l1_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1_direction);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1_diffintensity);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1_ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, l1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 28);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25);

	glEnable(GL_LIGHT1);

	// setup the directional light

	//	glLightfv(GL_LIGHT2, gl)
}


