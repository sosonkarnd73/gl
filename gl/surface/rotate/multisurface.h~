#ifndef __MULTISURFACE_H__
#define __MULTISURFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

/* Use glew.h instead of gl.h to get all the GL prototypes declared */
#include <GL/glew.h>
/* Using the GLUT library for the base windowing setup */
#include <GL/freeglut.h>
/* GLM */
// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>

#include "./common/shader_utils.h"

#define GROUND_SIZE 20
#define FPS 24.0
int num_obj;
class Mesh;

int screen_width=800, screen_height=600;
GLuint program;
GLint attribute_v_coord = -1;
GLint attribute_v_normal = -1;
GLint uniform_m = -1, uniform_v = -1, uniform_p = -1;
GLint uniform_m_3x3_inv_transp = -1, uniform_v_inv = -1;

using namespace std;

enum MODES { MODE_OBJECT, MODE_CAMERA, MODE_LIGHT, MODE_LAST } view_mode;
Mesh**  mesh_objects;
GLfloat camera_move_z, camera_rot_y, camera_rot_x;

glm::mat4 transforms[MODE_LAST];
int last_ticks = 0;

static unsigned int fps_start = glutGet(GLUT_ELAPSED_TIME);
static unsigned int fps_frames = 0;
short revolution = 0;
//Callbacks
void onDisplay();
void onTimer(int);
void onReshape(int width, int height);
void onSpecialUp(int key, int x, int y);
void processSpecialKeys(int key, int x, int y);
void onKey(unsigned char key, int x, int y);

void load_obj(const char* filenames, Mesh* mesh);
int init_resources(char* model_filename, char* vshader_filename, char* fshader_filename);
void init_view();  //Reset the obj2word and lookat
void logic();
void draw();
void free_resources();

#endif
