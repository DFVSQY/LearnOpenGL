// specify which version of glsl that we want to use 
// 400 indicates that we want to use the version of glsl associated with opengl4.0
// core indicates that we want to use opengl core profile
#version 400 core								

// define a variable, in indicates it's input variable and flow into shader
// this will be filled by data from application in runtime
// layout(location = 0) is called 'layout qualifier'
layout(location = 0) in vec4 vPosition;

void main()
{
    gl_Position = vPosition;
}
