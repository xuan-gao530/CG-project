#include "CSCIx229.h"
int axes=0;       //  Display axes
int obj=1;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int inc =2;
//projection
int proj=1;       //  Projection type，the initial state is firstperson navigation
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=2.5;   //  Size of world
int ah =180;      //first navigation view angle (left&right)
int bh =0;         //view angle (up &down)
double radius = 1.6;  // first navigation radius
double dt=0;        //FPN step
double Ex=0.5;        //FPN initial postion
double Ey=0;
double Ez=8;
char *text1[]={"Perpective","First person Navigation"}; 
//light
int move    =1;          //  Move light
int distance=4;          // Light distance
int zh      =90;        //  Light azimuth
float Ylight=2;         //  Light elevation
int one     =1;        //Normal vector 
double alpha=0.7;      //transparencyAlpha
int twoside = 1;       //use for twoside light 
int lightloop = 0;
int lightmove = 1;
//texture
unsigned int texture[21]; // Texture names
int tx = 0;              // texture number,tx=0 without texture
int sky[2];              //  Sky textures
//lantern
double l1  = 0.2;
double l2  = 0.75;
int flower = 0;        

//shader
int mode=0;       //  Shader mode
#define MODE 2
int shader[MODE] = {0,0}; //  Shader programs
// parameter using for light shader computation
float lightpos[5][4] = {{-2.7,-1.0,3.8,1.0},{4.2,-1.0,3,1.0},{4.0,-1,-1.8,1.0},{-2.7,-0.6,-0.9,1.0},{1.0,4.0,-5.0,1.0}};
float ambient[5][4] = {{0.1,0.1,0.1,1.0},{0.1,0.1,0.1,1.0},{0.1,0.1,0.1,1.0}, {0.1,0.1,0.1,1.0},{0.2,0.2,0.2,1.0}};
float diffuse[5][4] = {{0.4,0.4,0.0,1.0},{0.3,0.4,0.2,1.0},{0.45,0.45,0.45,1.0}, {0.5,0.5,0.5,1.0},{0.1,0.1,0.0,1.0}};
float specular[6][4] = {{0.2,0.2,0.2,1.0},{0.2,0.2,0.2,1.0},{0.1,0.1,0.1,1.0}, {0.1,0.1,0.1,1.0},{0.2,0.2,0.2,1.0},{0.2,0.2,0.2,1.0}};
float Emission[]  = {0.0,0.0,0.0,1.0};
float Shinyness[] = {16};
float Direction[] = {-1.0,-1.0,-1.0,1.0};
float Attenuation[] = {1,0.01,0.01};


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}


/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the z&y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th,double ph,int tx)
{ 
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glRotated(ph,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,texture[tx]);
   glBegin(GL_QUADS);
   //  Front&back for lantern1
   if(tx==1)
   {
   glColor3f(1,1,1);
   glNormal3f(0,0,1);
   glTexCoord2f(0.25,0); glVertex3f(0,0,1);
   glTexCoord2f(0.75,0); glVertex3f(1,0,1);
   glTexCoord2f(1,1);    glVertex3f(1,1,1);
   glTexCoord2f(0.5,1);  glVertex3f(0,1,1);
   //  Back
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0.75,0); glVertex3f(1,0,0);
   glTexCoord2f(0.25,0); glVertex3f(0,0,0);
   glTexCoord2f(0.5,1); glVertex3f(0,1,0);
   glTexCoord2f(1,1); glVertex3f(1,1,0);
   glEnd();
    }
