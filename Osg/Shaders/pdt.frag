uniform vec3 environmentTransfer[9];
uniform float transferCoefs[16];

uniform float transferScalars[144];
uniform sampler2D transfer0;
uniform sampler2D transfer1;
uniform sampler2D transfer2;
uniform sampler2D transfer3;
uniform sampler2D transfer4;
uniform sampler2D transfer5;
uniform sampler2D transfer6;
uniform sampler2D transfer7;
uniform sampler2D transfer8;

vec3 ShContribution(int index, sampler2D texture) {
	vec2 uv = vec2(gl_TexCoord[0]) / 4.0;
	int s = index * 16, i = 0;
	vec3 sum;

	for (float x = 0.0; x < 1.0; x += 0.25)
		for (float y = 0.75; y >= 0.0; y -= 0.25) {
			float minima = transferScalars[s++];
			float scale = transferScalars[s++];

			sum += transferCoefs[i++] * (texture2D(texture, vec2(uv.x + x, uv.y + y)).rgb * scale + minima);
		}

	return sum * environmentTransfer[index];
}

vec4 ResidualLight() {
	vec3 total = ShContribution(0, transfer0) +
				ShContribution(1, transfer1) +
				ShContribution(2, transfer2) +
				ShContribution(3, transfer3);/* +
				ShContribution(4, transfer4) +
				ShContribution(5, transfer5) +
				ShContribution(6, transfer6) +
				ShContribution(7, transfer7) +
				ShContribution(8, transfer8);*/

	return vec4(total, 1);
}