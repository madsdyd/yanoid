# Python file that defines a _very_ simple map

# Define a hit function for testing purposes
def powerup_ball_spawn():
    yanoid_map.PowerUp("powerup", "graphics/powerups/yellow_l.png",
                       "powerup_ball_hit()")
    
def powerup_ball_hit():
    yanoid_map.PowerUp("spawn-ball", "graphics/balls/red.png",
                       "ball_hit()")

# Add a number of bricks
for x in range(6, 750, 79):
    for y in range(91, 250, 29):
        if (y != 207):
            row = y % 3
            if row == 0:
                AddBrick("brick", x, y, "basic_brick_hit()",
                         "graphics/bricks/blue_75.png")
            elif row == 1:
                AddBrick("brick", x, y, "basic_brick_hit()",
                         "graphics/bricks/green_75.png")
            else:
                AddBrick("brick", x, y, "basic_brick_hit()",
                         "graphics/bricks/red_75.png")
        else:
            if (x % 2 == 1):
                AddBrick("brick-stay-3", x, y, "basic_brick_hit()",
                         "graphics/bricks/yellow_stay_75.png")
            else:
                AddBrick("brick", x, y,
#                         "power_up_add_ball_hit()",
                         "powerup_ball_spawn()",
                         "graphics/bricks/red_75.png")
                
            
# Add the walls
StandardPlayField()

# Add a paddle
SetPaddle(366, 450, "graphics/paddles/square2_75.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/balls/red.png")

# And, call it - probably will go
RoundStart();
