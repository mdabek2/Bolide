#version 330 core

out vec4 color;

//Direct lighting
struct Light
{
    vec3 direction;
    float ambient;
    float specular;
    vec3 color;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform Light light;

float shadowCalc(vec4 FragPosLightSpace);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    //vec3 spotLightDir = normalize(-spotLight.direction);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //vec3 spotLightReflectDir = reflect(-spotLightDir, norm);

    vec3 objectColor = vec3(1.0f, 1.0f, 0.0f);

    //ambient
    vec3 ambient = light.ambient * light.color;
    //diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;
    //specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * spec * light.color;
    
    //float shadow = shadowCalc(FragPosLightSpace);
    float shadow = 0.0;
    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if (texColor.a < 0.2) discard;

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb;

    color = vec4(result, texColor.a);
}

float shadowCalc(vec4 FragPosLightSpace)
{
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
   
    float bias = 0.002;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}