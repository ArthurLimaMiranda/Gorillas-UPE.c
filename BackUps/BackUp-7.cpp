#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.141592654
#include <ncurses.h>

//Printa os macacos na tela
int* MacacoTela(int step, int macaco, int yMax, int altura[], bool AttFrame, int macY, int macX, bool desafio){
	int yPos, xPos, predioM0, predioM1;
	WINDOW * macacoWin;
	char macacos[][3][51]={
		{"              __     ((****)) () (  ) () @ -  - @  ","    __     ((****)) () (**) ()() (  ) ()   -  -    ","  @        () __     ((****))    (  ) ()   -  - @ "},
		{"              __     ((****)) () (  ) () @ -  - @  ","    __     ((****)) () (**) ()() (  ) ()   -  -    ","        @     __  () ((****)) () (  )    @ -  -   "}
	};
	
	if(!AttFrame){
		//Seleciona um predio aleatorio para o macaco iniciar	
		if(desafio)	{
			
			predioM0 = rand()%(5);
			predioM1 = rand()%(5);
			
			if(macaco == 0){
				xPos = 3+(16*predioM0);
				yPos = yMax-3-altura[predioM0];
			}
			
			else{
				xPos = 4+(16*(predioM1+6));
				yPos = yMax-3-altura[predioM1+6]; 
			}
			
		}
		else{
			predioM0 = rand()%(2);
			predioM1 = rand()%(2);
			
			if(macaco == 0){
				xPos = 3+(16*predioM0);
				yPos = yMax-3-altura[predioM0];
			}
			
			else{
				xPos = 4+(16*(predioM1+9));
				yPos = yMax-3-altura[predioM1+9]; 
			}
		}
		//-------------------------------------------------//
	}
	
	else{
		//Atualiza o macaco ja nascido
		xPos = macX;
		yPos = macY;
		//--------------------------//			
	}
	
	macacoWin = newwin(5,10,(yPos),xPos);
	
	wattron(macacoWin,COLOR_PAIR(2));
	wattron(macacoWin,A_BOLD);
	mvwprintw(macacoWin, 0,0,"%s",macacos[macaco][step]);
	wattroff(macacoWin,A_BOLD);
	wattroff(macacoWin,COLOR_PAIR(2));
	wrefresh(macacoWin);
	
	static int absPos[2];
	absPos[0] = yPos;
	absPos[1] = xPos;
	return absPos;
}

//Cria uma janela para a entrada dos valores de angulo e velocidade
int EntradaNumerica(int modo, int tipo, int xTexto, int vMaximo){
	int valor=0, xDigitado=xTexto, yM, xM, def, tam=1; 
	bool sair = false;
	WINDOW * janelaEntrada;
	
	getmaxyx(stdscr, yM, xM);

	if(modo==0){
		janelaEntrada = newwin(3,xTexto+4,1,2);
		def=0;
	}
	
	else if(modo==1){
		janelaEntrada = newwin(3,xTexto+4,1,xM-2-xTexto-4);
		def=0;
	}
	
	else if(modo==2){
		janelaEntrada = newwin(3,15,27,xM/2-10);
		wbkgd(janelaEntrada, COLOR_PAIR(6));
		wattron(janelaEntrada,A_STANDOUT);
		mvwprintw(janelaEntrada,1,1,"Rodadas:");
		wattroff(janelaEntrada, A_STANDOUT);
		def=8;
	}
	
	else if(modo==3){
		janelaEntrada = newwin(3,16,32,xM/2-10);
		wbkgd(janelaEntrada, COLOR_PAIR(6));
		wattron(janelaEntrada,A_STANDOUT);
		mvwprintw(janelaEntrada,1,1,"Desafios:");
		wattroff(janelaEntrada, A_STANDOUT);
		def=7;
		tam=3;
	}	
	
	box(janelaEntrada, 0, 0);

	if(tipo==0){
		mvwprintw(janelaEntrada,1,1,"Velocidade(1->50m/s):");
	}
	
	else if(tipo==1){
		mvwprintw(janelaEntrada,1,1,"Angulo (0->80º):");
	}
	wrefresh(janelaEntrada);
	
	while(!sair){
		char input = wgetch(janelaEntrada);
				
		if((int)input== 10){
			//Valida o valor inserido antes de fechar a janela			
			if((valor>0)&&(valor<=vMaximo)){
				sair = true;
				wclear(janelaEntrada);
				wrefresh(janelaEntrada);
			}
			//----------------------------------------------//			
		}
		
		else{	
			if((int)input==127){
				//Apaga os caracteres inseridos na janela
				if(xDigitado>xTexto ){
					xDigitado--;
					mvwprintw(janelaEntrada,1,xDigitado+def," ");
					valor/=10;
					wrefresh(janelaEntrada);
				}
				//-------------------------------------//
			}
			else if((xDigitado-xTexto)<=tam){
				//Atualiza o valor da variavel a medida que é inserido				
				if(((xDigitado-xTexto)==0) && ((input>=49)&&(input<=57))){
					mvwprintw(janelaEntrada,1,xDigitado+def,"%c", input);
					valor = (int)input-48;
					xDigitado++;
				}
				
				else if(((input>=48)&&(input<=57))){
					mvwprintw(janelaEntrada,1,xDigitado+def,"%c", input);
					valor = (valor*10)+((int)input-48);	
					xDigitado++;
				}
				//--------------------------------------------------//
			}	
		}
		wrefresh(janelaEntrada);	
		refresh();
	}
	
	return valor;
}

