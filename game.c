#include "game.h"

void link_accessible_from(UI* ui, Content** b, const int side, int* nb, Hex* cells, int mode){
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
	int i, j, in_cells;
	for(i=0; i<nb_neighs; i++){
		if(get_ct(b, side, neighs[i]) != EMPTY){
			//Jump access is looked up wether or not we’re on our
			//source or not.
			tmp_h.r = h.r + 2*(neighs[i].r - h.r);
			tmp_h.q = h.q + 2*(neighs[i].q - h.q);
			if(get_ct(b, side, tmp_h) == EMPTY){
				link(ui, side, h, neighs[i], mode);
				link(ui, side, neighs[i], tmp_h, mode);
				in_cells = 0;
				for(j=0; j<*nb; j++)
					in_cells = in_cells || (tmp_h.r == cells[j].r && tmp_h.q == cells[j].q);
				if(!in_cells){
					cells[(*nb)++] = tmp_h;
					//Trace the rest of the path
					link_accessible_from(ui, b, side, nb, cells, mode);
				}
			}
		}else if(get_ct(b, side, h) != EMPTY){
			//Source is not empty:
			//This is the first cell from which we look, and only
			//now have we the right to go to the directly
			//neighboring cells
			link(ui, side, h, neighs[i], mode);
			cells[(*nb)++] = neighs[i];
		}
	}
	return;
}

