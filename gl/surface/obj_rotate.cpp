#include "multisurface.h"

int idisp = 0;
class Mesh {
private:
     GLuint vbo_vertices, vbo_normals, ibo_elements, vbo_axis_vertices, vbo_axis_normals;
public:
  
     GLushort id;
     GLushort rot_arount_id;
     vector<glm::vec4> vertices;
     vector<glm::vec3> normals;
     vector<GLushort> elements;
     vector<glm::vec4>   axis;
     glm::vec3   position;
     glm::mat4 object2world;
     GLfloat    revolutionAngle;
     glm::mat4 revolve;
     glm::mat4 rotation_mat;
     GLfloat   delta;
     Mesh() :  id(0), rot_arount_id(0), vbo_vertices(0), vbo_normals(0), ibo_elements(0), position(glm::vec3(0)), object2world(glm::mat4(1)), revolve(glm::mat4(1)), rotation_mat(glm::mat4(1)), delta(0), revolutionAngle(0) {}
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
	       if(1){
		    glm::vec3 RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); 
		    glm::quat rot_q = glm::angleAxis(glm::radians(this->delta), glm::normalize(RotationAxis));
		    this->rotation_mat = glm::toMat4(rot_q);	       
		    this->object2world = this->object2world * this->rotation_mat;
	       }
	       if(revolution && this->rot_arount_id != 0)
	       {
		    GLfloat radius = glm::distance(this->position , mesh_objects[0]->position); 
		    if (revolutionAngle != 360 ){
			 GLfloat x = (mesh_objects[0]->position .x) + radius * glm::sin(glm::radians(this->delta));
			 GLfloat z = (mesh_objects[0]->position .z) + radius * glm::cos(glm::radians(this->delta));
			 this->revolve = glm::translate(glm::mat4(1), glm::vec3( x, 0.0f, z) );
			 this->object2world = this->object2world * this->revolve;
			 revolutionAngle+=this->delta;
		    }
		    else{
			 revolutionAngle = 0;
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
	       this->axis.push_back( glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	       this->axis.push_back( glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
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
		    glGenBuffers(1, &this->ibo_elements);
		    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
		    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elements.size() * sizeof(this->elements[0]),
				 this->elements.data(), GL_STATIC_DRAW);
	       }

	       vector<glm::vec3>  axisnormal;
	       axisnormal.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	       axisnormal.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	       
	       glGenBuffers(1, &this->vbo_axis_vertices);
	       glGenBuffers(1, &this->vbo_axis_normals);
	      
	       glBindBuffer(GL_ARRAY_BUFFER, this->vbo_axis_normals);
	       glBufferData(GL_ARRAY_BUFFER, axisnormal.size() * sizeof(axisnormal[0]),
			    axisnormal.data(), GL_STATIC_DRAW);
	  }

     /**
      * Draw the object
      */  
     void draw()
	  {
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

	       glm::mat4 m = this->object2world;
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
		    glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	       } else {
		    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	       }
	       if (this->vbo_normals != 0)
		    glDisableVertexAttribArray(attribute_v_normal);
	       if (this->vbo_vertices != 0)
		    glDisableVertexAttribArray(attribute_v_coord);
	       
       
	       if(this->vbo_axis_vertices)
	       {		    
		    glEnableVertexAttribArray(attribute_v_coord);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_axis_vertices);
		    glBufferData(GL_ARRAY_BUFFER, this->axis.size() * sizeof(this->axis[0]), this->axis.data(), GL_STATIC_DRAW);
		    glVertexAttribPointer(attribute_v_coord, 4, GL_FLOAT,  GL_FALSE, 0, 0);
	       }
	       if(this->vbo_axis_normals)
	       {
		    glEnableVertexAttribArray(attribute_v_normal);
		    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_axis_normals);
		    glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	       }
	       
	       m = glm::mat4(1);
	       glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(m));	  
//	       glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(this->object2world));

	       /* Transform normal vectors with transpose of inverse of upper left
		  3x3 model matrix (ex-gl_NormalMatrix): */
	       m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(m)));
	       //glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(this->object2world)));
	  
	       glUniformMatrix3fv(uniform_m_3x3_inv_transp, 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));
	       glDrawArrays(GL_LINES, 0, this->axis.size());
	 	    
	       if (this->vbo_axis_vertices != 0)
		    glDisableVertexAttribArray(attribute_v_coord);
	       if (this->vbo_axis_normals != 0)
		    glDisableVertexAttribArray(attribute_v_normal);
	  }
};
	   
