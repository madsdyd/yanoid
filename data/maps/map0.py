# Python file that defines a _very_ simple map

# Add a number of bricks
AddBrick("brick", 460, 50, "basic_brick_hit()",
         "graphics/objects/basic_brick.png")
            
# Add the walls
AddEntity("static", -10, -10, 11, 620, "static_hit()")
AddEntity("static", 799, -10, 10, 620, "static_hit()")
AddEntity("static", 1, -10, 798, 11, "static_hit()")
AddEntity("hole", 1, 550, 798, 50, "")

# Add a paddle
SetPaddle(400, 400, "graphics/objects/paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();