//front& back for else cube
  else
  {
   glColor3f(1,1,1);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0,0,1);
   glTexCoord2f(1,0); glVertex3f(1,0,1);
   glTexCoord2f(1,1); glVertex3f(1,1,1);
   glTexCoord2f(0,1); glVertex3f(0,1,1);
   //  Back
   glNormal3f(0,0,-1);
   glTexCoord2f(1,0); glVertex3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(0,1); glVertex3f(0,1,0);
   glTexCoord2f(1,1); glVertex3f(1,1,0);
   glEnd();
  }

   //  Right 
   if(tx==10)
    glBindTexture(GL_TEXTURE_2D,texture[9]);
   else
    glBindTexture(GL_TEXTURE_2D,texture[tx+1]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(1,0,1);
   glTexCoord2f(1,0); glVertex3f(1,0,0);
   glTexCoord2f(1,1); glVertex3f(1,1,0);
   glTexCoord2f(0,1); glVertex3f(1,1,1);
   //Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(0,0,1);
   glTexCoord2f(1,1); glVertex3f(0,1,1);
   glTexCoord2f(0,1); glVertex3f(0,1,0);
   //  Top
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(0,1,1);
   glTexCoord2f(1,0); glVertex3f(1,1,1);
   glTexCoord2f(1,1); glVertex3f(1,1,0);
   glTexCoord2f(0,1); glVertex3f(0,1,0);

   //  Bottom
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(1,0,0);
   glTexCoord2f(1,1); glVertex3f(1,0,1);
   glTexCoord2f(0,1); glVertex3f(0,0,1);
   glEnd();
   //  Undo transformations and textures
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
   ErrCheck("cube");
}

/*
 *  Draw a right angle trapzoid
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y&x axis
 */
static void RAtrapzoid(double x,double y,double z,double dx, double dy, double dz,double th,double ph,int tx)
{
   double h  = l2*SQ(3);
   double l  = l1+l2;

   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glRotated(ph,1,0,0);
   glScaled(dx,dy,dz);

   //  RIGHT ANGLE trapzoid
   glBindTexture(GL_TEXTURE_2D,texture[tx]);
   glBegin(GL_QUADS);
   
   //  Front
   glColor3f(1,0.98,0.8);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0);    glVertex3f(0,0,1);
   glTexCoord2f(0.25,0); glVertex3f(l1,0,1);
   glTexCoord2f(0.5,1);  glVertex3f(l,h,1);
   glTexCoord2f(0,1);    glVertex3f(0,h,1);
   //  Back
   glNormal3f(0,0,-1);
   glTexCoord2f(0.25,0); glVertex3f(l1,0,0);
   glTexCoord2f(0,0);    glVertex3f(0,0,0);
   glTexCoord2f(0,1);    glVertex3f(0,h,0);
   glTexCoord2f(0.5,1);  glVertex3f(l,h,0);
   glEnd();
   //  Right
   if(tx==1)
    glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f(2/SQ(3), -0.5,0);
   glTexCoord2f(0,0); glVertex3f(l1,0,1);
   glTexCoord2f(1,0); glVertex3f(l1,0,0);
   glTexCoord2f(1,1); glVertex3f(l,h,0);
   glTexCoord2f(0,1); glVertex3f(l,h,1);
   //Left
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(0,0,1);
   glTexCoord2f(1,1); glVertex3f(0,h,1);
   glTexCoord2f(0,1); glVertex3f(0,h,0);
  
   //  Top
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(0,h,1);
   glTexCoord2f(1,0); glVertex3f(l,h,1);
   glTexCoord2f(1,1); glVertex3f(l,h,0);
   glTexCoord2f(0,1); glVertex3f(0,h,0);
   //  Bottom
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(l1,0,0);
   glTexCoord2f(1,1); glVertex3f(l1,0,1);
   glTexCoord2f(0,1); glVertex3f(0,0,1);
   glEnd();
    if(tx==1)    //draw the ornament
   {
   glBindTexture(GL_TEXTURE_2D,texture[14]);
   glBegin(GL_LINES);
   glColor4f(1,0.9,0,1);
   glVertex3f(0.5,h,0);
   glVertex3f(0,4,0.5);
   glVertex3f(0.5,h,1);
   glVertex3f(0,4,0.5);
   glEnd();
   }
   // Undo transformations
   glPopMatrix();

}

/*
 *  Draw a clinder
 *     at (x,y,z)
 *     radius r, hight h with angle (start,end)
 *     rotated th about the y&x axis
 */
