#version 330 core
out vec4 FragColor;
void main()
{
    vec2 temp = gl_PointCoord - vec2(0.5);
    float f = dot(temp, temp);
    if (f > 0.25) {
        discard;
    }
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);

}
