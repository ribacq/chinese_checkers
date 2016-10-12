#include "game.h"

int link_accessible_from(Content** b, int side, Hex h){
	/* Uses the link() function to make a display of cells that are
	 * currently accessible on the board from given hex.
	 * Returns the number of these cells.
	 */
	
	//Wrong input: source must not be empty.
	if(get_ct(b, side, h) == EMPTY) return 0;

	//Correct input
	int len;
	Hex* neighs = neighbors(side, h, &len);
	int i;
	for(i=0; i<len; i++){
		if(get_ct(b, side, neighs[i]) == EMPTY){
			link(side, h, neighs[i]);
		}
	}
	return len;
}

