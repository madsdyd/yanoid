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

#include "texteffects.hh"
#include "ConsoleSource/DT_drawtext.h"
#include "debug.hh"
#include "screen.hh"
#include <math.h>

TTextEffects::TTextEffects(const char *str_, SDL_Surface *surface_, 
			  fonthandle_t * Font, EffectType et) :
  type(et), surface(surface_), str(str_), font(Font)
{

}

void
TTextEffects::update(Uint32 currenttime)
{
  switch (type) {
  case SIMPLE_DISPLAY:
    simpleDisplay(currenttime);
    break;
  case CHARACTER_SPACED_ANIM:
    characterSpacedAnim(currenttime);
    break;
  default:
    Assert(0,"Tried to make a texteffect that doesn't exist");
  }
  
}

void
TTextEffects::simpleDisplay(Uint32 currenttime) {

  static Uint32 begin_time = 0;

  if (!begin_time)
    begin_time = currenttime;

  if ( (currenttime - begin_time) > Duration) {
    stop();
    begin_time = 0;
    return;
  }

  if (!surface || !str || !font)
    return;


  DT_DrawText(str , surface, *font, 
	      static_cast<int>(Location.x()), 
	      static_cast<int>(Location.y()));
}

void 
TTextEffects::characterSpacedAnim(Uint32 currenttime)
{
  static SDL_Surface * background = 0;
  int bgx,bgy;
  int loop;
  int characters;
  SDL_Rect SourceRect, DestRect;
  BitFont *CurrentFont;

  static Uint32 begin_time = 0;

  if (!begin_time)
    begin_time = currenttime;

  if ( (currenttime - begin_time) > Duration) {
    stop();
    begin_time = 0;
    return;
  }

  if ( (currenttime - begin_time) > Duration-500)
    return;

  if (!surface || !str || !font)
    return;

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


  if (!background) {
    
    //    bgx = DT_FontWidth(*font) * strlen(str);
    bgy = DT_FontHeight(*font);
    bgx = surface->w;
    background = SDL_CreateRGBSurface(SDL_HWSURFACE, bgx,
				      bgy, 32,
				      rmask, gmask, bmask, amask);
    if(background == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }


    DestRect.x = 0;
    DestRect.y = 0;
    DestRect.w = bgx;
    DestRect.h = bgy;
    
    SourceRect.y = static_cast<int>(Location.y());
    SourceRect.x = 0;
    SourceRect.w = bgx;
    SourceRect.h = bgy;
    
    /* save the background */
    SDL_BlitSurface(surface, &SourceRect, background, &DestRect);
  }

  DestRect.x = 0;
  DestRect.y = static_cast<int>(Location.y());
  DestRect.w = bgx;
  DestRect.h = bgy;

  SourceRect.x = 0;
  SourceRect.y = 0;
  SourceRect.w = bgx;
  SourceRect.h = bgy;

  /* draw the background */
  SDL_BlitSurface(background, &SourceRect, surface, &DestRect);

  int fontwidth = DT_FontWidth(*font);
  int fontheight = DT_FontHeight(*font);

  double timepct = static_cast<double>(currenttime-begin_time)/static_cast<double>(Duration-1000);
  double _x = sin(timepct*M_PI_2) * Location.x();
  DestRect.x = static_cast<Uint32>(_x);
  DestRect.y = static_cast<int>(Location.y());
  DestRect.w = fontwidth;
  DestRect.h = fontheight;

  SourceRect.y = 0;
  SourceRect.w = fontwidth;
  SourceRect.h = fontheight;
  
  characters = strlen(str);

  CurrentFont = DT_FontPointer(*font);
  
  /* Now draw it */
  for(loop=0; loop<characters; loop++) {
    double candtp = timepct - static_cast<double>(characters-loop) * 
      (1.0/static_cast<double>(characters));
    double newtp = (candtp >= 1) ? 1 : candtp; 
    double _x = sin(newtp*M_PI_2) * Location.x();
    DestRect.x = static_cast<Uint32>(_x) + loop * fontwidth;
    SourceRect.x = str[loop] * fontwidth;
    SDL_BlitSurface(CurrentFont->FontSurface, &SourceRect, surface, &DestRect);
    DestRect.x += CurrentFont->CharWidth;
  }

}