void EntradaNomes(int modo, char *entry, int xTexto){
	int yM, xM, xIni = 12, valorI; 
	bool sair = false;
	WINDOW * entradaWin;
	
	getmaxyx(stdscr, yM, xM);

	if(modo==0){
		entradaWin = newwin(3,24,17,xM/2-10);
		wbkgd(entradaWin, COLOR_PAIR(6));
		wattron(entradaWin,A_STANDOUT);
		mvwprintw(entradaWin,1,1,"Nome P1:");
		wattroff(entradaWin, A_STANDOUT);
	}
	
	else if(modo==1){
		entradaWin = newwin(3,24,22,xM/2-10);
		wbkgd(entradaWin, COLOR_PAIR(6));
		wattron(entradaWin,A_STANDOUT);
		mvwprintw(entradaWin,1,1,"Nome P2:");
		wattroff(entradaWin, A_STANDOUT);
	}
		
	box(entradaWin, 0, 0);
	wrefresh(entradaWin);
	int xDigitado=xTexto;
	
	for(valorI=0; valorI<xTexto;valorI++){
		entry[valorI]=32;
	}
	
	while(!sair){
				
		char input = wgetch(entradaWin);	
		
		if((int)input== 10){
			//Valida o valor inserido antes de fechar a janela			
			if((xDigitado-xTexto)>0){
				sair=true;
			}	
			//----------------------------------------------//			
		}
		
		else{	
			if((int)input==127){
				//Apaga os caracteres inseridos na janela
				if(xDigitado>xTexto ){
					xDigitado--;
					mvwprintw(entradaWin,1,(xDigitado-xTexto+11)," ");
					entry[xDigitado]= 32;
					wrefresh(entradaWin);
				}
				//-------------------------------------//
			}
			else if((xDigitado-xTexto)>=0){
				//Atualiza o valor da variavel a medida que é inserido
				if(((input>=33)&&(input<=126)) && (xDigitado-xTexto!=xTexto)){
					mvwprintw(entradaWin,1,(xDigitado-xTexto+11),"%c", input);
					entry[xDigitado-xTexto] = input;
					xDigitado++;		
				}					
				//--------------------------------------------------//
			}	
		}		
	}	
	wclear(entradaWin);
	wrefresh(entradaWin);  
}

