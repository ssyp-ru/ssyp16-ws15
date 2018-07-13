#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#define TIME 0.001
#define g 9.81 * TIME

#define K 0.6
GLfloat cameraFi = 0.0, cameraTeta = 0.0, xc = 0.0, yc = 0.0;

struct box {
	float x;
	float y;
	float z;
	float m;
	float radius;
	float speedx;
	float speedy;
	float speedz;
	struct box *next;
};

struct box *Head = NULL;

//List functions
void addElem(int x, int y, int z, int radius) {
	struct box *newElem;
	newElem = (struct box *)malloc (sizeof(struct box));
	newElem -> x = 30.0;
	newElem -> y = 30.0;
	newElem -> z = 30.0;
	newElem -> radius = radius;
	newElem -> speedx = x;
	newElem -> speedy = y;
	newElem -> speedz = z;
	newElem -> next = NULL;
	if (Head == NULL) {
		Head = newElem;
	}
	else { 
		struct box *copyHead = Head;
		while (copyHead -> next != NULL)
			copyHead = copyHead -> next;
		newElem -> y = (copyHead -> y) + (copyHead -> radius) + 10;
		copyHead -> next = newElem;
	}
}

void delElem(struct box *copyHead) {
	struct box *copyFree = copyHead -> next;
	copyHead -> next = (copyHead -> next) -> next;
	free(copyFree);
}

void clearList(struct box *head) {
	if (head != NULL) 
		clearList(head -> next);
	free(head);
}

//Logic spheres

void strikeBox() {
	struct box *copyHead = Head;
	while (copyHead -> next != NULL) {
		float x = copyHead -> x;
		float y = copyHead -> y;
		float z = copyHead -> z;
		float radius = copyHead -> radius;

		if (y - radius <= -50) 
			copyHead -> speedy = -1 * K * copyHead -> speedy;
		if (x - radius <= -50 || x + radius >= 50)
			copyHead -> speedx  = -1 * K * copyHead -> speedx;
		if (z - radius <= -50 || z + radius >= 50)
			copyHead -> speedz = -1 * K * copyHead -> speedz;
	}
	float x = copyHead -> x;
	float y = copyHead -> y;
	float z = copyHead -> z;
	float radius = copyHead -> radius;

	if (y - radius <= -50) 
		copyHead -> speedy = -1 * K * copyHead -> speedy;
	if (x - radius <= -50 || x + radius >= 50)
		copyHead -> speedx = -1 * K * copyHead -> speedx;
	if (z - radius <= -50 || z + radius >= 50)
		copyHead -> speedz = -1 * K * copyHead -> speedz;
}

void box() {
	struct box *copyHead = Head;
	if (Head != NULL) {
		copyHead -> speedy += -(g * 0.4);
		copyHead -> x += copyHead -> speedx;
		copyHead -> y += copyHead -> speedy;
		copyHead -> z += copyHead -> speedz;
		copyHead -> speedy -= g*0.4;
		if (((copyHead -> x) - (copyHead -> radius)) <= -49.5) {
			(copyHead -> speedx) = -(copyHead -> speedx);
			copyHead -> x = -49.5 + copyHead -> radius;
		}
		if (((copyHead -> x) + (copyHead -> radius)) >= 49.5) {
			(copyHead -> speedx) = -(copyHead -> speedx);
			copyHead -> x = 49.5 - copyHead -> radius;
		}
		if (((copyHead -> y) - (copyHead -> radius)) <= -49.5) {
			copyHead -> speedy = -(copyHead -> speedy);
			copyHead -> y = -49.5 + copyHead -> radius;
		}
		if (((copyHead -> y) + (copyHead -> radius)) >= 49.5) {
			(copyHead -> speedy) = -(copyHead -> speedy);
			copyHead -> y = 49.5 - copyHead -> radius;
		}
		if (((copyHead -> z) - (copyHead -> radius)) <= -29.5) {
			(copyHead -> speedz) = -(copyHead -> speedz);
			copyHead -> z = -29.5 + copyHead -> radius;
		}
		if (((copyHead -> z) + (copyHead -> radius)) >= 70.5) {
			(copyHead -> speedz) = -(copyHead -> speedz);
			copyHead -> z = 70.5 - copyHead -> radius;
		}
	}
}

