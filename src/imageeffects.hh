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

#ifndef __IMAGE_EFFECTS__
#define __IMAGE_EFFECTS__

#include "effects.hh"
#include <vector>

/*
class TImageInplaceEffects : public TEffect {
public:
  typedef enum EffectType { SIMPLE_DISPLAY, 
  } EffectType;
protected:
  std::vector<TPoint> char_points;
  EffectType type;
  SDL_Surface * surface;
public:
  TImageInplaceEffects(const char *str, SDL_Surface *surface, fonthandle_t * Font, EffectType et);
  ~TImageInplaceEffects();
  void update(Uint32 currenttime);

private:
  //
  // All the effects
  //
  void simpleDisplay(Uint32 currenttime);

  //
  // Helper functions
  //
  void blitChars();
  void saveCharsBackground();
  void blitCharsBackground();
};

*/

class TImageParticleEffect : public TEffect {
protected:
  struct particle_t {
    TPoint point;
  };
  std::vector<particle_t> particles;
  SDL_Surface * image;
  int img_w;
  int img_h;
  //
  // Helper functions
  //
  void blit();
  virtual void saveBackground();
  virtual void blitBackground();

public:
  TImageParticleEffect(SDL_Surface* Image,
		       int w, int h);
  ~TImageParticleEffect();
};


class TImageExplodeAllDirEffect : public TImageParticleEffect {
public:
  TImageExplodeAllDirEffect(SDL_Surface* img,
			    int w, int h) :
    TImageParticleEffect(img, w, h) {}
  virtual void doEffect(Uint32 currenttime);
};


class TImageImplodeAllDirEffect : public TImageParticleEffect {
public:
  TImageImplodeAllDirEffect(SDL_Surface* img, int w, int h) :
    TImageParticleEffect(img, w, h) {}
  virtual void doEffect(Uint32 currenttime);
};


class TImageExplodeHorizontalBlocksEffect : public TImageParticleEffect {
public:
  TImageExplodeHorizontalBlocksEffect(SDL_Surface* img,
			    int w, int h) :
    TImageParticleEffect(img, w, h) {}
  virtual void doEffect(Uint32 currenttime);
};


/*
class TImageExplodeHorizontalEffect : public TImageParticleEffect {
public:
TImageExplodeAllDirEffect(SDL_Surface* Image, 
EffectType et) :
  TImageParticleEffect(image) {}
  virtual void doEffect(Uint32 currenttime);
}


class TImageBuildImageEffect : public TImageBuildImageEffect {
public:
  TImageBuildImageEffect(SDL_Surface* Image, 
  EffectType et) :
  TImageParticleEffect(image) {}
  virtual void doEffect(Uint32 currenttime);
}
*/
#endif

