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
#include <string.h>
#include "ressourcemanager.hh"
#include "interprenter.hh"
#include "log.hh"

/* **********************************************************************
 * The interprenter
 * *********************************************************************/
TInterprenter * Interprenter;

/* A quick hack to test things ... */

/* Declare a function from main */
extern void PrintMe(char *String);

/* Declare a Python extension function */
static PyObject * put_console(PyObject * self, PyObject *args) {
  char * command;
  if (!PyArg_ParseTuple(args, "s", &command))
    return NULL;
  PrintMe(command);
  return Py_None;
};

static PyObject * reload(PyObject * self, PyObject *args) {
  /* Load the yanoid wrapper function */
  char * tmp = strdup(PathManager->Resolve("scripts/yanoid.py").c_str());
  FILE * tmpf = fopen(tmp, "r");
  PyRun_SimpleFile(tmpf, tmp);
  free(tmp);
  PyRun_SimpleString("help()");
  return Py_None;
};

static PyMethodDef yanoid_methods[] = {
  {"puts_console",  put_console,      1},
  {"yreload",  reload,      0},
  {NULL,          NULL}           /* sentinel */
};



/* **********************************************************************
 * The constructor
 * Starts an instance of the Python interprenter
 * *********************************************************************/
TInterprenter::TInterprenter() {
  Py_Initialize();
  if (Py_IsInitialized()) {
    LogLine(LOG_INFO, "Python interprenter initialized");
    LogLine(LOG_TODO, "The code in here must go");
    // LogLine(LOG_INFO, "Version is " << Py_GetVersion());
    PyImport_AddModule("yanoid");
    Py_InitModule("yanoid", yanoid_methods);
    PyRun_SimpleString("import yanoid");
    /* Load the yanoid wrapper function */
    char * tmp = strdup(PathManager->Resolve("scripts/yanoid.py").c_str());
    FILE * tmpf = fopen(tmp, "r");
    PyRun_SimpleFile(tmpf, tmp);
    free(tmp);
  } else {
    LogFatal("Unable to initialize Python interprenter");
    exit(-1);
  }
}

/* **********************************************************************
 * The destructor - shuts down the interprenter 
 * *********************************************************************/
TInterprenter::~TInterprenter() {
  Py_Finalize();
}

/* **********************************************************************
 * Run a simple string
 * *********************************************************************/
bool TInterprenter::RunSimpleString(char * script) {
  return (0 == PyRun_SimpleString(script));
}

bool TInterprenter::RunSimpleString(string script) {
  char * tmp = strdup(script.c_str());
  return (0 == PyRun_SimpleString(tmp));
  free(tmp);
}
