#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
	int a,b;
	//Variaveis janelas//
	int yMax, xMax, yScr, xScr; 
	WINDOW * jogoWin;
	WINDOW * balaWin;
	WINDOW * macacoWin;
	
	//Variavies predios//
	int predioSize=11, andarSize=26, casaSize=15, scape=0, minAlt=12, maxAlt=andarSize-1, altura[predioSize], pair;
	char predios[predioSize][andarSize][casaSize];
	char teto[]={"@@@@@@@@@@@@@@@"}, construcao[]={"@@@%%%%%%%%%@@@"}, vazio[]={"               "},teste[]={"aaaaaaaaaaaaaaa"};
	
	char macaco[]={"              __     ((****)) () (  ) () @ -  - @  "};
	
	//Variaveis contadoras//
	int nPredio, nAndar, nCasa, cont, cont2;
	
	//Variaveis bala//
	int start_y=2, start_x=2;
	
	initscr(); 
	noecho();
	curs_set(0);
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);//background
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);//macaco
	init_pair(3, COLOR_BLACK, COLOR_BLACK);//area_vazia
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);//predio
	
	getmaxyx(stdscr, yScr, xScr); 
	box(stdscr, 0, 0); 	
	
	jogoWin = newwin(yScr-6,xScr,4,0);
	getmaxyx(jogoWin, yMax, xMax); 
	balaWin = subwin(jogoWin, yScr-6,xScr,4,0);
	wbkgd(balaWin, COLOR_PAIR(1));
	box(jogoWin,0,0);
	refresh();
	wrefresh(jogoWin);
	touchwin(jogoWin);
	wrefresh(balaWin);
	
	//Cria predios com altura aleatoria//
	srand(time(NULL));	
	for(cont=0; cont<predioSize; cont++){
		altura[cont] = rand() % (maxAlt + 1 - minAlt) + minAlt;
	}
	
	for(nPredio=0; nPredio<predioSize; nPredio++){
		for(nAndar=0; nAndar<andarSize; nAndar++){
			if(nAndar<altura[nPredio]){
				for (cont2=0; cont2<casaSize; cont2++){
				 	predios[nPredio][nAndar][cont2]=construcao[cont2];
				 }
			}
			else if (nAndar==altura[nPredio]){
				for (cont2=0; cont2<casaSize; cont2++){
				 	predios[nPredio][nAndar][cont2]=teto[cont2];
				 }
			}
			else{
				for (cont2=0; cont2<casaSize; cont2++){
				 	predios[nPredio][nAndar][cont2]=vazio[cont2];
				 }
			}
		}
	}
	//---------------------------------//
	
	//Predios na tela//
	for(nPredio=0; nPredio<predioSize; nPredio++){
		for(nAndar=0; nAndar<andarSize; nAndar++){
			for(nCasa=0; nCasa<casaSize; nCasa++){
				pair=4;
				if(nAndar>altura[nPredio]){
					pair=3;
				}
				wattron(jogoWin,COLOR_PAIR(pair));
				mvwprintw(jogoWin,(yMax-2-nAndar),(1+nCasa+(casaSize*nPredio)+scape), "%c", predios[nPredio][nAndar][nCasa]);
				wattroff(jogoWin,COLOR_PAIR(pair));
			}
		}
		scape++;
	}
	wrefresh(jogoWin);
	refresh();
	//---------------//
	
	//Macaco na tela//
	macacoWin = newwin(5,10,(yMax-3-altura[0]),3);
	wattron(macacoWin,COLOR_PAIR(2));
	wattron(macacoWin,A_BOLD);
	mvwprintw(macacoWin, 0,0,"%s",macaco);
	wattroff(macacoWin,A_BOLD);
	wattroff(macacoWin,COLOR_PAIR(2));
	wrefresh(macacoWin);
	//--------------//
	
	while(1){
		keypad(balaWin, true);
		int input = wgetch(balaWin);
		switch(input){
			case KEY_UP:
				mvwprintw(balaWin, start_y,start_x," ");
				start_y--;
				break;
			case KEY_DOWN:
				mvwprintw(balaWin, start_y,start_x," ");
				start_y++;
				break;
			case KEY_LEFT:
				mvwprintw(balaWin, start_y,start_x," ");
				start_x--;
				break;
			case KEY_RIGHT:
				mvwprintw(balaWin, start_y,start_x," ");
				start_x++;
				break;
		}	
		
		a=(((yScr-yMax)+casaSize+maxAlt)-(start_y));
		b=(start_x-1-(start_x/16))-(15*(start_x/16));
		if(start_y>=yScr-yMax+casaSize){
			if((start_x>0) && (start_x<xMax) && (start_x%16!=0)){
				if(predios[start_x/16][a][b] != 32){
					mvwprintw(balaWin, start_y,start_x,"p");
				}
				else{
					mvwprintw(balaWin, start_y,start_x,"m");
				}
				
			}		
		}
		else{
			mvwprintw(balaWin, start_y,start_x,"@");
		}
		mvwprintw(balaWin, 2,2,"%d",((yScr-yMax)+casaSize+maxAlt)-(start_y));
		//mvwprintw(balaWin, 3,3,"%d",(start_x-1-(start_x/16))-(15*(start_x/16)));
		//mvwprintw(balaWin, 4,4,"%d",start_x);
		//mvwprintw(balaWin, 5,5,"%d",yMax);
		//mvwprintw(balaWin, 6,6,"%d",yScr);
		wrefresh(jogoWin);
		wrefresh(balaWin);
	}
	
	
	getch();
	
	endwin();
	return 0; 
}



	
	
	
