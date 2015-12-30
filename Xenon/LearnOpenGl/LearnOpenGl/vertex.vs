#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 d_color;

out vec3 our_color;

uniform vec3 trans;

void main()
{
   our_color  = d_color;
   vec3 f_pos = position + trans;
   gl_Position = vec4(f_pos, 1.0f);
   
}