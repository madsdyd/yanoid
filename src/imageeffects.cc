/*
    Yet Another Arkanoid
    Copyright (C) 2001 Mads Bondo Dydensborg <mads@dydensborg.dk>
    Copyright (C) 2001 Jonas Christian Drewsen <jcd@xspect.dk>
    Copyright (C) 2001 contributers of the yanoid project
    Please see the file "AUTHORS" for a list of contributers

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "imageeffects.hh"
#include "debug.hh"
#include <stdlib.h>
#include <math.h>

TImageParticleEffect::TImageParticleEffect(SDL_Surface *img,
					   int w, int h) :  
  image(img), img_w(w), img_h(h)
{
  /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
     as expected by OpenGL for textures */
  Uint32 rmask, gmask, bmask, amask;
  
  /* SDL interprets each pixel as a 32-bit number, so our masks must depend
     on the endianness (byte order) of the machine */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  
  if(img == NULL) {
    fprintf(stderr, "CreateRGBSurface of image failed: %s\n", SDL_GetError());
    exit(1);
  }

  background = SDL_CreateRGBSurface(SDL_HWSURFACE, img->w,
				    img->h, 32,
				    rmask, gmask, bmask, amask);
  if(background == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  
  particles = vector<particle_t>(w * h);

  unsigned int count = 0;
  for (std::vector<particle_t>::iterator iter = particles.begin() ;
       iter != particles.end() ; ++iter) {
    iter->point = TPoint(count % w, 
			 count / w);
    count++;
  }

}


TImageParticleEffect::~TImageParticleEffect() 
{
  if (background) {
    SDL_FreeSurface(background); 
    background = 0;
  }
}


void 
TImageExplodeAllDirEffect::doEffect(Uint32 currenttime)
{
  double timepct = static_cast<double>(currenttime-begin_time) /
    static_cast<double>(Duration-post_hold_time);
  timepct = (timepct > 1.0) ? 1.0 : timepct;
  double scalex = 3.0;
  double scaley = 2.0;
  int count = 0;

  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; i++) {
    double dx = 
      static_cast<double>(count % img_w) -
      static_cast<double>(img_w) / 2.0;
    dx = dx / (static_cast<double>(img_w) / 2.0);

    double dy = 
      static_cast<double>(count / img_h) -
      static_cast<double>(img_h) / 2.0;
    dy = dy / (static_cast<double>(img_h) / 2.0);

    double candx = i->point.x() + dx * scalex; 
    double candy = i->point.y() + dy * scaley; 
    i->point.setX( candx );
    i->point.setY( candy );
    count++;
  }

  saveBackground();
  blit();
}


void 
TImageImplodeAllDirEffect::doEffect(Uint32 currenttime)
{
  double timepct = static_cast<double>(currenttime-begin_time) /
    static_cast<double>(Duration-post_hold_time);
  timepct = (timepct > 1.0) ? 1.0 : timepct;
  double scalex = 0.05;
  double scaley = 0.05;
  int count = 0;
  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; i++) {
    double dx = 
      static_cast<double>(count % img_w) -
      static_cast<double>(img_w) / 2.0;

    double dy = 
      static_cast<double>(count / img_h) -
      static_cast<double>(img_h) / 2.0;

    i->point.setX( i->point.x() - dx * scalex); 
    i->point.setY( i->point.y() - dy * scaley) ; 
    count++;
  }

  saveBackground();
  blit();
}


void 
TImageExplodeHorizontalBlocksEffect::doEffect(Uint32 currenttime)
{
  double timepct = static_cast<double>(currenttime-begin_time) /
    static_cast<double>(Duration-post_hold_time);
  timepct = (timepct > 1.0) ? 1.0 : timepct;
  double scalex = 5.0;
  int blocksize = 6;
  int count = 0;
  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; i++) {
    double dx = 
      static_cast<double>(count % img_w);
    dx = dx / static_cast<double>(img_w);

    if ((count / (img_w * blocksize)) % 2)
      dx = -1.0 + dx;

    double candx = i->point.x() + dx * scalex; 
    i->point.setX( candx );
    count++;
  }

  saveBackground();
  blit();
}

