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

#ifndef __TEXTEFFECTS__
#define __TEXTEFFECTS__

#include "text.hh"
#include "effects.hh"
#include <vector>
#include "point.hh"

class TText;
class TTextEffect : public TEffect {
protected:
  std::vector<TPoint> char_points;
  const char* str;
  TText * TextRender;
  // Helper functions
  void blit();
  virtual void saveBackground();
  virtual void blitBackground();
public:
  TTextEffect(const char *str, TText * _TextRender);
  ~TTextEffect();
};


class TTextEffectDisplay : public TTextEffect {
public:
  TTextEffectDisplay(const char* str, TText * _TextRender) :
    TTextEffect(str,_TextRender) {}
  virtual void doEffect(Uint32 currenttime);
};


class TTextEffectSpaced : public TTextEffect {
public:
  TTextEffectSpaced(const char* str, TText * _TextRender) :
    TTextEffect(str,_TextRender) {}
  virtual void doEffect(Uint32 currenttime);
};


class TTextEffectJumping : public TTextEffect {
public:
  TTextEffectJumping(const char* str, TText * _TextRender) :
    TTextEffect(str,_TextRender) {}
  virtual void doEffect(Uint32 currenttime);
};


class TTextEffectSwirling : public TTextEffect {
public:
  TTextEffectSwirling(const char* str, TText * _TextRender) :
    TTextEffect(str,_TextRender) {}
  virtual void doEffect(Uint32 currenttime);
};

#endif







