#include "surface.h"
class Mesh {
private:
     GLuint vbo_vertices, vbo_normals, ibo_elements;
public:
     vector<glm::vec4> vertices;
     vector<glm::vec3> normals;
     vector<GLuint> elements;
     glm::mat4 object2world;
     glm::mat4 transform;
     glm::mat4 rotation_mat;
     GLushort  euler_rotation;
     GLushort  quat_rotation;
     glm::vec3 RotationAxisa;
     glm::vec3 RotationAxisb;
     GLfloat   RotationAngle;
     glm::vec3 RotAngle;
     GLfloat   delta;
     Mesh() :  vbo_vertices(0), vbo_normals(0), ibo_elements(0),  object2world(glm::mat4(1)), transform(glm::mat4(1)), rotation_mat(glm::mat4(1)), euler_rotation(0), quat_rotation(0) , RotationAxisa(glm::vec3(1,0,0)), RotationAxisb(glm::vec3(1,0,0)), RotationAngle(0),  RotAngle(0), delta(0.0f) {}
     ~Mesh() { 
	  if (vbo_vertices != 0)
	       glDeleteBuffers(1, &vbo_vertices);
	  if (vbo_normals != 0)
	       glDeleteBuffers(1, &vbo_normals);
	  if (ibo_elements != 0)
	       glDeleteBuffers(1, &ibo_elements);
     }     
     void logic()
	  {
	       glm::vec3 RotationAxis = glm::vec3(0);
	       if (view_mode == MODE_OBJECT) {
			    if(this->RotationAngle && quat_rotation)
		    {
			 glm::mat4 Txyz(glm::mat4(1)), TxyzInv(glm::mat4(1));
			 RotationAxis = glm::vec3(this->RotationAxisa - this->RotationAxisb);
			 Txyz[3] = glm::vec4((-1.0f)*(this->RotationAxisb), 1.0f);
			 TxyzInv[3] = glm::vec4((this->RotationAxisb), 1.0f);
			 
			 this->RotationAngle -=5;
			 if(this->quat_rotation){
			      glm::quat rot_q = glm::angleAxis(glm::radians(5.0f), glm::normalize(RotationAxis));			      			      this->rotation_mat = TxyzInv * glm::toMat4(rot_q) * Txyz;
			 }
			 else
			 {			      
			      GLfloat V = glm::sqrt( pow((RotationAxis.y), 2) + pow((RotationAxis.z), 2) );
			      GLfloat L = glm::length( RotationAxis );
			      glm::mat4 Rx(glm::mat4(1)),  Ry(glm::mat4(1)), Rz(glm::mat4(1));
			      glm::mat4 RxInv(glm::mat4(1)),  RyInv(glm::mat4(1));
			      if(V != 0)
			      {
				   Rx[1][1] = Rx[2][2] =  RotationAxis.z / V;		Rx[1][2] = Rx[2][1] =  RotationAxis.y / V;  	Rx[2][1] = (-1.0f)*Rx[2][1];
				   RxInv = Rx;
				   RxInv[2][1] = Rx[1][2];
				   RxInv[1][2] = Rx[2][1];
			      }
			      if(L != 0)
			      {
				   Ry[0][0] = Ry[2][2] =  V / L;	Ry[0][2] = Ry[2][0] =  RotationAxis.x / L;	Ry[2][0] = (-1.0f)*Ry[2][0];
				   RyInv = Ry;
				   RyInv[2][0] = Ry[0][2];
				   RyInv[0][2] = Ry[2][0];
			      }
			      Rz[0][0] = Rz[1][1] = glm::cos(glm::radians(5.0f));
			      Rz[1][0] = Rz[0][1] = glm::sin(glm::radians(5.0f));
			      //Rz[0][0] = Rz[1][1] = glm::cos(glm::radians(this->RotationAngle));
			      //Rz[1][0] = Rz[0][1] = glm::sin(glm::radians(this->RotationAngle));
			      Rz[1][0] = (-1.0f)*Rz[1][0];
			      this->rotation_mat = TxyzInv*RxInv*RyInv*Rz*Ry*Rx*Txyz;
			 }
		    }
	       }
	  }
     void print_mat(glm::mat4 m)
	  {
	       for(int i = 0; i<4; i++){
		    for(int j = 0; j<4; j++)
		    {
			 cout<<"\t"<<m[i][j]<<"\t";
		    }
		    cout<<endl;
	       }
	       cout<<endl;
	  }
     /**
      * Store object vertices, normals and/or elements in graphic card
      * buffers
      */
     void upload()
	  {
	       if (this->vertices.size() > 0) {
		    glGenBuffers(1, &this->vbo_vertices);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
		    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]),
				 this->vertices.data(), GL_STATIC_DRAW);
	       }    
	       if (this->normals.size() > 0) {
		    glGenBuffers(1, &this->vbo_normals);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
		    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(this->normals[0]),
				 this->normals.data(), GL_STATIC_DRAW);
	       }
    
	       if (this->elements.size() > 0) {
		    cout<<"this->elements.size() : "<<this->elements.size()<<endl;
		    glGenBuffers(1, &this->ibo_elements);
		    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
		    
		    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elements.size() * sizeof(this->elements[0]),
				 this->elements.data(), GL_STATIC_DRAW);
	       }
	  }

     /**
      * Draw the object
      */
     void draw()
	  {
	       cout<<"x "<<this->RotAngle.x<<"y "<<this->RotAngle.y<<"z "<<RotAngle.z<<endl;
	       if (this->vbo_vertices != 0) {
		    glEnableVertexAttribArray(attribute_v_coord);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
		    glVertexAttribPointer(attribute_v_coord, 4, GL_FLOAT,  GL_FALSE, 0, 0);
	       }

	       if (this->vbo_normals != 0) {
		    glEnableVertexAttribArray(attribute_v_normal);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
		    glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	       }
	       
	       glm::mat4 m = this->object2world * this->rotation_mat * this->transform;	  
	       /* Apply object's transformation matrix */
	       glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(m));	  
