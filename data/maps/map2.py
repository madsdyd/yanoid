# Python file that defines a _very_ simple map

# Add a number of bricks
for x in range(60, 540, 50):
    for y in range(30, 120, 18):
        if (y <= 30):
            AddBrick(x, y, "graphics/objects/weird_brick.png")
        elif (y <= 60):
            AddBrick(x, y, "graphics/objects/red_oval_brick.png")
        else:
            AddBrick(x, y, "graphics/objects/basic_brick.png")
            
# Add the walls
AddStatic(0, 0, 10, 400)
AddStatic(630, 0, 10, 400)
AddStatic(10, 0, 620, 10)
AddStatic(0, 400, 640, 100)

# Add a paddle
SetPaddle(300, 300, "graphics/objects/basic_paddle.png")
