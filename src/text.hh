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
#ifndef __TEXT_HH__
#define __TEXT_HH__

#include <string>
#include <SDL/SDL.h>

using namespace std;

/* Implement a text renderer */
class TText {
private:
  SDL_Surface * font;
  unsigned short int GlyphWidth;
  unsigned short int GlyphHeight;
public:
  /* The fontname should point to a 16x16 glyph image */
  TText(string fontname);
  ~TText();
  /* Return the width and height of a glyph */
  inline unsigned short int GetGlyphWidth() { return GlyphWidth; }
  inline unsigned short int GetGlyphHeight() { return GlyphHeight; }
  /* Print string to surface at x, y 
     Note, the surface is not updated ...
   */
  void Print(SDL_Surface * surface, int x, int y, string text);
  void Print(SDL_Surface * surface, int x, int y, const char ch);
};

#endif
