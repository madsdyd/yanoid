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

/* This unit declares to very simple menus, a pre game menu, and an in game menu 
   These are very simpleminded things. */

class TMenu {
private:
  
public:
  TMenu(); 
  ~TMenu();
}

class TPreGameMenu {
private:
  
public:
  TPreGameMenu();
  ~TPreGameMenu();
  /* This procedure takes over everything, which is probably not
     a good idea, but easiest to do now 
     returns true if the player wants to start a new game, false if
     we wants to exit */
  bool Run();
};

class TInGameMenu {
private:
  
public:
  T
}
