#include <stdio.h>
#include <ctype.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

void display(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);

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

	/* enter the glut main event handling loop, this never returns */
	glutMainLoop();
	return 0;
}

/* the display callback is called when we need to redraw the graphics in our
 * OpenGL window (due to X11 Expose event, or call to glutPostRedisplay).
 */
void display(void)
{
	/* clear the image (by default clears to black) */
	glClear(GL_COLOR_BUFFER_BIT);

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
	printf("window resized: %dx%d\n", x, y);
}

/* the keyboard callback is called when a key is pressed
 * (X11 KeyPress event)
 */
void keyb(unsigned char key, int x, int y)
{
	if(isprint(key)) {
		printf("pressed: '%c'\n", key);
	} else {
		printf("pressed: 0x%x\n", key);
	}
}
