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
#include "text.hh"
#include "surfacemanager.hh"

using namespace std;

/* **********************************************************************
 * Constructor - loads the font
 * *********************************************************************/
TText::TText(string fontname) {
  font = SurfaceManager->RequireRessource(fontname);
  if (!font) {
    LogLineExt(LOG_ERROR, ("Unable to load font %s", fontname.c_str()));
  }
  if (0 != (font->w % 16)) {
    LogLineExt(LOG_ERROR, 
	       ("Font width of %s must be multiple of 16", fontname.c_str()));
  }
  GlyphWidth = font->w / 16;
  if (0 != (font->h % 16)) {
    LogLineExt(LOG_ERROR, 
	       ("Font height of %s must be multiple of 16", fontname.c_str()));
  }
  GlyphHeight = font->h / 16;
  // cout << "GW, GH " << GlyphWidth << ", " << GlyphHeight << endl;
}
/* **********************************************************************
 * Destructor, leave font
 * *********************************************************************/
TText::~TText() {
  if (font) {
    SurfaceManager->ReleaseRessource(font);
  }
}

/* **********************************************************************
 * Print
 * *********************************************************************/
void TText::Print(SDL_Surface * surface, int x, int y, string text) {
  int size = text.size();
  SDL_Rect src;
  SDL_Rect dest;
  src.w = GlyphWidth;
  src.h = GlyphHeight;
  dest.y = y;
  dest.x = x;
  dest.w = src.w;
  dest.h = src.h;
  unsigned char glyph;
  int i;
  for (i = 0; i < size; i++) {
    glyph = text[i];
    // cout << "Got -" << glyph << "- as a glyph" << endl;
    src.x = GlyphWidth * (glyph % 16);
    src.y = GlyphHeight * (glyph / 16);
    // cout << "Blitting from (" << src.x << ", " << src.y << ")" << endl;
    SDL_BlitSurface(font, &src, surface, &dest);
    dest.x += GlyphWidth;
  }
}

void TText::Print(SDL_Surface * surface, int x, int y, const char ch) {
  SDL_Rect src;
  SDL_Rect dest;
  src.w = GlyphWidth;
  src.h = GlyphHeight;
  dest.y = y;
  dest.x = x;
  dest.w = src.w;
  dest.h = src.h;
  unsigned char glyph;
  glyph = ch; // static_cast<unsigned char>(ch);
  // cout << "Got -" << glyph << "- as a glyph" << endl;
  src.x = GlyphWidth * (glyph % 16);
  src.y = GlyphHeight * (glyph / 16);
  // cout << "Blitting from (" << src.x << ", " << src.y << ")" << endl;
  SDL_BlitSurface(font, &src, surface, &dest);
}
