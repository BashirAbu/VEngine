#shader vertex
#version 330


in vec3 vertexPosition;
in vec2 vertexTexCoord;      
in vec3 vertexNormal;    
in vec4 vertexColor;
//tangents
//texcoords2
//bones
//bone weight

out vec2 fragTexCoord;       
out vec4 fragColor;          
out vec3 fragNormal;
out vec3 fragPosition;

uniform mat4 mvp;
uniform mat4 modelMatrix;               
uniform mat4 projectionMatrix;               
uniform mat4 normalMatrix;               
void main()                        
{                                  
    fragTexCoord = vertexTexCoord; 
    fragColor = vertexColor;      
    fragNormal = mat3(transpose(inverse(modelMatrix))) * vertexNormal;
    fragPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
#shader fragment

#version 330
in vec2 fragTexCoord;         
in vec4 fragColor;    
in vec3 fragNormal;
in vec3 fragPosition;
out vec4 finalColor;      


struct TextureMap
{
    vec4 color;
    float value;
    int useTexture;
};

struct Material
{
    TextureMap albedo;
    TextureMap specular;
    TextureMap ambientOcclusion;
};

uniform sampler2D albedoTexture;
uniform sampler2D specularTexture;
uniform sampler2D ambientOcclusionTexture;

uniform Material material;
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct Light
{
    vec4 color;
    vec4 ambient;
    vec4 specular;
    vec3 position;
    vec3 direction;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
    int type;
};
#define MAX_LIGHTS 10

uniform Light light[MAX_LIGHTS];

uniform int lightCount;

uniform vec3 viewPosition;

vec4 CalculateLight(Light light)
{
    vec4 ambientLight;
    if ((material.albedo.useTexture + material.ambientOcclusion.useTexture) == 2)
    {
        
        ambientLight = light.ambient * material.ambientOcclusion.color * material.albedo.color * texture(albedoTexture, fragTexCoord) * texture(ambientOcclusionTexture, fragTexCoord);
        
    }
    else if (material.albedo.useTexture == 1)
    {
        ambientLight = light.ambient * material.ambientOcclusion.color * material.albedo.color * texture(albedoTexture, fragTexCoord);
    }
    else
    {
        ambientLight = light.ambient * material.albedo.color  * material.ambientOcclusion.color;
    }

    vec3 normal = normalize(fragNormal);
    vec3 lightDirection;
    float attenuation = 1.0;
    float intensity = 1.0;
    if(light.type == DIRECTIONAL_LIGHT)
    {
        lightDirection = normalize(-light.direction);
    }
    else if(light.type == POINT_LIGHT)
    {
        lightDirection = normalize(light.position - fragPosition);
        float distance = length(light.position - fragPosition);
        attenuation = 1.0 / (1.0 + light.constant + light.linear * distance + light.quadratic * (distance * distance));
    }
    else if(light.type == SPOT_LIGHT)
    {
        lightDirection = normalize(light.position - fragPosition);
        float theta = dot(lightDirection, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    }

    float diffuseValue = max(dot(normal, lightDirection), 0.0);

    vec4 diffuse;
    if (material.albedo.useTexture == 1)
    {
        diffuse = (diffuseValue * material.albedo.color) * light.color * texture(albedoTexture, fragTexCoord);
    }
    else
    {
        diffuse = (diffuseValue * material.albedo.color) * light.color;
    }


    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);

    vec4 specular;

    if (material.specular.useTexture == 1)
    {
        specular = light.specular * (spec * material.specular.color) * texture(specularTexture, fragTexCoord); 
    }
    else
    {
        specular = light.specular * (spec * material.specular.color); 
    }
  
    ambientLight = vec4(ambientLight.rgb * attenuation * intensity, ambientLight.a);
    diffuse = vec4(diffuse.rgb * attenuation * intensity, diffuse);
    specular = vec4(specular.rgb * attenuation * intensity, specular.a);
    return (ambientLight + diffuse + specular);  
}

void main()                
{  
    
    vec4 lightColor = vec4(0.0, 0.0, 0.0, 1.0);

    for (int i = 0; i < lightCount; i++)
    {
        lightColor += CalculateLight(light[i]);
    }

    finalColor = lightColor * fragColor;  
}
