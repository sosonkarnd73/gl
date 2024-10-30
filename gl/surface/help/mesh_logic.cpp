void logic()
{
     if(1){
	  glm::vec3 RotationAxis = glm::vec3(0.0f, 1.0f, 0.0f); 
	  glm::quat rot_q = glm::angleAxis(glm::radians(this->delta), glm::normalize(RotationAxis));
	  this->rotation_mat = glm::toMat4(rot_q);	       
	  this->object2world = this->object2world * this->rotation_mat;
     }
     if(revolution && ((this->parent) != NULL))
     {
	  if (revolutionAngle != 360){
	       GLfloat radius = glm::distance( this->position , (this->parent)->position ); 
	       GLfloat x = (parent->position .x) + radius * glm::sin(glm::radians(this->delta));
	       GLfloat z = (parent->position .z) + radius * glm::cos(glm::radians(this->delta));
	       this->revolve = glm::translate(glm::mat4(1), glm::vec3( x, 0.0f, z) );
	       this->object2world = this->object2world * this->revolve;
	       this->position = glm::vec3(this->object2world * glm::vec4(this->position , 1.0f)) + (this->parent)->position;
	       revolutionAngle+=this->delta;
	  }
	  else{
	       revolutionAngle = 0;
	  }
     }
     int i = 0;
     if( this->childs != NULL )
	  while ( this->childs[i] != NULL && (i < this->numChilds))
	  {
	       (this->childs[i])->logic();
	       i++;
	  }
}
