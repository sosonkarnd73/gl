#include <stdio.h>
#include <ctype.h>
#include <GL/glut.h>

void display(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);

float angle;

int main(int argc, char **argv)
{
	/* initialize glut, and create a window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL window");

	/* register event callback handlers */
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);

	glEnable(GL_DEPTH_TEST);

	/* enter the glut main event handling loop, this never returns */
	glutMainLoop();
	return 0;
}

/* the display callback is called when we need to redraw the graphics in our
 * OpenGL window (due to X11 Expose event, or call to glutPostRedisplay).
 */
void display(void)
{
	/* clear the image (by default clears to black), and the depth buffer 
	 * (by default clears to 1.0, which is the farthest possible value)
	 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); /* the following will affect the modelview matrix */
	glLoadIdentity();        /* reset the transformation matrix to identity */
	glTranslatef(0, 0, -5);  /* translate 5 units towards the -z axis */
	glRotatef(angle, 1, 1, 1);  /* rotate by "angle" around the y axis */

	glBegin(GL_QUADS);
	/* far face (-Z) */
	glColor3f(0, 0, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	/* top face (+Y) */
	glColor3f(0, 1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	/* bottom face (-Y) */
	glColor3f(1, 0, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);
	/* right face (+X) */
	glColor3f(0, 1, 0);
	glVertex3f(1, -1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	/* left face (-X) */
	glColor3f(1, 1, 0);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, -1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1, -1);
	/* near face (+Z) */
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glEnd();


	/* since we requested a double-buffered visual (GLUT_DOUBLE flag at
	 * glutInit), we need to "show" the back buffer by calling glutSwapBuffers.
	 */
	glutSwapBuffers();
}

/* the reshape callback is called when our window is resized
 * (X11 ConfigureNotify event)
 */
void reshape(int x, int y)
{
	glViewport(0, 0, x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)x / (float)y, 1.0, 1000.0);
}

/* the keyboard callback is called when a key is pressed
 * (X11 KeyPress event)
 */
void keyb(unsigned char key, int x, int y)
{
	switch(key) {
	case ',':
		angle -= 2;
		glutPostRedisplay();
		break;

	case '.':
		angle += 2;
		glutPostRedisplay();
		break;
	}
}
