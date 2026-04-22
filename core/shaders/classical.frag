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

//    float normalizedSpeed = (vSpeed - minSpeed) / (maxSpeed - minSpeed);

    vec4 color;
    float normalizedSpeed = vValue;

//    if (normalizedSpeed < 0.5) {
//        normalizedSpeed = normalizedSpeed / 0.5;
//        color = mix(vec4(0.565f, 0.835f, 1.0f, 1.0f), vec4(1.0f, 0.3f, 0.0f, 1.0f), normalizedSpeed);
//    }
    if (normalizedSpeed < 0.4) {
        normalizedSpeed = normalizedSpeed / 0.4;
        color = mix(vec4(1.0f, 0.3f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f), normalizedSpeed);
    }
    else if (normalizedSpeed > 0.4 && normalizedSpeed <= 0.7) {
        normalizedSpeed = (normalizedSpeed - 0.4) / (0.7 - 0.4);
        color = mix(vec4(1.0f, 1.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), normalizedSpeed);
    }
    else if (normalizedSpeed > 0.7) {
        normalizedSpeed = (normalizedSpeed - 0.7) / (1.0 - 0.7);
        color = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(0.565f, 0.835f, 1.0f, 1.0f), normalizedSpeed);
    }

    FragColor = color;

}