//	       glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(this->object2world));

	       /* Transform normal vectors with transpose of inverse of upper left
		  3x3 model matrix (ex-gl_NormalMatrix): */
	       glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(m)));
	       //glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(this->object2world)));
	  
	       glUniformMatrix3fv(uniform_m_3x3_inv_transp, 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp)); 
    
	       /* Push each element in buffer_vertices to the vertex shader */
	       if (this->ibo_elements != 0) {
		    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
		    int size;
		    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		    glDrawElements(GL_TRIANGLES, size/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		    cout<<"size "<<size<<"size/sizeof(GLushort) "<<(size/sizeof(GLuint))<<endl;
	       } else {
		    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	       }

	       if (this->vbo_normals != 0)
		    glDisableVertexAttribArray(attribute_v_normal);
	       if (this->vbo_vertices != 0)
		    glDisableVertexAttribArray(attribute_v_coord);
	  }
};
Mesh  main_object;  //Objects 
void load_obj(const char* filename, Mesh* mesh)
{
     ifstream in(filename, ios::in);
     if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
     vector<int> nb_seen;

     string line;
     while (getline(in, line)) {
	  if (line.substr(0,2) == "v ") {
	       istringstream s(line.substr(2));
	       glm::vec4 v;
	       s >> v.x; s >> v.y; s >> v.z; v.w = 1.0;
	       mesh->vertices.push_back(v);
	  }
	  else if (line.substr(0,2) == "n ") {
	       istringstream s(line.substr(2));
	       glm::vec3 n;
	       s >> n.x; s >> n.y; s >> n.z;
	       mesh->normals.push_back(n);
	  }
	  else if (line.substr(0,2) == "e ") {
	       istringstream s(line.substr(2));
	       GLushort a, b, c;
	       s >> a; s >> b; s >> c;
	       mesh->elements.push_back(a);
	       mesh->elements.push_back(b);
	       mesh->elements.push_back(c);
	  }
	  else {
	  }
     }
     GLfloat
	  min_x, max_x,
	  min_y, max_y,
	  min_z, max_z;
     min_x = max_x = mesh->vertices[0].x;
     min_y = max_y = mesh->vertices[0].y;
     min_z = max_z = mesh->vertices[0].z;
     for (unsigned int i = 0; i < mesh->vertices.size(); i++) {
	  if (mesh->vertices[i].x < min_x) min_x = mesh->vertices[i].x;
	  if (mesh->vertices[i].x > max_x) max_x = mesh->vertices[i].x;
	  if (mesh->vertices[i].y < min_y) min_y = mesh->vertices[i].y;
	  if (mesh->vertices[i].y > max_y) max_y = mesh->vertices[i].y;
	  if (mesh->vertices[i].z < min_z) min_z = mesh->vertices[i].z;
	  if (mesh->vertices[i].z > max_z) max_z = mesh->vertices[i].z;
     }
     glm::vec3 size = glm::vec3(1.0f/(max_x-min_x), 1.0f/(max_y-min_y), 1.0f/(max_z-min_z));
     glm::vec3 center = glm::vec3((min_x+max_x)/2.0f, (min_y+max_y)/2.0f, (min_z+max_z)/2.0f);
     mesh->transform = glm::scale(glm::mat4(1), size) * glm::translate(glm::mat4(1), center);
}

