uniform vec3 environmentTransfer[9];
uniform float transferCoefs[3];
uniform float transferMinima;
uniform sampler2D transfer0;
uniform sampler2D transfer1;
uniform sampler2D transfer2;
uniform sampler2D transfer3;
uniform sampler2D transfer4;
uniform sampler2D transfer5;
uniform sampler2D transfer6;
uniform sampler2D transfer7;
uniform sampler2D transfer8;

vec3 SampleTransfer(sampler2D texture, vec2 uv) {
	return texture2D(texture, uv).rgb + transferMinima;
}

vec3 ShContribution(int index, sampler2D texture) {
	vec2 uv = vec2(gl_TexCoord[0]) / 2.0;
	vec2 uv2 = uv + 0.5;

	vec3 average = SampleTransfer(texture, vec2(uv.x, uv2.y));
	vec3 first = SampleTransfer(texture, uv);
	vec3 second = SampleTransfer(texture, uv2);
	vec3 third = SampleTransfer(texture, vec2(uv2.x, uv.y));
	vec3 sum = average +
			   first * transferCoefs[0] +
			   second * transferCoefs[1] +
			   third * transferCoefs[2];

	return sum * environmentTransfer[index];
}

vec4 ResidualLight() {
	vec3 total = ShContribution(0, transfer0);/* +
				ShContribution(1, transfer1) +
				ShContribution(2, transfer2) +
				ShContribution(3, transfer3) +
				ShContribution(4, transfer4) +
				ShContribution(5, transfer5) +
				ShContribution(6, transfer6) +
				ShContribution(7, transfer7) +
				ShContribution(8, transfer8);*/

	return vec4(total, 1);
}