# This script defines a number of interfaces to be used
# for "client" like interaction.

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

