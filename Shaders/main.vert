varying vec3 position, normal;

void main() {   
    vec4  pos  = gl_ModelViewMatrix * gl_Vertex;
    normal = normalize(gl_NormalMatrix * gl_Normal);
    position = pos.xyz / pos.w;            
                                                          
    gl_TexCoord[1].s = dot(pos, gl_EyePlaneS[1]);             
    gl_TexCoord[1].t = dot(pos, gl_EyePlaneT[1]);             
    gl_TexCoord[1].p = dot(pos, gl_EyePlaneR[1]);             
    gl_TexCoord[1].q = dot(pos, gl_EyePlaneQ[1]);                                                                                                              
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;                                                                                                                            
    gl_FogFragCoord = length(pos);
    gl_FrontColor = gl_Color;                
}