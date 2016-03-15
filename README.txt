Describe how and what each group member contributed for the past two weeks. 
If you are solo, these points will be part of the next section.
I am the only person in my group.


Document the improvements that your group made to your AI to make it tournament-worthy. 
Explain why you think your strategy(s) will work. 
Feel free to discuss any ideas were tried but didn't work out.

I had little time to improve my AI due to medical difficulties, 
so rather than implementing a new alogorithm (i.e. alpha-beta pruing), I chose to imporove upon minimax.
Minimax normally uses a score of (# of my color)-(# of other color).  This is quite weak.
I created a new function getHeuristic that takes into account peice number of both sides, motility 
of both sides, # edge peices of both sides, and # of corner peices of both sides.

I set up minimax to use getHeuristic until there are only 10 moves left in the game, and then use getScore, 
since at the end the final winner is determined by score, not my heuristic
[getScore = (# of my color)-(# of other color) ]

The number-of-moves-left cutoff value of 10 was determined by testing many values and seeing which 
resulted in a highest win % against ConstantTimePlayer.  I found 10 to be best.

The weights for peice#, motility, cornerpeice#, and edgepeice# (as shown below) were determined via 
the same method of trying values and picking the ones that resulted in the highest win %.


int Board::getHeuristic(Side mySide)
{
	int score = 0;
	if (mySide == BLACK)
	{
		score = countBlack() - countWhite();
		score += getMoves(BLACK).size() - getMoves(WHITE).size();
		score += 7 * getCornerNumber(BLACK) - 7 * getCornerNumber(WHITE);
		score += 3 * getEdgeNumber(BLACK) - 3 * getEdgeNumber(WHITE);
	}
	else
	{
		score = countWhite() - countBlack();
		score += getMoves(WHITE).size() - getMoves(BLACK).size();
		score += 7 * getCornerNumber(WHITE) - 7 * getCornerNumber(BLACK);
		score += 3 * getEdgeNumber(WHITE) - 3 * getEdgeNumber(BLACK);
	}
	return score;
}


Though this modified minimax is not the best, it will best constant time player very reliably.
