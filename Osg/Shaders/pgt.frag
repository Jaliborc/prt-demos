uniform sampler2D transfer1;
uniform sampler2D transfer2;
uniform sampler2D transfer3;
uniform sampler2D transfer4;

void main() {
	vec2 uv = vec2(gl_TexCoord[0]) / 2.0;
	gl_FragColor = vec4(texture2D(uv).rgb - 4, 1);
}