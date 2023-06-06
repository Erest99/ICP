#version 440
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexcoord;

uniform mat4 uP_m = mat4(1.0f);
uniform mat4 uM_m = mat4(1.0f);
uniform mat4 uV_m = mat4(1.0f);

out vec3 position;
out vec4 color;
out vec2 texcoord;

void main()
{
    // Outputs the positions/coordinates of all vertices

    position = aPos;
    color = vec4(aColor, 1.0f);
    texcoord = vec2(aTexcoord.x,aTexcoord * -1.0f);
    gl_Position = uP_m * uV_m * uM_m * vec4(aPos, 1.0f);

}