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
#include "ressourcemanager.hh"
#include "log.hh"
/* **********************************************************************
 * The PathManager
 * *********************************************************************/

TPathManager * PathManager;

/* **********************************************************************
 * The destructor should empty the pathmap
 * *********************************************************************/

TPathManager::~TPathManager() {
  LogLine(LOG_WARNING, "Need to empty the PathMap");
};

/* **********************************************************************
 * Adding a path should insert all filenames found into the map, with
 * their absolut pathnames.
 * *********************************************************************/

void TPathManager::AddPath(string path) {
  LogLine(LOG_VERBOSE, "PathManager is adding " + path);
  LogLine(LOG_INFO, "AddPath not implemented");
};

/* **********************************************************************
 * Resolve looks up a path in the map, and returns a filename.
 * Returns the empty
 * *********************************************************************/

string TPathManager::Resolve(string filepath) {
  LogLine(LOG_INFO, "Resolve not implemented");
  return "";
};
