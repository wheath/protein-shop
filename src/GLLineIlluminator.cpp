/***************************************************************************************
*cr								
*cr					Copyright (c) 2004, The Regents of the 
*cr	University of California, through Lawrence Berkeley National Laboratory, 
*cr	Univ. of Calif. at Davis, and Lawrence Livermore National Laboratory 
*cr	(subject to receipt of any required approvals from U.S. Dept. of Energy).  
*cr							All rights reserved.
*cr		
*cr		Please see the accompanying LICENSE file for further information.
*cr
***************************************************************************************/

/***********************************************************************
GLLineIlluminator - Class to render illuminated lines.
***********************************************************************/

#undef NONSTANDARD_TEMPLATES
#define NONSTANDARD_TEMPLATES 1

#include <Math/Math.h>
#include <Geometry/ComponentArray.h>
#include <Geometry/Matrix.h>
#include <GLColorOps.h>

#include "GLLineIlluminator.h"

/**********************************
Methods of class GLLineIlluminator:
**********************************/

GLLineIlluminator::GLLineIlluminator(void)
    :sceneCenter(0.0,0.0,0.0),
     autoViewDirection(true),
     autoLightDirection(true),autoLightIndex(GL_LIGHT0)
{
}

void GLLineIlluminator::init(GLContextData& contextData) const
{
    DataItem* dataItem=new DataItem;
    contextData.addDataItem(this,dataItem);
}

void GLLineIlluminator::setMaterial(GLContextData& contextData,GLfloat ambient,GLfloat diffuse,GLfloat specular,GLfloat shininess) const
{
    /* Create a 2D texture map encoding Phong's lighting model: */
    static GLfloat texture[32][32];
    for(int x=0;x<32;++x)
    {
        GLfloat s=2.0f*(GLfloat(x)+0.5f)/32.0f-1.0f;
        GLfloat oneMinusS2=1.0f-s*s;
        GLfloat ambientDiffuse=diffuse;
        ambientDiffuse*=GLfloat(pow(Math::sqrt(oneMinusS2),2.0f));
        ambientDiffuse+=ambient;
        for(int y=0;y<32;++y)
        {
            GLfloat t=2.0f*(GLfloat(y)+0.5f)/32.0f-1.0f;
            GLfloat oneMinusT2=1.0f-t*t;
            GLfloat color=specular;
            color*=GLfloat(pow(Math::abs(Math::sqrt(oneMinusS2*oneMinusT2)-s*t),shininess));
            color+=ambientDiffuse;
            texture[y][x]=color;
        }
    }
    
    /* Get the context data item and upload the created texture: */
    DataItem* dataItem=contextData.retrieveDataItem<DataItem>(this);
    dataItem->materialType=INTENSITY;
    glBindTexture(GL_TEXTURE_2D,dataItem->materialTextureId);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_INTENSITY,32,32,0,GL_LUMINANCE,GL_FLOAT,texture);
    glBindTexture(GL_TEXTURE_2D,0);
}

void GLLineIlluminator::setMaterial(GLContextData& contextData,const GLLineIlluminator::Color& ambient,const GLLineIlluminator::Color& diffuse,const GLLineIlluminator::Color& specular,GLfloat shininess) const
{
    /* Create a 2D texture map encoding Phong's lighting model: */
    static Color texture[32][32];
    for(int x=0;x<32;++x)
    {
        GLfloat s=2.0f*(GLfloat(x)+0.5f)/32.0f-1.0f;
        GLfloat oneMinusS2=1.0f-s*s;
        Color ambientDiffuse=diffuse;
        ambientDiffuse*=GLfloat(pow(Math::sqrt(oneMinusS2),2.0f));
        ambientDiffuse+=ambient;
        for(int y=0;y<32;++y)
        {
            GLfloat t=2.0f*(GLfloat(y)+0.5f)/32.0f-1.0f;
            GLfloat oneMinusT2=1.0f-t*t;
            Color color=specular;
            color*=GLfloat(pow(Math::abs(Math::sqrt(oneMinusS2*oneMinusT2)-s*t),shininess));
            color+=ambientDiffuse;
            texture[y][x]=color;
        }
    }
    
    /* Get the context data item and upload the created texture: */
    DataItem* dataItem=contextData.retrieveDataItem<DataItem>(this);
    dataItem->materialType=RGBA;
    glBindTexture(GL_TEXTURE_2D,dataItem->materialTextureId);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,32,32,0,GL_RGBA,GL_FLOAT,texture);
    glBindTexture(GL_TEXTURE_2D,0);
}

void GLLineIlluminator::setMaterial(GLContextData& contextData,const GLMaterial& material) const
{
    /* Create a 2D texture map encoding Phong's lighting model: */
    static Color texture[32][32];
    for(int x=0;x<32;++x)
    {
        GLfloat s=2.0f*(GLfloat(x)+0.5f)/32.0f-1.0f;
        GLfloat oneMinusS2=1.0f-s*s;
        Color ambientDiffuse=material.getDiffuse();
        ambientDiffuse*=GLfloat(pow(Math::sqrt(oneMinusS2),2.0f));
        ambientDiffuse+=material.getAmbient();
        for(int y=0;y<32;++y)
        {
            GLfloat t=2.0f*(GLfloat(y)+0.5f)/32.0f-1.0f;
            GLfloat oneMinusT2=1.0f-t*t;
            Color color=material.getSpecular();
            color*=GLfloat(pow(Math::abs(Math::sqrt(oneMinusS2*oneMinusT2)-s*t),material.getShininess()));
            color+=ambientDiffuse;
            texture[y][x]=color;
        }
    }
    
    /* Get the context data item and upload the created texture: */
    DataItem* dataItem=contextData.retrieveDataItem<DataItem>(this);
    dataItem->materialType=RGBA;
    glBindTexture(GL_TEXTURE_2D,dataItem->materialTextureId);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_BASE_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAX_LEVEL,0);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,32,32,0,GL_RGBA,GL_FLOAT,texture);
    glBindTexture(GL_TEXTURE_2D,0);
}

void GLLineIlluminator::setViewDirection(const GLLineIlluminator::Vector& newViewDirection)
{
    autoViewDirection=false;
    viewDirection=newViewDirection;
    GLfloat viewDirectionLen=Math::sqrt(Math::sqr(viewDirection[0])+Math::sqr(viewDirection[1])+Math::sqr(viewDirection[2]));
    viewDirection[0]/=viewDirectionLen;
    viewDirection[1]/=viewDirectionLen;
    viewDirection[2]/=viewDirectionLen;
}

void GLLineIlluminator::setLightDirection(const GLLineIlluminator::Vector& newLightDirection)
{
    autoLightDirection=false;
    lightDirection=newLightDirection;
    GLfloat lightDirectionLen=Math::sqrt(Math::sqr(lightDirection[0])+Math::sqr(lightDirection[1])+Math::sqr(lightDirection[2]));
    lightDirection[0]/=lightDirectionLen;
    lightDirection[1]/=lightDirectionLen;
    lightDirection[2]/=lightDirectionLen;
}

void GLLineIlluminator::enableLighting(GLContextData& contextData) const
{
}

void GLLineIlluminator::disableLighting(GLContextData& contextData) const
{
    GLenum previousMatrixMode=glGet<GLint>(GL_MATRIX_MODE);
    
    /* Reset the texture matrix: */
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    
    /* Reset the OpenGL rendering mode: */
    glPopAttrib();
    
    /* Clean up: */
    glMatrixMode(previousMatrixMode);
}
