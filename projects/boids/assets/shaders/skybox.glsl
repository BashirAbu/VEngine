#shader vertex
#version 330

// Input vertex attributes
in vec3 vertexPosition;

// Input uniform values
uniform mat4 matProjection;
uniform mat4 matView;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;

void main()
{
    // Calculate fragment position based on model transformations
    fragPosition = vertexPosition;

    // Remove translation from the view matrix
    mat4 rotView = mat4(mat3(matView));
    vec4 clipPos = matProjection*rotView*vec4(vertexPosition, 1.0);

    // Calculate final vertex position
    gl_Position = clipPos;
}

#shader fragment
#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform samplerCube environmentMap;
uniform bool vflipped;
uniform bool doGamma;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Fetch color from texture map
    vec3 color = vec3(0.0);

    if (vflipped) color = texture(environmentMap, vec3(fragPosition.x, -fragPosition.y, fragPosition.z)).rgb;
    else color = texture(environmentMap, fragPosition).rgb;

    if (doGamma)// Apply gamma correction
    {
        color = color/(color + vec3(1.0));
        color = pow(color, vec3(1.0/2.2));
    }

    // Calculate final fragment color
    finalColor = vec4(color, 1.0);
}
