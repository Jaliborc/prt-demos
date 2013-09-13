uniform sampler2D transfer0;
uniform sampler2D transfer1;
uniform sampler2D transfer2;
uniform sampler2D transfer3;
uniform sampler2D transfer4;
uniform sampler2D transfer5;
uniform sampler2D transfer6;
uniform sampler2D transfer7;
uniform sampler2D transfer8;

void main() {
	vec2 uv = vec2(gl_TexCoord[0]) / 2.0;
	vec4 color =  vec4(texture2D(transfer0, vec2(uv.x, uv.y + 0.5)).rgb - 4.0, 1);

	gl_FragColor = color;
}