# BEER BRAWL

A strategy game for 2 players and 12 blinks.

## LORE:

Stop me if you've heard this one before. A ragtag bunch of adventurers walks into a tavern seeking a quest.

The bartender says "Oh, well, there's only one treasure dungeon around here, and the entrance is right behind the bar. There's an evil
lich down there brewing an unholy potion it plans to unleash on the population. If you get that potion back here, my alchemist friend
can deactivate it, and I'll reward you handsomely. But be warned--the potion brainwashes people to fight on his behalf, so you might meet
fierce resistance on the way. And if they get to the counteragents in my draft cellar, we won't be able to counteract that potion!"

Little did the adventurers know, the dungeon was really just a tunnel leading to the rival tavern on the other side of town.
The two barkeeps are constantly trying to steal each other's beer by sending every ragtag bunch of unsuspecting adventurers that comes through down
to do their dirty work. So, of course, halfway down the tunnel, the ragtag bunch of unsuspecting adventurers meet another ragtag bunch of unsuspecting
adventurers--each defending a cask of beer against an enemy they believed to be demon-possessed, each trying to steal the other's cask for a great reward.

## BOARD:

A 12.5" by 20" square board is needed, divided into a 5x8 grid of 2.5"x2.5" squares. 
One can easily make one from gridded foamboard, which is sold in 15"x20" at Walmart.
The rows of 5 are the ranks and the columns of 8 are the files. The board should be
situated lengthwise between the players.

## SETUP:

After training, all 12 blinks will be flashing orange. Each player takes a cluster of 6. One player clicks any blink to select the blue team.
Both players double-click any blink to go to drafting mode.

All blinks in the cluster will show a random collection of stats:
- Red, the strongest color, represents STRENGTH, and determines melee attack/defense power.
- Purple, the color of magic, represents MAGIC, and determines spellcasting attack/defense power.
- Yellow, the color of lightning, represents SPEED, and determines how far an adventurer can move each turn.
- Green, the color of safety, represents ARMOR, and determines how many pieces of armor an adventurer can wear.

Clicking any blink in the cluster rerolls the stats for all of them. Any time a blink displays a set of stats that the player deems useful,
that blink can be long pressed to send it to ready mode, removed from the cluster, and added to the board in one of the three ranks closest
to the player.

Players get one minute (or some other agreed-upon period of time) to draft all their adventurers. The first player to place all their blinks
in ready mode on the board goes first. At the end of the allotted time, all remaining unplayed blinks must be put in ready mode and placed on the board
without any further rerolling. If players cannot decide who placed their party first, one player may promise to buy the other a drink of their choice in
order to go first.

Note that the stats-rolling process takes place in secret--neither player should know the stats of any of their opponent's adventurers. Stats
will not be visible when the blink is in ready mode.

## GAMEPLAY:

Play proceeds in turns, and each turn has two phases.

The first phase is movement. The player selects and adventurer and clicks it to put it into active/movement mode. It will appear bright and
spinning to signify it is able to move. The player may move it forward, backward, left, or right to any adjacent square. (It may not move diagonally.)
After moving one square, the player clicks once to indicate the move is complete. The blink will either flash yellow and return to spinning (if it can be
moved again) or flash red and return to ready (dim and breathing) mode if it cannot be moved further. The number of moves a piece gets is equal to its
SPEED plus 1 (SPD+1). 

After the first move, all further movement is optional. A piece can be triple-clicked to return it to ready mode immediately without revealing any further
information about the adventurer's speed.

The second phase may consist either of combat or rearmament and is also entirely optional UNLESS the active adventurer is going to end the turn on the same
square that it started, in which case a combat MUST take place--no turn may have no net effect on the visible game state.

A player may choose to attach the adventurer that was moved to any horizontally or vertically adjacent adventurer.

## COMBAT

If an adventurer is attached to an adventurer from the other team, clicking it will trigger an automatic knockout battle. After the battle,
one blink will turn sparkling green to signify victory, and the other will turn red to indicate it has been knocked unconscious. The winning adventurer remains in its square and the dead
blink is detached and removed from the board.

Internally, this is how a battle is conducted:
- The adventurer that was clicked takes the first turn. It flips a coin to determine whether it will take a swing with STRENGTH or cast a spell with MAGIC.
- If the other adventurer has more STRENGTH or MAGIC (whichever was used), it will block the attack and make a counterstrike/counterspell using the same
  stat, which will certainly succeed in scoring a hit. Otherwise (STRENGTH or MAGIC is less or equal to that of the attack), the adventurer will take the 
  hit itself.
