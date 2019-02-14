// Physics-engine.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <ctime>
#include <string>
#include <iostream>

#include "Quantum.h"
#include "GxWorld.h"
#include "GxParticle.h"
#include "MyGxUpdater.h"

using namespace std;
using namespace Quantum;


GxWorld gxWorld;
QmWorld pxWorld;

glm::vec3* mousePointer;

int scene = 0;
float gravity = -9.81;

// ********************** GLUT 
// Variables globales

int SCREEN_X = 800;
int SCREEN_Y = 800;
int VIEWPORT_X = 5; int VIEWPORT_Y = 5; int VIEWPORT_Z = 200;

GLuint DrawListSphere;

int buttons = 0;
float camRotate = 90.f;
float camDist = 30.f;
float camHeight = 0.f;
float mx = 0.f;
float my = 0.f;

// Variables du calcul de framerate 
int timeold = 0;
int frame = 0;
float accTime = 0.0;
bool paused = false;

GLfloat light_pos[] = { 0.0, 6.0, 1.0, 1.0 };

// ********************************************

glm::vec3 randomColor()
{
	float red = (rand() % 100) / 100.f;
	float green = (rand() % 100) / 100.f;
	float blue = (rand() % 100) / 100.f;
	return glm::vec3(red, green, blue);

}

glm::vec3 randomVector(float min, float max)
{
	float x = min + (max - min)*((rand() % 100) / 100.f);
	float y = min + (max - min)*((rand() % 100) / 100.f);
	float z = min + (max - min)*((rand() % 100) / 100.f);
	return glm::vec3(x, y, z);

}

