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
#include "statistics.hh"
#include "log.hh"
/* **********************************************************************
 * Constructor - sets up the array to hold a number of entries.
 * *********************************************************************/
TStatistics::TStatistics(SDL_Surface * surface, int numframes) {
  // LogLineExt(LOG_VER_2, ("Stat module created numframes = %i", numframes));
  int i;
  FrameTimes.reserve(numframes);
  for (i = 0; i < numframes; i++) {
    FrameTimes.push_back(10);
  }
  // LogLineExt(LOG_VER_2, ("FrameTimes.size() = %i", FrameTimes.size()));
  begin = FrameTimes.begin();
  end = FrameTimes.end();
  next_element = begin;
  StatSurface = surface;
  StatColor = SDL_MapRGB(surface->format, 255, 0, 0);
}

/* **********************************************************************
 * Register the time for a frame.
 * *********************************************************************/
void TStatistics::RegisterFrameTime(unsigned int ms) {
  /* Insert the element at lasts position and increase the position */
  *next_element = ms;
  next_element++;
  /* Make sure last is not end */
  if (next_element == end) {
    next_element = begin;
  }
}

/* **********************************************************************
 * Render the statistics
 * *********************************************************************/
void TStatistics::Render(int x, int y) {
  SDL_Rect dest;
  dest.w = 1; 
  int count = x;

  /* Iterate over all stat items, twice */
  TFrameTimesIterator i;
  for (i = next_element; i != end; i++) {
    dest.h = *i;
    dest.x = count++; 
    dest.y = y - dest.h;
    SDL_FillRect(StatSurface, &dest, StatColor);
  }
  for (i = begin; i != next_element; i++) {
    dest.h = *i;
    dest.x = count++; 
    dest.y = y - dest.h;
    SDL_FillRect(StatSurface, &dest, StatColor);
  }
}
