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
/* Entities are the basic units that can be manipulated in the game */

#ifndef __PIXMAP_ENTITY_HH__
#define __PIXMAP_ENTITY_HH__

#include "entity.hh"

class TPixmapEntity : public TEntity {
protected:
  // The surface consists of several frames in an animation
  SDL_Surface * currentsurface; 
  int surface_width, surface_height;
  // The frame in the currentsurface to blit next time
  int currentframe;
  // Number of ticks to show currentframe
  int ticks_per_frame;
  // Number of ticks the currentframe has been shown
  int ticks_in_current_frame;
  // Number if ticks to wait between to runs of the animation
  int anim_interval;
  // Min and max time for random anim_interval when random 
  // generated
  int anim_random_min_time;
  int anim_random_max_time;
  // Is animation pause interval random
  bool anim_random;
public:
  TPixmapEntity(int x_, int y_, Angle a_, 
		const std::string& pixmap_path,
		EntityType e = "BRICK", 
		MoveType m = STATIONARY, 
		CollisionGranularity c = BOX);
  TPixmapEntity(const TOrientedPoint& p, 
		const std::string& pixmap_path,
		EntityType e = "BRICK", 
		MoveType m = STATIONARY, 
		CollisionGranularity c = BOX);
  static void init(const std::string& path, TPixmapEntity& e);
  virtual ~TPixmapEntity();
  
  virtual void setPixmap(const std::string& pixmap_path);
  /* This is assumes to be a ressource, and it is duplicated */
  virtual void setPixmap(SDL_Surface * pixmap);
  void MakeMask();
  virtual void Update(Uint32 deltatime);
  virtual void Render(SDL_Surface * surface);
  virtual bool pixelCollision(TEntity&);

  /* If the loaded surface represents an animation
     we must tell the dimension of a frame */
  void setDimensions(int w, int h) { setW(w); setH(h); }
  void setAnimSpeed(int tpf) { ticks_per_frame = tpf; }
  void setAnimInterval(int ticks);
  void setAnimIntervalRandom(int min, int max);
private:
  int memand(unsigned char *s1, unsigned char *s2, 
	     int shift1, int shift2, int N);
};


/* Define a list of pixmap entities */
//typedef std::list<TEntity *> TPixmapEntities;
//typedef TEntities::iterator TPixmapEntitiesIterator;

#endif