static void clinder(double x,double y,double z,double r,double h,int start, int end,int th,int ph,int tx,int roof,int opaque)
{
  
  double ch=0;
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glRotated(ph,1,0,0);
  glScaled(r,r,h);


  if (!opaque)            //transparency 
   {
      glEnable(GL_BLEND);
      glColor4f(1,1,1,alpha);
      glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(0);
    }
    else
      glColor4f(1,1,1,1); 
  glBindTexture(GL_TEXTURE_2D,texture[tx]);
  //sides
  glBegin(GL_QUAD_STRIP);
  for(ch=start;ch<=end;ch+=inc)
  { 
    glNormal3d(Cos(ch),Sin(ch),0);
    glTexCoord2d(1,ch/(end-start));glVertex3d(Cos(ch),Sin(ch),1);
    glTexCoord2d(0,ch/(end-start)); glVertex3d(Cos(ch),Sin(ch),0);
  }
  glEnd();
  if(!roof)               //draw the top and bottom
  {
   //top
  glBegin(GL_TRIANGLE_FAN);
  glNormal3d(0,0,1);
  for(ch=start;ch<=end;ch+=inc)
  {
   glTexCoord2f(0.5*Cos(ch)+0.5,0.5*Sin(ch)+0.5);
   glVertex3d(Cos(ch),Sin(ch),1);
   }
   glEnd();

  //bottom
  glBegin(GL_TRIANGLE_FAN);
  glNormal3d(0,0,-1);
  for(ch=start;ch<=end;ch+=inc)
  {
   glTexCoord2f(0.5*Cos(ch)+0.5,0.5*Sin(ch)+0.5);
   glVertex3d(Cos(ch),Sin(ch),0);
   }
   glEnd();
  }
   if (!opaque)
   {
      glDisable(GL_BLEND);
      glDepthMask(1);
    }
  glPopMatrix();
} 

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius r  rotated th about the z&x axis
 *     can draw part of ball as a petal
 */

static void ball(double x,double y,double z,double r,int ah,int bh,int tx,int flower,int rh,int oh)
{
   int th,ph;
   int ch = 0;
   int dh = 60;   //the width of petal
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(-90,1,0,0);
   glRotated(rh,0,0,1);
   glRotated(oh,1,0,0);
   glScaled(r,r,r);

   glBindTexture(GL_TEXTURE_2D,texture[tx]);
   // draw a petal of flower
   if(flower==1)
  {
   
    for (ph=-90;ph<ah;ph+=inc)
   {   
      for (th=ch;th<=dh;th+=2*inc)
      {  glBegin(GL_QUAD_STRIP); 
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
   }
    glEnd();
    for (ph=ah;ph<bh;ph+=2*inc)
   {
      glBegin(GL_TRIANGLE_STRIP);
      for (th=ch;th<dh;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th+inc,ph+2*inc);
      }
       Vertex(dh,ph);
      ch+=inc;
      dh-=inc;
       glEnd();
    }
   }
  // draw a whole ball
 else 
 {
   glBegin(GL_QUAD_STRIP);
   for (ph=-90;ph<90;ph+=inc)
   {
      for (th=360;th>=0;th-=2*inc)
      {
         
         Vertex(th,ph);
         Vertex(th,ph-inc);
         
      }
   }
    glEnd();
  }
   //  Undo transofrmations
   glPopMatrix();
}

/*draw the ormament of lantern
*/
static void ornament(double x,double y,double z)
{
  glPushMatrix();
  glTranslated(x,y,z);
  ball(0,1,0,0.02,0,0,5,0,0,0);
  glBegin(GL_LINES);
  glColor4f(1,0.9,0,0);
  glVertex3f(0,1,0);
  glVertex3f(0,-0.5,0);
  glEnd();
  glPopMatrix();

}

/*
 *  Draw lantern1
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void lantern1(double x, double y,double z,double dx, double dy,double dz, double th,int tx)
{

    glPushMatrix();
   //  Offset, scale and rotate
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    cube(l1,0,0,l2,2*l2,1,-30,0,tx);
    RAtrapzoid(0,0,0,1,1,1,0,0,tx);
    ball(0,4,0.5,0.1,0,0,5,0,0,0);
   //  Undo transofrmations
   glPopMatrix();   
}

/*
 *  Draw lantern2
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the x axis
 */
