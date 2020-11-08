#version 330 core
in float lv;
uniform vec3 topColor;
uniform vec3 botColor;
out vec4 frag_color;
void main(){
    frag_color = vec4(botColor * (1.0-lv) + topColor*lv,1.0);
}