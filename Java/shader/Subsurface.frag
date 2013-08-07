uniform float fovY;

const numSamples = 25;
const vec4 kernel[] = {
    vec4(0.530605, 0.613514, 0.739601, 0),
    vec4(0.000973794, 1.11862e-005, 9.43437e-007, -3),
    vec4(0.00333804, 7.85443e-005, 1.2945e-005, -2.52083),
    vec4(0.00500364, 0.00020094, 5.28848e-005, -2.08333),
    vec4(0.00700976, 0.00049366, 0.000151938, -1.6875),
    vec4(0.0094389, 0.00139119, 0.000416598, -1.33333),
    vec4(0.0128496, 0.00356329, 0.00132016, -1.02083),
    vec4(0.017924, 0.00711691, 0.00347194, -0.75),
    vec4(0.0263642, 0.0119715, 0.00684598, -0.520833),
    vec4(0.0410172, 0.0199899, 0.0118481, -0.333333),
    vec4(0.0493588, 0.0367726, 0.0219485, -0.1875),
    vec4(0.0402784, 0.0657244, 0.04631, -0.0833333),
    vec4(0.0211412, 0.0459286, 0.0378196, -0.0208333),
    vec4(0.0211412, 0.0459286, 0.0378196, 0.0208333),
    vec4(0.0402784, 0.0657244, 0.04631, 0.0833333),
    vec4(0.0493588, 0.0367726, 0.0219485, 0.1875),
    vec4(0.0410172, 0.0199899, 0.0118481, 0.333333),
    vec4(0.0263642, 0.0119715, 0.00684598, 0.520833),
    vec4(0.017924, 0.00711691, 0.00347194, 0.75),
    vec4(0.0128496, 0.00356329, 0.00132016, 1.02083),
    vec4(0.0094389, 0.00139119, 0.000416598, 1.33333),
    vec4(0.00700976, 0.00049366, 0.000151938, 1.6875),
    vec4(0.00500364, 0.00020094, 5.28848e-005, 2.08333),
    vec4(0.00333804, 7.85443e-005, 1.2945e-005, 2.52083),
    vec4(0.000973794, 1.11862e-005, 9.43437e-007, 3),
};

vec4 blur(vec2 coords, vec2 direction, sampler2D colors, sampler2D depths, float width) {
	vec4 color = texture2D(colors, coords);
	float depth = texture2D(depths, coords).r;

	float distance = 1.0 / tan(0.5 * radians(fovY));
	float scale = distance / depth;

	vec2 off = scale * width * direction * color.a / 3;
	vec4 blurred = vec4(color.rgb * kernel[0].rgb, color.a);

	for (int i = 1; i < numSamples; i++) {
        vec2 offset = coords + kernel[i].a * off;
        vec4 c = texture2D(colors, offset);
        
        float d = texture2D(depths, offset).r;
        float s = saturate(300.0f * distance * width * abs(depth - d));

        c.rgb = lerp(c.rgb, color.rgb, s);
        blurred.rgb += kernel[i].rgb * c.rgb;
    }
};