int init_resources(char* model_filename, char* vshader_filename, char* fshader_filename)
{
     load_obj(model_filename, &main_object);
     // mesh position initialized in init_view()
     /* Compile and link shaders */
     main_object.upload();
     GLint link_ok = GL_FALSE;
     GLint validate_ok = GL_FALSE;

     GLuint vs, fs;
     if ((vs = create_shader(vshader_filename, GL_VERTEX_SHADER))   == 0) return 0;
     if ((fs = create_shader(fshader_filename, GL_FRAGMENT_SHADER)) == 0) return 0;

     program = glCreateProgram();
     glAttachShader(program, vs);
     glAttachShader(program, fs);
     glLinkProgram(program);
     glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
     if (!link_ok) {
	  fprintf(stderr, "glLinkProgram:");
	  print_log(program);
	  return 0;
     }
     glValidateProgram(program);
     glGetProgramiv(program, GL_VALIDATE_STATUS, &validate_ok);
     if (!validate_ok) {
	  fprintf(stderr, "glValidateProgram:");
	  print_log(program);
     }

     const char* attribute_name;
     attribute_name = "v_coord";
     attribute_v_coord = glGetAttribLocation(program, attribute_name);
     if (attribute_v_coord == -1) {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  return 0;
     }
     attribute_name = "v_normal";
     attribute_v_normal = glGetAttribLocation(program, attribute_name);
     if (attribute_v_normal == -1) {
	  fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
	  return 0;
     }
     const char* uniform_name;
     uniform_name = "m";
     uniform_m = glGetUniformLocation(program, uniform_name);
     if (uniform_m == -1) {
	  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	  return 0;
     }
     uniform_name = "v";
     uniform_v = glGetUniformLocation(program, uniform_name);
     if (uniform_v == -1) {
	  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	  return 0;
     }
     uniform_name = "p";
     uniform_p = glGetUniformLocation(program, uniform_name);
     if (uniform_p == -1) {
	  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	  return 0;
     }
     uniform_name = "m_3x3_inv_transp";
     uniform_m_3x3_inv_transp = glGetUniformLocation(program, uniform_name);
     if (uniform_m_3x3_inv_transp == -1) {
	  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	  return 0;
     }
     uniform_name = "v_inv";
     uniform_v_inv = glGetUniformLocation(program, uniform_name);
     if (uniform_v_inv == -1) {
	  fprintf(stderr, "Could not bind uniform %s\n", uniform_name);
	  return 0;
     }
     return 1;
}

void init_view()
{
     main_object.object2world = glm::mat4(1);
     transforms[MODE_CAMERA] = glm::lookAt(
	  glm::vec3(0.0,  0.0, 4.0),   // eye
	  glm::vec3(0.0,  0.0, 0.0),   // direction
	  glm::vec3(0.0,  1.0, 0.0));  // up
}

void logic()
{
     main_object.logic();
     if (view_mode == MODE_CAMERA) {
     }
     
     // View
     glm::mat4 world2camera = transforms[MODE_CAMERA];
     
     // Projection
     glm::mat4 camera2screen = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 100.0f);

     glUseProgram(program);
     glUniformMatrix4fv(uniform_v, 1, GL_FALSE, glm::value_ptr(world2camera));
     glUniformMatrix4fv(uniform_p, 1, GL_FALSE, glm::value_ptr(camera2screen));
     
     glm::mat4 v_inv = glm::inverse(world2camera);
     glUniformMatrix4fv(uniform_v_inv, 1, GL_FALSE, glm::value_ptr(v_inv));
}
void draw()
{
     glClearColor(0.45, 0.45, 0.45, 1.0);
     glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
     glUseProgram(program);
     main_object.draw();    
}
int idisp = 0;
void onDisplay()
{
     cout<<"idisp "<<++idisp<<endl;
     logic();
     draw();
     glutSwapBuffers();
}
void onTimer(int a)
{
     /*can Used to create animation*/
     glutPostRedisplay(); 
     glutTimerFunc(1000.0/24.0, onTimer, 0); 
}
void onReshape(int width, int height)
{
     screen_width = width;
     screen_height = height;
     glViewport(0, 0, screen_width, screen_height);
     glutPostRedisplay();
}

