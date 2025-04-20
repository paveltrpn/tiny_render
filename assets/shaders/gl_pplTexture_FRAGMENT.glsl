#version 450 core

in vec3 EyePosition;
in vec3 DiffuseColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;  
in vec2 FragmentTexcrd;  
in flat uint LightsCount;

out vec4 outColor;

layout(binding=0) uniform sampler2D albedoTexture;  
layout(binding=1) uniform sampler2D normalmapTexture;

struct OmniLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_LIGHTS 15  
uniform OmniLight omniLights[MAX_LIGHTS];

vec3 CalcPointLight(OmniLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);

    float materialShininess = 128;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient * vec3(texture(albedoTexture, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(albedoTexture, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(normalmapTexture, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (diffuse + ambient + specular);
} 

void main()
{
    // Pass alreaady normalized?
    vec3 norm = normalize(Normal);

    // Get normal from normal map, but we need tangent space...
    //vec3 norm = texture(normalmapTexture, FragmentTexcrd).rgb;
    //norm = normalize(norm * 2.0 - 1.0);  

    vec3 viewDir = normalize(EyePosition - FragmentPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < LightsCount; i++)
        result += CalcPointLight(omniLights[i], norm, FragmentPosition, viewDir);    
    
    outColor = vec4(DiffuseColor.rgb*result, 1.0);
}
