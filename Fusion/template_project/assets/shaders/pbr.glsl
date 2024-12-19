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
uniform sampler2D texture0;    
uniform vec4 colDiffuse;
uniform vec4 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
void main()                
{   
    float ambientStrength = 0.1;
    vec4 ambientLight =  ambientStrength * lightColor;
    ambientLight.w = 1.0f;
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec4 diffuse = diffuseValue * lightColor;

    float specularStrength = 0.5;

    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec4 specular = specularStrength * spec * lightColor;  

    vec4 texelColor = texture(texture0, fragTexCoord);
    finalColor = (ambientLight + diffuse + specular) * texelColor * colDiffuse;  
    //finalColor = vec4(normalize(fragNormal) * 0.5 + 0.5, 1.0);

}