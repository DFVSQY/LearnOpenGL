#version 450 core

// define a variable, out indicates it flow out from shader
layout(location = 0) out vec4 fColor;

void main()
{
    fColor = vec4(0.5, 1.0, 0.8, 1.0);
}
