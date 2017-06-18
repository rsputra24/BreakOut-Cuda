
void main(void)
{
   vec4 a = gl_Vertex;
   a.x = a.x * 1;
   a.y = a.y * 1;


   gl_Position = gl_ModelViewProjectionMatrix * a;

}   