QmParticle* createParticle()
{
	GxParticle* g = new GxParticle(randomColor(), 0.1f + 0.2f*((rand() % 100) / 100.f));
	QmParticle* p = new QmParticle(glm::vec3(0.0f, gravity, 0.0f), randomVector(-3, 3) + glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	p->gxUpdater = new MyGxUpdater(g);
	gxWorld.addParticle(g);
	pxWorld.addParticle(p);
	p->addForce(new Drag(p, 0.9f, 0.0f));

	return p;
}

QmParticle* createParticle2(glm::vec3 acc, glm::vec3 vel, glm::vec3 pos)
{
	GxParticle* g = new GxParticle(randomColor(), 0.2f);
	QmParticle* p = new QmParticle(acc, vel, pos);
	p->gxUpdater = new MyGxUpdater(g);
	gxWorld.addParticle(g);
	pxWorld.addParticle(p);
	p->addForce(new Drag(p, 0.3f, 0.1f));

	return p;
}


QmParticle* createParticle3(float charge)
{
	glm::vec3 color = glm::vec3((charge * 0.5f) + 0.5f, 0.0f, (-charge * 0.5f) + 0.5f);
	GxParticle* g = new GxParticle(color, 0.2f);
	//QmParticle* p = new QmParticle(glm::vec3(0.0f, gravity, 0.0f), randomVector(-3, 3) + glm::vec3(0.0f, 25.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	QmParticle* p = new QmParticle(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), randomVector(-5, 5), charge);
	p->gxUpdater = new MyGxUpdater(g);
	gxWorld.addParticle(g);
	pxWorld.addParticle(p);
	p->addForce(new Drag(p, 0.9f, 0.01f));

	for each (QmParticle* particle in pxWorld.particles)
	{
		p->addForce(new Magnetism(p, particle));
		particle->addForce(new Magnetism(particle, p));
	}

	return p;
}

void initScene1()
{
	printf("Scene 1: Random particles.\n");
	printf("Type space to pause.\n");
	//mousePointer = new glm::vec3(0, 4.5, 0);
	//for (int i = 0; i < 100; i++)
	//	createParticle();
}

void initScene2()
{
	printf("Scene 2: Spring string.\n");
	//printf("Empty.\n");

	float length = 2.0f;
	float strength = 20.0f;

	QmParticle* fixe = createParticle2(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));


	QmParticle* p1 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f));
	QmParticle* p2 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	QmParticle* p3 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 1.0f));
	p1->addForce(new Spring(p1, fixe, length, strength));
	p2->addForce(new Spring(p2, fixe, length, strength));
	p3->addForce(new Spring(p3, fixe, length, strength));

	p1->addForce(new Spring(p1, p2, length, strength));
	p2->addForce(new Spring(p2, p1, length, strength));
	p1->addForce(new Spring(p1, p3, length, strength));
	p3->addForce(new Spring(p3, p1, length, strength));
	p2->addForce(new Spring(p2, p3, length, strength));
	p3->addForce(new Spring(p3, p2, length, strength));


	QmParticle* p4 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -2.0f, 1.0f));
	p1->addForce(new Spring(p1, p4, length, strength));
	p4->addForce(new Spring(p4, p1, length, strength));
	p2->addForce(new Spring(p2, p4, length, strength));
	p4->addForce(new Spring(p4, p2, length, strength));
	p3->addForce(new Spring(p3, p4, length, strength));
	p4->addForce(new Spring(p4, p3, length, strength));


	QmParticle* p5 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -3.0f, 1.0f));
	p4->addForce(new Spring(p4, p5, length, strength*3.0f));
	p5->addForce(new Spring(p5, p4, length, strength*3.0f));


	QmParticle* p6 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, -4.0f, 0.0f));
	QmParticle* p7 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -4.0f, 0.0f));
	QmParticle* p8 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -4.0f, 1.0f));
	p6->addForce(new Spring(p6, p5, length, strength));
	p5->addForce(new Spring(p5, p6, length, strength));
	p7->addForce(new Spring(p7, p5, length, strength));
	p5->addForce(new Spring(p5, p7, length, strength));
	p8->addForce(new Spring(p8, p5, length, strength));
	p5->addForce(new Spring(p5, p8, length, strength));

	p6->addForce(new Spring(p6, p7, length, strength));
	p7->addForce(new Spring(p7, p6, length, strength));
	p6->addForce(new Spring(p6, p8, length, strength));
	p8->addForce(new Spring(p8, p6, length, strength));
	p7->addForce(new Spring(p7, p8, length, strength));
	p8->addForce(new Spring(p8, p7, length, strength));


	QmParticle* p9 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -5.0f, 1.0f));
	p9->addForce(new Spring(p9, p8, length, strength*3.0f));
	p8->addForce(new Spring(p8, p9, length, strength*3.0f));


	QmParticle* p10 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, -6.0f, 0.0f));
	QmParticle* p11 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -6.0f, 0.0f));
	QmParticle* p12 = createParticle2(glm::vec3(0.0f, gravity, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -6.0f, 1.0f));
	p10->addForce(new Spring(p10, p9, length, strength));
	p9->addForce(new Spring(p9, p10, length, strength));
	p11->addForce(new Spring(p11, p9, length, strength));
	p9->addForce(new Spring(p9, p11, length, strength));
	p12->addForce(new Spring(p12, p9, length, strength));
	p9->addForce(new Spring(p9, p12, length, strength));

	p10->addForce(new Spring(p10, p11, length, strength));
	p11->addForce(new Spring(p11, p10, length, strength));
	p10->addForce(new Spring(p10, p12, length, strength));
	p12->addForce(new Spring(p12, p10, length, strength));
	p11->addForce(new Spring(p11, p12, length, strength));
	p12->addForce(new Spring(p12, p11, length, strength));
}

void initScene3()
{
	printf("Scene 3: Magnetic particles.\n");
	printf("Type space to pause.\n");
	//mousePointer = new glm::vec3(0, 4.5, 0);
	for (int i = 0; i < 500; i++)
		createParticle3(rand()%2 * 2.0f - 1.0f);
}
// ***************************** GLUT methods

