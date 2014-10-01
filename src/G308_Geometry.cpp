//---------------------------------------------------------------------------
//
// Copyright (c) 2012 Taehyun Rhee
//
// This software is provided 'as-is' for assignment of COMP308
// in ECS, Victoria University of Wellington,
// without any express or implied warranty.
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "define.h"
#include "G308_Geometry.h"
#include "G308_ImageLoader.h"
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

G308_Geometry::G308_Geometry(void) {
	m_pVertexArray = NULL;
	m_pNormalArray = NULL;
	m_pUVArray = NULL;
	m_pTriangles = NULL;

	mode = G308_SHADE_POLYGON;

	m_nNumPoint = m_nNumUV = m_nNumPolygon = m_nNumNormal = 0;
	m_glGeomListPoly = m_glGeomListWire = 0;
	texture = NULL;
	normal = NULL;
}

G308_Geometry::~G308_Geometry(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	if (m_pUVArray != NULL)
		delete[] m_pUVArray;
	if (m_pTriangles != NULL)
		delete[] m_pTriangles;
	free(texture);
}

//-------------------------------------------------------
// Read in the OBJ (Note: fails quietly, so take care)
//--------------------------------------------------------
void G308_Geometry::ReadOBJ(const char *filename) {
	FILE* fp;
	char mode, vmode;
	char str[200];
	int v1, v2, v3, n1, n2, n3, t1, t2, t3;
	int numVert, numNorm, numUV, numFace;
	float x, y, z;
	float u, v;

	numVert = numNorm = numUV = numFace = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("Error reading %s file\n", filename);
	else
		printf("Reading %s file\n", filename);

	// Check number of vertex, normal, uvCoord, and Face
	while (fgets(str, 200, fp) != NULL) {
		sscanf(str, "%c%c", &mode, &vmode);
		switch (mode) {
		case 'v': /* vertex, uv, normal */
			if (vmode == 't') // uv coordinate
				numUV++;
			else if (vmode == 'n') // normal
				numNorm++;
			else if (vmode == ' ') // vertex
				numVert++;
			break;
		case 'f': /* faces */
			numFace++;
			break;
		}
	}

	m_nNumPoint = numVert;
	m_nNumUV = numUV;
	m_nNumPolygon = numFace;
	m_nNumNormal = numNorm;

	printf("Number of Point %d, UV %d, Normal %d, Face %d\n", numVert, numUV,
			numNorm, numFace);
	//-------------------------------------------------------------
	//	Allocate memory for array
	//-------------------------------------------------------------

	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	m_pVertexArray = new G308_Point[m_nNumPoint];

	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	m_pNormalArray = new G308_Normal[m_nNumNormal];

	if (m_pUVArray != NULL)
		delete[] m_pUVArray;
	m_pUVArray = new G308_UVcoord[m_nNumUV];

	if (m_pTriangles != NULL)
		delete[] m_pTriangles;
	m_pTriangles = new G308_Triangle[m_nNumPolygon];

	//-----------------------------------------------------------
	//	Read obj file
	//-----------------------------------------------------------
	numVert = numNorm = numUV = numFace = 0;

	fseek(fp, 0L, SEEK_SET);
	while (fgets(str, 200, fp) != NULL) {
		sscanf(str, "%c%c", &mode, &vmode);
		switch (mode) {
		case 'v': /* vertex, uv, normal */
			if (vmode == 't') // uv coordinate
					{
				sscanf(str, "vt %f %f", &u, &v);
				m_pUVArray[numUV].u = u;
				m_pUVArray[numUV].v = v;
				numUV++;
			} else if (vmode == 'n') // normal
					{
				sscanf(str, "vn %f %f %f", &x, &y, &z);
				m_pNormalArray[numNorm].x = x;
				m_pNormalArray[numNorm].y = y;
				m_pNormalArray[numNorm].z = z;
				numNorm++;
			} else if (vmode == ' ') // vertex
					{
				sscanf(str, "v %f %f %f", &x, &y, &z);
				m_pVertexArray[numVert].x = x;
				m_pVertexArray[numVert].y = y;
				m_pVertexArray[numVert].z = z;
				numVert++;
			}
			break;
		case 'f': /* faces : stored value is index - 1 since our index starts from 0, but obj starts from 1 */
			if (numNorm > 0 && numUV > 0) {
				sscanf(str, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
			} else if(numNorm > 0 && numUV ==0){
				sscanf(str, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
			} else if(numUV > 0 && numNorm==0){
				sscanf(str, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3);
			} else if(numUV==0 && numNorm==0){
				sscanf(str, "f %d %d %d", &v1, &v2, &v3);
			}
			// Vertex indicies for triangle:
			if (numVert != 0) {
				m_pTriangles[numFace].v1 = v1 - 1;
				m_pTriangles[numFace].v2 = v2 - 1;
				m_pTriangles[numFace].v3 = v3 - 1;
			}

			// Normal indicies for triangle
			if (numNorm != 0) {
				m_pTriangles[numFace].n1 = n1 - 1;
				m_pTriangles[numFace].n2 = n2 - 1;
				m_pTriangles[numFace].n3 = n3 - 1;
			}

			// UV indicies for triangle
			if (numUV != 0) {
				m_pTriangles[numFace].t1 = t1 - 1;
				m_pTriangles[numFace].t2 = t2 - 1;
				m_pTriangles[numFace].t3 = t3 - 1;
			}

			numFace++;
			break;
		default:
			break;
		}
	}

	fclose(fp);
	printf("Reading OBJ file is DONE.\n");

}

//--------------------------------------------------------------
// [Assignment4]
// Create a 2D GL texture from the file given
//--------------------------------------------------------------
void G308_Geometry::ReadTexture(const char* filename, int type, GLfloat scale) {
	//Your code here
	unsigned int i;
	for (i = 0; i < strlen(filename); i++) {
		if (filename[i] == '.') {
			break;
		}
	}
	char extension[5];
	strcpy(extension, &filename[i + 1]);
	//printf(extension);

	GLuint* tex = new GLuint;
	TextureInfo t;

	if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0)
		loadTextureFromJPEG(const_cast<char *>(filename), &t);
	else if (strcmp(extension, "png") == 0)
		loadTextureFromPNG(const_cast<char *>(filename), &t);
	else {
		printf("Invalid format. Only supports JPEG and PNG.\n");
		exit(1);
	}

	//Init the texture storage, and set some parameters.
	//(I high recommend reading up on these commands)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Only useful for PNG files, since JPEG doesn't support alpha
	if (t.hasAlpha) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t.width, t.height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, t.textureData);
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.width, t.height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, t.textureData);
	}
	//Once the texture has been loaded by GL, we don't need this anymore.
	free(t.textureData);
	if (type==TEXTURE){
		texture = tex;
	} else if (type==NORMAL){
		normal = tex;
	}
	textureScale = scale;
}

