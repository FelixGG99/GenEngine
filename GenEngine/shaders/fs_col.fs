#version 330 core

uniform vec3 color;
out vec4 fin_color;

void main(){
    fin_color = vec4(color,1.0);
}