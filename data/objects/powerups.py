# This file defines a number of functions that are related to the powerups

# This interface is probably a bit akward - I know to little python...

# This global variable can be set from the maps, and define
# the probability of NOT spawning a powerup when RandomlySpawnPowerup
# is called, in percent. Default is 90 %, which means that in 10 % of the
# cases, a powerup is spawned
powerup_spawn_probability = 90;

from random import randrange

# This function spawns a powerup
def RandomlySpawnPowerup():
    test = randrange (0, 100, 1)
    print test
    if (test > powerup_spawn_probability):
        # Spawn one from the list
        powerup_spawn_ball()


powerups = [ "powerup_spawn_ball()" ]

# Here comes the actual functions

def powerup_spawn_ball():
    yanoid_map.PowerUp("powerup", "graphics/powerups/yellow_l.png",
                       "powerup_ball_hit()")
    
def powerup_ball_hit():
    yanoid_map.PowerUp("spawn-ball", "graphics/balls/red.png",
                       "ball_hit()")
