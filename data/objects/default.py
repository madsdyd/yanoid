# Load the bricks code

bricks = [ "objects/basic_brick.py" ]

for script in bricks:
    put_console("Executing " + yanoid.resolve(script))
    runthis = "execfile('" + yanoid.resolve(script) + "')"
    eval(runthis)


# Define a number of smaller functions here.
def static_hit():
    PlaySound("sounds/blip.wav")

def paddle_hit():
    PlaySound("sounds/blip.wav")

def ball_hit():
    a = 0
    # print "ball_hit is empty"
    #PlaySound("sounds/blip.wav")

def power_up_add_ball_hit():
    print "in power_up_add_ball_hit()"
    AddBall("graphics/objects/basic_ball.png")
