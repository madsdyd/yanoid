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
/* This provides a general ressource manager class, using templates */
#ifndef __RESSOURCEMANAGER_HH__
#define __RESSOURCEMANAGER_HH__
#include <map>
#include <string>
#include <vector>

/* **********************************************************************
 * The PathManager
 * *********************************************************************/

/* The pathmanager maps from a "filepath" (eg graphics/fonts/menu_font.png)
   to /usr/local/share/yanoid/graphics/fonts/menu_font.png)
   Paths added will override earlier mappings. This way we can experiment
   with new (or partially new) datasets */
typedef map <string, string> TPathMap;
typedef TPathMap::iterator TPathMapIterator;

class TPathManager {
protected:
  TPathMap pathmap;
  void AddPathRecursivly(size_t base_size, string path);
  void AddMapping(string name, string absolute);
public:
  TPathManager() {};
  ~TPathManager();
  /* Add path will add the files found in that path to our list of 
     path mappings. Files found will override earlier paths */
  void AddPath(string path);
  /* This will return a absolute path to a file from the 
     filepath name. Will return an empty string, if file not found. */
  string Resolve(string filepath);
};

/* Our global pathmanager */
extern TPathManager * PathManager;

/* **********************************************************************
 * The acutal ressource managers
 * *********************************************************************/

#ifdef NEVER
/* The overall strategy is to have a general ressource manager, that
   will have several handlers registered to handle the different
   ressources */

/* The base ressource handler uses templates to abstract away the type of
   the ressource to be managed */
template <typename res_t>
class TRessourceHandler {
protected:
  /* The ressources that have been loaded from disk are stored
     within the handlers */
  class storage_t {
  public:
    string id;
    res_t ressource;
    int refs; /* Manipulated from the outside */
    storage_t(string _id, res_t _ressource) {
      id = _id; ressource = _ressouce;
    }
  }; /* storage_t */
  /* The actuaæ ressource are located in here */
  std::vector<storage_t> ressources;
}
#endif /* NEVER */

#endif
