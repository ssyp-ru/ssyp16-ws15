#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

struct Planet {
	double speedx;
	double speedy;
	double mass;
	double radius;
	double x;
	double y;
	struct Planet *next;
};

struct Planet *Head = NULL;

void addElem(double xx, double yy, double m, double rad ) {
	struct Planet *newElem;
	newElem = (struct Planet *)malloc (sizeof(struct Planet));
	newElem -> x = xx;
	newElem -> y = yy;
	newElem -> mass = m;
	newElem -> next = NULL;
	newElem -> radius = rad;
	if (Head == NULL ) {
		Head = newElem;
	}
	else { 
		struct Planet *copyHead = Head;
		while (copyHead -> next != NULL)
			copyHead = copyHead -> next;
		copyHead -> next = newElem;

	}
}

void clearList() {
	struct Planet *delElem = Head;
	if (Head != NULL) 
		clearList(delElem -> next);
	free(delElem);
}

void init(void) {
	glClearColor(0.0,0.0,0.0,1.0);
	glShadeModel(GL_FLAT);
}

void drawSphere (double x, double y, double r) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, r);
	glColor3f(0.0, 10.0, 2.0);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();
}

void display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	if (Head != NULL) {
		struct Planet *copyHead = Head;
		while (copyHead -> next != NULL) {		
			drawSphere(copyHead -> x, copyHead -> y, copyHead -> radius);
			copyHead  = copyHead -> next;
		}
		glColor3f(0.0, 1.0, 0.0);
		drawSphere(copyHead -> x, copyHead -> y, copyHead -> radius);
	}
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

void delElem(int num) {
	struct Planet *copyHead = Head, *copyFree;
	if (num == 0) {
		copyFree = Head;
		Head = Head -> next;
	}
	else {
		for (int i = 0; i < num - 1; i++) {
			copyHead = copyHead->next;
		}
		copyFree = (copyHead -> next);
		if (copyHead ->next != NULL){
			copyHead -> next = copyHead -> next -> next;
		}
	}
	free(copyFree);
}

double distance(double x1, double y1, double x2, double y2) {
	double  s;
	s = (sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2))));
	return s;
}


double accel(double x1, double y1, double x2, double y2, double mass, double mass2, char ord) {
	double s, a;
	s = distance(x1, y1, x2, y2);
	a = ((6.67 * mass * mass2) / s) / mass;
	if (ord == 'x')
		return ((y2 - y1) * a) /s;
	if (ord == 'y')
		return ((x2 - x1) * a) /s;
}

void idle(void) {
	struct Planet *copyHead;
	struct Planet *copyHeadNext;

	if (Head != NULL) {
		int num1 = 0, num2 = 1, flag = 0;
		copyHead = Head;
		copyHeadNext = copyHead -> next;
		while((copyHead -> next)!=NULL) {
			copyHeadNext=copyHead -> next;
			num2 = num1 + 1;
			while((copyHeadNext -> next)!=NULL) {

				if((distance(copyHead -> x, copyHead -> y, copyHeadNext -> x, copyHeadNext ->y)) <= copyHead -> radius + copyHeadNext -> radius) { 
					flag = 1;
					break;
				}
				copyHeadNext=copyHeadNext -> next;
				num2++;
				if((distance(copyHead -> x, copyHead -> y, copyHeadNext -> x, copyHeadNext ->y)) <= copyHead -> radius + copyHeadNext -> radius) { 
					flag = 1;
					delElem(num2);
					delElem(num1);
					num1--;
					num2--;
				}
			}
			if((distance(copyHead -> x, copyHead -> y, copyHeadNext -> x, copyHeadNext ->y)) <= copyHead -> radius + copyHeadNext -> radius) { 
				flag = 1;
				delElem(num2);
				delElem(num1);
				num1--;
				num2--;
			}
			if (flag)
				break;
			copyHead = copyHead -> next;
			num1++;
		}
	}

	if (Head != NULL) {

		copyHead = Head;
		copyHeadNext = copyHead -> next;
		while((copyHead -> next)!=NULL) {
			copyHeadNext=copyHead -> next;	
			while((copyHeadNext -> next)!=NULL) {			

				double x1 = copyHead -> x;
				double y1 = copyHead -> y;
				double x2 = copyHeadNext -> x;
				double y2 = copyHeadNext -> y;
				double mass1 = copyHead -> mass;
				double mass2 = copyHeadNext -> mass;

				copyHead -> speedx += accel(x1, y1, x2, y2,	mass1, mass2, 'y');
				copyHead -> speedy += accel(x1, y1, x2, y2,	mass1, mass2, 'x');
				copyHeadNext -> speedy += accel(x2, y2, x1, y1,	mass1, mass2, 'x');
				copyHeadNext -> speedx += accel(x2, y2, x1, y1,	mass1, mass2, 'y');

				copyHeadNext=copyHeadNext -> next;
			}
			double x1 = copyHead -> x;
			double y1 = copyHead -> y;
			double x2 = copyHeadNext -> x;
			double y2 = copyHeadNext -> y;
			double mass1 = copyHead -> mass;
			double mass2 = copyHeadNext -> mass;

			copyHead -> speedx += accel(x1, y1, x2, y2,	mass1, mass2, 'y');
			copyHead -> speedy += accel(x1, y1, x2, y2,	mass1, mass2, 'x');
			copyHeadNext -> speedy += accel(x2, y2, x1, y1,	mass1, mass2, 'x');
			copyHeadNext -> speedx += accel(x2, y2, x1, y1,	mass1, mass2, 'y');

			copyHead = copyHead -> next;			
		}
	}
	if (Head != NULL) {
	copyHead = Head;
	while (copyHead -> next != NULL) {
		copyHead -> x += copyHead -> speedx;
		copyHead -> y += copyHead -> speedy;

		copyHead = copyHead -> next;
	}
	copyHead -> x += copyHead -> speedx;
	copyHead -> y += copyHead -> speedy;

	}
	glutPostRedisplay();
}
void reshape(int w, int h) {
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1000.0,1000.0,-1000.0,1000.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char **argv) {		
	int i, count;
	float x ,y , m ; 
	float rad=10;
	srand(time(NULL));
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);

	glutInitWindowSize(700,700);
	glutInitWindowPosition(100,100);
	glutCreateWindow("SPACE");
	init();
		scanf("%d",&count);
	for(i = 0; i < count; i++){
		x = rand()%2000 - 1000;
		y = rand()%2000 - 1000;
		m = 1;
	addElem(x, y, m, rad);
	}
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	clearList(Head);
	return 0;
} 
