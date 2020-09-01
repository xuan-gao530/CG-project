/*
 *  Set projection
 */
#include "CSCIx229.h"

void Project(int mode,int fov, double dim, double asp,double radius)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode==0)
      gluPerspective(fov,asp,dim/4,4*dim);
   // first person navigation
   else 
      gluPerspective(fov,asp,radius*2,12*radius);
  // Orthogonal projection
  // else 
    //  glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matri
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

