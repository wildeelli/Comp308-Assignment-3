/*
 * materials.hpp
 *
 *  Created on: 15/09/2014
 *      Author: Elliot Wilde
 */

#include <gl/glut.h>

#ifndef MATERIALS_HPP_
#define MATERIALS_HPP_

GLfloat mat_zero[] = {0,0,0,1};

GLfloat mat_plastic_specular[] = { .7, .6, .6, 1. };
GLfloat mat_plastic_diffuse[] = { .5, .0, .0, 1.};
GLfloat mat_plastic_shininess[] = { 32. };

GLfloat mat_bronze_specular[] = { .393548, .271906, .166721, 1. };
GLfloat mat_bronze_diffuse[] = { .714, .4284, .18144, 1.};
GLfloat mat_bronze_shininess[] = { 25.6 };

GLfloat l0_direction[] = { 1.0f, 1.0f, 1.0f, 0.0f };
GLfloat l0_diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat l0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

GLfloat l1_direction[] = { -.70f, -1.0f, -.70f, 0.0f };
GLfloat l1_position[] = { 6.0f, 10.0f, 7.0f, 1.0f };
GLfloat l1_diffintensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat l1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat l1_specular[] = { 1.f, 1.f, 1.f, 1.0f };




#endif /* MATERIALS_HPP_ */
