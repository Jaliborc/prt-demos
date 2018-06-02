#define NUM_LIGHTS 1                                                                                                     
                                 
void SpotLight(in int i, in vec3 eye, in vec3 ecPosition3, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) {                                                                      
    float nDotVP;          // normal . light direction                 
    float nDotHV;          // normal . light half vector               
    float pf;              // power factor                             
    float spotDot;         // cosine of angle between spotlight        
    float spotAttenuation; // spotlight attenuation factor             
    float attenuation;     // computed attenuation factor              
    float d;               // distance from surface to light source    
    vec3 VP;               // direction from surface to light position 
    vec3 halfVector;       // direction of maximum highlights          
                                                                       
    // Compute vector from surface to light position                   
    VP = vec3(gl_LightSource[i].position) - ecPosition3;               
                                                                       
    // Compute distance between surface and light position             
    d = length(VP);                                                    
                                                                       
    // Normalize the vector from surface to light position             
    VP = normalize(VP);                                                
                                                                       
    // Compute attenuation                                             
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +       
                         gl_LightSource[i].linearAttenuation * d +     
                         gl_LightSource[i].quadraticAttenuation *d*d); 
                                                                       
    // See if point on surface is inside cone of illumination          
    spotDot = dot(-VP, normalize(gl_LightSource[i].spotDirection));    
                                                                       
    if (spotDot < gl_LightSource[i].spotCosCutoff)                     
        spotAttenuation = 0.0; // light adds no contribution           
    else                                                               
        spotAttenuation = pow(spotDot, gl_LightSource[i].spotExponent);
                                                                       
    // Combine the spotlight and distance attenuation.                 
    attenuation *= spotAttenuation;                                    
                                                                       
    halfVector = normalize(VP + eye);                                  
                                                                       
    nDotVP = max(0.0, dot(normal, VP));                                
    nDotHV = max(0.0, dot(normal, halfVector));                        
                                                                       
    if (nDotVP == 0.0)                                                 
        pf = 0.0;                                                      
    else                                                               
        pf = pow(nDotHV, gl_FrontMaterial.shininess);                  
                                                                       
    ambient  += gl_LightSource[i].ambient * attenuation;               
    diffuse  += gl_LightSource[i].diffuse * nDotVP * attenuation;      
    specular += gl_LightSource[i].specular * pf * attenuation;         
}                                                                      
                                                                       
void PointLight(in int i, in vec3 eye, in vec3 ecPosition3, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) {                                                                      
    float nDotVP;      // normal . light direction                     
    float nDotHV;      // normal . light half vector                   
    float pf;          // power factor                                 
    float attenuation; // computed attenuation factor                  
    float d;           // distance from surface to light source        
    vec3  VP;          // direction from surface to light position     
    vec3  halfVector;  // direction of maximum highlights              
                                                                       
    // Compute vector from surface to light position                   
    VP = vec3(gl_LightSource[i].position) - ecPosition3;               
                                                                       
    // Compute distance between surface and light position             
    d = length(VP);                                                    
                                                                       
    // Normalize the vector from surface to light position              
    VP = normalize(VP);                                                
                                                                       
    // Compute attenuation                                             
    attenuation = 1.0 / (gl_LightSource[i].constantAttenuation +
                        gl_LightSource[i].linearAttenuation * d +
                        gl_LightSource[i].quadraticAttenuation * d*d);
                                                                       
    halfVector = normalize(VP + eye);                                  
                                                                       
    nDotVP = max(0.0, dot(normal, VP));                                
    nDotHV = max(0.0, dot(normal, halfVector));                        
                                                                       
    if (nDotVP == 0.0)                                                 
        pf = 0.0;                                                      
    else                                                               
        pf = pow(nDotHV, gl_FrontMaterial.shininess);                  
                                                                       
    ambient += gl_LightSource[i].ambient * attenuation;                
    diffuse += gl_LightSource[i].diffuse * nDotVP * attenuation;       
    specular += gl_LightSource[i].specular * pf * attenuation;         
}                                                                      
                                                                       
void DirectionalLight(in int i, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) {                                                                                                                                     
    float incidence = dot(normal, normalize(vec3(gl_LightSource[i].position)));
    if (incidence > 0.0)
        diffuse += gl_LightSource[i].diffuse * incidence;

    float reflection = dot(normal, vec3(gl_LightSource[i].halfVector));
    if (reflection > 0.0)
        specular += gl_LightSource[i].specular * pow(reflection, gl_FrontMaterial.shininess);

    ambient += gl_LightSource[i].ambient;          
}                                                                      
                                                                       
void AllLights(in vec3 position, in vec3 normal, out vec4 ambient, out vec4 diffuse, out vec4 specular) {                                              
    ambient  = vec4(0);                                             
    diffuse = vec4(0);                                             
    specular = vec4(0);                                          
                        
    const vec3 eye = vec3(0, 0, 1);                                                     
    for (int i = 0; i < NUM_LIGHTS; i++) { 
       if (gl_LightSource[i].position.w == 0.0)                        
           DirectionalLight(i, normal, ambient, diffuse, specular);               
       else if (gl_LightSource[i].spotCutoff == 180.0)                 
           PointLight(i, eye, position, normal, ambient, diffuse, specular);   
       else                                                            
           SpotLight(i, eye, position, normal, ambient, diffuse, specular);    
    }                                                                                                         
}