void resetView()
{
	camRotate = 90.f;
	camDist = 30.f;
	camHeight = 0.f;
	mx = 0.f;
	my = 0.f;
	if (mousePointer) {
		delete mousePointer;
		mousePointer = NULL;
	}
}

void createDisplayListSphere()
{
	DrawListSphere = glGenLists(1);
	glNewList(DrawListSphere, GL_COMPILE);
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 1, 12, 8);
	glEndList();
}

void initLight()
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	GLfloat ambient_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
}

void calculateFPS(float dt)
{
	accTime += dt;

	if (accTime > 0.5)
	{
		char t[200];
		sprintf_s(t, "Quantum Physics Engine: Scene %d, %.2f FPS", scene, frame / accTime);
		glutSetWindowTitle(t);
		accTime = 0.0;
		frame = 0;
	}
}

void idleFunc()
{
	int timer = glutGet(GLUT_ELAPSED_TIME);
	float dt = (float)(timer - timeold) / 1000.f;
	timeold = timer;
	
	if (scene==1)
		createParticle();

	calculateFPS(dt);
	if (!paused) pxWorld.integrate(dt);

	glutPostRedisplay();
}

void drawFunc()
{
	frame++;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camDist * cos(camRotate), camHeight, camDist * sin(camRotate), 0, camHeight, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	for each (GxParticle* p in gxWorld.getParticles())
	{
		glPushMatrix();
		glm::vec3 color = p->getColor();
		glColor3f(color.x, color.y, color.z);
		glTranslatef(p->getPos().x, p->getPos().y, p->getPos().z);
		glScaled(p->getRadius(), p->getRadius(), p->getRadius());
		glCallList(DrawListSphere);
		glPopMatrix();
	}
	if (mousePointer) {
		glPushMatrix();
		glColor3f(1.f, 1.f, 1.f);
		glTranslatef(mousePointer->x, mousePointer->y, mousePointer->z);
		glScaled(0.1f, 0.1f, 0.1f);
		glCallList(DrawListSphere);
		glPopMatrix();
	}
	glutSwapBuffers();
}


void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) buttons = 1;
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) buttons = 2;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) buttons = 3;
	if (button == 3) camDist /= 1.07f;
	if (button == 4) camDist *= 1.07f;
	if (state == GLUT_UP) buttons = 0;

	mx = (float)x;
	my = (float)y;
}

void motionFunc(int x, int y)
{
	if (buttons == 1)
	{
		camRotate += (x - mx) / 100.f;
		camHeight += (y - my) * camDist / 1000.f;
	}
	if (buttons == 3)
	{
		if (mousePointer)
		*mousePointer += glm::vec3(x - mx, my - y, 0.f) / 15.f;
	}

	mx = (float)x;
	my = (float)y;
}

void clearWorld()
{
	gxWorld.clear();
	pxWorld.clear();

}

void toggleScene(int s)
{
	clearWorld();
	scene = s;
	resetView();

	switch (scene)
	{
	case 1: initScene1(); break;
	case 2: initScene2(); break;
	case 3: initScene3(); break;
	}
}


void keyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': case 'Q': case 27:
		clearWorld();
		glDeleteLists(DrawListSphere, 1);
		exit(0);
		break;
	case '1':
		toggleScene(1);
		break;
	case '2':
		toggleScene(2);
		break;
	case '3':
		toggleScene(3);
		break;
	case ' ':
		paused = !paused;
		break;
	case 'g':
		gravity = -9.81f - gravity;
		break;
	default:
		break;
	}
}

void initGraphics(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	glutCreateWindow("Quantum Demo");

	glutIdleFunc(idleFunc);
	glutDisplayFunc(drawFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyFunc);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = SCREEN_X / (float)SCREEN_Y;
	gluPerspective(45.0, aspect, 1.0, VIEWPORT_Z);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
	createDisplayListSphere();
	initLight();
}

// ************************** end GLUT methods

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	initGraphics(argc, argv);

	toggleScene(1);

	glutMainLoop();
	return 0;
}




