int load_obj(const char** filename, Mesh* parent) 
{
     ifstream in(filename[0], ios::in);
     if (!in) {
	  cerr << "Cannot open " << filename[0] << endl;
	  return 0;
     }
     string line;
     while (getline(in, line))
     {
	  if (line.substr(0,2) == "v ") {
	       istringstream s(line.substr(2));
	       glm::vec4 v;
	       s >> v.x; s >> v.y; s >> v.z; v.w = 1.0;
	       this->vertices.push_back(v);
	  }
	  else if (line.substr(0,2) == "n ") {
	       istringstream s(line.substr(2));
	       glm::vec3 n;
	       s >> n.x; s >> n.y; s >> n.z;
	       this->normals.push_back(n);
	  }
	  else if (line.substr(0,2) == "e ") {
	       istringstream s(line.substr(2));
	       GLushort a, b, c;
	       s >> a; s >> b; s >> c;
	       this->elements.push_back(a);
	       this->elements.push_back(b);
	       this->elements.push_back(c);
	  }
	  else if(line.substr(0,2) == "c "){
	       istringstream s(line.substr(2));
	       s>>this->numChilds; 
	  }
	  else if(line.substr(0,2) == "d "){
	       istringstream s(line.substr(2));
	       s>>this->delta; 
	  }
	  else if (line.substr(0,2) == "p ") {
	       istringstream s(line.substr(2));
	       glm::vec3 n;
	       s >> n.x; s >> n.y; s >> n.z;
	       this->position = n;
	       if(parent ! = NULL)
		    this->position += parent->position;
	  }
     }
     in.close();
	       
     GLfloat
	  min_x, max_x,
	  min_y, max_y,
	  min_z, max_z;
     min_x = max_x = this->vertices[0].x;
     min_y = max_y = this->vertices[0].y;
     min_z = max_z = this->vertices[0].z;
     for (unsigned int i = 0; i < this->vertices.size(); i++) {
	  if (this->vertices[i].x < min_x) min_x = this->vertices[i].x;
	  if (this->vertices[i].x > max_x) max_x = this->vertices[i].x;
	  if (this->vertices[i].y < min_y) min_y = this->vertices[i].y;
	  if (this->vertices[i].y > max_y) max_y = this->vertices[i].y;
	  if (this->vertices[i].z < min_z) min_z = this->vertices[i].z;
	  if (this->vertices[i].z > max_z) max_z = this->vertices[i].z;
     }
     
     glm::vec3 size = glm::vec3(1.0f/(max_x - min_x), 1.0f/(max_y - min_y), 1.0f/(max_z - min_z));
     glm::vec3 center   = glm::vec3((min_x + max_x)/2.0f, (min_y + max_y)/2.0f, (min_z + max_z)/2.0f);
       
     glm::mat4 transform  =  glm::scale(glm::mat4(1), size) * glm::translate(glm::mat4(1), (-1.0f)*center);
     
     for (unsigned int i = 0; i < this->vertices.size(); i++) {
	  this->vertices[i] = transform * this->vertices[i]; 
     }
	       
     this->object2world = glm::mat4(1);
     if(this->numChilds > 0)
	  this->childs = new Mesh*[(this->numChilds)];
     int i = 0;
     while(i < (this->numChilds)){
	  this->childs[i] = new Mesh;
	  (this->childs[i])->load_mesh(&filename[i+1], this);
	  i++;
     }
     return 1;
}
