uniform vec3 environmentTransfer[9];
uniform float transferCoefs[3];
uniform sampler2D transfer0;
uniform sampler2D transfer1;
uniform sampler2D transfer2;
uniform sampler2D transfer3;
uniform sampler2D transfer4;
uniform sampler2D transfer5;
uniform sampler2D transfer6;
uniform sampler2D transfer7;
uniform sampler2D transfer8;

vec3 sampleTransfer(sampler2D texture, vec2 uv) {
	return texture2D(texture, uv).rgb - 4.0;
}

vec3 shContribution(int index, sampler2D texture) {
	vec2 uv = vec2(gl_TexCoord[0]) / 2.0;
	vec2 uv2 = uv + 0.5;

	vec3 average = sampleTransfer(texture, vec2(uv.x, uv2.y));
	vec3 first = sampleTransfer(texture, uv2);
	vec3 second = sampleTransfer(texture, uv);
	vec3 third = sampleTransfer(texture, vec2(uv2.x, uv.y));
	vec3 sum = average +
			   first * transferCoefs[0] +
			   second * transferCoefs[1] +
			   third * transferCoefs[2];

	return sum * environmentTransfer[index];
}

void main() {
	vec3 total = shContribution(0, transfer0) +
				shContribution(1, transfer1) +
				shContribution(2, transfer2) +
				shContribution(3, transfer3) +
				shContribution(4, transfer4) +
				shContribution(5, transfer5) +
				shContribution(6, transfer6) +
				shContribution(7, transfer7) +
				shContribution(8, transfer8);

	gl_FragColor = vec4(total, 1);
}