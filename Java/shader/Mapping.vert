attribute mat4 visibility;
attribute vec3 normal;
attribute vec2 coords;

varying mat4 v;
varying vec3 n;
varying vec2 u;

void main() {
    v = visibility;
    u = coords;
    n = normalize(normal);

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}