void load_obj(const char* filename, Mesh* mesh)
{
     ifstream in(filename, ios::in);
     if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }
          
     string line;
     while (getline(in, line))
     {
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
	  else if(line.substr(0,3) == "id "){
	       istringstream s(line.substr(3));
	       s>>mesh->id;
	  }
	  else if(line.substr(0,2) == "a "){
	       istringstream s(line.substr(2));
	       s>>mesh->rot_arount_id;
	  } 
	  else if(line.substr(0,2) == "d "){
	       istringstream s(line.substr(2));
	       s>>mesh->delta; 
	  }
	  else if (line.substr(0,2) == "p ") {
	       istringstream s(line.substr(2));
	       glm::vec3 n;
	       s >> n.x; s >> n.y; s >> n.z;
	       mesh->position = n;
	  }
     }
     in.close();
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
     
     glm::vec3 size = glm::vec3(1.0f/(max_x - min_x), 1.0f/(max_y - min_y), 1.0f/(max_z - min_z));
     glm::vec3 center   = glm::vec3((min_x + max_x)/2.0f, (min_y + max_y)/2.0f, (min_z + max_z)/2.0f);
       
     glm::mat4 transform  =  glm::scale(glm::mat4(1), size) * glm::translate(glm::mat4(1), (-1.0f)*center);
     
     for (unsigned int i = 0; i < mesh->vertices.size(); i++) {
	  mesh->vertices[i] = transform * mesh->vertices[i]; 
     }
     mesh->object2world = glm::mat4(1);
}

int init_resources(char** model_filename, char* vshader_filename, char* fshader_filename)
{
     int i = 0;
     Mesh* origin = NULL;
     while(i < num_obj){
	  mesh_objects[i] = new Mesh;
	  load_obj(model_filename[i], mesh_objects[i]);
	  mesh_objects[i]->upload();
	  if(mesh_objects[i]->rot_arount_id == 0)
	  {
	       origin = mesh_objects[i];
	  }
	  i++;
     }
     i = 0;
     origin->object2world = glm::mat4(1); 
     while(i < num_obj)
     {
	  cout<<" id origin : "<< (mesh_objects[i]->rot_arount_id) <<" i "<< i <<endl;
	  if((mesh_objects[i]->rot_arount_id  > 0)){
	       mesh_objects[i]->position     = origin->position + mesh_objects[i]->position;
	       mesh_objects[i]->object2world = glm::translate(mesh_objects[i]->object2world, (-1.0f)*mesh_objects[i]->position);
	  }
	  cout<<"\tposition.  x :"<<mesh_objects[i]->position.x<<", :"<<mesh_objects[i]->position.y<<", : "<<mesh_objects[i]->position.z<<endl;
	  i++;
     }
     
     /* Compile and link shaders */
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
     transforms[MODE_CAMERA] = glm::lookAt(
	  glm::vec3(0.0,  0.0, 10.0),   // eye
	  glm::vec3(0.0,  0.0, 0.0),   // direction
	  glm::vec3(0.0,  1.0, 0.0));  // up
}

void logic()
{
     if (view_mode == MODE_OBJECT) {
	  for(int i = 0; i < num_obj; i++)
	       mesh_objects[i]->logic();
     }
     if (view_mode == MODE_CAMERA) {
	  // Camera is reverse-facing, so reverse Z translation and X rotation.
	  // Plus, the View matrix is the inverse of the camera2world (it's
	  // world->camera), so we'll reverse the transformations.
	  // Alternatively, imagine that you transform the world, instead of positioning the camera.
	  transforms[MODE_CAMERA] = glm::lookAt(
	       glm::vec3(0.0,  0.0, 10.0),   // eye
	       glm::vec3(0.0,  0.0, 0.0),   // direction
	       glm::vec3(0.0,  1.0, 0.0));  // up
	  
	  transforms[MODE_CAMERA] = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, camera_move_z)) * transforms[MODE_CAMERA];	  
	  glm::vec3 y_axis_world = glm::mat3(transforms[MODE_CAMERA]) * glm::vec3(0.0, 1.0, 0.0);
	  transforms[MODE_CAMERA] = glm::rotate(glm::mat4(1.0), glm::radians(camera_rot_y), (-1.0f) * y_axis_world) * transforms[MODE_CAMERA];
	  glm::vec3 x_axis_world = glm::mat3(transforms[MODE_CAMERA]) * glm::vec3(01.0, 0.0, 0.0);
	  transforms[MODE_CAMERA] = glm::rotate(glm::mat4(1.0), glm::radians(camera_rot_x), (-1.0f) * x_axis_world) * transforms[MODE_CAMERA];
	  	  
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
     for(int i = 0; i < num_obj; i++)
     {
	  mesh_objects[i]->draw();
     }
}

