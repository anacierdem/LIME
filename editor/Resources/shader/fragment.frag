//
// Created by anacierdem on 6/20/16.
//

uniform sampler2D texture;
uniform vec2 resolution;
uniform float range;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    float dist = distance(gl_FragCoord.xy, resolution * 0.5);

    //float lengthX = (gl_FragCoord.x - (resolution.x * 0.5))/2.0;
    //float lengthY = (gl_FragCoord.y - (resolution.y * 0.5));

	//float dist = sqrt(lengthX*lengthX + lengthY*lengthY);

    float att = clamp(1.0 - (dist*dist)/range, 0.0, 1.0);
    att *= att;

    gl_FragColor = vec4((gl_Color * pixel * att).rgb, pixel.a);
}