#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <malloc.h>
#include <conio.h>
#pragma warning(disable : 4996)
struct komorka {
	int odl;
	int odw_sas;
	char znak;
	int vis;
	int vis_dfs;
};
void matryca(struct komorka*** tab,int wiersze) {
	//struct komorka** tab;
	*tab = malloc(wiersze * sizeof(struct komorka));
	if (tab != NULL) {
		for( int i = 0; i < wiersze; i++ ){
			(*tab)[i] = malloc(wiersze * sizeof(struct komorka));
			if ((*tab)[i] == NULL) {
				for (int j = 0; j <= i; j++)
					free((*tab)[j]);
				free(*tab);
				printf("Problem z alokacja pamieci! Koniec programu.");
				exit(0);
			}
		}
	}
	for (int i = 0; i < wiersze; i++) { //(*(tab + i * wiersze) + j)
		for (int j = 0; j < wiersze; j++) {
			(*tab)[i][j].odl = 0;
			(*tab)[i][j].odw_sas = 0;
			(*tab)[i][j].znak = '0';
			if(i == 0 || i == wiersze - 1 || j == 0 || j == wiersze - 1)
				(*tab)[i][j].vis = 1;
			else
				(*tab)[i][j].vis = 0;
			(*tab)[i][j].vis_dfs = (*tab)[i][j].vis;
		}
	}
}
void wyswietl(struct komorka **dane,int wiersze) {
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < wiersze; j++) {
			printf("%c ", dane[i][j].znak);
			_sleep(5);
		}
		_sleep(50);
		printf("\n");
	}
}
void wyswietl_ruch(struct komorka** dane, int wiersze) {
	printf("Znaki oznaczaja\nM - myszka\nS - ser\n0 - sciana.\n");
	printf("Za pomoca klawiszy W A S D mozesz sterowac ruchami myszki.\nPo wcisnieciu specjalnego przecisku P myszka aktywuje swoj wech i ulatwi w dotarciu do sera\n\n\n");
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < wiersze; j++) {
			printf("%c ", dane[i][j].znak);
		}
		printf("\n");
	}
}
void delokuj_pamiec(struct komorka*** tab, int wiersze) {
	for (int i = 0; i < wiersze; i++)
		free((*tab)[i]);
	free(*tab);
}
void generuj_maze(struct komorka*** tab, int y, int x, int* y_kon, int* x_kon, int* odleglosc) {
	(*tab)[y][x].vis = 1;
	(*tab)[y][x].znak = ' ';
	(*tab)[y + 1][x].odw_sas++;
	(*tab)[y - 1][x].odw_sas++;
	(*tab)[y][x + 1].odw_sas++;
	(*tab)[y][x - 1].odw_sas++;
	if ((*tab)[y + 1][x].odw_sas > 1)
		(*tab)[y + 1][x].vis = 1;
	if ((*tab)[y - 1][x].odw_sas > 1)
		(*tab)[y - 1][x].vis = 1;
	if ((*tab)[y][x + 1].odw_sas > 1)
		(*tab)[y][x + 1].vis = 1;
	if ((*tab)[y][x - 1].odw_sas > 1)
		(*tab)[y][x - 1].vis = 1;
	while ((*tab)[y + 1][x].vis == 0 || (*tab)[y][x + 1].vis == 0 || (*tab)[y - 1][x].vis == 0 || (*tab)[y][x - 1].vis == 0 ) {
		int wybor = rand() % 4;
		int x_nowa;
		int y_nowa; 
		if (wybor == 0) {
			x_nowa = x + 1;
			y_nowa = y;
		}
		else if (wybor == 1) {
			x_nowa = x - 1;
			y_nowa = y;
		}
		else if (wybor == 2) {
			x_nowa = x;
			y_nowa = y - 1;
		}
		else if (wybor == 3) {
			x_nowa = x;
			y_nowa = y + 1;
		}
		if ((*tab)[y_nowa][x_nowa].vis == 0 && (*tab)[y_nowa][x_nowa].odw_sas < 2) {
			(*tab)[y_nowa][x_nowa].odl = (*tab)[y][x].odl + 1;
			if ((*tab)[y_nowa][x_nowa].odl >= (*odleglosc)) {
				*y_kon = y_nowa;
				*x_kon = x_nowa;
				*odleglosc = (*tab)[y_nowa][x_nowa].odl;
			}
			generuj_maze(tab, y_nowa, x_nowa,y_kon,x_kon,odleglosc);
		}
	}
}
void rysuj_pocz_kon(struct komorka*** tab, int y_pocz, int x_pocz, int y_kon, int x_kon, int wiersze) {
	(*tab)[y_pocz][x_pocz].znak = 'M';
	(*tab)[y_kon][x_kon].znak = 'S';
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < wiersze; j++)
			if ((*tab)[i][j].znak == '0')
				(*tab)[i][j].vis_dfs = 1;
	}
}
void czysc_dfs(struct komorka*** tab, int wiersze) {
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < wiersze; j++) {
			if ((*tab)[i][j].znak == '0')
				(*tab)[i][j].vis_dfs = 1;
			else
				(*tab)[i][j].vis_dfs = 0;
		}
	}
}
void dfs(struct komorka*** tab, int y_pocz, int x_pocz, int y_kon, int x_kon) {
	(*tab)[y_pocz][x_pocz].vis_dfs = 1;
	if (((*tab)[y_pocz + 1][x_pocz].znak == 'S' || (*tab)[y_pocz - 1][x_pocz].znak == 'S' || (*tab)[y_pocz][x_pocz + 1].znak == 'S' || (*tab)[y_pocz][x_pocz - 1].znak == 'S') && (*tab)[y_pocz][x_pocz].znak != 'M') {
		(*tab)[y_pocz][x_pocz].znak = '.';

	}
	else {
		if ((*tab)[y_pocz + 1][x_pocz].vis_dfs == 0) {
			dfs(tab, y_pocz + 1, x_pocz,y_kon,x_kon);
			if ((*tab)[y_pocz + 1][x_pocz].znak == '.' && (*tab)[y_pocz][x_pocz].znak != 'M')
				(*tab)[y_pocz][x_pocz].znak = '.';
		}
		if ((*tab)[y_pocz - 1][x_pocz].vis_dfs == 0) {
			dfs(tab, y_pocz - 1, x_pocz, y_kon, x_kon);
			if ((*tab)[y_pocz - 1][x_pocz].znak == '.' && (*tab)[y_pocz][x_pocz].znak != 'M')
				(*tab)[y_pocz][x_pocz].znak = '.';
		}
		if ((*tab)[y_pocz][x_pocz + 1].vis_dfs == 0) {
			dfs(tab, y_pocz, x_pocz + 1, y_kon, x_kon);
			if ((*tab)[y_pocz][x_pocz + 1].znak == '.' && (*tab)[y_pocz][x_pocz].znak != 'M')
				(*tab)[y_pocz][x_pocz].znak = '.';
		}
		if ((*tab)[y_pocz][x_pocz - 1].vis_dfs == 0) {
			dfs(tab, y_pocz, x_pocz - 1, y_kon, x_kon);
			if ((*tab)[y_pocz][x_pocz - 1].znak == '.' && (*tab)[y_pocz][x_pocz].znak != 'M')
				(*tab)[y_pocz][x_pocz].znak = '.';
		}
	}
}
void zapisz_obraz(struct komorka** dane, int wiersze) {

	FILE* plik;
	errno_t err;
	printf("Podaj nazwe pliku (.txt), ktory stworze i zapisze tam labirynt: ");
	char nazwa[100];
	scanf("%s", nazwa);
	err = fopen_s(&plik, nazwa, "w");
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < wiersze; j++) {
			fprintf(plik, "%c ", dane[i][j].znak);
		}
		fprintf(plik, "\n");
	}
	fclose(plik);
}
int main() {
	srand(time(NULL));
	int n;
	printf("Szaleni naukowcy z Politechniki Wroclawskiej uwiezili mala myszke w wielkim labiryncie!\n");
	printf("Pomoz myszcze wydostac sie z labiryntu.\nNa poczatku podaj dlugosc boku labiryntu do wygenerowania (min 3): ");
	scanf_s("%d", &n);
	printf("Znaki oznaczaja\nM - myszka\nS - ser\n0 - sciana.\n");
	printf("Za pomoca klawiszy W A S D mozesz sterowac ruchami myszki.\nPo wcisnieciu specjalnego przecisku P myszka aktywuje swoj wech i pomoze ci w dotarciu do sera.\n\n\n");
	struct komorka** labirynt;
	matryca(&labirynt,n);
	int y_start = 1;
	int x_start = 1;
	int y_finish = 1;
	int x_finish = 1;
	int odl = 0;
	generuj_maze(&labirynt,1,1, &y_finish, &x_finish, &odl);
	rysuj_pocz_kon(&labirynt,1,1,y_finish,x_finish,n);
	wyswietl(labirynt, n);
	char ruch;
	while ((x_start != x_finish || y_start != y_finish)) {
		ruch = getch();
		switch (ruch) {
		case 'a':
			if (labirynt[y_start][x_start - 1].znak == ' ' || labirynt[y_start][x_start - 1].znak == 'S' || labirynt[y_start][x_start - 1].znak == '.') {
				labirynt[y_start][x_start].znak = ' ';
				labirynt[y_start][x_start - 1].znak = 'M';
				x_start--;
			}
			break;
		case 's':
			if (labirynt[y_start + 1][x_start].znak == ' ' || labirynt[y_start + 1][x_start].znak == 'S' || labirynt[y_start + 1][x_start].znak == '.') {
				labirynt[y_start][x_start].znak = ' ';
				labirynt[y_start + 1][x_start].znak = 'M';
				y_start++;
			}
			break;
		case 'd':
			if (labirynt[y_start][x_start + 1].znak == ' ' || labirynt[y_start][x_start + 1].znak == 'S' || labirynt[y_start][x_start + 1].znak == '.') {
				labirynt[y_start][x_start].znak = ' ';
				labirynt[y_start][x_start + 1].znak = 'M';
				x_start++;
			}
			break;
		case 'w':
			if (labirynt[y_start - 1][x_start].znak == ' ' || labirynt[y_start - 1][x_start].znak == 'S' || labirynt[y_start - 1][x_start].znak == '.') {
				labirynt[y_start][x_start].znak = ' ';
				labirynt[y_start - 1][x_start].znak = 'M';
				y_start--;
			}
			break;
		case 'p':
			dfs(&labirynt, y_start, x_start, y_finish, x_finish);
			czysc_dfs(&labirynt,n);
			break;
		default:;
		}
		system("cls");
		wyswietl_ruch(labirynt, n);
	}
	printf("\nUdalo sie, myszka dotarla do celu!\n");
	zapisz_obraz(labirynt, n);
	delokuj_pamiec(&labirynt, n);
	return 0;
}