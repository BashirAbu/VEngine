#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float id;
// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    if(texelColor.w > 0)
    {
        finalColor = vec4(id, id, id, 1.0f);
    }
    else
    {
        finalColor = vec4(0.0f);
    }
}