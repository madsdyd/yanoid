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

#include "motion.hh"
#include <math.h>
#include "debug.hh"

TPathMotion::TPathMotion(TPath* p, double vel) : _max(0.0)
{
  _current_time = 0;
  _vel = vel;
  if (p) {
    _paths.push_back(p);
    _max = p->getMax();
  }
}

TPathMotion::~TPathMotion()
{
  for(std::vector<TPath*>::iterator i = _paths.begin() ;
      i != _paths.end() ; ++i) {
    delete *i;
  }
}

void TPathMotion::addPath(TPath* p)
{
  _paths.push_back(p);
  _max += p->getMax();
}

void TPathMotion::Update(Uint32 deltatime, TEntity& e)
{
  // This function should really be rewritten to
  // be much more efficient than now!!
  if (! _paths.size() )
    return;

  _current_pos += (static_cast<double>(deltatime)*_vel)/10.0;
  TPath* p = 0;
  double ppos = 0.0;
  if (_current_pos >= _max) {
    _current_pos = _max;
    p = _paths.back();
    ppos = p->getMax();
  }else{
    double curMax = 0.0;
    double oldMax = 0.0;
    std::vector<TPath*>::iterator i = _paths.begin();
    for ( ; i != _paths.end() ; ++i) {
      oldMax = curMax;
      curMax += (*i)->getMax();
      if (curMax >= _current_pos) 
	break;
    }
    p = *i;
    ppos = _current_pos - oldMax;
  }

  Assert(p != 0, "Path in TPathMotion was NULL."); 

  e.position = p->getPoint(static_cast<int>(ppos));
}


