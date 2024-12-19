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

    // Calculate final vertex position
    gl_Position = matProjection*matView*vec4(vertexPosition, 1.0);
}
#shader fragment
#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;

// Input uniform values
uniform sampler2D equirectangularMap;

// Output fragment color
out vec4 finalColor;

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    // Normalize local position
    vec2 uv = SampleSphericalMap(normalize(fragPosition));

    // Fetch color from texture map
    vec3 color = texture(equirectangularMap, uv).rgb;

    // Calculate final fragment color
    finalColor = vec4(color, 1.0);
}
