#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 
#include <conio.h>
#include <locale.h>
#include <string.h>

//CONSTANTES
////////////
const int MINA=-1;   //valor de una casilla con mina (antes era 7, pero una casilla podia tener 7 minas alrededor y se confundia)
const int OCULTA=9;  //valor de una casilla sin descubrir
const int FILAS=15;
const int COLUMNAS=8;
const int NUM_MINAS=20;

//cambia el color del texto y del fondo de la consola
void setColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + (bgColor << 4); 
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

//descubre una casilla y, si es 0, descubre las vecinas (recursivo)
//zona[f][c][1] = valor real, zona[f][c][2] = lo que ve el jugador
void descubrir(int zona[15][8][3], int f, int c)
{
	//fuera del tablero: no hacer nada
	if(f<0||f>=FILAS||c<0||c>=COLUMNAS)
	{
		return;
	}
	//casilla ya descubierta: no hacer nada
	if(zona[f][c][2]!=OCULTA)
	{
		return;
	}
	
	//descubrir la casilla
	zona[f][c][2]=zona[f][c][1];
	
	//si no tiene minas alrededor, descubrir las 8 vecinas
	if(zona[f][c][1]==0)
	{
		for(int a=-1;a<=1;a++)
		{
			for(int b=-1;b<=1;b++)
			{
				descubrir(zona,f+a,c+b);
			}
		}
	}
}

//imprime el tablero. si todo==true ensena el valor real de todas las casillas
void imprimir(int zona[15][8][3], bool todo)
{
	int i,j,valor;
	
	//numeros de las columnas
	for(i=0;i<COLUMNAS;i++)
	{
		setColor(5, 0); //MAGENTA
		printf("\t%i",i+1);
	}
	printf("\n");
	
	//filas
	for(i=0;i<FILAS;i++)
	{
		setColor(5, 0); //MAGENTA
		printf("\n%i\t",i+1);
		for(j=0;j<COLUMNAS;j++)
		{
			//que valor se muestra: el real o el que ve el jugador
			if(todo==true)
			{
				valor=zona[i][j][1];
			}else
				{
					valor=zona[i][j][2];
				}
			
			if(valor==OCULTA)
			{
				setColor(7, 0); //WHITE
				printf("-\t");
			}else
				{
					if(valor==MINA)
					{
						setColor(4, 0); //RED
						printf("B\t");
					}else
						{
							//color segun el numero
							if(valor==1)
							{
								setColor(2, 0); //GREEN
							}else
								{
									if(valor==2)
									{
										setColor(6, 0); //YELLOW
									}else
										{
											if(valor==3)
											{
												setColor(3, 0); //CYAN
											}else
												{
													setColor(7, 0); //WHITE
												}
										}
								}
							printf("%i\t",valor);
						}
				}
		}
	}
	setColor(7, 0);
}
    
int main()
{  
    setlocale (LC_ALL,"Spanish");
    
	//VARIABLES
	///////////
	
	int zona[15][8][3],i,j,a,b,cuenta;  //[3] porque se usan los indices 1 y 2
	int AF=0,AC=0,n;
	bool jugar=true;
	//INICIO DE CODIGO
	//////////////////
	
	//inicializar todo
	for(i=0;i<FILAS;i++)
	{
		for(j=0;j<COLUMNAS;j++)
		{
			zona[i][j][1]=0;
			zona[i][j][2]=OCULTA;
		}
	}
		
	//colocacion de minas
	n=NUM_MINAS;
	srand(time(NULL));
	while(n>0)
	{
		AF=rand()%FILAS;
		AC=rand()%COLUMNAS;
		if(zona[AF][AC][1]!=MINA)
		{
			zona[AF][AC][1]=MINA;
			n-=1;	
		}
	}
	
	//contar minas alrededor de cada casilla
	for(i=0;i<FILAS;i++)
	{
		for(j=0;j<COLUMNAS;j++)
		{
			if(zona[i][j][1]==MINA)
			{
				//sumar 1 a las 8 vecinas (sin salirse del tablero ni tocar otras minas)
				for(a=-1;a<=1;a++)
				{
					for(b=-1;b<=1;b++)
					{
						if(i+a>=0&&i+a<FILAS&&j+b>=0&&j+b<COLUMNAS)
						{
							if(zona[i+a][j+b][1]!=MINA)
							{
								zona[i+a][j+b][1]=zona[i+a][j+b][1]+1;
							}
						}
					}
				}
			}
		}
	}

	//BUCLE DE JUEGO
	////////////////
	while(jugar==true)
	{
		//imprimir en pantalla
		system("CLS");
		imprimir(zona,false);
		
		//pedir casilla
		printf("\n\n\tFILA: ");
		if(scanf("%i",&AF)!=1)
		{
			while(getchar()!='\n'); //limpiar lo que haya escrito mal
			AF=0;
		}
		printf("\n\tCOLUMNA: ");
		if(scanf("%i",&AC)!=1)
		{
			while(getchar()!='\n');
			AC=0;
		}
		
		//comprobar que la casilla existe
		if(AF<1||AF>FILAS||AC<1||AC>COLUMNAS)
		{
			printf("\n\tCasilla no valida");
			Sleep(1000);
		}else
			{
				//identificar casilla
				descubrir(zona,AF-1,AC-1);
				
				//resultado: perder
				if(zona[AF-1][AC-1][1]==MINA)
				{
					system("CLS");
					imprimir(zona,false);
					printf("\n\n\n\tHAS PERDIDO");
					
					Sleep(2000);
					system("CLS");
					
					//ensenar todo el tablero
					imprimir(zona,true);
					
					jugar=false;
				}else
					{
						//resultado: ganar (todas las casillas sin mina descubiertas)
						cuenta=0;
						for(i=0;i<FILAS;i++)
						{
							for(j=0;j<COLUMNAS;j++)
							{
								if(zona[i][j][2]!=OCULTA)
								{
									cuenta+=1;
								}
							}
						}
						
						if(cuenta==FILAS*COLUMNAS-NUM_MINAS)
						{
							system("CLS");
							imprimir(zona,true);
							printf("\n\n\n\tHAS GANADO");
							jugar=false;
						}
					}
			}
	}
	
    //FIN CODIGO
    ////////////
    
    printf("\n\n");
    return 0;
}