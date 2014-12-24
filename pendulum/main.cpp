//
//  main.cpp
//  pendulum
//	¼òÌåÖÐÎÄ
//  Created by Ran Bao on 9/22/13.
//  Copyright (c) 2013 R&B. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include <string>
#include "FPS.h"
#include "GLColor.h"
#include "TextBox.h"

#define M_PI 3.141592653f

using namespace std;

const float L1 = 1.0f;
const float L2 = 1.0f;
const float m1 = 0.5f;
const float m2 = 0.5f;
const float g = 10.0f;
const float h = 0.001f;

float theta1_0 = M_PI / 2.0f;
float theta2_0 = M_PI / 2.0f;
float omega1_0 = 0;
float omega2_0 = 0;

float theta1 = theta1_0;
float theta2 = theta2_0;

float omega1 = omega1_0;
float omega2 = omega2_0;

float x_coord_1;
float y_coord_1;

float x_coord_2;
float y_coord_2;

float x_coord_0 = 0;
float y_coord_0 = 0;

NewTextBox fps = NewTextBox(-(L1 + L2), (L1 + L2) - (L1 + L2) / 2 * 0.1f, GLColor::GL_WHITE_COLOR, GLUT_BITMAP_HELVETICA_12);
NewTextBox text_m1 = NewTextBox(-(L1 + L2), (L1 + L2) - (L1 + L2) / 2 * 0.2f, GLColor::GL_WHITE_COLOR, GLUT_BITMAP_HELVETICA_12);
NewTextBox text_m2 = NewTextBox(-(L1 + L2), (L1 + L2) - (L1 + L2) / 2 * 0.3f, GLColor::GL_WHITE_COLOR, GLUT_BITMAP_HELVETICA_12);


float g_theta1(float omega1){
	return omega1;
}

float g_theta2(float omega2){
	return omega2;
}

float g_omega1(float omega1, float omega2, float theta1, float theta2){
	;
	return (-g*(2 * m1 + m2)*sin(theta1) - m2*g*sin(theta1 - 2 * theta2) - 2 * sin(theta1 - theta2)*m2*(omega2*omega2*L2 + omega1*omega1*L1*cos(theta1 - theta2))) / (L1*(2 * m1 + m2 - m2*cos(2 * theta1 - 2 * theta2)));
}

float g_omega2(float omega1, float omega2, float theta1, float theta2){
	;
	return (2 * sin(theta1 - theta2)*(omega1*omega1*L1*(m1 + m2) + g*(m1 + m2)*cos(theta1) + omega2*omega2*L2*m2*cos(theta1 - theta2))) / (L1*(2 * m1 + m2 - m2*cos(2 * theta1 - 2 * theta2)));
}

void simulate_pendulum(void){
	theta1 = theta1 + h * g_theta1(omega1);
	theta2 = theta2 + h * g_theta2(omega2);
	omega1 = omega1 + h * g_omega1(omega1, omega2, theta1, theta2);
	omega2 = omega2 + h * g_omega2(omega1, omega2, theta1, theta2);

	/* convert theta to actual coordinates */
	x_coord_1 = x_coord_0 + L1 * sin(theta1);
	y_coord_1 = y_coord_0 - L1 * cos(theta1);

	x_coord_2 = x_coord_1 + L2 * sin(theta2);
	y_coord_2 = y_coord_1 - L2 * cos(theta2);
	//printf("x1 = %f, y1 = %f\n", x_coord_1, y_coord_1);
}


void draw(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);

	/*draw rods*/
	glBegin(GL_LINES);
	glColor3fv(GLColor::GL_WHITE_COLOR);
	glVertex2f(x_coord_0, y_coord_0);
	glVertex2f(x_coord_1, y_coord_1);
	glVertex2f(x_coord_1, y_coord_1);
	glVertex2f(x_coord_2, y_coord_2);
	glEnd();

	/*draw mass*/
	glPointSize(10);
	glBegin(GL_POINTS);

	//point0
	glColor3fv(GLColor::GL_BLUE_COLOR);
	glVertex2f(x_coord_0, y_coord_0);

	//point1
	glColor3fv(GLColor::GL_RED_COLOR);
	glVertex2f(x_coord_1, y_coord_1);

	//point2
	glColor3fv(GLColor::GL_GREEN_COLOR);
	glVertex2f(x_coord_2, y_coord_2);
	glEnd();

	glFlush();

	/*draw fps*/
	fps.update("FPS = " + to_string(get_fps_2()));
	fps.draw();

	/*diag info*/
	string p_m1, p_m2;
	p_m1 = "M1 = (" + to_string(x_coord_1) + ", " + to_string(y_coord_1) + ": " + to_string(theta1) + ")";
	p_m2 = "M2 = (" + to_string(x_coord_2) + ", " + to_string(y_coord_2) + ": " + to_string(theta2) + ")";

	text_m1.update(p_m1);
	text_m2.update(p_m2);

	text_m1.draw();
	text_m2.draw();

	glutSwapBuffers();
}


void Idle(void){
	simulate_pendulum();
	draw();
}

void init(void)
{
	/* clearing background */
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(L1 + L2), (L1 + L2), -(L1 + L2), (L1 + L2), -1.0, 1.0);
	glLineWidth(1.0f);//default line width =1
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Double Pendulum");
	init();
	glutDisplayFunc(simulate_pendulum);
	glutIdleFunc(Idle);
	glutMainLoop();

	return 0;
}



