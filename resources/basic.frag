#version 330 core

in vec3 position;
in vec4 color;
in vec2 texcoord;

out vec4 FragColor0;

uniform sampler2D texture0;

void main()
{

    FragColor0 = (texture(texture0,texcoord)+vec4(0.2f,0.2f,0.2f,0.1f)) * color;

}