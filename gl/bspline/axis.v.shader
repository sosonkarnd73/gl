attribute vec3 v_coord;
attribute vec3 v_color;
varying vec3 color;
void main(void)                   
{                                 
	gl_Position  = vec4(v_coord, 1.0f);             
	gl_PointSize = 2.0f;                                 
  	color = v_color;
}