#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.141592654
#include <ncurses.h>

int* MacacoTela(int macaco, int yMax, int altura[]){
	int yPos, xPos;
	WINDOW * macacoWin;
	char macacos[][3][51]={
		{"              __     ((****)) () (  ) () @ -  - @  ","    __     ((****)) () (**) ()() (  ) ()   -  -    ","  @        () __     ((****))    (  ) ()   -  - @ "},
		{"              __     ((****)) () (  ) () @ -  - @  ","    __     ((****)) () (**) ()() (  ) ()   -  -    ","        @     __  () ((****)) () (  )    @ -  -   "}
	};
	
	int predioM0 = rand()%(101);
	int predioM1 = rand()%(101);
	
	if(macaco == 0){
		if(predioM0>=50){
			xPos = 3;
			yPos = yMax-3-altura[0];
		}
		else{
			xPos = 19;
			yPos = yMax-3-altura[1];
		}
	}
	
	else{
		if(predioM1>=50){
			xPos = (10*16)+4;
			yPos = yMax-3-altura[10]; 
		}
		else{
			xPos = (9*16)+4;
			yPos = yMax-3-altura[9];
		}
	}
	
	macacoWin = newwin(5,10,(yPos),xPos);
	
	wattron(macacoWin,COLOR_PAIR(2));
	wattron(macacoWin,A_BOLD);
	mvwprintw(macacoWin, 0,0,"%s",macacos[macaco][0]);
	wattroff(macacoWin,A_BOLD);
	wattroff(macacoWin,COLOR_PAIR(2));
	wrefresh(macacoWin);
	
	static int absPos[2];
	absPos[0] = yPos;
	absPos[1] = xPos;
	return absPos;
}

int Entrada(int telaSize, int xTexto, int vMaximo){
	int valor=0, xDigitado=xTexto;
	bool sair = false;
	WINDOW * entradaWin;

	entradaWin = newwin(3,telaSize,1,2);
	box(entradaWin, 0, 0);
	
	mvwprintw(entradaWin,1,1,"Velocidade(1->50m/s):");
	wrefresh(entradaWin);
	
	while(!sair){
		char input = wgetch(entradaWin);
				
		if((int)input== 10){
			
			if((valor>=0)&&(valor<=vMaximo)){
				sair = true;
			}			
		}
		
		else{	
			if((int)input==127){
				if(xDigitado>xTexto ){
					xDigitado--;
					mvwprintw(entradaWin,1,xDigitado," ");
					valor/=10;
					wrefresh(entradaWin);
				}
			}
			else if((xDigitado-xTexto)<=1){
				if(((xDigitado-xTexto)==0) && ((input>=49)&&(input<=57))){
					mvwprintw(entradaWin,1,xDigitado,"%c", input);
					valor = (int)input-48;
					xDigitado++;
				}
				
				else if(((input>=48)&&(input<=57))){
					mvwprintw(entradaWin,1,xDigitado,"%c", input);
					valor = (valor*10)+((int)input-48);	
					xDigitado++;
				}
							
			}
			
		}
		refresh();	
	}
	
	return valor;
}
int main(int argc, char *argv[]) {
	
	//Variaveis janelas//
	int yMax, xMax, yScr, xScr; 
	WINDOW * jogoWin;
	WINDOW * balaWin;
	
	//Variavies predios//
	int predioSize=11, andarSize=26, casaSize=15, scape=0, minAlt=12, maxAlt=andarSize-1, altura[predioSize], pair;
	char predios[predioSize][andarSize][casaSize];
	char teto[]={"@@@@@@@@@@@@@@@"}, construcao[]={"@@@%%%%%%%%%@@@"}, vazio[]={"               "};
	
	//Variaveis contadoras//
	int nPredio, nAndar, nCasa, cont, cont2, a, b;
	
	//Variaveis atores//
	int balaPos[] = {2,2}; 
	int *posTemp, macaco0Pos[2], macaco1Pos[2];
	double y, v0=20, v0x, angulo=45, tg, gravidade=9.8;
	int x=0, x0, dX, posX, y0, posY;
	bool novoTiro=true, atirado=false, col=false, colMac0=false, colMac1=false;
	
	initscr(); 
	noecho();
	curs_set(0);
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);//background
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);//macaco
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);//area_vazia
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
	posTemp = MacacoTela(0, yMax, altura);
	macaco0Pos[0] = posTemp[0];
	macaco0Pos[1] = posTemp[1];
	
	posTemp = MacacoTela(1, yMax, altura);
	macaco1Pos[0] = posTemp[0];
	macaco1Pos[1] = posTemp[1];
	//--------------//  
	
	int xText = 23, velocidade = 0, angulo = 0, vMax = 50;
	
	
	velocidade = Entrada(27, xText, vMax);
	//mvwprintw(entradaWin,2,1,"Angulo (0->80º):");

	
	
	
	
		
	
	
	
	
	getch();
	
	endwin();
	return 0; 
}


		/*	
		keypad(balaWin, true);
		int input = wgetch(balaWin);
		switch(input){
			case KEY_UP:
				mvwprintw(balaWin,balaPos[0],balaPos[1]," ");
				posY--;
				break;
			case KEY_DOWN:
				mvwprintw(balaWin,balaPos[0],balaPos[1]," ");
				posY++;
				break;
			case KEY_LEFT:
				mvwprintw(balaWin,balaPos[0],balaPos[1]," ");
				posX--;
				break;
			case KEY_RIGHT:
				mvwprintw(balaWin,balaPos[0],balaPos[1]," ");
				posX++;
				break;
		}
		*/	
	
	
