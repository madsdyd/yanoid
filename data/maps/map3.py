# Python file that defines map3

# Add a number of bricks
for x in range(0, 9):
        AddBrick(400-x*40, 50+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")
        AddBrick(400+x*40, 50+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")
        AddBrick(400-x*40, 150+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")
        AddBrick(400+x*40, 150+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")

for x in range(0, 3):
        AddBrick(400-x*40, 250+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")
        AddBrick(400+x*40, 250+x*20, "basic_brick_hit()",
                 "graphics/objects/red_oval_brick.png")

for x in range(-3, 4):
        AddBrick(400-x*40, 320, "basic_brick_hit()",
                 "graphics/objects/weird_brick.png")
            
# Add the walls
AddEntity("static", -10, -10, 11, 620, "static_hit()")
AddEntity("static", 799, -10, 10, 620, "static_hit()")
AddEntity("static", 1, -10, 798, 11, "static_hit()")
AddEntity("hole", 1, 550, 798, 50, "static_hit()")

# Add a paddle
SetPaddle(400, 500, "graphics/objects/basic_paddle.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/objects/basic_ball.png")

# And, call it - probably will go
RoundStart();
