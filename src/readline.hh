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
/* This implements a very simple class to mimick gnu readline(3) */
#include <string>
#include <vector>
#include <SDL/SDL.h>

class TText; /* To support rendering of glyphs */
class TReadline {
protected:
  int cursorpos;
  string currentline;
  string savedline;
  int savedpos;
  // string * value;
  static string killed;
  vector<string> backlog; /* Of lines/commands */
  int nextbacklogcommand;
  unsigned int maxlinesinbacklog;
  bool isreadingline;
  void AdjustCursor();
public:
  TReadline(int backlogsize);
  void Render(SDL_Surface * surface, int x, int y, string prompt, TText * textrender);
  bool HandleEvent(SDL_Event * event);
  /* Call this, when you want readline to start accepting events */
  void Readline() {
    isreadingline = true; 
    currentline = ""; 
    cursorpos = 0;
  };
  /* And this, when you want to check if readline is done with a line
     (enter was pressed) */
  bool IsReadingLine() { return isreadingline; };
  /* And this, if you wish to stop the readline */
  void StopReadingLine() { isreadingline = false; };
  /* And, this after figuring out that we are no longer reading */
  string GetCurrentLine() { return currentline; };
};
