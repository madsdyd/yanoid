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
#include "log.hh"

/* **********************************************************************
 * The PathManager
 * *********************************************************************/

/* The pathmanager maps from a name - which is a partial path - (eg
   graphics/fonts/menu_font.png) to an absolute path - a filename (eg
   /usr/local/share/yanoid/graphics/fonts/menu_font.png).
   Paths added will override earlier mappings. This way we can
   experiment with new (or partially new) datasets */
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
  string Resolve(string name);
};

/* Our global pathmanager */
extern TPathManager * PathManager;

/* **********************************************************************
 * The acutal ressource managers
 * *********************************************************************/

/* The overall strategy is to have a general ressource manager, that
   will have several handlers registered to handle the different
   ressources. Note - the only point is to be able to access ressources,
   and share references to them. We perform no memory management
   we assume that this will never be a problem and let the OS handle it
   Clearly we are not developing for a console machine .. ;-) */

/* The base ressource handler uses templates to abstract away the type of
   the ressource to be managed - one will need to inherit from them, 
   to make a usefull manager. */
typedef int THandle;

template <typename res_t>
class TRessourceManager {
protected:
  /* The ressources that have been loaded from disk are stored
     within the handlers */
  class storage_t {
  protected:
    THandle handle; /* Handle assigned by the manager, and our 
		   index into the storage */
    string name; /* Ressource name */
    res_t * ressource;
    int hrefs, rrefs; 
  public:
    storage_t(THandle _handle, string _name, res_t * _ressource) {
      handle    = _handle;
      name      = _name; 
      ressource = _ressource;
      hrefs     = 0;
      rrefs     = 0;
    };
    string GetName() { return name; };
    THandle TakeHandle() { ++hrefs; return handle; };
    void ReleaseHandle() { --hrefs; };
    res_t * TakeRessource() { ++rrefs; return ressource; };
    void ReleaseRessource() { --rrefs; };
    
  }; /* storage_t */

  /* **********************************************************************
   * Variables for the ressource manager, etc.
   * *********************************************************************/
  THandle NextHandle;
  
  /* The actual ressource are store in here */
  typedef std::map<THandle, storage_t *> THandleToRessourceMap;
  typedef THandleToRessourceMap::iterator THandleToRessourceMapIterator;
  /* This is for looking up handles, when we are required to load a name */
  typedef std::map<string, THandle> TNameToHandleMap;
  typedef TNameToHandleMap::iterator TNameToHandleMapIterator;
  THandleToRessourceMap HandleToRessourceMap;
  TNameToHandleMap NameToHandleMap;

  /* **********************************************************************
   * Virtual methods to ensure that different types of ressouces can be 
   * loaded, etc.
   * *********************************************************************/
  
  /* Load ressouce should do anything that is required to make
     the res_t ressource valid, from a given filename */
  virtual res_t * LoadRessource(string filename) = 0;
  /* UnloadRessource should totally get rid of the ressource -
     res will be meaningless afterwards and will never be referenced */
  virtual void UnloadRessource(res_t *) = 0;
  
public:
  /* **********************************************************************
   * This is the meat of the handler
   * *********************************************************************/
  TRessourceManager() { NextHandle = 1;};
  virtual ~TRessourceManager() {
    LogLine(LOG_TODO, "Free ressources and storage_t in maps");
  };
  /* Require will load or locate the ressource, and return a handle
     for it */
  THandle RequireHandle(string name);
  /* Release will let the manager know that the handle is no longer
     needed - the mapping between handle and data may be lost */
  void ReleaseHandle(THandle handle);
  /* This is for getting access to the actual data */
  res_t * RequireRessource(THandle handle);
  /* And this invalidates the pointer */ 
  void ReleaseRessource(res_t * res);
};



/* **********************************************************************
 * The RessourceManager class comes here
 * *********************************************************************/

/* **********************************************************************
 * Manipulating handles.
 * *********************************************************************/

template <typename res_t>
THandle TRessourceManager <res_t>::RequireHandle(string name) {
  /* Check if name is already mapped, return that */
  THandle result = NameToHandleMap[name];
  if (0 != result) {
    LogLine(LOG_VERBOSE, "Reused handle for " + name);
    return result;
  };
  
  /* Not loaded. Load ressource, create record, insert into 
     handleresource map, insert into namehandle map, return handle */
  string filename = PathManager->Resolve(name);
  if ("" == filename) {
    LogLine(LOG_ERROR, "Unable to resolve " + name);
    return 0;
  };
  res_t * ressource = LoadRessource(filename);
  if (NULL == ressource) {
    LogLine(LOG_ERROR, "Unable to load ressource " + filename);
    return 0;
  };

  storage_t * storage = new storage_t(NextHandle, name, ressource);
  if (NULL == storage) {
    LogLine(LOG_ERROR, "Unable to allocate storage_t");
    return 0;
  };
  /* Should be OK now */
  result = NextHandle++;
  LogLine(LOG_VERBOSE, "Inserting storage for " + name);
  NameToHandleMap[name] = result;
  HandleToRessourceMap[result] = storage;
  return storage->TakeHandle();
};


template <typename res_t>
res_t * TRessourceManager <res_t>::RequireRessource(THandle handle) {
  if (0 == handle) {
    LogLine(LOG_ERROR, "RequireRessource with invalid handle");
    return NULL;
  }
  storage_t * tmp = HandleToRessourceMap[handle];
  if (tmp) {
    return tmp->TakeRessource();
  } else {
    LogLine(LOG_ERROR, "Handle != 0, no storage_t in map");
    return NULL;
  }
}

#endif
