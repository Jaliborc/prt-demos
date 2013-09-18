varying vec3 position, normal;

void main() {
	vec3 n = normalize(normal);
	gl_FragColor = vec4(n, 1);
}