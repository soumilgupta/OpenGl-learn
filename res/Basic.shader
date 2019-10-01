#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec3 uColor;

out vec3 oColor;

void main(){
    oColor = uColor;
    gl_Position = aPos;
}

#shader fragment
#version 330 core

out vec4 color;
in vec3 oColor;

void main(){
    color = vec4(oColor,1.0f);
}