- When an adventurer takes a hit from a strike or spell, it sacrifices one piece of armor to block the attack.
- If an adventurer has no remaining armor to block the attack, it dies.
- If both adventurers survive the round, the defending adventurer becomes the attacker for the next round, and this continues until one of them dies.

## REARMAMENT

If an adventurer is attached to another member of its own party, either of them can be clicked in order to cause it to try to give a piece of armor to the
other. If it has no armor to give or the other does not have any armor slots to put it in, this will fail.

An animation will be shown on both blinks regardless of which was clicked and regardless of whether a piece of armor was actually transferred. As such, it
is possible (and recommended) to bluff about armor transfers. For example, by placing a finger on both blinks when clicking, one can hide which direction
the armor is being moved if at all.

Multiple pieces of armor may be transmitted on the same turn, including back and forth, but only between the two adventurers that have been attached. Once
all armor has been transferred, the adventurers are separated to their respective squares.

## BONUS ACTIONS

At any time, regardless of whose turn it is, either player may pick up any of their own adventurers and double-click to have it briefly display its stats. If it
has lost armor in a battle, this will be apparent as an unlit face.

Players may also opt to allow multiple second phases per turn as a rule--for example, combat followed by immediate rearmament--as long as no further movement
occurs.

## REVIVEMENT

This gameplay mechanic is optional, and can be used to extend the length of the game. As unconscious adventurers are removed from the board, they are taken by 
the player who controls them and linked together. Once at the beginning of each turn, any unconscious adventurer in the cluster can be clicked once. Each 
unconscious adventurer has a 1/6 chance of waking up and returning to ready mode--however, none of its broken armor will be restored.

In lieu of a movement phase, the player may take any single conscious adventurer and return it to the board in any open square on its own team's back rank. 
If a combat or rearmament action is performed in the second phase of this turn, this revived adventurer must participate.

## GOAL

If either player has a living adventurer on their opponent's back row at the end of their opponent's turn, they win. A player can also win by eliminating
their opponent's entire party. 

When the game is over, cluster all the blinks and long press any of them to reset the game.

## EXTRA CHALLENGE:

Players can mark certain squares on the board as impassable or difficult terrain (requiring two moves to cross) before the game starts. For example, each player
could get a "lake token" to place on any square that cannot be occupied.

## STRATEGY IDEAS:

Here are some ways that various adventurers can be used:

HEALER: 3 SPEED and 3 ARMOR. Keep this adventurer close to home as it is very likely to lose any battles. However, it can rush to the aid of a stronger warrior 
on the other side of the board to rearm it after a battle.

PARAMEDIC: 5 SPEED and 1 ARMOR. Like a healer, but can reach almost any adventurer on the board.

WARRIOR: 4 STRENGTH and 2 ARMOR.
MAGE: 4 MAGIC and 2 ARMOR.
They can't move fast, but there's a good chance they'll win at least one battle.

SCOUT: 3 STRENGTH or MAGIC and 3 SPEED. Rush to a part of the board that is poorly defended, or attack an adventurer that is being protected.

GLASS CANNON: 5 MAGIC or STRENGTH and 1 ARMOR. 75% chance of scoring at least one hit. Useful for taking out adventurers who are believed to have no (remaining)
armor.

MULTICLASS: 1 STRENGTH, 1 MAGIC, 4 ARMOR. Specifically designed to counter adventurers that are spec'ed entirely into STRENGTH or MAGIC. Can also be used
as a healer.

ALL-ROUNDER: 3 STRENGTH or MAGIC, 2 MAGIC or STRENGTH, 1 ARMOR. Will defeat weak opponents, can be used to test their power and wear down the armor of stronger
adventurers.

LANCER: 3 STRENGTH or MAGIC, 2 SPEED, 1 ARMOR. Likely to win a first battle it starts against any but the most imbalanced adventurers, and can dart into
battle quickly.

ROGUE: 6 SPEED. Keep it on your back or second rank. If the way is clear and the enemy is overextended or spread out, move all the way to the opponent's
back rank in a single turn, out of reach of any defenders.

QUARTERMASTER: 6 ARMOR. If playing with the adventurer revivement rule, keep this on your second rank in front of an empty square. It can be used to
rearmor revived adventurers as soon as they return to the board.

