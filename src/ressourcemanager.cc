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
#include "debug.hh"
#include "log.hh"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

#undef DEBUG_RES_MAN
#ifndef DEBUG_RES_MAN
#ifndef LogLine
#define LogLine(a, b)
#define LogLineExt(a, b)
#endif
#endif

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <windows.h>

#ifndef _POSIX_PATH_MAX
#  define _POSIX_PATH_MAX MAX_PATH
#endif

#define WINPATHFUNCS
#define WINPATHMAPPINGS
#endif

#ifdef WINPATHFUNCS
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C"{
#endif
char *realpath(const char *path,char *outputPath)
{
  char *ret;
  ret = _fullpath(outputPath, path, _POSIX_PATH_MAX);
  if(ret != NULL)
  {
    if ((_access(outputPath, F_OK))==-1)
    {
      ret = NULL;
      errno = ENOFILE;
    }
  }
  return ret;
}

int alphasort (struct dirent **a, struct dirent **b) {
  return strcmp ((*a)->d_name, (*b)->d_name);
}

int scandir(const char *dirname, struct dirent ***namelist,
    int (*select)(struct dirent *),
    int (*compar)(struct dirent **, struct dirent **)) {

  int nDir = 0;
  int i;
  struct dirent **dir, **tmpdir;
  struct dirent *dir2;
  DIR *openedDir;

  dir = tmpdir = NULL;
  
  openedDir = opendir(dirname); 
  while ((dir2 = readdir(openedDir)))
  {
    if(!select || select(dir2))
    {
      
      if((tmpdir = (struct dirent **)realloc(dir, (sizeof(struct dirent) * (nDir+1)))) != NULL){ 
	dir = tmpdir;
	*(dir + nDir) = (struct dirent *)malloc(sizeof(struct dirent));
	memcpy(dir[nDir], (const void*)dir2, sizeof(struct dirent));
	dir[nDir]->d_name = (char *)malloc(sizeof(char)*(dir2->d_namlen+1));
	memcpy(dir[nDir]->d_name, (const void*)(dir2->d_name), (sizeof(char) * (dir2->d_namlen + 1)));

	nDir++;
      }
    }
  }
  free(dir2);
  closedir(openedDir);

  if (compar) qsort (dir, nDir, sizeof(*dir),
		     (int(*)(const void*, const void*))compar);

  *namelist = dir;
  return nDir;
}
#ifdef __cplusplus
}
#endif

#endif


/* **********************************************************************
 * The PathManager
 * *********************************************************************/

TPathManager * PathManager;

/* **********************************************************************
 * The destructor should empty the pathmap
 * *********************************************************************/

TPathManager::~TPathManager() {};

/* **********************************************************************
 * Add a mapping
 * *********************************************************************/
void TPathManager::AddMapping(string name, string absolute) {
  pathmap[name] = absolute;
  if (PathCompleter) {
    PathCompleter->Insert(name);
  }
#ifdef WINPATHMAPPINGS
  int i = 0;
  int replace = 0;
  while(i != -1)
  {
    i = name.find("\\",i);
    if(i!= -1)
    {
      name.replace(i,1,"/");
      replace = 1;
    }
  }
  if(replace)
  {
    pathmap[name] = absolute;
    if (PathCompleter) {
      PathCompleter->Insert(name);
    }
  }
  
#endif
}
/* **********************************************************************
 * Adding a path should insert all filenames found into the map, with
 * their absolut pathnames.
 * *********************************************************************/

void TPathManager::AddPath(string path) {
  char absolute_path[_POSIX_PATH_MAX];
  if (NULL == realpath(path.c_str(), absolute_path)) {
    LogLine(LOG_WARNING, "Unable to get absolute path for " + path);
    return;
  };
  /* Find the length of the base */
  size_t tmp = strlen(absolute_path);
  Assert(tmp != 0, "Problem with obtaining absolute path");
  /* We add one, because any file found will have a directory
     seperator in between */
  AddPathRecursivly(tmp+1, path);
}

