#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aSpeed;

out float vValue;

uniform mat4 transform;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
   gl_Position = projection * transform * vec4(aPos, 1.0);
   float distance = length(cameraPos - aPos);
   gl_PointSize = 20.0f / distance;
   vValue = aSpeed;
}
