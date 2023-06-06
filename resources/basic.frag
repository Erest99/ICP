#version 330 core

in vec3 position;
in vec4 color;
in vec2 texcoord;

out vec4 FragColor0;
//out vec4 FragColor1;
//out vec4 FragColor2;

uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
    //FragColor = color;
    FragColor0 = (texture(texture0,texcoord)+vec4(0.2f,0.2f,0.2f,0.1f)) * color;
    //FragColor1 = (texture(texture1,texcoord)+vec4(0.2f,0.2f,0.2f,1.0f)) * color;
    //FragColor2 = (texture(texture2,texcoord)+vec4(0.2f,0.2f,0.2f,1.0f)) * color;
    //FragColor0 = texture(texture0,texcoord);
}