# This script defines a number of interfaces to be used
# for "client" like interaction.

######################################################################
# Define console interaction functions
######################################################################

# Define a function to put a python object that can be converted into
# a string - such as a string or a number - onto the console
def put_console(a):
    "Wrapper to put a string printable on the yanoid console"
    yanoid_client.PutConsole(str(a))

# Play a sound
def PlaySound(soundname):
    "Play a sound"
    yanoid_client.PlaySound(soundname)

# Load a sound (used for caching)
def LoadSound(soundname):
    "Load a sound"
    yanoid_client.LoadSound(soundname)

# Load a surface (used for caching)
def LoadSurface(surfacename):
    "Load a surface"
    yanoid_client.LoadSurface(surfacename)

