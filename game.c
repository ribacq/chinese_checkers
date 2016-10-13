#include "game.h"

void link_accessible_from(Content** b, const int side, int* nb, Hex* cells){
	/* Uses the link() function to make a display of cells currently
	 * accessible on the board from the last hex in given array.
	 */
	
	//Wrong input: source not on board
	if(get_zone(side, cells[*nb-1]) == NOWHERE) return;

	//Correct input
	int nb_neighs;
	Hex h = cells[*nb-1];
	Hex* neighs = neighbors(side, h, &nb_neighs);
	Hex tmp_h;
	int i, j, k, in_cells;
	for(i=0; i<nb_neighs; i++){
			
		mvprintw(10, 52, "h: (%d, %d)", h.r, h.q);
		for(k=0; k<*nb; k++){
			mvprintw(k+11, 52, "| %d:(%d, %d)", k, cells[k].r, cells[k].q);
			refresh();
		}
		
		if(get_ct(b, side, neighs[i]) != EMPTY){
			//Jump access is looked up wether or not we’re on our
			//source or not.
			tmp_h.r = h.r + 2*(neighs[i].r - h.r);
			tmp_h.q = h.q + 2*(neighs[i].q - h.q);
			if(get_ct(b, side, tmp_h) == EMPTY){
				link(side, h, neighs[i]);
				link(side, neighs[i], tmp_h);
				in_cells = 0;
				for(j=0; j<*nb; j++)
					in_cells = in_cells || (tmp_h.r == cells[j].r && tmp_h.q == cells[j].q);
				if(!in_cells){
					cells[(*nb)++] = tmp_h;
					//Trace the rest of the path
					link_accessible_from(b, side, nb, cells);
				}
			}
		}else if(get_ct(b, side, h) != EMPTY){
			//Source is not empty:
			//This is the first cell from which we look, and only
			//now have we the right to go to the directly
			//neighboring cells
			link(side, h, neighs[i]);
			cells[(*nb)++] = neighs[i];
		}
	}

	return;
}