int main(int argc, char *argv[]) {
	
	//Variaveis janelas//
	int yMax, xMax, yScr, xScr; 
	WINDOW * jogoWin;
	WINDOW * balaWin;
	WINDOW * menuWin;
	WINDOW * abasWin;
	WINDOW * menuWinSub;
	WINDOW * abasWinSub;
	
	//Variavies predios//
	int predioSize=11, andarSize=26, casaSize=15, scape=0, minAlt=12, maxAlt=andarSize-1, altura[predioSize], pair;
	char predios[predioSize][andarSize][casaSize];
	char teto[]={"@@@@@@@@@@@@@@@"}, construcao[]={"@@@%%%%%%%%%@@@"}, vazio[]={"               "};
	
	//Variaveis contadoras//
	int nPredio, nAndar, nCasa, cont, cont2, a, b, macacoTurn, frame=0, fps=0, M0score=0,M1score=0;
	//Variaveis atores//
	int control = 0, tempo;
	int xText=0, vMax=0;
	int balaPos[] = {2,2}; 
	int *posTemp, macaco0Pos[2], macaco1Pos[2], macRand=0;
	double y, v0=20, v0x, angulo=45, tg, gravidade=9.8;
	int x=0, x0, dX, posX, y0, posY;
	bool novoTiro=true, atirado=false, col=false, colMac0=false, colMac1=false, reset=false, exitar=false;
	
	//Variaveis menu//
	int menuPos=0, menuOpt=0, tamanhoNome=10,desafios, tamanhoDesafios=4, rodadas, tamanhoRodadas=2, rodadaQtd=0, contRounds=0, cont1=0;
	char nomeP1[tamanhoNome], nomeP2[tamanhoNome];
	bool sairMenu=false, nP1=false, nP2=false, nR=false, nD=false, D1=false,D2=false,D3=false;;
	
	initscr(); 
	noecho();
	curs_set(0);
	
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);//background
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);//macaco
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW);//area_vazia
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);//predio
	init_pair(5, COLOR_WHITE, COLOR_MAGENTA);//menu
	init_pair(6, COLOR_WHITE, COLOR_CYAN);//menu2
	
	box(stdscr, 0, 0);
	getmaxyx(stdscr, yScr, xScr); 
	
	while(!exitar){	
	
		if(reset){
			mvwprintw(stdscr,(yScr-2),(macaco0Pos[1]),"          ");
			mvwprintw(stdscr,(yScr-2),(macaco1Pos[1]),"          ");
			wclear(stdscr);
			wclear(jogoWin);
			wrefresh(jogoWin);	
			box(stdscr, 0, 0);
			refresh();
		}
				
		while(!sairMenu){										
			if(menuPos==0){
				menuWin = newwin(yScr,21,0,xScr/2-10); 
				menuWinSub = subwin(menuWin, yScr,21,0,xScr/2-10);
				wbkgd(menuWinSub, COLOR_PAIR(5));
				box(menuWin,0,0);
						
				if(menuOpt==0){wattron(menuWinSub,A_STANDOUT);}
				mvwprintw(menuWinSub,15,6,"Novo Jogo");
				wattroff(menuWinSub, A_STANDOUT);
				
				if(menuOpt==1){wattron(menuWinSub,A_STANDOUT);}
				mvwprintw(menuWinSub,20,7,"Ranking");
				wattroff(menuWinSub,A_STANDOUT);
				
				if(menuOpt==2){wattron(menuWinSub,A_STANDOUT);}
				mvwprintw(menuWinSub,25,7,"Regras");
				wattroff(menuWinSub,A_STANDOUT);
				
				if(menuOpt==3){wattron(menuWinSub, A_STANDOUT);}
				mvwprintw(menuWinSub,30,8,"Sair");
				wattroff(menuWinSub,A_STANDOUT);
				
				refresh();
				wrefresh(menuWin);
				touchwin(menuWin);
				wrefresh(menuWinSub);
				
				control = wgetch(menuWinSub);
				switch(control){
					case 65:
						menuOpt--;	
						break;
					case 66:
						menuOpt++;
						break;
				}
							
				if(menuOpt<0){
						menuOpt=3;
				}
				if(menuOpt>3){
						menuOpt=0;
				}
				
				if(control==10){
					switch(menuOpt){
						case 0:
							menuPos=1;
							menuOpt=0;
							break;
						case 1:
							menuPos=2;
							break;
						case 2:
							menuPos=3;
							break;
						case 3:
							system("clear");
							exit(0);
							break;					
					}
					wclear(menuWinSub);
					wclear(menuWin);
					wrefresh(menuWinSub);
					wrefresh(menuWin);	
					box(stdscr, 0, 0);
					refresh();
				}	
			}	
		
			else if(menuPos==1){

				menuWin = newwin(yScr,13,0,5); 
				menuWinSub = subwin(menuWin, yScr,13,0,5);
				wbkgd(menuWinSub, COLOR_PAIR(5));
				box(menuWin,0,0);
				
				abasWin = newwin(40,50,8,xScr/2-15); 
				abasWinSub = subwin(abasWin, 40,50,8,xScr/2-15);
				wbkgd(abasWinSub, COLOR_PAIR(6));
				box(abasWin,0,0);
	
				wattron(menuWinSub, A_STANDOUT);
				mvwprintw(menuWinSub,yScr/2,2,"Novo Jogo");
				wattroff(menuWinSub,A_STANDOUT);
				
				if(menuOpt==0){wattron(abasWinSub,A_STANDOUT);}
					mvwprintw(abasWinSub,10,6,"Nome P1:");
					if(nP1){for(cont1=0;cont1<tamanhoNome;cont1++){mvwprintw(abasWinSub,10,16+cont1,"%c", nomeP1[cont1]);}}	
				wattroff(abasWinSub, A_STANDOUT);
				
				if(menuOpt==1){wattron(abasWinSub,A_STANDOUT);}
					mvwprintw(abasWinSub,15,6,"Nome P2:");
					if(nP2){for(cont1=0;cont1<tamanhoNome;cont1++){mvwprintw(abasWinSub,15,16+cont1,"%c", nomeP2[cont1]);}}
				wattroff(abasWinSub,A_STANDOUT);
				
				if(menuOpt==2){wattron(abasWinSub,A_STANDOUT);}
					mvwprintw(abasWinSub,20,6,"Rodadas:");
					if(nR){mvwprintw(abasWinSub,20,16,"%d", rodadas);}
				wattroff(abasWinSub,A_STANDOUT);
				
				if(menuOpt==3){wattron(abasWinSub,A_STANDOUT);}
					mvwprintw(abasWinSub,25,6,"Desafios:");
					if(nD){mvwprintw(abasWinSub,25,16,"%d", desafios);}
				wattroff(abasWinSub, A_STANDOUT);
				
				if(menuOpt==4){wattron(abasWinSub,A_STANDOUT);}
				mvwprintw(abasWinSub,33,21,"Enviar");
				wattroff(abasWinSub,A_STANDOUT);
				
				if(menuOpt==5){wattron(abasWinSub, A_STANDOUT);}
				mvwprintw(abasWinSub,35,21,"Voltar");
				wattroff(abasWinSub,A_STANDOUT);
												
				refresh();
				wrefresh(abasWin);
				touchwin(abasWin);
				wrefresh(abasWinSub);
				wrefresh(menuWin);
				wrefresh(menuWinSub);	
				
				control = wgetch(menuWinSub);
				switch(control){
					case 65:
						menuOpt--;	
						break;
					case 66:
						menuOpt++;
						break;
				}
							
				if(menuOpt<0){
						menuOpt=5;
				}
				if(menuOpt>5){
						menuOpt=0;
				}
				
				if(control==10){
					switch(menuOpt){
						case 0:
							EntradaNomes(0, nomeP1, tamanhoNome);
							nP1=true;
							break;
						case 1:
							EntradaNomes(1, nomeP2, tamanhoNome);
							nP2=true;
							break;
						case 2:
							rodadas = EntradaNumerica(2, 3, tamanhoRodadas, 99);
							nR=true;
							break;
						case 3:
							desafios = EntradaNumerica(3, 3, tamanhoDesafios, 9999);
							nD=true;
							break;	
						case 4:
							if(nP1 && nP2 && nR && nD){
								M0score=0, M1score=0, contRounds=0;
								wclear(menuWinSub);
								wclear(menuWin);
								wrefresh(menuWinSub);
								wrefresh(menuWin);	
								box(stdscr, 0, 0);
								refresh();
								sairMenu=true;	
							}
							
							break;
						case 5:
							wclear(menuWinSub);
							wclear(menuWin);
							wrefresh(menuWinSub);
							wrefresh(menuWin);	
							box(stdscr, 0, 0);
							refresh();
							menuOpt=0;
							menuPos=0;
							nP1=false;
							nP2=false;
							nR=false;
							nD=false;
							break;					
					}	
				}
			}
	
			else if(menuPos==2){

				menuWin = newwin(yScr,11,0,5); 
				menuWinSub = subwin(menuWin, yScr,11,0,5);
				wbkgd(menuWinSub, COLOR_PAIR(5));
				box(menuWin,0,0);
				
				abasWin = newwin(40,40,8,xScr/2-10); 
				abasWinSub = subwin(abasWin, 40,40,8,xScr/2-10);
				wbkgd(abasWinSub, COLOR_PAIR(6));
				box(abasWin,0,0);
				
				wattron(menuWinSub, A_STANDOUT);
				mvwprintw(menuWinSub,yScr/2,2,"Ranking");
				wattroff(menuWinSub,A_STANDOUT);
				
				wattron(abasWinSub,A_UNDERLINE);
				mvwprintw(abasWinSub,5,5,"Ranking das 5 ultimas partidas");
				wattroff(abasWinSub, A_UNDERLINE);
				
				wattron(abasWinSub, A_STANDOUT);
				mvwprintw(abasWinSub,36,17,"Voltar");
				wattroff(abasWinSub,A_STANDOUT);
				               
				refresh();
				wrefresh(abasWin);
				touchwin(abasWin);
				wrefresh(abasWinSub);
				wrefresh(menuWin);
				wrefresh(menuWinSub);
				
				
				control = wgetch(menuWinSub);
				if(control==10){
					wclear(menuWinSub);
					wclear(menuWin);
					wrefresh(menuWinSub);
					wrefresh(menuWin);	
					box(stdscr, 0, 0);
					refresh();
					menuPos=0;
				}	
			}
			
			else if(menuPos==3){

				menuWin = newwin(yScr,10,0,5); 
				menuWinSub = subwin(menuWin, yScr,10,0,5);
				wbkgd(menuWinSub, COLOR_PAIR(5));
				box(menuWin,0,0);
				
				abasWin = newwin(40,60,8,xScr/2-20); 
				abasWinSub = subwin(abasWin, 40,60,8,xScr/2-20);
				wbkgd(abasWinSub, COLOR_PAIR(6));
				box(abasWin,0,0);
				
				wattron(menuWinSub, A_STANDOUT);
				mvwprintw(menuWinSub,yScr/2,2,"Regras");
				wattroff(menuWinSub,A_STANDOUT);
				                                                    
				wattron(abasWinSub,A_UNDERLINE);
				mvwprintw(abasWinSub,5,27,"Regras");
				wattroff(abasWinSub, A_UNDERLINE);
				
				mvwprintw(abasWinSub,8,2,"Gorillas eh um jogo onde o principal objetivo eh atingir");                  
				mvwprintw(abasWinSub,9,2,"o macaco adversario com um explosivo.");                  
				  
				mvwprintw(abasWinSub,12,2,"O explosivo eh atirado com base em um angulo e uma ve-");                  
				mvwprintw(abasWinSub,13,2,"locidade inseridos pelo usuario. O vencedor eh escolhi-");                  
				mvwprintw(abasWinSub,14,2,"do dentre quem ganhar mais rodadas");                

				mvwprintw(abasWinSub,17,2,"Esta versao do jogo possui 4 niveis de dificuldade:");                  
				mvwprintw(abasWinSub,19,2,"0 -> Dificuldade padrao, sem adesao de dificuldade;");                  
				mvwprintw(abasWinSub,21,2,"1 -> Caso ativa, o tiro tera um tempo limite de exis-");                
				mvwprintw(abasWinSub,22,2,"     tencia antes de se auto destruir;");  
				mvwprintw(abasWinSub,24,2,"2 -> Se esta dificuldade for escolhida, a cada 2 tiros");
				mvwprintw(abasWinSub,25,2,"     um novo cenario sera gerado (para este modo fun-");
				mvwprintw(abasWinSub,26,2,"     cionar coloque mais que 2 rodadas)");                
				mvwprintw(abasWinSub,28,2,"3 -> Se esta dificuldade for escolhida, os macacos irao");  
				mvwprintw(abasWinSub,29,2,"     nascer em predios aleatorios dos seus respectivos ");  
				mvwprintw(abasWinSub,30,2,"     lados");  
				
				mvwprintw(abasWinSub,33,2,"*Todas as dificuldades podem estar ativas juntas");  				
				                  
				wattron(abasWinSub, A_STANDOUT);
				mvwprintw(abasWinSub,36,27,"Voltar");
				wattroff(abasWinSub,A_STANDOUT);
			
				refresh();
				wrefresh(abasWin);
				touchwin(abasWin);
				wrefresh(abasWinSub);
				wrefresh(menuWin);
				wrefresh(menuWinSub);
								
				control = wgetch(menuWinSub);
				if(control==10){
					wclear(menuWinSub);
					wclear(menuWin);
					wrefresh(menuWinSub);
					wrefresh(menuWin);	
					box(stdscr, 0, 0);
					refresh();
					menuPos=0;
				}	
			}	
		}
		
		reset = false;
		colMac0=0, colMac1=0, rodadaQtd=0;	
		nP1=false, nP2=false, nR=false, nD=false;
		
		/*
		for(cont1=0;cont1<tamanhoDesafios;cont1++){
			if(desafios[cont1]==1){
				D1=true;	

			}
			else if(desafios[cont1]==2){
				D2=true;
			}
			else if(desafios[cont1]==3){
				D3=true;
			}
		}
		*/
		
		D3=true;
		
		//Configurações iniciais das janelas	
		jogoWin = newwin(yScr-6,xScr,4,0);
		getmaxyx(jogoWin, yMax, xMax); 
		balaWin = subwin(jogoWin, yScr-6,xScr,4,0);
		wclear(balaWin);
		wbkgd(balaWin, COLOR_PAIR(1));
		box(jogoWin,0,0);
		refresh();
		wrefresh(jogoWin);
		touchwin(jogoWin);
		wrefresh(balaWin);
		//--------------------------------//
			
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
		scape=0;
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
		//---------------//
		wrefresh(jogoWin);
		//Macaco e nome na tela//
		posTemp = MacacoTela(0, 0, yMax, altura, false, 0, 0, D3);
		macaco0Pos[0] = posTemp[0];
		macaco0Pos[1] = posTemp[1];
		for(cont1=0;cont1<tamanhoNome;cont1++){mvwprintw(stdscr,(yScr-2),(macaco0Pos[1]+cont1),"%c", nomeP1[cont1]);}

		posTemp = MacacoTela(0, 1, yMax, altura, false, 0, 0, D3);
		macaco1Pos[0] = posTemp[0];
		macaco1Pos[1] = posTemp[1];
		for(cont1=0;cont1<tamanhoNome;cont1++){mvwprintw(stdscr,(yScr-2),(macaco1Pos[1]+cont1),"%c", nomeP2[cont1]);}	
		//--------------//  
		
		refresh();
		while(!reset){
			
			if(novoTiro){
					mvwprintw(stdscr, yScr-2, xScr/2-1,"%d vs %d", M0score, M1score);
					mvwprintw(stdscr, 3, xScr/2-14,"Aperter qualquer tecla");				
				}
			
			if(contRounds==(2*rodadas)){
				int input = getch();
				reset=true;
				sairMenu=false;
			}
			
			//Desafio 2
			else if((D2) && (rodadaQtd>=2)){
				rodadaQtd=0;
				reset=true;
			}
			//-------//
			
			else{				
				int input = getch();
				
				if(novoTiro){			
					mvwprintw(stdscr, 3, xMax/2-14,"                      ");
					refresh();
					
					macacoTurn = 1-macacoTurn;
					
					//Pega os valores da velocidade e angulo
					xText = 23;
					vMax = 50;
					v0 = EntradaNumerica(macacoTurn,0,xText, vMax);
				
					xText = 18;
					vMax = 80;
					angulo = EntradaNumerica(macacoTurn,1,xText, vMax);
					//------------------------------------//
					
					//Definição da trajetoria do tiro			
					if(macacoTurn==0){
						x0 = macaco0Pos[1]+3;
						y0 = macaco0Pos[0]-5; 
						v0x = v0*cos(angulo*PI/180);
					    tg = tan(angulo*PI/180);
					    posTemp = MacacoTela(2, macacoTurn, yMax, altura, true, macaco0Pos[0], macaco0Pos[1], false);
					}
					  
					else{
						x0 = macaco1Pos[1]+7;
						y0 = macaco1Pos[0]-5; 
						v0x = v0*cos((180-angulo)*PI/180); 
						tg = tan((180-angulo)*PI/180); 
						posTemp = MacacoTela(2, macacoTurn, yMax, altura, true, macaco1Pos[0], macaco1Pos[1], false);
					}
					//-----------------------------//
					
					tempo=0;
					atirado=true;
					novoTiro=false;
				}
				
				if(atirado){
					timeout(150);
					tempo++;
					//Atualiza o frame do macaco
					frame++;	
					if(frame==3){
						posTemp = MacacoTela(fps, 1, yMax, altura, true, macaco1Pos[0], macaco1Pos[1], false);
						posTemp = MacacoTela(fps, 0, yMax, altura, true, macaco0Pos[0], macaco0Pos[1], false);
						fps = 1-fps;
						frame=0;
					}
					//------------------------//
					
					//Atualiza a proxima posicao da bala						
					x++;
									
					if(balaPos[0]>1){
						mvwprintw(balaWin,balaPos[0],balaPos[1]," ");
					}
				 	else{
				 		mvwprintw(balaWin, 1,balaPos[1]," ");
					}
					
				 	if(macacoTurn==0){
				 		posX = x+x0; 
						dX = (posX-x0);	                                  
				        y = (dX*tg) - ((gravidade/2)*((dX/v0x)*(dX/v0x)));
				        posY = y0 - y;  
					 }	                 
			
				 	else{
				 		posX = x0-x; 
						dX = (posX-x0);	                                  
				        y = (dX*tg) - ((gravidade/2)*((dX/v0x)*(dX/v0x)));
				        posY = y0 - y; 
					 }
				 	                  				
					balaPos[0]= posY;
					balaPos[1]= posX;
					//--------------------------------//
				}
				
				//Colisao bala macaco//
				if(balaPos[1] >= macaco0Pos[1] && balaPos[1] <= macaco0Pos[1]+10){
					if(balaPos[0] >= macaco0Pos[0]-3 && balaPos[0] <= macaco0Pos[0]){
						col = true;
						colMac0 = true;
					}
				}
				else if(balaPos[1] >= macaco1Pos[1] && balaPos[1] <= macaco1Pos[1]+10){
					if(balaPos[0] >= macaco1Pos[0]-3 && balaPos[0] <= macaco1Pos[0]){
						col = true;
						colMac1 = true;
					}
				}
				//-------------------//
				
				//Colisao bala predio//
				a=(((yScr-yMax)+casaSize+maxAlt)-(balaPos[0]));
				b=(balaPos[1]-1-(balaPos[1]/16))-(15*(balaPos[1]/16));
				if((balaPos[0]>=yScr-yMax+casaSize)&&(balaPos[1]>0)&&(balaPos[1]<xMax)&&(balaPos[1]%16!=0)&&(predios[balaPos[1]/16][a][b]!=32)){
					mvwprintw(balaWin, balaPos[0],balaPos[1]," ");
					predios[balaPos[1]/16][a][b] = 32;
					col = true;
				}
				//-------------------//		
				
				//Bala fora do alcance
				if((balaPos[0]>yMax-1)||(balaPos[1]<2)||(balaPos[1]>xMax-3)){
					col = true;
				}		
				//------------------//
				
				//Desafio 1
				if((D1) && (tempo>=150)){
					col=true;
				}
				//-------//
				
				//Controle de colisao	
				if(col){
					contRounds++;
					rodadaQtd++;
					timeout(-1);
					x=0;
					novoTiro=true;
					atirado=false;
					col=false;
					if(colMac0){
						M1score++;
						reset=true;				
					}
					else if(colMac1){
						M0score++;
						reset=true;				
					}	
				}
				
				//-----------------//
				
				//Controle do tiro
				if(atirado){
					if(balaPos[0]>1){
						mvwprintw(balaWin, balaPos[0],balaPos[1],"@");
					}
					else{
						mvwprintw(balaWin, 1,balaPos[1],"^");
					}		
				}
				//--------------//			
				wrefresh(balaWin);	
			}
		}		
	}		
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
	
	
