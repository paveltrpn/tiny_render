varying highp vec3 vLighting;
varying lowp vec4 vColor;

void main(void) {
    gl_FragColor = vec4(vColor.rgb * vLighting, 1.0);
}