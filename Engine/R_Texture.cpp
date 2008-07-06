#include "R_Texture.h"

#include "U_Collection.h"
#include "JK_Colormap.h"

#include "JK_GOB.h"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

void R_Texture::Register(U_VectorMap<JK_Colormap*> &colormaps)
{
	int i, j, x;
	int cel;
	UCHAR *tempData;
	
	names=new unsigned int*[colormaps.size()];
	for(i=0;i<colormaps.size();i++)
	{
		names[i]=new unsigned int[numCels];
		glGenTextures(numCels,names[i]);
		for(j=0;j<numCels;j++)
		{
			glBindTexture(GL_TEXTURE_2D,names[i][j]);
			
			if(transparent)
			{
				tempData=new UCHAR[sizeX*sizeY*4];
				for(x=0;x<sizeX*sizeY;x++)
				{
					tempData[x*4]=colormaps[i]->Palette(data[j][x]).r;
					tempData[x*4+1]=colormaps[i]->Palette(data[j][x]).g;
					tempData[x*4+2]=colormaps[i]->Palette(data[j][x]).b;
					if(data[j][x]==0)
						tempData[x*4+3]=0;
					else
						tempData[x*4+3]=255;
				}
			
				gluBuild2DMipmaps(GL_TEXTURE_2D, 4, sizeX, sizeY, GL_RGBA, GL_UNSIGNED_BYTE, tempData);
				delete[] tempData;
			}
			else
			{
				tempData=new UCHAR[sizeX*sizeY*3];
				for(x=0;x<sizeX*sizeY;x++)
				{
					tempData[x*3]=colormaps[i]->Palette(data[j][x]).r;
					tempData[x*3+1]=colormaps[i]->Palette(data[j][x]).g;
					tempData[x*3+2]=colormaps[i]->Palette(data[j][x]).b;
				}
				
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_RGB, GL_UNSIGNED_BYTE, tempData);
				delete[] tempData;
			}
		
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	
		}
	}
}

void R_Texture::Select(int colormap, int cel)
{
	glBindTexture(GL_TEXTURE_2D, names[colormap][cel]);
}

int R_Texture::SizeX()
{
	return sizeX;
}

int R_Texture::SizeY()
{
	return sizeY;
}

bool R_Texture::NeedsBlending()
{
	return transparent;
}

int R_Texture::NumCels()
{
	return numCels;
}
/*
char *R_Texture::GetName()
{
	return name;
}

void R_Texture::SetNum(int i)
{

}*/