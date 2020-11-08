#version 330 core
out float lv;
void main(){
    gl_Position = vec4(gl_VertexID & 1, gl_VertexID >> 1, 0.0,0.5) * 4.0 - 1.0;
    lv = gl_Position.y*0.5 + 0.5;
}
