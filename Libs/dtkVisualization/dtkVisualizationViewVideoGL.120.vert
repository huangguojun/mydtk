// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#version 120

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 vertexPosition;
attribute vec3 vertexNormal;
attribute vec2 texCoord2d;

varying vec2 v_texCoord2d;

void main()
{
    v_texCoord2d = texCoord2d;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1);
}

//
// dtkVisualizationViewVideoGL.vert ends here
