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

#version 150

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 texCoord2d;

out vec2 coord;

void main()
{
    coord = texCoord2d;

    gl_Position = projectionMatrix * modelViewMatrix * vec4(vertexPosition, 1);
}

//
// dtkVisualizationViewVideoGL.vert ends here
