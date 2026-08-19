#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D Texture0; 


void main()
{
   
   vec3 result = vec3(1.0f, 1.0f, 1.0f) * vec3(texture(Texture0, TexCoords)); 
   color = vec4(result, 1.0);
}