static void lantern2(double x,double y,double z,double dx, double dy,double dz,double th,int tx)
{
  int i = 0;
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,1,0,0);
  glScaled(dx,dy,dz);
  ball(0,-0.5,0,0.1,0,0,5,0,0,0);
  //draw the petals
  for(i=0;i<5;i+=1)
  {
  ball(0,0,0,0.5,-10,50,tx,1,i*72+zh,0);
  ball(0,0.1,0,0.7,-60,0,tx,1,i*72+30+zh,0);
  ball(0,0.2,0,0.8,-80,-20,tx,1,i*72+80+zh,0);
  }
  if(th==180)
  {
    ornament(0,-1.2,0);
  }
  else
    ornament(0,0,0);
  glPopMatrix();
}

/*
 *  Draw lantern3
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void lantern3(double x,double y,double z,double dx, double dy,double dz,int tx, int opaque)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(dx,dy,dz);
  clinder(0,0,0,1,2,0,360,0,-90,tx,0,opaque);
  ornament(0,2.5,0);
  glPopMatrix();
}

/*
 *  Draw lantern4
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void lantern4(double x,double y,double z,double r, int tx)
{
   double th,ph;
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(-90,1,0,0);
   glScaled(2.5*r,2*r,2.5*r);

   glBindTexture(GL_TEXTURE_2D,texture[tx]);
   glColor4f(1,1,1,1);
   glBegin(GL_QUAD_STRIP);
   for (ph=-90;ph<90;ph+=inc)
   {
      
      for (th=360;th>=0;th-=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph-inc);
         
      }
 
   }
    glEnd();
   ornament(0,0,0);
   glPopMatrix();
   
}

/*
 *  Draw lantern5
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 */
static void lantern5(double x,double y,double z,double dx, double dy,double dz,int tx, int layer)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glScaled(dx,dy,dz);
  ball(0,0,0,0.3,0,0,tx,0,0,0);
  clinder(0,-0.4,0,0.1,0.2,0,360,0,-90,tx+1,0,1);
  ball(0,-0.5,0,0.2,0,0,tx,0,0,0);
  if(layer)
  {
  clinder(0,-0.75,0,0.08,0.08,0,360,0,-90,tx+1,0,1);
  ball(0,-0.85,0,0.15,0,0,tx,0,0,0);
  }
  ornament(0,0,0);
  glPopMatrix();
}


/*
 *  Draw a roof ,which is consisted by clinder and fan
 *     at (x,y,z)
 *     radius r, hight h with angle (start,end)
 *     rotated th about the y&x axis
 */
static void Roof(double x,double y,double z,double dx,double dy,double dz,int th,int ph,int tx)
{
   int fh = 0;
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glRotated(ph,1,0,0);
   glScaled(dx,dy,dz);

   glBindTexture(GL_TEXTURE_2D,texture[tx]); 
   //front 
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1,1,1,1);
   glNormal3d(0,0,1);
   glTexCoord2f(0,0);glVertex3d(0,0,1);
   for(fh=180;fh<=270;fh+=2)
   {
   glTexCoord2f(1+Cos(fh),Sin(fh)+1); glVertex3d(1+Cos(fh),1+Sin(fh),1);
   }
   glEnd();
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1,1,1,1);
   glNormal3d(0,0,1);
   glTexCoord2f(0,0);glVertex3d(0,0,1);
   for(fh=270;fh<=360;fh+=2)
   {
   glTexCoord2f(1-Cos(fh),Sin(fh)+1); glVertex3d(Cos(fh)-1,1+Sin(fh),1);
   }
   glEnd();
   
   //back 
   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1,1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0,0);glVertex3d(0,0,0);
   for(fh=180;fh<=270;fh+=2)
   {
   glTexCoord2f(1+Cos(fh),Sin(fh)+1); glVertex3d(1+Cos(fh),1+Sin(fh),0);
   }
   glEnd();

   glBegin(GL_TRIANGLE_FAN);
   glColor4f(1,1,1,1);
   glNormal3d(0,0,-1);
   glTexCoord2f(0,0);glVertex3d(0,0,0);
   for(fh=270;fh<=360;fh+=2)
   {
   glTexCoord2f(1-Cos(fh),Sin(fh)+1); glVertex3d(Cos(fh)-1,1+Sin(fh),0);
   }
   glEnd();
   //sides
   clinder(1,1,0,1,1,180,270,0,0,tx,1,1);
   clinder(-1,1,0,1,1,270,360,0,0,tx,1,1);
  
  glPopMatrix();
}

