#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 d_color;

out vec3 our_color;

void main()
{
   gl_Position = vec4(position, 1.0f);
   our_color  = d_color;
}