#version 120
#define WIDTH 0.012
#define FOV 0.3491
#define PROJECTION_DISTANCE 1.0 / tan(0.5 * FOV)
#define PROJECTION_SCALE 300.0 * PROJECTION_DISTANCE * WIDTH
#define NEAR 0.5
#define FAR 40.0

uniform sampler2D diffuse;
uniform sampler2D reflected;
uniform sampler2D depth;

const vec4 kernel[25] = vec4[25] (
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
    vec4(0.000973794, 1.11862e-005, 9.43437e-007, 3)
);

float LinearDepth(vec2 uv) {
    return (2.0 * NEAR) / (FAR + NEAR - texture2D(depth, uv).r * (FAR - NEAR));   
}

vec4 SSSS(vec4 color, vec2 direction) {
    vec2 uv = vec2(gl_TexCoord[0]);
    vec4 blurred = color;
    blurred.rgb *= kernel[0].rgb;

    float d = LinearDepth(uv);
    float scale = PROJECTION_DISTANCE / d;
    vec2 off = scale * WIDTH * direction * color.a / 3.0;

    for (int i = 1; i < 25; i++) {
        vec2 sample = uv + kernel[i].a * off;
        float amount = clamp(PROJECTION_SCALE * abs(d - LinearDepth(sample)), 0.0, 1.0);

        vec4 s_color = texture2D(diffuse, sample);
        s_color.rgb = mix(s_color.rgb, color.rgb, amount);
        blurred.rgb += kernel[i].rgb * s_color.rgb;
    }

    return blurred;
}

void main() {
    vec2 uv = vec2(gl_TexCoord[0]);
    vec4 d = texture2D(diffuse, uv);
    vec4 r = texture2D(reflected, uv);

    gl_FragColor = r + SSSS(SSSS(d, vec2(1, 0)), vec2(0, 1));
}