/*Draw the house
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
*/
static void house(double x,double y,double z,double dx, double dy,double dz,double th, int tx)
{
  
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  Roof(-3.5,2,1,0.8,0.8,2,0,0,tx);
  cube(-4,0.5,3,2,1.5,1,0,90,tx+1);
  RAtrapzoid(-4,0.5,2.8,1,0.5,1,-90,180,tx);
  RAtrapzoid(-3.2,0.5,3,1,0.5,2,0,180,tx);
  RAtrapzoid(-3,0.5,1.2,1,0.5,1,90,180,tx);
  RAtrapzoid(-3.8,0.5,1,1,0.5,2,180,180,tx);
  cube(-4,-1.5,3,2,2,1,0,90,tx+3);
  glPopMatrix();
}


/* 
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);

   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);
   glEnd();

   //  bottom
   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-1.6,+D);
   glTexCoord2f(1,0); glVertex3f(+D,-1.6,+D);
   glTexCoord2f(1,1); glVertex3f(+D,-1.6,-D);
   glTexCoord2f(0,1); glVertex3f(-D,-1.6,-D);
   glEnd();

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   glClearColor(0.1,0.1,0.1,0);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

    //  Perspective - set eye position
   if (proj==0)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else        //first person navigation 
   {
      double dx= radius*Sin(ah)*Cos(bh);
      double dy= radius*Sin(bh);
      double dz= radius*Cos(ah)*Cos(bh);
      Ex=Ex+dt*dx;
      Ey=Ey+dt*dy;
      Ez=Ez+dt*dz;
      gluLookAt(Ex,Ey,Ez,Ex+dx,Ey+dy,Ez+dz,0,1,0);
   }

//  Flat or smooth shading
   glShadeModel(GL_SMOOTH);
  //  Draw direct light position 
  
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  glColor sets ambient and diffuse color materials
   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   //  Enable light 4 using for direction light 
   glEnable(GL_LIGHT4);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT4,GL_AMBIENT ,&ambient[4][0]);
   glLightfv(GL_LIGHT4,GL_DIFFUSE ,&diffuse[4][0]);
   glLightfv(GL_LIGHT4,GL_SPECULAR,&specular[4][0]);
   glLightfv(GL_LIGHT4,GL_POSITION,&lightpos[4][0]);
   glLightfv(GL_LIGHT4,GL_SPOT_DIRECTION,Direction);
   
   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,&specular[5][0]);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   
   glEnable(GL_TEXTURE_2D);
   glUseProgram(shader[mode]);
  
  int id;
   id = glGetUniformLocation(shader[mode],"twoside");
   if (id>=0) glUniform1i(id,twoside);
    id = glGetUniformLocation(shader[mode],"lightloop");
   if (id>=0) glUniform1i(id,lightloop);
    id = glGetUniformLocation(shader[mode],"lightmove");
   if (id>=0) glUniform1i(id,lightmove);
   id = glGetUniformLocation(shader[mode],"lightpos");
   if (id>=0) glUniformMatrix4fv(id,1,GL_FALSE,&lightpos[0][0]);
   id = glGetUniformLocation(shader[mode],"Ambient");
   if (id>=0) glUniformMatrix4fv(id,1,GL_FALSE,&ambient[0][0]);
   id = glGetUniformLocation(shader[mode],"Diffuse");
   if (id>=0) glUniformMatrix4fv(id,1,GL_FALSE,&diffuse[0][0]);
   id = glGetUniformLocation(shader[mode],"Specular");
   if (id>=0) glUniformMatrix4fv(id,1,GL_FALSE,&specular[0][0]);
   id = glGetUniformLocation(shader[mode],"attenuation");
   if (id>=0) glUniform3fv(id,1,&Attenuation[0]);


   //Shader 0 texture and light(with twoside light and dig holes for lantern)
   glUseProgram(shader[0]);
   glColor3f(1,1,1);
   Sky(3.5*dim); 
   glColor3f(0.3,0.3,0);
   glPushMatrix();
   glTranslated(lightpos[4][0],lightpos[4][1],lightpos[4][2]);
   glutSolidSphere(0.2,10,10);
   glPopMatrix();
  
  //draw the scene
   // pumpkin to dig hole
  glColor3f(1,1,1);
  lantern4(-2.7,-1,3.8,0.15,16);
  clinder(-2.7,-0.65,3.8,0.05,0.1,0,360,20,-90,19,0,1);
  lantern4(4,-1,-1.8,0.14,17);
  lantern4(4.2,-1,3,0.15,18);
  Roof(4.2,-0.65,3,0.15,0.15,0.15,0,0,19);
  //left
  lantern5(-2.7,-0.5,2.8,0.6,0.6,0.6,14,1);
  lantern3(-2.7,-0.8,2,0.1,0.2,0.1,3,1); 
  lantern2(-2.7,-0.7,1,0.4,0.4,0.4,180,12);
  lantern1(-2.7,-0.7,-0.2,0.2,0.2,0.2,0,1);
  lantern1(-2.7,-0.7,0,0.2,0.2,0.2,-180,1);
  lantern5(-2.7,-0.6,-0.9,0.7,0.7,0.7,14,0);
  lantern2(-2.7,-0.5,-3.3,0.3,0.3,0.3,0,4);
  lantern1(-2.7,-0.7,-4.2,0.18,0.2,0.2,0,1);
  lantern1(-2.7,-0.7,-4,0.18,0.2,0.2,-180,1);
  lantern5(-2.7,-0.5,-4.7,0.6,0.6,0.6,14,1);
  //right
  lantern5(4.5,-0.6,1.8,0.6,0.6,0.6,5,1);
  lantern1(4.5,-0.7,0.3,0.2,0.2,0.2,0,1);
  lantern1(4.5,-0.7,0.5,0.2,0.2,0.2,-180,1);
  lantern2(4.5,-0.6,-0.5,0.35,0.4,0.35,0,6);
  lantern2(4.5,-1.0,-0.5,0.35,0.4,0.35,180,6);
  lantern3(4.5,-0.8,-1.3,0.1,0.2,0.1,3,1); 
  lantern5(4.5,-0.7,-4,0.7,0.7,0.7,14,1);
  lantern2(4.5,-0.7,-4.5,0.35,0.4,0.35,0,13);
  lantern1(4.5,-0.8,-5.2,0.2,0.2,0.2,0,1);
  lantern1(4.5,-0.8,-5,0.2,0.2,0.2,-180,1);
  //house
  house(0,0,0,1,1.2,1,0,7);
  house (0,0,-3,1,1,1,0,7);
  house (0,0,-6,1,1.1,1,0,7);
  house (0,0,-9,1,1.2,1,0,7);
  house(2,0,3,1,1.3,1,-180,7);
  house(2,0,1,1,1.3,1,-180,7);
  house(2,0,-2.8,1,1,1,-180,7);
  house(2,0,-7,1.2,1.2,1,-180,7);
  
 
  //draw the tranparency at last
  lantern3(-2.7,-0.8,-1.8,0.2,0.2,0.2,11,0);    //draw the tranparency last 
  lantern2(-2.7,-0.7,-1.8,0.15,0.15,0.15,0,13);
  lantern3(4.5,-0.9,1.2,0.15,0.25,0.15,11,0);    //draw the tranparency last 
  lantern2(4.5,-0.8,1.2,0.15,0.15,0.15,0,4);
  
   glDisable(GL_TEXTURE_2D);
   //  No shader for what follows
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
    if(axes==1)          //choose to display the axes
   {
   glBegin(GL_LINES);
   glColor4d(1,1,1,1);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(len,0.0,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,len,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,0.0,len);
   glEnd();
   //  Label axes
   glRasterPos3d(len,0.0,0.0);
   Print("X");
   glRasterPos3d(0.0,len,0.0);
   Print("Y");
   glRasterPos3d(0.0,0.0,len);
   Print("Z");
   }

   //  Display parameters
   glColor4d(1,1,1,1);
   glWindowPos2i(5,5);
   glColor4d(1,1,1,1);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s,dt=%.2f,lightmove=%d",
     th,ph,dim,proj?"firstperson":"Perspective",dt,lightmove);
    glWindowPos2i(20,20);
    glColor4d(1,1,1,1);
    Print("ah=%d bh=%d r=%f Ex=%5.2f Ey=%5.2f Ez=%5.2f",ah,bh,radius,Ex,Ey,Ez);
    dt=0;
    ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   static double t0 = -1.;
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
  if (t-t0 > 2)             //change the color per second
   {  t0 = t;

  if(lightloop<4)
    lightloop+=1;
  else
     lightloop =0;
   }
   
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

void special(int key,int x,int y)
{   dt=0;
   //  Right arrow key - increase azimuth by 5 degrees
  
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
      
    //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
  
    //change the distance of ball

   th %= 360;
   ph %= 360;

      //  Update projection
   Project(proj,fov,dim,asp,radius);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle light movement
   else if (ch == 't' || ch == 'T')
      twoside = 1-twoside;
   else if (ch == 'm' || ch == 'M')
      lightmove = 1-lightmove;
    //change the right or left(for first person navigation)
    else if (ch =='q')         
      {
        dt=0;
        ah+=5;
      }
    else if (ch== 'Q')
    {  
         dt=0; 
         ah-=5;
    }
    // forward or back (for first person navigation)
    else if (ch=='w')
      dt= 0.1;
    else if (ch == 'W')
      dt=-0.1;

  //view of up and down(for first person navigation)
    else if (ch =='z')
    { if(bh<45)
         bh+=5;
    }
    else if (ch == 'Z')
      {    
        if(bh>-45)
           bh-=5;
      }
   //  Reproject
     Project(proj,fov,dim,asp,radius);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(proj,fov,dim,asp,radius);
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ALPHA);
   glutInitWindowSize(1200,900);
   //  Create the window
   glutCreateWindow("Xuan Gao-poject");
   // glutFullScreen();
   #ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   #endif
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   //  Load textures
   texture[1] = LoadTexBMP("ink1.bmp");
   texture[2] = LoadTexBMP("ink2.bmp");
   texture[3]=  LoadTexBMP("ink3.bmp");
   texture[4] = LoadTexBMP("petal.bmp");
   texture[5] = LoadTexBMP("pi.bmp");
   texture[6] = LoadTexBMP("blue.bmp");
   texture[7] = LoadTexBMP("roof.bmp");
   texture[8] = LoadTexBMP("window.bmp");
   texture[9] = LoadTexBMP("brick.bmp");
   texture[10] = LoadTexBMP("door.bmp");
   texture[11] = LoadTexBMP("paper.bmp");
   texture[12] = LoadTexBMP("green.bmp");
   texture[13] = LoadTexBMP("orange.bmp");
   texture[14] = LoadTexBMP("red.bmp");
   texture[15] = LoadTexBMP("metal.bmp");
   texture[16] = LoadTexBMP("pump1.bmp");
   texture[17] = LoadTexBMP("pump2.bmp");
   texture[18] = LoadTexBMP("pump3.bmp");
   texture[19] = LoadTexBMP("stem.bmp");

   sky[0] = LoadTexBMP("sky0.bmp");
   sky[1] = LoadTexBMP("sky1.bmp");

    //  Create Shader Programs
  shader[0] = CreateShaderProg("texture.vert","texture.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
