/*
    Yet Another Arkanoid
    Copyright (C) 2001 Mads Bondo Dydensborg <mads@dydensborg.dk>
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
#include "surfacemanager.hh"
#include "log.hh"
#include <SDL/SDL_image.h>

/* **********************************************************************
 * The surface manager
 * *********************************************************************/
TSurfaceManager * SurfaceManager;

/* **********************************************************************
 * Load and realease
 * *********************************************************************/
SDL_Surface * TSurfaceManager::LoadRessource(string filename) {
  return IMG_Load(filename.c_str()); 
}

void TSurfaceManager::UnloadRessource(SDL_Surface * surface) {
  LogLine(LOG_TODO, "Implement TSurfaceManager::UnloadRessource");
}
