# This script is the most "basic" of all the scripts in yanoid.
# It defines a number of functions for basic communication between the
# main program and the interprenter, and loads a number of other
# scripts in the different subdirectories.

# NOTE: All modules that the main program exports, MUST be exported
# prior to running this script.

def put_console(a):
    "Temporary function"
    print(a)

def ReloadScripts():
    "Reload the yanoid modules"
    yanoid.yreload()


######################################################################
# Load the different submodules.
######################################################################

# Initialize the preload sounds and preload surfaces, and define two functions
# to add to the list
preload_sounds   = [ "sounds/pop.wav",
                     "sounds/fire.wav",
                     "sounds/peep.wav",
                     "sounds/bang_on_metal.wav"]
preload_surfaces = []

# The item you put in here, should be a list in it self
def PreloadAddSoundList(sound):
    global preload_sounds
    preload_sounds = preload_sounds + sound

# The item you put in here, should be a list in it self
def PreloadAddSurfaceList(surface):
    global preload_surfaces
    preload_surfaces = preload_surfaces + surface

# A list of python scripts to run
# We collect them here
# Note, that the order may matter.
default_scripts = [ "scripts/client_interface.py",
                    "scripts/game_interface.py",
                    "scripts/map_interface.py",
                    "objects/default.py"
                    ]

# OK; this is weird, but I was totally unable to get the
# execfile to work otherwise....
for script in default_scripts:
    put_console("Executed " + yanoid.resolve(script))
    runthis = "execfile('" + yanoid.resolve(script) + "')"
    eval(runthis)


######################################################################
# A simple help function - will probably go.
def help():
    "Print some help"
    put_console("")
    put_console("Interprenter help(TM)")
    put_console("---------------------")
    put_console("Try one of the following commands")
    put_console("(remember the i in front and trailing())")
    put_console("put_console(a) - where a is a python expression")
    put_console("ReloadScripts() - reloads scripts/yanoid.py")
    put_console("                   note the _y_")
    put_console("examples:")
    put_console("  i a = [ 'this', 'is', 'a', 'list' ]")
    put_console("  i put_console(a)")
    put_console("  i yanoid.yreload()")


######################################################################
# Load the preload lists
for sound in preload_sounds:
    put_console("Caching sound " + yanoid.resolve(sound))
    LoadSound(sound)

for surface in preload_surfaces:
    put_console("Caching surface " + yanoid.resolve(surface))
    LoadSurface(surface)

######################################################################
# Write a message
put_console("Yanoid scripting engine initialized")




             


