varying vec3 position, normal;
uniform sampler2D specularity;
uniform sampler2D normals;

void AllLights(in vec3 position, in vec3 normal, out vec4 ambient, out vec4 diffuse, out vec4 specular);
vec4 ResidualLight();
float SoftShadow();

void main() {
	vec2 uv = vec2(gl_TexCoord[0]);
	vec3 n = normalize(vec3(2.0, 2.0, 1.0) * texture2D(normals, uv).xyz - vec3(1.0, 1.0, 0.0) + normalize(normal));
	vec4 ambient, diffuse, specular;
	AllLights(position, n, ambient, diffuse, specular);

	ambient = gl_FrontLightModelProduct.sceneColor + ambient * gl_FrontMaterial.ambient;
    specular = specular * (gl_FrontMaterial.specular + texture2D(specularity, uv).r * 0.055);
    diffuse = diffuse * gl_FrontMaterial.diffuse;

	gl_FragData[1] = vec4(diffuse.rgb * SoftShadow(), gl_FragCoord.z);
	gl_FragData[0] = ResidualLight() + ambient + specular;
}