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
	vec2 uv0 = vec2(gl_TexCoord[0]) / 4.0;
	vec2 uv1 = uv0 + 0.25;
	vec2 uv2 = uv0 + 0.5;
	vec2 uv3 = uv0 + 0.75;

	int s = index * 32;
	vec3 sum;
		
	sum += transferCoefs[0] * (texture2D(texture, vec2(uv0.x, uv3.y)).rgb * transferScalars[s+1] + transferScalars[s]);
	sum += transferCoefs[1] * (texture2D(texture, vec2(uv0.x, uv2.y)).rgb * transferScalars[s+3] + transferScalars[s+2]);
	sum += transferCoefs[2] * (texture2D(texture, vec2(uv0.x, uv1.y)).rgb * transferScalars[s+5] + transferScalars[s+4]);
	sum += transferCoefs[3] * (texture2D(texture, uv0).rgb * transferScalars[s+7] + transferScalars[s+6]);
	
	sum += transferCoefs[4] * (texture2D(texture, vec2(uv1.x, uv3.y)).rgb * transferScalars[s+9] + transferScalars[s+8]);
	sum += transferCoefs[5] * (texture2D(texture, vec2(uv1.x, uv2.y)).rgb * transferScalars[s+11] + transferScalars[s+10]);
	sum += transferCoefs[6] * (texture2D(texture, uv1).rgb * transferScalars[s+13] + transferScalars[s+12]);
	sum += transferCoefs[7] * (texture2D(texture, vec2(uv1.x, uv0.y)).rgb * transferScalars[s+15] + transferScalars[s+14]);
	
	sum += transferCoefs[8] * (texture2D(texture, vec2(uv2.x, uv3.y)).rgb * transferScalars[s+17] + transferScalars[s+16]);
	sum += transferCoefs[9] * (texture2D(texture, uv2).rgb * transferScalars[s+19] + transferScalars[s+18]);
	sum += transferCoefs[10] * (texture2D(texture, vec2(uv2.x, uv1.y)).rgb * transferScalars[s+21] + transferScalars[s+20]);
	sum += transferCoefs[11] * (texture2D(texture, vec2(uv2.x, uv0.y)).rgb * transferScalars[s+23] + transferScalars[s+22]);
	
	sum += transferCoefs[12] * (texture2D(texture, uv3).rgb * transferScalars[s+25] + transferScalars[s+24]);
	sum += transferCoefs[13] * (texture2D(texture, vec2(uv3.x, uv2.y)).rgb * transferScalars[s+27] + transferScalars[s+26]);
	sum += transferCoefs[14] * (texture2D(texture, vec2(uv3.x, uv1.y)).rgb * transferScalars[s+29] + transferScalars[s+28]);
	sum += transferCoefs[15] * (texture2D(texture, vec2(uv3.x, uv0.y)).rgb * transferScalars[s+31] + transferScalars[s+30]);

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