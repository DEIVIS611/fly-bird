#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>

#define ANCHO_PANTALLA 90
#define ALTURA_PANTALLA 26
#define ANCHO_JUEGO 70
#define TAMANO_GAP 7

using namespace std;

HANDLE consola = GetStdHandle(STD_OUTPUT_HANDLE);
COORD PosicionCursor;

int posicionTuberia[3];
int posicionGap[3];
int banderaTuberia[3];
char pajaro[2][6] = { '/','-','-','o','\\',' ',
					'|','_','_','_',' ','>' };
int posicionPajaro = 6;
int puntaje = 0;

void gotoxy(int x, int y) {
	PosicionCursor.X = x;
	PosicionCursor.Y = y;
	SetConsoleCursorPosition(consola, PosicionCursor);
}

void configurarCursor(bool visible, DWORD tamano) {
	if(tamano == 0) tamano = 20;

	CONSOLE_CURSOR_INFO infoCursor;
	infoCursor.bVisible = visible;
	infoCursor.dwSize = tamano;
	SetConsoleCursorInfo(consola, &infoCursor);
}

void dibujarBordes() {
	for(int i = 0; i < ANCHO_PANTALLA; i++) {
		gotoxy(i, 0); cout << "±";
		gotoxy(i, ALTURA_PANTALLA); cout << "±";
	}

	for(int i = 0; i < ALTURA_PANTALLA; i++) {
		gotoxy(0, i); cout << "±";
		gotoxy(ANCHO_PANTALLA, i); cout << "±";
	}
	for(int i = 0; i < ALTURA_PANTALLA; i++) {
		gotoxy(ANCHO_JUEGO, i); cout << "±";
	}
}

void generarTuberia(int indice) {
	posicionGap[indice] = 3 + rand() % 14;
}

void dibujarTuberia(int indice) {
	if(banderaTuberia[indice]) {
		for(int i = 0; i < posicionGap[indice]; i++) {
			gotoxy(ANCHO_JUEGO - posicionTuberia[indice], i + 1); cout << "***";
		}
		for(int i = posicionGap[indice] + TAMANO_GAP; i < ALTURA_PANTALLA - 1; i++) {
			gotoxy(ANCHO_JUEGO - posicionTuberia[indice], i + 1); cout << "***";
		}
	}
}

void borrarTuberia(int indice) {
	if(banderaTuberia[indice]) {
		for(int i = 0; i < posicionGap[indice]; i++) {
			gotoxy(ANCHO_JUEGO - posicionTuberia[indice], i + 1); cout << "   ";
		}
		for(int i = posicionGap[indice] + TAMANO_GAP; i < ALTURA_PANTALLA - 1; i++) {
			gotoxy(ANCHO_JUEGO - posicionTuberia[indice], i + 1); cout << "   ";
		}
	}
}

void dibujarPajaro() {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + posicionPajaro); cout << pajaro[i][j];
		}
	}
}

void borrarPajaro() {
	for(int i = 0; i < 2; i++) {
		for(int j = 0; j < 6; j++) {
			gotoxy(j + 2, i + posicionPajaro); cout << " ";
		}
	}
}

int detectarColision() {
	if(posicionTuberia[0] >= 61) {
		if(posicionPajaro < posicionGap[0] || posicionPajaro > posicionGap[0] + TAMANO_GAP) {
			return 1;
		}
	}
	return 0;
}

void finDelJuego() {
	system("cls");
	cout << endl;
	cout << "\t\t--------------------------" << endl;
	cout << "\t\t----- Fin del Juego ------" << endl;
	cout << "\t\t--------------------------" << endl << endl;
	cout << "\t\tPresiona cualquier tecla para volver al menú.";
	getch();
}

void actualizarPuntaje() {
	gotoxy(ANCHO_JUEGO + 7, 5); cout << "Puntaje: " << puntaje << endl;
}

void instrucciones() {
	system("cls");
	cout << "Instrucciones";
	cout << "\n----------------";
	cout << "\n Presiona la barra espaciadora para hacer volar al pájaro";
	cout << "\n\nPresiona cualquier tecla para volver al menú";
	getch();
}

void jugar() {
	posicionPajaro = 6;
	puntaje = 0;
	banderaTuberia[0] = 1;
	banderaTuberia[1] = 0;
	posicionTuberia[0] = posicionTuberia[1] = 4;

	system("cls");
	dibujarBordes();
	generarTuberia(0);
	actualizarPuntaje();

	gotoxy(ANCHO_JUEGO + 5, 2); cout << "FLAPPY BIRD";
	gotoxy(ANCHO_JUEGO + 7, 12); cout << "Control";
	gotoxy(ANCHO_JUEGO + 2, 14); cout << " Barra espaciadora = saltar";

	gotoxy(10, 5); cout << "Presiona cualquier tecla para empezar";
	getch();
	gotoxy(10, 5); cout << "                             ";

	while(1) {
		if(kbhit()) {
			char tecla = getch();
			if(tecla == 32 && posicionPajaro > 3) {
				posicionPajaro -= 3;
			}
			if(tecla == 27) {
				break;
			}
		}

		dibujarPajaro();
		dibujarTuberia(0);
		dibujarTuberia(1);
		if(detectarColision() == 1) {
			finDelJuego();
			return;
		}
		Sleep(100);
		borrarPajaro();
		borrarTuberia(0);
		borrarTuberia(1);
		posicionPajaro += 1;

		if(posicionPajaro > ALTURA_PANTALLA - 2) {
			finDelJuego();
			return;
		}

		if(banderaTuberia[0] == 1) posicionTuberia[0] += 2;
		if(banderaTuberia[1] == 1) posicionTuberia[1] += 2;

		if(posicionTuberia[0] >= 40 && posicionTuberia[0] < 42) {
			banderaTuberia[1] = 1;
			posicionTuberia[1] = 4;
			generarTuberia(1);
		}
		if(posicionTuberia[0] > 68) {
			puntaje++;
			actualizarPuntaje();
			banderaTuberia[1] = 0;
			posicionTuberia[0] = posicionTuberia[1];
			posicionGap[0] = posicionGap[1];
		}
	}
}

int main() {
	configurarCursor(0, 0);
	srand((unsigned)time(NULL));

	do {
		system("cls");
		gotoxy(10, 5); cout << " -------------------------- ";
		gotoxy(10, 6); cout << " |      Flappy Bird       | ";
		gotoxy(10, 7); cout << " --------------------------";
		gotoxy(10, 9); cout << "1. Iniciar Juego";
		gotoxy(10, 10); cout << "2. Instrucciones";
		gotoxy(10, 11); cout << "3. Salir";
		gotoxy(10, 13); cout << "Selecciona una opción: ";
		char opcion = getche();

		if(opcion == '1') jugar();
		else if(opcion == '2') instrucciones();
		else if(opcion == '3') exit(0);

	} while(1);

	return 0;
}
