#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSpeed;

out float vValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform float fov;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   float scale = 1.0 / tan(radians(fov) / 2.0);
   float distance = length(cameraPos - aPos);
   gl_PointSize = 10.0f * scale / distance;
   vValue = aSpeed;
}
