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
#include "text.hh"
#include "debug.hh"
#include "screen.hh"
#include <math.h>

TTextEffect::TTextEffect(const char *str_, TText * _TextRender) :
  str(str_), TextRender(_TextRender)
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
  
  int slen = static_cast<int>(strlen(str_));
  int cw = //DT_FontWidth(*font) * slen;
    TextRender->GetGlyphWidth() * slen;
  int ch = // DT_FontHeight(*font);
    TextRender->GetGlyphHeight();
  background = SDL_CreateRGBSurface(SDL_HWSURFACE, cw,
				    ch, 32,
				    rmask, gmask, bmask, amask);
  if(background == NULL) {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
    exit(1);
  }
  
  char_points = vector<TPoint>(slen);
}


TTextEffect::~TTextEffect() 
{
  cerr << "erasiong bg" << endl;
  if (background) {
    SDL_FreeSurface(background);
    background = 0;
  }
}


void
TTextEffectDisplay::doEffect(Uint32 currenttime) {

  TextRender->Print(surface, 	      
		    static_cast<int>(Location.x()), 
		    static_cast<int>(Location.y()),
		    str);
}


void 
TTextEffectSpaced::doEffect(Uint32 currenttime)
{
  int fontwidth = TextRender->GetGlyphWidth();
  double timepct = static_cast<double>(currenttime-begin_time)/static_cast<double>(Duration-1000);
  int characters = static_cast<int>(strlen(str));
  
  /* Now draw it */
  int count=0;
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    double candtp = timepct - static_cast<double>(characters-count) * 
      (1.0/static_cast<double>(characters));
    double newtp = (candtp >= 1) ? 1 : candtp; 
    double _x = sin(newtp*M_PI_2) * Location.x();
    i->setX(_x + count * fontwidth);
    i->setY(Location.y());
    count++;
  }
  saveBackground();
  blit();
}


void 
TTextEffectJumping::doEffect(Uint32 currenttime)
{
  int fontwidth = TextRender->GetGlyphWidth();
  int fontheight = TextRender->GetGlyphHeight();
  int characters = static_cast<int>(strlen(str));
  double timepct = static_cast<double>(currenttime-begin_time)/static_cast<double>(Duration-1000);
  int _i = static_cast<int>(timepct * static_cast<double>(characters));
  int _y = static_cast<int>(Location.y());
  int _x = (surface->w - fontwidth * characters) / 2;
  int count = 0;
  
  /* Now draw it */
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    if (count > 1 && count == (_i-2)) { 
      i->setY(_y + fontheight/4);
    } else if ( count > 0 && count == (_i-1)) { 
      i->setY(_y + fontheight/2);
    } else if (count == (_i)) { 
      i->setY(_y + fontheight);
    } else if (count != (characters-2) && count == (_i+1)) { 
      i->setY(_y + fontheight/2);
    } else if (count != (characters-3) && count == (_i+2)) { 
      i->setY(_y + fontheight/4);
    } else {
      i->setY(_y);
    }
    i->setX(_x - characters/2 + count * fontwidth);
    count++;
  }
  saveBackground();
  blit();
}


void 
TTextEffectSwirling::doEffect(Uint32 currenttime)
{
  int count = 0;
  int characters = static_cast<int>(strlen(str));
  double timepct = static_cast<double>(currenttime-begin_time) /
    static_cast<double>(Duration-post_hold_time);
  timepct = (timepct > 1.0) ? 1.0 : timepct;
  count = 0;
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    int scale = (count - characters/2) * TextRender->GetGlyphWidth();
    i->setX( static_cast<double>(surface->w/2) + 
	     static_cast<double>(scale) * cos(timepct * 2.0 * M_PI) ); 
    i->setY( static_cast<double>(surface->w/2) + 
	     static_cast<double>(scale) * sin(timepct * 2.0 * M_PI) ); 
    count++;
  }

  saveBackground();
  blit();
}


void TTextEffect::blit() 
{
  int count = 0;
  // Print the character to the right place on the screen
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; ++i) {
    TextRender->Print(surface,
		      static_cast<int>(i->x()),
		      static_cast<int>(i->y()), 
		      str[count]);
    count++;
  }
}

void TTextEffect::saveBackground()
{
  SDL_Rect SourceRect, DestRect;
  int count = 0;
  int font_width = TextRender->GetGlyphWidth();
  int font_height = TextRender->GetGlyphHeight();

  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {

    SourceRect.x = static_cast<short int>(i->x());
    SourceRect.y = static_cast<short int>(i->y());
    SourceRect.w = static_cast<short int>(font_width);
    SourceRect.h = static_cast<short int>(font_height);
    
    DestRect.x = static_cast<short int>(count * font_width);
    DestRect.y = 0;
    DestRect.w = static_cast<short int>(SourceRect.w);
    DestRect.h = static_cast<short int>(SourceRect.h);
 
   /* save the background */
    SDL_BlitSurface(surface, &SourceRect, background, &DestRect);
    count++;
  }

}

void TTextEffect::blitBackground()
{
  SDL_Rect SourceRect, DestRect;
  int count = 0;
  int font_width = TextRender->GetGlyphWidth();
  int font_height = TextRender->GetGlyphHeight();

  // Now blit back the backgrounds for each charaters
  for (std::vector<TPoint>::iterator i = char_points.begin();
       i != char_points.end() ; i++) {
    DestRect.x = static_cast<short int>(i->x());
    DestRect.y = static_cast<short int>(i->y());
    DestRect.w = static_cast<short int>(font_width);
    DestRect.h = static_cast<short int>(font_height);
    
    SourceRect.x = static_cast<short int>(count * font_width);
    SourceRect.y = 0;
    SourceRect.w = DestRect.w;
    SourceRect.h = DestRect.h;
    count++;
    /* draw the background */
    SDL_BlitSurface(background, &SourceRect, surface, &DestRect);
  }
}





