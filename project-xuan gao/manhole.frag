//  Set the fragment color

uniform sampler2D tex;

void main()
{
   vec4 color  = texture2D(tex,gl_TexCoord[0].xy);
   if(color.rgb==vec3(0,0,0))
   {
     discard;
   }

   gl_FragColor = gl_Color * color;
}