//--------------------------------------------------------------
// [Assignment4]
// Fill the following function to create display list
// of the obj file to show it as polygon, using texture and normal information (if any)
//--------------------------------------------------------------
void G308_Geometry::CreateGLPolyGeometry(GLuint shaderID) {
	if (m_glGeomListPoly != 0)
		glDeleteLists(m_glGeomListPoly, 1);

	// Assign a display list; return 0 if err
	m_glGeomListPoly = glGenLists(1);
	glNewList(m_glGeomListPoly, GL_COMPILE);

	GLint T = glGetUniformLocation(shaderID, "T");
	GLint B = glGetUniformLocation(shaderID, "B");


	//Your code here
	for (int i = 0; i < m_nNumPolygon; i++){
		G308_Triangle t = m_pTriangles[i];
			if (normal && shaderID){
				glm::vec3 v0 = (glm::vec3)(m_pVertexArray[t.v1].x);
				glm::vec3 v1 = (glm::vec3)(m_pVertexArray[t.v2].x);
				glm::vec3 v2 = (glm::vec3)(m_pVertexArray[t.v3].x);

				glm::vec2 uv0 = (glm::vec2)(m_pUVArray[t.t1].u);
				glm::vec2 uv1 = (glm::vec2)(m_pUVArray[t.t2].u);
				glm::vec2 uv2 = (glm::vec2)(m_pUVArray[t.t3].u);

				glm::vec3 deltaPos1 = v1-v0;
				glm::vec3 deltaPos2 = v2-v0;

				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.f/ (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

				glUniform3fv(T, 1, &tangent[0]);
				glUniform3fv(B, 1, &bitangent[0]);

			}
		glBegin( GL_TRIANGLES );
			glNormal3fv(&m_pNormalArray[t.n1].x);
//			if (texture) glTexCoord2fv(&m_pUVArray[t.t1].u);
			if (texture || normal) glTexCoord2f(m_pUVArray[t.t1].u*textureScale, m_pUVArray[t.t1].v*textureScale);
			glVertex3fv(&m_pVertexArray[t.v1].x);

			glNormal3fv(&m_pNormalArray[t.n2].x);
//			if (texture) glTexCoord2fv(&m_pUVArray[t.t2].u);
			if (texture || normal) glTexCoord2f(m_pUVArray[t.t2].u*textureScale, m_pUVArray[t.t2].v*textureScale);
			glVertex3fv(&m_pVertexArray[t.v2].x);

			glNormal3fv(&m_pNormalArray[t.n3].x);
//			if (texture) glTexCoord2fv(&m_pUVArray[t.t3].u);
			if (texture || normal) glTexCoord2f(m_pUVArray[t.t3].u*textureScale, m_pUVArray[t.t3].v*textureScale);
			glVertex3fv(&m_pVertexArray[t.v3].x);


		glEnd();
	}
//	glFlush();


	glEndList();
}

void G308_Geometry::CreateGLWireGeometry() {
	if (m_glGeomListWire != 0)
		glDeleteLists(m_glGeomListWire, 1);

	// Assign a display list; return 0 if err
	m_glGeomListWire = glGenLists(1);
	glNewList(m_glGeomListWire, GL_COMPILE);

	for (int i = 0; i < m_nNumPolygon; i++) {
		glBegin(GL_LINE_LOOP);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n1].x,
				m_pNormalArray[m_pTriangles[i].n1].y,
				m_pNormalArray[m_pTriangles[i].n1].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v1].x,
				m_pVertexArray[m_pTriangles[i].v1].y,
				m_pVertexArray[m_pTriangles[i].v1].z);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n2].x,
				m_pNormalArray[m_pTriangles[i].n2].y,
				m_pNormalArray[m_pTriangles[i].n2].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v2].x,
				m_pVertexArray[m_pTriangles[i].v2].y,
				m_pVertexArray[m_pTriangles[i].v2].z);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n3].x,
				m_pNormalArray[m_pTriangles[i].n3].y,
				m_pNormalArray[m_pTriangles[i].n3].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v3].x,
				m_pVertexArray[m_pTriangles[i].v3].y,
				m_pVertexArray[m_pTriangles[i].v3].z);
		glEnd();
	}

	glEndList();

}
void G308_Geometry::toggleMode() {
	if (mode == G308_SHADE_POLYGON) {
		mode = G308_SHADE_WIREFRAME;
	} else {
		mode = G308_SHADE_POLYGON;
	}
}

void G308_Geometry::RenderGeometry() {

	if (mode == G308_SHADE_POLYGON) {
		if (texture || normal){
			glDisable(GL_COLOR_MATERIAL);
			glEnable(GL_TEXTURE_2D);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		if (texture	) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, *texture);
		}
		if (normal){
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, *normal);
		}
		}
		glCallList(m_glGeomListPoly);
		if (texture || normal ){
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_COLOR_MATERIAL);
		}
	} else if (mode == G308_SHADE_WIREFRAME) {
		glCallList(m_glGeomListWire);
	} else {
		printf("Warning: Wrong Shading Mode. \n");
	}

}
