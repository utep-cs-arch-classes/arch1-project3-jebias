Playing the game:

Move bottom paddle (player 1) with S1 and S2.

Move top paddle (player 2) with S3 and S4.

The first player to score twice will win the game.

The buzz sound works, but I couldn't figure out how to make it stop. The sound plays when the ball bounces off of a paddle, but it doesn't stop. I had planned on only making it stop the buzz after a second, but that didn't work in time.

For the assembly portion I have an init.c file that I converted to assembly, which is the init.s file. I hope that counts.

My state machine is pretty simple, with a switch statement that only has two cases. The two states are playing the game and ending that game.
