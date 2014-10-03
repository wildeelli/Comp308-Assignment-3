/*
 * materials.hpp
 *
 *  Created on: 15/09/2014
 *      Author: Elliot Wilde
 */

#include <GL/freeglut.h>

#ifndef MATERIALS_HPP_
#define MATERIALS_HPP_

// not sure why i'm putting this here, oh well
#define PI 3.141592653589

GLfloat mat_zero[] = {0,0,0,1};

GLfloat mat_red_plastic_ambient[] = {0,0,0,1};
GLfloat mat_red_plastic_specular[] = { .7, .6, .6, 1. };
GLfloat mat_red_plastic_diffuse[] = { .5, .0, .0, 1.};
GLfloat mat_red_plastic_shininess[] = { 32. };

GLfloat mat_white_plastic_ambient[] = {0,0,0,1};
GLfloat mat_white_plastic_specular[] = { .7, .7, .7, 1. };
GLfloat mat_white_plastic_diffuse[] = { .85, .85, .85, 1.};
GLfloat mat_white_plastic_shininess[] = { 32. };


// bronze with brass specular highlights?
GLfloat mat_bronze_ambient[] = {.2125 ,.1275 , .054 ,1};
GLfloat mat_bronze_diffuse[] = { .714, .4284, .18144, 1.};
//GLfloat mat_bronze_specular[] = { .393548, .271906, .166721, 1. };
GLfloat mat_bronze_shininess[] = { 25.6 };

//GLfloat mat_bronze_ambient[] = {0.329412, 0.223529, 0.027451, 1 };
//GLfloat mat_bronze_diffuse[] = { 0.780392, 0.568627, 0.113725, 1 };
GLfloat mat_bronze_specular[] = {	0.992157, 0.941176, 0.807843, 1 };
//GLfloat mat_bronze_shininess[] = { 27.8974 };

//GLfloat mat_chrome_ambient[] = {.25 ,.25 , .25 ,1};
//GLfloat mat_chrome_diffuse[] = { .4, .4, .45, 1.};
//GLfloat mat_chrome_specular[] = { .774597, .774597, .794597, 1. };
//GLfloat mat_chrome_shininess[] = { 76.8 };

// pewter?
GLfloat mat_chrome_ambient[] = {0.105882, 0.058824, 0.113725, 1 };
GLfloat mat_chrome_diffuse[] = {0.427451, 0.470588, 0.541176, 1 };
GLfloat mat_chrome_specular[] = {0.333333, 0.333333, 0.521569, 1 };
GLfloat mat_chrome_shininess[] = { 19.84615 };

GLfloat mat_psilver_ambient[] = {0.23125, 0.23125, 0.23125, 1 };
GLfloat mat_psilver_diffuse[] = {0.2775, 0.2775, 0.2775, 1 };
GLfloat mat_psilver_specular[] = {0.773911, 0.773911, 0.773911, 1 };
GLfloat mat_psilver_shininess[] = { 89.6 };

// this is actually ruby, oh well
GLfloat mat_wood_ambient[] = { .1745, .01175, .01175, 1 };
GLfloat mat_wood_diffuse[] = { .61424, .04136, .04136, 1 };
GLfloat mat_wood_specular[] = { .727811, .626959, .626959, 1};
GLfloat mat_wood_shininess[] = { 76.8 };

GLfloat l0_position[] = { 4.0f, 7.0f, 5.0f, 1.0f };
GLfloat l0_diffintensity[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat l0_specular[] = { .5f, .5f, .5f, 1.0f };
//GLfloat l0_diffintensity[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat l0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

GLfloat l1_direction[] = { -16.0f, -10.0f, -17.0f, 0.0f };
GLfloat l1_position[] = { 16.0f, 10.0f, 17.0f, 1.0f };
GLfloat l1_diffintensity[] = { 0.85f, 0.85f, 0.85f, 1.0f };
GLfloat l1_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat l1_specular[] = { 1.f, 1.f, 1.f, 1.0f };

GLfloat l2_direction[] = { 0.0f, 1.0f, -1.0f, 0.0f };
GLfloat l2_diffintensity[] = { 0.35f, 0.35f, 0.35f, 1.0f };
GLfloat l2_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat l2_specular[] = { 1.f, 1.f, 1.f, 1.0f };


//
#endif /* MATERIALS_HPP_ */
