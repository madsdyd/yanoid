# Python file that defines a _very_ simple map

SetMapName("Bonus level - by MBD")



# Add a number of bricks
for x in range(6, 750, 79):
    for y in range(2, 9, 1):
        row = y % 7
        AddEntity("brick-delay", x, y, x*15 + (7-row)*2000, 0,
                  "RandomlySpawnBonusPowerup()")
                
            
# Add the walls
StandardPlayField()

# Add a paddle
SetPaddle(366, 530, "graphics/paddles/square2_75.png")

# define a round start function
def RoundStart():
    AddBallDefault("graphics/balls/red.png")

# And, call it - probably will go
RoundStart();


