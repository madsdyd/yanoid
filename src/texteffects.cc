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
  
  int slen = strlen(str_);
  int cw = DT_FontWidth(*font) * slen;
  int ch = DT_FontHeight(*font);
  background = SDL_CreateRGBSurface(SDL_HWSURFACE, cw,
				    ch, 32,
				    rmask, gmask, bmask, amask);
  if(background == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  
  char_points = vector<TPoint>(slen);
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
  case CHARACTER_JUMPING_ANIM:
    characterJumpingAnim(currenttime);
    break;
  case CHARACTER_SWIRLING_ANIM:
    characterSwirlingAnim(currenttime);
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
  static int bgx,bgy;
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


void 
TTextEffects::characterJumpingAnim(Uint32 currenttime)
{
  static SDL_Surface * background = 0;
  static int bgx,bgy;
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
    bgy = DT_FontHeight(*font) * 2;
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
  characters = strlen(str);

  double timepct = static_cast<double>(currenttime-begin_time)/static_cast<double>(Duration-1000);
  int _i = static_cast<int>(timepct * static_cast<double>(characters));
  int _y = static_cast<int>(Location.y());
  DestRect.x = (surface->w - fontwidth * characters) / 2;
  DestRect.y = _y;;
  DestRect.w = fontwidth;
  DestRect.h = fontheight;

  SourceRect.y = 0;
  SourceRect.w = fontwidth;
  SourceRect.h = fontheight;
  
  CurrentFont = DT_FontPointer(*font);
  
  /* Now draw it */
  for(loop=0; loop<characters; loop++) {
    if (loop > 1 && loop == (_i-2)) { 
      DestRect.y =  static_cast<int>(_y + fontheight/4);
    } else if ( loop > 0 && loop == (_i-1)) { 
      DestRect.y =  static_cast<int>(_y + fontheight/2);
    } else if (loop == (_i)) { 
      DestRect.y =  static_cast<int>(_y + fontheight);
    } else if (loop != (characters-2) && loop == (_i+1)) { 
      DestRect.y =  static_cast<int>(_y + fontheight/2);
    } else if (loop != (characters-3) && loop == (_i+2)) { 
      DestRect.y =  static_cast<int>(_y + fontheight/4);
    } else {
      DestRect.y =  static_cast<int>(_y);
    }
    SourceRect.x = str[loop] * fontwidth;
    SDL_BlitSurface(CurrentFont->FontSurface, &SourceRect, surface, &DestRect);
    DestRect.x += CurrentFont->CharWidth;
  }

}


void 
TTextEffects::characterSwirlingAnim(Uint32 currenttime)
{

  if (!surface || !str || !font)
    return;

  // now blit back the backgrounds for each charaters
  int count = 0;
  if (!no_erase) {
    blitCharsBackground();
  }else{
    no_erase = false;
    begin_time = currenttime;
  }

  if ( (currenttime - begin_time) > Duration) {
    stop();
    begin_time = 0;
    SDL_FreeSurface(background);
    background = 0;
    char_points.clear();
    return;
  }
  if ( (currenttime - begin_time) > Duration-500)
    return;

  int characters = strlen(str);
  double timepct = static_cast<double>(currenttime-begin_time)/static_cast<double>(Duration-1000);
  timepct = (timepct > 1.0) ? 1.0 : timepct;
  count = 0;
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    int scale = (count - characters/2) * DT_FontWidth(*font);
    i->setX( static_cast<double>(surface->w/2) + 
	     static_cast<double>(scale) * cos(timepct * 2.0 * M_PI) ); 
    i->setY( static_cast<double>(surface->w/2) + 
	     static_cast<double>(scale) * sin(timepct * 2.0 * M_PI) ); 
    count++;
  }

  saveCharsBackground();
  blitChars();
}

void TTextEffects::blitChars() 
{

  SDL_Rect SourceRect, DestRect;
  BitFont *CurrentFont;
  CurrentFont = DT_FontPointer(*font);
  int count = 0;
  int font_width = DT_FontWidth(*font);
  int font_height = DT_FontHeight(*font);

  // OK now blit the character to the right place on the screen
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {

    SourceRect.x = str[count] * font_width;
    SourceRect.y = 0;
    SourceRect.w = font_width;
    SourceRect.h = font_height;

    DestRect.x = static_cast<int>(i->x());
    DestRect.y = static_cast<int>(i->y());
    DestRect.w = SourceRect.w;
    DestRect.h = SourceRect.h;
    
    count++;
    /* draw the characters */
    SDL_BlitSurface(CurrentFont->FontSurface, &SourceRect, surface, &DestRect);
  }

}

void TTextEffects::saveCharsBackground()
{
  SDL_Rect SourceRect, DestRect;
  int count = 0;
  int font_width = DT_FontWidth(*font);
  int font_height = DT_FontHeight(*font);

  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {

    SourceRect.x = static_cast<int>(i->x());
    SourceRect.y = static_cast<int>(i->x());
    SourceRect.w = font_width;
    SourceRect.h = font_height;
    
    DestRect.x = count * font_width;
    DestRect.y = 0;
    DestRect.w = SourceRect.w;
    DestRect.h = SourceRect.h;

    /* save the background */
    SDL_BlitSurface(surface, &SourceRect, background, &DestRect);
    count++;
  }

}

void TTextEffects::blitCharsBackground()
{
  SDL_Rect SourceRect, DestRect;
  int count = 0;
  int font_width = DT_FontWidth(*font);
  int font_height = DT_FontHeight(*font);

  // now blit back the backgrounds for each charaters
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    DestRect.x = static_cast<int>(i->x());
    DestRect.y = static_cast<int>(i->y());
    DestRect.w = font_width;
    DestRect.h = font_height;
    
    SourceRect.x = count * font_width;
    SourceRect.y = 0;
    SourceRect.w = DestRect.w;
    SourceRect.h = DestRect.h;
    count++;
    /* draw the background */
    SDL_BlitSurface(background, &SourceRect, surface, &DestRect);
  }
}

