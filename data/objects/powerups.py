# This file defines a number of functions that are related to the powerups

# This interface is probably a bit akward - I know to little python...

# This global variable can be set from the maps, and define
# the probability of spawning a powerup when RandomlySpawnPowerup
# is called, in percent. Default is 19 %, which means that in 10 % of the
# cases, a powerup is spawned
# NOTE: There are currently no way to reset this value. 

powerup_spawn_probability = 10;

from random import randrange

# The powerups are defined in here. The first number in each sublist
# is the "weight" of the powerup.
powerups = [ [10, "powerup_spawn_ball()"],
             [5, "powerup_spawn_shot()"] ]
totalweights = 15

# This function spawns a powerup and is the one scripts should call.
def RandomlySpawnPowerup():
    if (randrange(0, 100, 1) > (100 - powerup_spawn_probability)):
        # Spawn one from the list
        weight = randrange(0, totalweights, 1)
        sum = 0
        for powerup in powerups:
            sum = sum + powerup[0]
            if (weight <= sum):
                # Use this powerup
                eval(powerup[1])
                break
                
# Here comes the actual functions - this could be done smarter, I guess

# Extra ball
def powerup_spawn_ball():
    yanoid_map.PowerUp("powerup", "graphics/powerups/yellow_l.png",
                       "powerup_ball_hit()")
    
def powerup_ball_hit():
    yanoid_map.PowerUp("spawn-ball", "graphics/balls/red.png",
                       "ball_hit()")

# Shot
def powerup_spawn_shot():
    yanoid_map.PowerUp("powerup", "graphics/powerups/blue_1UP.png",
                       "powerup_shot_hit()")
    
def powerup_shot_hit():
    yanoid_map.PowerUp("spawn-ball", "graphics/powerups/yellow_l.png",
                       "ball_hit()")

