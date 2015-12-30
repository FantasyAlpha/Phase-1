#version 330 core

in vec3 our_color;
out vec4 color;

uniform vec3 m_color;

void main()
{ 
   color = vec4((our_color.x * m_color.x),(our_color.y * m_color.y),(our_color.z * m_color.z), 1.0f);
}