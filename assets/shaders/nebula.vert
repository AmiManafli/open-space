#version 100
precision highp float;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

attribute vec3 aPosition;
varying vec3 pos;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1);
    pos = (model * vec4(aPosition, 1)).xyz;
}