void onDisplay()
{
     //     cout<<"idisp "<<++idisp<<endl;
     idisp++;
     logic();
     draw();
     glutSwapBuffers();     
}

void onTimer(int a)
{
     glutPostRedisplay(); 
     glutTimerFunc(1000.0/FPS, onTimer, 0); 
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

     case 27 :
	  glutLeaveMainLoop();
	  break;
     default :
	  break;
     }
     glutPostRedisplay();
}

void onSpecial(int key, int x, int y) {
     switch (key) {
     case GLUT_KEY_HOME:
	  init_view();
	  revolution  = 0;
	  camera_move_z = 0, camera_rot_y = 0, camera_rot_x = 0;
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
     case GLUT_KEY_F4 :
	  revolution = !revolution;
	  break;
     case GLUT_KEY_UP :
	  if(view_mode == MODE_CAMERA) camera_rot_x  +=5.0f;
	  if(camera_rot_x == 360) camera_rot_x = 0;
	  break;
     case GLUT_KEY_DOWN :
	  if(view_mode == MODE_CAMERA) camera_rot_x  -=5.0f;
	  if(camera_rot_x == -360) camera_rot_x = 0;
	  break;
     case GLUT_KEY_LEFT :
	  if(view_mode == MODE_CAMERA) camera_rot_y  +=5.0f;
	  if(camera_rot_y == 360) camera_rot_y = 0;
	  break;
     case GLUT_KEY_RIGHT :
	  if(view_mode == MODE_CAMERA) camera_rot_y  -=5.0f;
	  if(camera_rot_y == -360) camera_rot_y = 0;
	  break;
     case GLUT_KEY_PAGE_UP :

	  if(view_mode == MODE_CAMERA) camera_move_z +=5.0f;
	  if(camera_move_z == 360) camera_move_z = 0;
	  break;
     case GLUT_KEY_PAGE_DOWN :

	  if(view_mode == MODE_CAMERA) camera_move_z  -=5.0f;
	  if(camera_move_z == -360) camera_move_z = 0;
	  break;
     default :
	  break;
     }
     glutPostRedisplay();
}

void onSpecialUp(int key, int x, int y)
{
     switch (key) {
     case GLUT_KEY_UP :
     case GLUT_KEY_DOWN :
     case GLUT_KEY_LEFT :
     case GLUT_KEY_RIGHT :
     case GLUT_KEY_PAGE_UP :
     case GLUT_KEY_PAGE_DOWN :
	  cout<<"camera_move_z << "<<camera_move_z<<", camera_rot_y "<<camera_rot_y<<", camera_rot_x "<<camera_rot_x<<endl;
	  break;
     default :
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
     num_obj = argc - 3;
     mesh_objects = new Mesh*[(num_obj)];
     char** obj_filenames = new char*[(num_obj)];
     char* v_shader_filename = NULL;
     char* f_shader_filename = NULL;
     if (argc < 4) {
	  fprintf(stderr, "Usage: %s  vertex_shader.v.glsl fragment_shader.f.glsl model.obj\n", argv[0]);
	  return 0;
     } 
     v_shader_filename = argv[1];
     f_shader_filename = argv[2];
     int i = 3;
     while(i < argc)
     {
	  obj_filenames[i-3] = argv[i];
	  i++;
     }
     if (init_resources(obj_filenames, v_shader_filename, f_shader_filename)) {
	  init_view();
	  glutDisplayFunc(onDisplay);

	  glutKeyboardFunc(onKey);
	  glutSpecialFunc(onSpecial);
 	  glutSpecialUpFunc(onSpecialUp);
	  glutReshapeFunc(onReshape);
	  glutTimerFunc(1000.0/FPS, onTimer, 0); 
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
