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

#ifndef __MOTION_HH__
#define __MOTION_HH__

#include <SDL/SDL.h>
#include "path.hh"
#include <vector>
#include "entity.hh"
#include <math.h>

class TMotion {
protected:
  double _vel;
  Uint32 _current_time;
public:
  virtual void setVelocity(double vel) { _vel = vel; }
  virtual double getVelocity() { return _vel; }
  virtual void Update(Uint32 deltatime, TEntity& e) = 0;
  void setDirection(bool forward) { _vel = (forward) ? fabs(_vel): -fabs(_vel); }
  bool getDirection() const { return (_vel >= 0.0); }
  void reverseDirection() { _vel = - _vel; }
};

class TNullMotion : public TMotion {
protected:
  int _x;
  int _y;
public:
  TNullMotion(int x, int y) : _x(x), _y(y) {}
  virtual ~TNullMotion() {}
  virtual void Update(Uint32 deltatime, TEntity& e) { e.position.setX(_x); e.position.setY(_y); }
  void setX(int x_) { _x = x_; }
  void setY(int y_) { _y = y_; }
  int x() const { return _x; }
  int y() const { return _y; }
};

class TPathMotion : public TMotion {
protected:
  std::vector<TPath*> _paths;
  TPathPos _max;
  TPathPos _current_pos;
public:
  TPathMotion(TPath* p = 0, double vel = 0 );
  virtual ~TPathMotion();
  void addPath(TPath* p);
  virtual void Update(Uint32 deltatime, TEntity& e);
};

#endif
