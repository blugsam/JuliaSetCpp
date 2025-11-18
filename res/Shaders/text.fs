#version 330 core
in vec3 vTexCoord;
out vec4 FragColor;

uniform sampler2DArray ourTexture;
uniform vec3 textColor;

void main()
{
    float alpha = texture(ourTexture, vTexCoord).r;
    
    FragColor = vec4(textColor, alpha);
}