//draw picture
void init(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_FLAT);
	glDepthRange (-200.0, 200.0);
}

void drawBox() {
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glColor4f (1.0, 1.0, 0.0, 0.5);
	glTranslatef (0, 0, 20);
	glutWireCube (100.0);
	glBegin(GL_POLYGON);
	glVertex3f(-50, -50, -50);
	glVertex3f(-50, -50, 50);
	glVertex3f(50, -50, 50);
	glVertex3f(50, -50, -50);
	glEnd();
	glPopMatrix ();
}

void drawSphere(float x, float y, float i,float radius,float z) {
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glTranslatef (x, y, z);
	glColor3f (i - 0.1, i, i + 0.1);
	glutSolidSphere (radius, radius * 10, radius * 10);
	glPopMatrix ();
}

void display() {
	float i = 0.1;
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix ();
	glLoadIdentity ();
	glColor3f (50.0, 50.0, -50.0);
	glTranslatef (0.0, 0.0, -100.0);
	glRotatef (cameraFi, 1, 0, 0);
	glRotatef (cameraTeta, 0, 1, 0);
	if (Head != NULL) {
		struct box *copyHead = Head;
		while (copyHead -> next != NULL) {
			i += 0.2;
			drawSphere (copyHead -> x, copyHead -> y, i, copyHead -> radius, copyHead -> z);
			copyHead  = copyHead -> next;
		}
		drawSphere (copyHead -> x, copyHead -> y, i, copyHead -> radius, copyHead -> z);	
	}
	drawBox();
	glPopMatrix ();
	glutSwapBuffers ();
	glFlush ();
}

void idle(void) {
	box();
	glutPostRedisplay();
}

void reshape(int w, int h) {
	glViewport (0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-100.0, 100.0, -100.0, 100.0, -100.0, 200.0);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void keyboard(unsigned char key,int x, int y) {
   switch(key)
   {
      case 'q':
        cameraFi += 1;
		if (cameraFi > 360)
			cameraFi -= 360;
		if (cameraFi < 0)
			cameraFi += 360;
        glutPostRedisplay();
         break;
      case 'w':
        cameraTeta += 1;
		if (cameraTeta > 360)
			cameraTeta -= 360;
		if (cameraTeta < 0)
			cameraTeta += 360;
        glutPostRedisplay();
         break;
      case 'a':
        cameraFi -= 1;
		if (cameraFi > 360)
			cameraFi -= 360;
		if (cameraFi < 0)
			cameraFi += 360;
        glutPostRedisplay();
         break;
      case 's':
        cameraTeta -= 1;
		if (cameraTeta > 360)
			cameraTeta -= 360;
		if (cameraTeta < 0)
			cameraTeta += 360;
        glutPostRedisplay();
         break;
      default:
         break;
   }
}

int main(int argc, char **argv) {
	float	x, y, z, m, radius; 
	printf ("Pleace write speed of x\n");
	scanf ("%f", &x);
	printf ("Pleace write speed of y\n");
	scanf ("%f", &y);
	printf ("Pleace write speed of z\n");
	scanf ("%f", &z);
	printf ("Pleace write radius of sphere (mm)\n");
	scanf ("%f", &radius);
	addElem(x, y, z, radius);
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	glDepthFunc (GL_LEQUAL);
	glShadeModel (GL_SMOOTH);
	glutInitWindowSize (720, 720);
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Box with spheres");
	init ();
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	glutKeyboardFunc (keyboard);
	glutIdleFunc (idle);
	glutMainLoop ();
	return 0;
}
