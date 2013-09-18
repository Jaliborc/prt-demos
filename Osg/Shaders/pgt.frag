varying vec3 position, normal;
uniform sampler2D normals;
uniform sampler2D specular;

void AllLights(in vec3 position, in vec3 normal, out vec4 ambient, out vec4 diffuse, out vec4 specular);
float SoftShadow();

void main() {
	vec3 n = normalize(normal);
	vec4 ambient, diffuse, spec;
    AllLights(position, n, ambient, diffuse, spec);

    ambient = gl_FrontLightModelProduct.sceneColor + ambient * gl_FrontMaterial.ambient;
   	diffuse = diffuse * gl_FrontMaterial.diffuse;
    spec = spec * gl_FrontMaterial.specular;

	gl_FragData[0] = diffuse * SoftShadow();
	gl_FragData[1] = gl_Color + ambient + spec;
}