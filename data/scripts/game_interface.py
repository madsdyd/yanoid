# This script defines a number of interfaces to be used when the
# scripts need to change the game state.

# AdjustScore(int dscore)
# Add dscore (can be negative) to the current score
def AdjustScore(dscore):
    "Adjust the current score"
    yanoid_game.AdjustScore(dscore)