/* A helper function, that assures we never see ".." and "." */
int SelectFile(const struct dirent * ent) {
  /* We do not want ".", ".." or things that we can not read */
  if (0 == strcmp(ent->d_name, ".")
      || 0 == strcmp(ent->d_name, "..")) {
    //LogLine(LOG_VER_2, "NOT OK for ");
    //LogLine(LOG_VER_2, ent->d_name);
    return 0;
  } else {
    //LogLine(LOG_VER_2, "OK for ");
    //LogLine(LOG_VER_2, ent->d_name);
    return 1;
  };
}

void TPathManager::AddPathRecursivly(size_t base_size, string path) {
  LogLine(LOG_TRACE, "TPathManager::AddPathRecursivly(" + path + ")");
  
  /* Data structures for handling our path'n stuff 
     buf ends up having the absolute value of the path */
  char absolute_path[_POSIX_PATH_MAX];
  if (NULL == realpath(path.c_str(), absolute_path)) {
    LogLine(LOG_WARNING, "Unable to get absolute path for " + path);
    return;
  };

  /* Save cwd, move to the dir we are investigating */
  char cwd[_POSIX_PATH_MAX];
  if (NULL == getcwd(cwd, _POSIX_PATH_MAX)) {
    LogLine(LOG_WARNING, "Unable to get current working directory");
    return;
  };
  LogLine(LOG_VER_2, cwd);
  if (0 != chdir(absolute_path)) {
    LogLine(LOG_WARNING, "Unable to change to " + path);
    return;
  }

  /* We have changed paths. Must change back on any return */
  
  /* Data structures for scandir */
  int numfiles;
  struct dirent ** files;
  
  numfiles = scandir(".", &files, SelectFile, NULL);
  if (0 > numfiles) {
    /* Some error occured */
    //LogLine(LOG_ERROR, "Error running scandir on " + path);
    LogLine(LOG_WARNING, "Ignoring " + path);
    goto exit;
  }
  
  if (0 == numfiles) {
    goto exit;
  }

  /* Free the stuff that was allocated, while moving the 
     info to a map */
  /* Use some variables */
  char absolute_filename [_POSIX_PATH_MAX];
  struct stat tmpstat;
  char * filename;
  while(numfiles--) {
    dirent * ent = files[numfiles];
    /* Get the realpath */
    if (NULL != realpath(ent->d_name, absolute_filename)) {
      /* Check the type of the d_entry. Files get registered, 
	 dirs are visited */
      stat(absolute_filename, &tmpstat);
      if (S_ISREG(tmpstat.st_mode)) {
	/* Add this file */
	// LogLine(LOG_VERBOSE, "Ready to add");
	// LogLine(LOG_VERBOSE, absolute_filename);
	Assert(strlen(absolute_filename) > base_size,
	       "Problem with base_size and absolute_filename size");
	filename = &absolute_filename[base_size];
	AddMapping(filename, absolute_filename);
      } else {
	if (S_ISDIR(tmpstat.st_mode)) {
	  /* Visit this directory */
	  AddPathRecursivly(base_size, absolute_filename);
	}
      }
      
    } else {
      LogLineExt(LOG_WARNING, ("Could not get real path for %s", 
			    ent->d_name));
    }
    free(files[numfiles]);
  }
  free(files);

 exit:
  /* Move to the old cwd */
  if (0 != chdir(cwd)) {
    LogLine(LOG_ERROR, "Unable to return to previously current directory");
  }
};

/* **********************************************************************
 * Resolve looks up a path in the map, and returns a filename.
 * Returns the empty
 * *********************************************************************/

string TPathManager::Resolve(string name) {
#ifdef DEBUG
  string tmp = pathmap[name];
  if ("" == tmp) {
    LogLine(LOG_WARNING, "Pathmanager could not resolve " + name);
  }
  return tmp;
#else
  return pathmap[name];
#endif
};