void TImageParticleEffect::blit() 
{
  int count = 0;
  // Print the character to the right place on the screen

  SDL_LockSurface( surface );
  SDL_LockSurface( image );
  int maxIndex = surface->w * surface->h;
  int bpp = surface->format->BytesPerPixel;

  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; ++i) {

    int x = (unsigned int)(i->point.x() + Location.x());
    int y = (unsigned int)(i->point.y() + Location.y()) * surface->w;
    int index = ((x < 0) ? 0 : ((x > surface->w) ? surface->w : x)) + y;

    if (index >= maxIndex)
      continue;

    switch(bpp) {
    case 1:
      ((Uint8*)surface->pixels)[index] = ((Uint8*)image->pixels)[count];
      break;
    case 2:
      ((Uint16*)surface->pixels)[index] = ((Uint16*)image->pixels)[count];
      break;
    case 3: {
      int i = index * 3;
      int c = count * 3;
      ((Uint8*)surface->pixels)[i] = ((Uint8*)image->pixels)[c];
      ((Uint8*)surface->pixels)[i+1] = ((Uint8*)image->pixels)[c+1];
      ((Uint8*)surface->pixels)[i+2] = ((Uint8*)image->pixels)[c+2];
      break;
    }
    case 4:
      ((Uint32*)surface->pixels)[index] = ((Uint32*)image->pixels)[count];
    default:
      Assert(0,"Error blitting pixels");
    }

    count++;
  }
  SDL_UnlockSurface( image );
  SDL_UnlockSurface( surface );
}


void TImageParticleEffect::saveBackground()
{
  int count = 0;
  SDL_LockSurface( surface );
  SDL_LockSurface( background );
  // Print the character to the right place on the screen
  int maxIndex = surface->w * surface->h;
  int bpp = surface->format->BytesPerPixel;

  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; ++i) {
    int x = (unsigned int)(i->point.x() + Location.x());
    int index = ((x < 0) ? 0 : ((x > surface->w) ? surface->w : x)) + 
      (int)(i->point.y() + Location.y()) * surface->w;

    if (index >= maxIndex)
      continue;

    switch(bpp) {
    case 1:
      ((Uint8*)background->pixels)[count] = ((Uint8*)surface->pixels)[index];
      break;
    case 2:
      ((Uint16*)background->pixels)[count] = ((Uint16*)surface->pixels)[index];
      break;
    case 3: {
      int i = index * 3;
      int c = count * 3;
      ((Uint8*)background->pixels)[c] = ((Uint8*)surface->pixels)[i];
      ((Uint8*)background->pixels)[c+1] = ((Uint8*)surface->pixels)[i+1];
      ((Uint8*)background->pixels)[c+2] = ((Uint8*)surface->pixels)[i+2];
      break;
    }
    case 4:
      ((Uint32*)background->pixels)[count] = ((Uint32*)surface->pixels)[index];
    default:
      Assert(0,"Error blitting pixels");
    }

    count++;
  }
  SDL_UnlockSurface( surface );
  SDL_UnlockSurface( background );
}


void TImageParticleEffect::blitBackground()
{
  int count = 0;
  SDL_LockSurface( surface );
  SDL_LockSurface( background );
  // Print the character to the right place on the screen
  int maxIndex = surface->w * surface->h;
  int bpp = surface->format->BytesPerPixel;

  for (std::vector<particle_t>::iterator i = particles.begin();
       i != particles.end() ; ++i) {
    int x = (unsigned int)(i->point.x() + Location.x());
    int index = ((x < 0) ? 0 : ((x > surface->w) ? surface->w : x)) + 
      (int)(i->point.y() + Location.y()) * surface->w;
    if (index >= maxIndex)
      continue;
    switch(bpp) {
    case 1:
      ((Uint8*)surface->pixels)[index] = ((Uint8*)background->pixels)[count];
      break;
    case 2:
      ((Uint16*)surface->pixels)[index] = ((Uint16*)background->pixels)[count];
      break;
    case 3: {
      int i = index * 3;
      int c = count * 3;
      ((Uint8*)surface->pixels)[i] = ((Uint8*)background->pixels)[c];
      ((Uint8*)surface->pixels)[i+1] = ((Uint8*)background->pixels)[c+1];
      ((Uint8*)surface->pixels)[i+2] = ((Uint8*)background->pixels)[c+2];
      break;
    }
    case 4:
      ((Uint32*)surface->pixels)[index] = ((Uint32*)background->pixels)[count];
    default:
      Assert(0,"Error blitting pixels");
    }

    count++;
  }
  SDL_UnlockSurface( surface );
  SDL_UnlockSurface( background );
}







