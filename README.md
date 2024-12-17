# Adventure-dungoen-game
Game Structure:
Item, Creature, Rooms, Player
Game Intialization:
Create a few rooms, connects them, and populates them with items or creature.
The implement Commands:
look: displayes the description of the current room, lists any item or creature present in room
move <direction>: moves the player in the specified directionif possible, check the room connections by means of move (up, down, left and right)
displayes an appropriate message if the move is not possible
inventory: lists the items the player has collected, displayes empty if the inventory empty
pickup: allows the player to pickup an item in the current room, check if the inventory is full before picking up item
attack: allows the player to attack a creature in the current room, the attack uses a turn based combat system
if the player is health drops to zero the game ends
save: save the player state
exit: ends the game
