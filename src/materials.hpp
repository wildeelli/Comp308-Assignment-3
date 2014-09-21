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

GLfloat mat_red_plastic_ambient[] = {0,0,0,1};
GLfloat mat_red_plastic_specular[] = { .7, .6, .6, 1. };
GLfloat mat_red_plastic_diffuse[] = { .5, .0, .0, 1.};
GLfloat mat_red_plastic_shininess[] = { 32. };

GLfloat mat_white_plastic_ambient[] = {0,0,0,1};
GLfloat mat_white_plastic_specular[] = { .7, .7, .7, 1. };
GLfloat mat_white_plastic_diffuse[] = { .55, .55, .55, 1.};
GLfloat mat_white_plastic_shininess[] = { 32. };

GLfloat mat_bronze_ambient[] = {.2125 ,.1275 , .054 ,1};
GLfloat mat_bronze_specular[] = { .393548, .271906, .166721, 1. };
GLfloat mat_bronze_diffuse[] = { .714, .4284, .18144, 1.};
GLfloat mat_bronze_shininess[] = { 25.6 };

GLfloat mat_chrome_ambient[] = {.25 ,.25 , .25 ,1};
GLfloat mat_chrome_diffuse[] = { .4, .4, .4, 1.};
GLfloat mat_chrome_specular[] = { .774597, .774597, .774597, 1. };
GLfloat mat_chrome_shininess[] = { 76.8 };

// this is actually ruby, oh well
GLfloat mat_wood_ambient[] = { .1745, .01175, .01175, 1 };
GLfloat mat_wood_diffuse[] = { .61424, .04136, .04136, 1 };
GLfloat mat_wood_specular[] = { .727811, .626959, .626959, 1};
GLfloat mat_wood_shininess[] = { 76.8 };

GLfloat l0_direction[] = { 0.0f, 1.0f, 1.0f, 0.0f };
GLfloat l0_diffintensity[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat l0_specular[] = { 1.f, 1.f, 1.f, 1.0f };
//GLfloat l0_diffintensity[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat l0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

GLfloat l1_direction[] = { -.70f, -1.0f, -.70f, 0.0f };
GLfloat l1_position[] = { 6.0f, 10.0f, 7.0f, 1.0f };
GLfloat l1_diffintensity[] = { 0.85f, 0.85f, 0.85f, 1.0f };
GLfloat l1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat l1_specular[] = { 1.f, 1.f, 1.f, 1.0f };




#endif /* MATERIALS_HPP_ */
