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

uniform sampler2D tex;

varying vec2 v_texCoord2d;

void main()
{
    gl_FragColor = vec4(texture2D(tex, v_texCoord2d));
}

//
// dtkVisualizationViewVideoGL.frag ends here
