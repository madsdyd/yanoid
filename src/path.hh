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
 * A path is used to constrain an entity to certain positions and a
 * and/or angles. It is typically used for the paddle to describe 
 * where it can be on the screen. It may also be used to describe 
 * a predetermined behaviour of an enemy.
 * *********************************************************************/

#ifndef __PATH_HH__
#define __PATH_HH__

#include "point.hh"

typedef double TPathPos;

/* Abstract base class for all path types */
class TPath {
protected:
  TPathPos _max;
public:
  TPath() : _max(0.0) {}
  virtual ~TPath() {}
  inline TOrientedPoint getPoint(TPathPos i) const;
  virtual TOrientedPoint getPoint(int i) const = 0;
  virtual inline TPathPos getMax() const { return _max; }
};

/* 
   A simple path that follows a line, and also 
   constrains the orientation by interpolation between 
   the orientation of the begin and en points.
*/
class TOrientedLinePath : public TPath {
protected:
  TOrientedPoint _begin;
  TOrientedPoint _end;
  int _w, _h;
public:
  TOrientedLinePath(const TOrientedPoint& begin, const TOrientedPoint& end);
  virtual ~TOrientedLinePath() {}
  virtual TOrientedPoint getPoint(int i) const;
};

class TOrientedCirkelPath : public TPath {
  
};

#endif