void onKey(unsigned char key, int x, int y)
{
     switch (key) {
     case 'y':
	  main_object.RotAngle.y += 5;
	  if (main_object.RotAngle.y == 360) main_object.RotAngle.y = 0;
	  break;
     case 'Y':
	  main_object.RotAngle.y -= 5;
	  if (main_object.RotAngle.y == -360) main_object.RotAngle.y = 0;
	  break;
     case 'x':
	  main_object.RotAngle.x  += 5;
	  if (main_object.RotAngle.x == 360) main_object.RotAngle.x = 0;
	  break;
     case 'X':
	  main_object.RotAngle.x -= 5;
	  if (main_object.RotAngle.x == -360) main_object.RotAngle.x = 0;
	  break;
     case 'z':
	  main_object.RotAngle.z += 5;
	  if (main_object.RotAngle.z == 360) main_object.RotAngle.z = 0;
	  break;
     case 'Z':
	  main_object.RotAngle.z -= 5;
	  if (main_object.RotAngle.z == -360) main_object.RotAngle.z = 0;
	  break;
     case 'a' :
     	  cout<<"Axis of rotation a: ";
	  cin>>main_object.RotationAxisa.x>>main_object.RotationAxisa.y>>main_object.RotationAxisa.z;
	  cout<<"Axis of rotation b: ";
	  cin>>main_object.RotationAxisb.x>>main_object.RotationAxisb.y>>main_object.RotationAxisb.z;
	  
	  cout<<"Anlge(degres) of rotation : ";
	  cin>>main_object.RotationAngle;
	  break;
     case 27 :
	  glutLeaveMainLoop();
	  break;
     default :
	  break;
     }
     glutPostRedisplay();
}

void onKeyUP(unsigned char key, int x, int y)
{}

void onSpecial(int key, int x, int y) {
     switch (key) {
     case GLUT_KEY_HOME:
	  init_view();
	  main_object.rotation_mat = glm::mat4(1);
	  main_object.RotAngle = glm::vec3(0.0f);	  
	  
	  main_object.euler_rotation = 0;
	  main_object.euler_rotation = 0;
 	  break;
	  // Reset to default.
     case  GLUT_KEY_END:
	  glutLeaveMainLoop();
	  break;
     case GLUT_KEY_F1:
	  view_mode = MODE_OBJECT;
	  break;
     case GLUT_KEY_F2:
	  view_mode = MODE_CAMERA;
	  break;
	  //change the visualization view	  
     case GLUT_KEY_F4:
	  main_object.euler_rotation = !main_object.euler_rotation;
	  break;
     case GLUT_KEY_F5 :
	  main_object.quat_rotation = !main_object.quat_rotation;
	  break;
     default :
	  break;
     }
     glutPostRedisplay();
}

void onSpecialUp(int key, int x, int y)
{
     switch (key) {
     case GLUT_KEY_LEFT:
     case GLUT_KEY_RIGHT:
	  break;
     case GLUT_KEY_UP:
     case GLUT_KEY_DOWN:
	  break;
     case GLUT_KEY_PAGE_UP:
     case GLUT_KEY_PAGE_DOWN:
	  break;
     }
}

void free_resources()
{
     glDeleteProgram(program);
}

int main(int argc, char** argv)
{
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
     glutInitWindowSize(screen_width, screen_height);
     glutCreateWindow("OBJ viewer");

     GLenum glew_status = glewInit();
     if (glew_status != GLEW_OK) {
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
	  return 1;
     }

     if (!GLEW_VERSION_2_0) {
	  fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
	  return 1;
     }

     char* obj_filename = NULL;
     char* v_shader_filename = (char*) "phong-shading.v.glsl";
     char* f_shader_filename = (char*) "phong-shading.f.glsl";
     if (argc != 4) {
	  fprintf(stderr, "Usage: %s model.obj vertex_shader.v.glsl fragment_shader.f.glsl\n", argv[0]);
     } else {
	  obj_filename = argv[1];
	  v_shader_filename = argv[2];
	  f_shader_filename = argv[3];
     }

     if (init_resources(obj_filename, v_shader_filename, f_shader_filename)) {
	  init_view();
	  glutDisplayFunc(onDisplay);
	  glutKeyboardFunc(onKey);
	  //	  glutKeyboardUpFunc(onKeyUP);
	  glutSpecialFunc(onSpecial);
	  glutReshapeFunc(onReshape);
	  glutTimerFunc(1000.0/24.0, onTimer, 0); 
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	  glEnable(GL_DEPTH_TEST);
	  glDepthFunc(GL_LEQUAL);
	  glDepthRange(1, 0);	  
	  glutMainLoop();
     }
     free_resources();
     return 0;
}
