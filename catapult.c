#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>

#define p 3.14159265
#define radius 8

struct ball {
	float V;
	float t ;
	float corner;
	float windx;
	float windz;
	float m;
	float x;
	float y;
	float z;
};

int camCorner = 0.0,camX = 0,camY = 0,Y = 0,X = 0, Z = 0;
float res,Vsave, h,k = 0;

struct ball *ball;

void init(void) {
	glClearColor(0.0,0.0,0.0,1.0);
	glShadeModel(GL_FLAT);
}
void drawSphere (float x, float y,float z) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(radius, 60, 60);
	glPopMatrix();
}
void keyboard(unsigned char key,int x,int y) {
	switch(key) {
		case 'z':
			camX=1;
			camCorner=-60;
			camY = 1.28;
			Y = 0;
			X = 172.0;
			glutPostRedisplay();
			break;
		case 'x':
			camX=0.0;
			camCorner=0;
			camY=0;
			Y = 0;
			X = 0;
			glutPostRedisplay();
			break;
		case 'y':
			X = 0;
			Y = 0;
			Z = 600;
			camCorner = -90;
			camX = 1;
			camY = 0;
			glutPostRedisplay();
			break;	
		case 'r':
			ball -> x = -50;
			ball -> y = -40;
			ball -> z = 0;
			ball -> t = 0;
			ball -> V = Vsave;
			glutPostRedisplay();
			break;
	}

}
void display() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(X,Y,Z);
	glRotatef(camCorner,camX,camY,0);
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.6, 0.0);
	glVertex3f(-58.0, -48.0, 0.0);
	glVertex3f(600.0, -48.0, 0.0);
	glVertex3f(600.0, -60.0, 0.0);
	glVertex3f(-58.0, -60.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(600.0, -48.0, 0.0);
	glVertex3f(600.0, -48.0, 658.0);
	glVertex3f(600.0 ,-60.0, 658.0);
	glVertex3f(600.0, -60.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-58.0, -48.0, 0.0);
	glVertex3f(-58.0, -48.0, 658.0);
	glVertex3f(-58.0, -60.0, 658.0);
	glVertex3f(-58.0, -60.0, 0.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(-58.0, -48.0, 658.0);
	glVertex3f(600, -48.0, 658.0);
	glVertex3f(600, -60.0, 658.0);
	glVertex3f(-58.0, -60.0, 658.0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-58.0, -48.0, 0.0);
	glVertex3f(600, -48.0, 0.0);
	glVertex3f(600, -48.0, 658.0);
	glVertex3f(-58.0, -48.0, 658.0);
	glEnd();
	glColor3f(1.0, 0.0, 0.0);

	drawSphere(ball -> x, ball -> y, ball -> z);

	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

float result() {

	h = (sin(ball->corner * p /180) * ball->V *sin(ball->corner * p / 180) * ball->V/20) ;
	if (ball->windx != 0) {
		res = cos(ball->corner * p / 180) * cos(ball->corner * p / 180) * ball->V * ball->V / (2*ball->windx) ;
	}
	else {
		res = cos(ball->corner*p/180) * ball->V * (h / (sin(ball->corner * p /180)*ball->V))*2 ;
	}
	printf("h: %f\n",h);
	return res;
}

void idle(void) {	
	if ((ball -> y < -39) && (ball -> x < -51 || ball->t > 0.11 ))  {
		if (ball ->V < 0.005)
			return;
			ball -> t = 0;
		ball->V = ball->V * k;	
		ball -> t = ball -> t + 0.1;
		float Vx = ball->V * cos(ball->corner * p /180) + ball->windx * ball->t;
		float Vy = ball->V * sin(ball->corner * p / 180) - 10 * ball->t;
		ball -> x = ball -> x + 0.1 * (ball->V * cos(ball->corner * p / 180) + (ball->windx * ball->t/2) -   (0.3 * 1.22 * Vx * Vx * p * 0.05 * 0.05)/(ball->m * 2));
		ball -> y = ball -> y + 0.1 *(ball->V * sin(ball->corner * p / 180) - (5*ball->t) - (0.3*1.22*Vy*Vy*p*0.05*0.05)/(ball->m*2));
		ball -> z = ball -> z + 0.1*(ball->V * cos(ball->corner * p / 180));
		glutPostRedisplay();	
	}
	if (ball->m != 0) {
		ball->t = ball->t + 0.1;
		float Vx = ball->V * cos(ball->corner * p /180) + ball->windx * ball->t;
		float Vy = ball->V * sin(ball->corner * p / 180) - 10 * ball->t;
		ball -> x = ball -> x + 0.1 * (ball->V * cos(ball->corner * p / 180) + (ball->windx * ball->t/2) -   (0.3 * 1.22 * Vx * Vx * p * 0.05 * 0.05)/(ball->m * 2));
		ball -> y = ball -> y + 0.1 *(ball->V * sin(ball->corner * p / 180) - (5*ball->t) - (0.3*1.22*Vy*Vy*p*0.05*0.05)/(ball->m*2));
		ball -> z = ball -> z + 0.1*(ball->V * cos(ball->corner * p / 180));
		glutPostRedisplay();	
	}
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-58.0,600.0,-60.0,600.0,-2000.0,2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void scan() {
	ball -> x = -50;
	ball -> y = -40;
	ball -> z = 0;
	ball -> t = 0;
	printf("corner is: ");
	scanf("%f", &(ball -> corner));	
	printf("ball's speed is: ");
	scanf("%f", &(ball -> V));	
	printf("wind's x  speed is: ");
	scanf("%f", &(ball -> windx));
	printf("wind's z  speed is: ");
	scanf("%f", &(ball -> windz));
	printf("mass is: ");
	scanf("%f" ,&(ball -> m));	
	printf("K is: ");
	scanf("%f",&k);
	printf("result %f\n",result());
	printf("%f ", h);
	Vsave = ball -> V;
}

int main(int argc, char **argv) {
	ball = malloc(sizeof(struct ball));
	scan();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);

	glutInitWindowSize(1000,1000);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Catapult");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle); 
	glutMainLoop();	
	free(ball);
	return 0;
}
