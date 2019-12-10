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

uniform sampler2D tex;

in vec2 coord;

out vec4 color;

void main()
{
    color = vec4(texture(tex, coord));
}

//
// dtkVisualizationViewVideoGL.frag ends here
