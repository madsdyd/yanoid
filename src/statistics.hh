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
/* **********************************************************************
 * This adds a simple statistics gatherer
 * In this first shot, it only tracks framerate 
 * *********************************************************************/

#include <vector>
#include <SDL/SDL.h>

/* It might actually be smarter to use a deque for this, but I am not sure about
   the time it uses to allocate entries */

class TStatistics {
protected:
  typedef std::vector<unsigned int> TFrameTimes;
  typedef TFrameTimes::iterator TFrameTimesIterator;
  TFrameTimes FrameTimes;
  TFrameTimesIterator begin;
  TFrameTimesIterator end;
  TFrameTimesIterator next_element;
  SDL_Surface * StatSurface;
  Uint32 StatColor;
public:
  TStatistics(SDL_Surface * surface, int numframes);
  /* Register a frametime in ms */
  void RegisterFrameTime(unsigned int ms);
  /* Render to surface at x, y */
  void Render(int x, int y);
};
