#version 330 core
in float vSpeed;

uniform float minSpeed;
uniform float maxSpeed;

out vec4 FragColor;
void main()
{
    vec2 temp = gl_PointCoord - vec2(0.5);
    float f = dot(temp, temp);
    if (f > 0.25) {
        discard;
    }

    float normalizedSpeed = (vSpeed - minSpeed) / (maxSpeed - minSpeed);
    float logSpeed = log(normalizedSpeed + 1.0); // Add 1 to avoid log(0)
    FragColor = vec4(logSpeed, 0.0f, 0.0f, 1.0f);

}
