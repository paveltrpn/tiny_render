#version 450

layout( location = 0 ) in vec3 fragColor;
layout( location = 1 ) in vec3 vLighting;

layout( location = 0 ) out vec4 outColor;

void main() {
    outColor = vec4(fragColor.rgb*vLighting, 1.0f);
}
