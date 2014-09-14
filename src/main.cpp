/*
 * main.cpp
 *
 *  Created on: 14/09/2014
 *      Author: Elliot Wilde
 */

#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include "define.h"
#include "G308_Geometry.h"

GLuint g_mainWnd;
GLuint g_nWinWidth = G308_WIN_WIDTH;
GLuint g_nWinHeight = G308_WIN_HEIGHT;

void G308_Reshape(int w, int h);
void G308_display();
void G308_SetCamera();
void G308_SetLight();

void G308_LoadFiles();

G308_Geometry* table = NULL;
G308_Geometry* sphere = NULL;
G308_Geometry* cube = NULL;
G308_Geometry* teapot = NULL;
G308_Geometry* torus = NULL;
G308_Geometry* bunny = NULL;
G308_Geometry* envmap = NULL;
G308_Geometry* normap = NULL;


int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA );
	glutInitWindowSize(g_nWinWidth, g_nWinHeight);
	g_mainWnd = glutCreateWindow("COMP308 MidTerm Project");

	glutDisplayFunc(G308_display);
	glutReshapeFunc(G308_Reshape);
//	glutIdleFunc(idle);
//	glutMouseFunc(mouseFunc);

	G308_SetCamera();
	G308_SetLight();

	G308_LoadFiles();

	glutMainLoop();

	delete table;
	delete cube;

	return 0;
}

void G308_display(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("%s\n", gluErrorString(err));
	}

	G308_SetCamera();
	glColor3f(.8, .8, .2);

	glPushMatrix();
	// do arcball for camera control?



	if (table) {
		// wood with wood.jpg as texture map
		table->RenderGeometry();
	}
	if (sphere) {
		// bronze metal
		glPushMatrix();
		glTranslatef(-4, 2, 4);
		glColor3f(205./255., 127./255., 50./255.);
		sphere->RenderGeometry();
		glPopMatrix();
	}
	if (cube) {
		// brick block with brick.jpg as the texture map
		glPushMatrix();
		glScalef(1.2, 1.2, 1.2);
		glTranslatef(4.5, 2, -4.5);
		cube->RenderGeometry();
		glPopMatrix();
	}
	if (teapot) {
		// bluish metal (lead?)
		glPushMatrix();
		glTranslatef(-4, 2, -4);
		glColor3f(.8, .8, .91);
		teapot->RenderGeometry();
		glPopMatrix();
	}
	if (torus) {
		// red plastic
		glPushMatrix();
		glTranslatef(5, 2, 6);
		glColor3f(.9, .05, .01);
		torus->RenderGeometry();
		glPopMatrix();
	}
	if (bunny) {
		// white bone china
		glPushMatrix();
		glTranslatef(1, 2, 2);
		glColor3f(.9, .9, .95);
		bunny->RenderGeometry();
		glPopMatrix();
	}

	glPopMatrix();

	// and ends here
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

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
}

void G308_LoadFiles(){


	table = new G308_Geometry;
	table->ReadOBJ("assets/Table.obj");
	table->ReadTexture("assets/wood.jpg");
	table->CreateGLPolyGeometry();

	sphere = new G308_Geometry;
	sphere->ReadOBJ("assets/Sphere.obj");
	sphere->CreateGLPolyGeometry();

	cube = new G308_Geometry;
	cube->ReadOBJ("assets/Box.obj");
	cube->ReadTexture("assets/brick.jpg");
	cube->CreateGLPolyGeometry();

	teapot = new G308_Geometry;
	teapot->ReadOBJ("assets/Teapot.obj");
	teapot->CreateGLPolyGeometry();

	torus = new G308_Geometry;
	torus->ReadOBJ("assets/Torus.obj");
	torus->CreateGLPolyGeometry();

	bunny = new G308_Geometry;
	bunny->ReadOBJ("assets/Bunny.obj");
	bunny->CreateGLPolyGeometry();
}

void G308_SetLight(){
	float direction[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHT0);
}

