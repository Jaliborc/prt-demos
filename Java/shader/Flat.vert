attribute mat4 visibility;
attribute vec3 normal;

varying mat4 v;
varying vec3 n;

void main() {
    v = visibility;
    n = normalize(normal);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}