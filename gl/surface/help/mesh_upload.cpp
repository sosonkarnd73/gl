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
	  glGenBuffers(1, &this->ibo_elements);
	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
	  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elements.size() * sizeof(this->elements[0]),
		       this->elements.data(), GL_STATIC_DRAW);
     }
     int i = 0;
     if( this->childs != NULL )
	  while ( this->childs[i] != NULL && (i < this->numChilds))
	  {
	       (this->childs[i])->upload();
	       i++;
	  }
}
