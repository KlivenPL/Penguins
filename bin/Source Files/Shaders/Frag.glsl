#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 color;
uniform sampler2D ourTexture;

void main(){
    vec4 texColor = texture(ourTexture, TexCoord) * vec4(color, 1f);
    if(texColor.a < 1)
        discard;
    FragColor = texColor;
}
