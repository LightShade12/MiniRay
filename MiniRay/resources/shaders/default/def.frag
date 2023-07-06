#version 460 core

layout(pixel_center_integer) in vec4 gl_FragCoord;

out vec4 FragColor;
in vec2 texCoords;
//--------------------------------------------------
uniform vec2 resolution;
uniform sampler2D screentex1;
//--------------------------------------------------

//vec2 uv = gl_FragCoord.xy/resolution;

//--------------------------------------------------

void main()
{

//FragColor=vec4(0.1,0.2,0.25,1);
FragColor=texture(screentex1,texCoords);

}