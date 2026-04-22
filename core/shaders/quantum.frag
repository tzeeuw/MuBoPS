#version 330 core
in float vValue;

// uniform float minSpeed;
// uniform float maxSpeed;

out vec4 FragColor;
void main()
{
    vec2 temp = gl_PointCoord - vec2(0.5);
    float f = dot(temp, temp);
    if (f > 0.05) {
        discard;
    }


    vec4 color;
    float normalizedProb = vValue;

    color = vec4(0.0f, 1.0f, 0.0, normalizedProb);

    FragColor = color;

}
