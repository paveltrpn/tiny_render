#version 450 core
out vec4 FragColor;
in vec3 outColor;
void main() {
    FragColor = vec4(outColor, 0.5);
};
