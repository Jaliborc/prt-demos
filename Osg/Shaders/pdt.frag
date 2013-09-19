uniform vec3 environmentTransfer[9];
uniform float transferCoefs[15];
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

vec3 SampleTransfer(sampler2D texture, float u, float v) {
	return texture2D(texture, vec2(u, v) / 4.0).rgb + transferMinima;
}

vec3 ShContribution(int index, sampler2D texture) {
	vec2 uv = vec2(gl_TexCoord[0]);
	vec3 sum = SampleTransfer(texture, uv.x, uv.y + 3.0);

	float y = 2.0;
	int i = 0;
	
	for (float x = 0.0; x < 4.0; x++) {
		for (; y >= 0.0; y--) {
			sum += transferCoefs[i] * SampleTransfer(texture, uv.x + x, uv.y + y);
			i++;
		}

		y = 3.0;
	}

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