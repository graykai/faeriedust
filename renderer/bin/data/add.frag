
// fragment shader

#version 460

uniform vec3 resolution;
uniform sampler2D channel0;
uniform sampler2D channel1;
uniform sampler2D base;
uniform vec2 blurScale;
out vec4 outputColor;

void main()
{
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);
    outputColor = blurScale.x * texture2D(channel0, uv) + blurScale.y * texture2D(channel1, uv) + texture2D(base, uv);
}
