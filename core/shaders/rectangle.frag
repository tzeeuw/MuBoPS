#version 330 core
in vec3 worldPos;

out vec4 FragColor;
void main()
{
    // find normal of the rectangle based on pixel position
    vec3 normal = normalize(cross(dFdx(worldPos), dFdy(worldPos)));
    vec3 lightDir = normalize(vec3(0.1, 1.0, 0.3));
    float diff = max(dot(normal, lightDir) * 0.5 + 0.5, 0.0);
    float ambient = 0.1;
    vec3 colour = vec3(1.0, 0.5, 0.5);
    FragColor = vec4(colour * (diff + ambient), 1.0);
}
