
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <list>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std; 
/*************************************************************************************/
struct Cell {
public:
	int x, y, k;

	Cell() {}								//DOMYŚLNY KONSTRUKTOR
	Cell(int x, int y, int k) {				//KONSTRUKTOR TWORZĄCY OBIEKT STRUKTURY CELL
		this->x = x;						//GDZIE X, Y - WSPÓŁRZĘDNE, K - NUMER KOMÓRKI
		this->y = y;
		this->k = k;

	}
	bool operator== (const Cell &q) {			//PRZEŁADOWANIE OPERTORA == KONIECZNE BY MÓC USUWAĆ Z LISTY KONKRETNY ELEMENT
		return k == q.k;						//OPERATOR == PO PRZEŁADOWANIU PORÓWNUJE WARTOŚCI k DWÓCH OBIEKTÓW
	}			
};
/*************************************************************************************/
class Maze {							//KLASA MAZE BĘDĄCA IMPLEMENTACJĄ LABIRYNTU

	stack<Cell> stack;					//TWORZENIE STOSU UMOŻLIWIAJĄCEGO POWRÓT DO KOMÓRKI KTÓRA POSIADA JESZCZE NIEODWIEDONYCH SĄSIADÓW
	list<Cell> visited;					//LISTA PRZECHOWUJĄCA KOMÓRKI, KTÓRE ZOSTAŁY JUŻ ODWIEDZONE
	list<Cell> unvisited;				//LISTA PRZECHOWUJĄCA KOMÓRKI, KTÓRE JESZCZE NIE ZOSTAŁY ODWIEDZONE
	Cell current;						//KOMÓRKA NA KTÓREJ SĄ AKTUALNIE WYKONYWANE OPERACJE
	vector<vector<vector<bool>> > wall;	//TRÓJWYMIAROWY WEKTOR PRZECHOWUJĄCY INFORMACJE O ŚCIANIE KONKRETNEJ KOMÓRKI;
										//wall[x][y][z] x - wsp. X komórki, y - wsp. Y komórki, z - numer ściany komórki;
										//JEŚLI TRUE - ŚCIANA JEST, JEŚLI FALSE - NIE MA ŚCIANY 
										//(ALE MOŻE BYĆ W SĄSIEDNIEJ KOMÓRCE, CO JEST PÓŹNIEJ BRANE POD UWAGĘ)
	vector<Cell> cell;					//WEKTOR ZAWIERAJĄCY KOMÓRKI TWORZĄCE LABIRYNT
	int N;								//ILOŚĆ WIERSZY LABIRYNTU
	int M;								//ILOŚĆ KOMULN LABIRYNTU

public:

	Maze(int n, int m) {				//KONSTRUKTOR TWORZĄCY KOMÓRKI I ŚCIANY WCHODZĄCE W SKŁAD LABIRYNTU

		N = n;
		M = m;

		vector<bool> v1;
		vector<vector<bool>> v2;

		for (int k = 0; k < 4; k++) {
			v1.push_back(true);
		}

		for (int j = 0; j < m; j++) {
			v2.push_back(v1);
		}

		for (int i = 0; i < n; i++){
					
					wall.push_back(v2);
		}
			
			int q = 0;
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < N; j++) {
					cell.push_back(Cell(j, i, q));
					q++;
				}
			}

		for (int i = 0; i < (n * m); i++) {
			unvisited.push_back(cell[i]);
		}
	}

	void generateMaze();							//FUNKCJA GENERUJĄCA LOSOWY LABIRYNT
	void drawMaze();								//FUNKCJA RYSUJĄCA WCZEŚNIEJ UTWORZONY LABIRYNT
	int get_neighbours(Cell c);						//FUNKCJA ZWRACAJĄCA -1 GDY KOMÓRKA NIE MA NIEODWIEDZONEGO SĄSIADA
													//					  0 GDY WYLOSOWANO NIEDWIEDZONEGO SĄSIADA Z GÓRY
													//					  1 GDY WYLOSOWANO NIEDWIEDZONEGO SĄSIADA Z LEWEJ
													//					  2 GDY WYLOSOWANO NIEDWIEDZONEGO SĄSIADA Z PRAWEJ
													//					  2 GDY WYLOSOWANO NIEDWIEDZONEGO SĄSIADA Z DOŁU
};
/*************************************************************************************/
Maze labirynt(20, 20);								//TWORZENIE OBIEKTU KLASY MAZE
/*************************************************************************************/
void Maze::generateMaze()
{
	current = cell[rand() % (N + M)];				//LOSOWANIE JEDNEJ Z KOMÓREK OD KTÓREJ ZOSTANIE ROZPOCZĘTE GENEROWANIE LABIRYNTU
	visited.push_back(current);						//DODANIE WYLOSOWANEJ KOMÓRKI NA LISTĘ ODWIEDZONYCH
	unvisited.remove(current);						//USUNIĘCIE AKTUALNEJ KOMÓRKI Z LISTY NIEODWIEDZONYCH 

	int neighbours;									//ZMIENNA DO PRZECHOWYWANIA NUMERU WYLOSOWANEGO SĄSIADA
	Cell help;										//ZMIENNA POMOCNICZA PRZECHOWUJĄCA OBIEKT KLASY CELL
	int x = 0;										//ZMIENNA - LICZNIK 

	while (!unvisited.empty()) {					//PĘTLA WYKONUJĄCA SIĘ DOPÓKI ISTNIEJĄ NIEODWIEDZONE KOMÓRKI

		x++;
		neighbours = get_neighbours(current);		//WYLOSOWANIE NIEODWIEDZONEGO SĄSIADA
		help = current;

		if (neighbours == 0) {						//JEŻELI WYLOSOWANY SĄSIAD JEST Z GÓRY

			wall[current.x][current.y][0] = false;	//USUŃ ŚCIANĘ
			stack.push(current);					//ODŁÓŻ AKTUALNĄ KOMÓRKĘ NA STOS
			current = cell[current.k - N];			//ZASTĄP AKTUALNĄ KOMÓRKĘ NOWĄ
			visited.push_back(current);				//DODAJ NOWĄ AKTUALNĄ KOMÓRKĘ DO ODWIEDZONYCH 
			unvisited.remove(current);				//USUŃ NOWĄ AKTUALNĄ KOMÓRKĘ Z NIEODWIEDZONYCH
		}
		
		else if (neighbours == 1) {					//JEŻELI WYLOSOWANY SĄSIAD JEST Z LEWEJ

			wall[current.x][current.y][1] = false;
			stack.push(current);
			current = cell[current.k - 1];
			visited.push_back(current);
			unvisited.remove(current);
		}
		
		else if (neighbours == 2) {					//JEŻELI WYLOSOWANY SĄSIAD JEST Z PRAWEJ

			wall[current.x][current.y][2] = false;
			stack.push(current);
			current = cell[current.k + 1];
			visited.push_back(current);
			unvisited.remove(current);
		}
		
		else if (neighbours == 3) {					//JEŻELI WYLOSOWANY SĄSIAD JEST Z DOŁU

			wall[current.x][current.y][3] = false;
			stack.push(current);
			current = cell[current.k + N];
			visited.push_back(current);
			unvisited.remove(current);

		}
		
		else if (neighbours == -1) {				//JEŻELI AKTUALNA KOMÓRKA NIE POSIADA ŻADNYCH NIEODWIEDZONYCH SĄSIADÓW
			if (!stack.empty()) {					//JEŻELI STOS NIE JEST PUSTY
				Cell pomoc;							//ZMIENNA POMOCNICZA
				pomoc = stack.top();				//PRZYPISANIE DO POMOCY KOMÓRKI Z GÓRY STOSU
				stack.pop();						//ZDJĘCIE KOMÓRKI ZE STOSU
				current = pomoc;					//POMOC STAJE SIE AKTUALNĄ KOMÓRKĄ
			}
		}
		else {
			current = unvisited.front();			//AKTUALNĄ KOMÓRKĄ STAJE SIĘ PIERWSZA ZNAJDUJĄCA SIĘ NA LIŚCIE NIEODWIEDZONYCH 
			visited.push_back(current);
			unvisited.remove(current);
		}
	}
}
void Maze::drawMaze()
{
	
	for (int i = 0; i < M; i++) {					//PĘTLA ODPOWIEDZIALNA ZA KOLUMNY
		for (int j = 0; j < N; j++) {				//PĘTLA ODPOWIEDZIALNA ZA WIERSZE
			for (int k = 0; k < 4; k++) {			//PĘTLA ODPOWIEDZIALNA ZA KONKRETNA ŚCIANĘ 

					if (i > 0 && j > 0 && j < N - 1 && i < M - 1) {				//DALEJ SPRAWDZANY CZY KTÓRAŚ Z ODPOWIEDNICH ŚCIAN SĄSIEDNIEJ KOMÓRKI
																				//NIE JEST USTAWIONA NA FALSE
																				//JEŻELI TAK TO ODPOWIADAJACĄ ŚCIANĘ SĄSIEDNIEJ KOMÓRKI RÓWNIEŻ USTAWIAMY NA FALSE
					if (wall[j][i][0] == false || wall[j][i-1][3] == false) {

						wall[j][i][0] = false;
						wall[j][i - 1][3] = false;
					}
					if (wall[j][i][1] == false || wall[j-1][i][2] == false) {

						wall[j][i][1] = false;
						wall[j-1][i][2] = false;
					}
					if (wall[j][i][2] == false || wall[j+1][i][1] == false) {

						wall[j][i][2] = false;
						wall[j+1][i][1] = false;
					}
					if (wall[j][i][3] == false || wall[j][i + 1][0] == false) {

						wall[j][i][3] = false;
						wall[j][i + 1][0] = false;
					}

			}
				if(j == 0 && i < M-1){

					if (wall[j][i][3] == false || wall[j][i + 1][0] == false) {

						wall[j][i][3] = false;
						wall[j][i + 1][0] = false;
					}
				}
				if (i == 0 && j < N-1) {

					if (wall[j][i][2] == false || wall[j + 1][i][1] == false) {

						wall[j][i][2] = false;
						wall[j + 1][i][1] = false;
					}
				}
				if (j == N - 1 && i < M-1) {
					if (wall[j][i][3] == false || wall[j][i + 1][0] == false) {

						wall[j][i][3] = false;
						wall[j][i + 1][0] = false;
					}
				}
				if (i == M - 1 && j < N-1) {

					if (wall[j][i][2] == false || wall[j + 1][i][1] == false) {

						wall[j][i][2] = false;
						wall[j + 1][i][1] = false;
					}
				}
			}
		}
	}

													//RYSOWANIE KOLEJNYCH ŚCIAN
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 4; k++) {

				if (wall[j][i][k] == true) {			//JEŻELI DANA ŚCIANA JEST USTAWIONA NA TRUE TO JĄ NARYSUJ
														//DŁUGOŚCI ŚCIAN SĄ SKALOWANE PO TO BY ZAWSZE ZMIEŚCIĆ SIĘ W USTAWIONYM OKNIE
						if (k == 0) {
							glBegin(GL_LINES);
							glVertex2f((150 * j / N) - 75, (150 * i / M) - 75);
							glVertex2f(((j + 1) * 150)/N - 75, (i * 150)/M - 75);
							glEnd();
						}
						if (k == 3) {
							glBegin(GL_LINES);
							glVertex2f((150 * j)/N - 75, (150 * (i+1))/M - 75);
							glVertex2f(((j + 1) * 150)/N - 75, ((i+1) * 150)/M - 75);
							glEnd();
						}

						if (k == 1) {
							glBegin(GL_LINES);
							glVertex2f((150 * j)/N - 75, (150 * i)/M - 75);
							glVertex2f((j * 150 / N) - 75, (i + 1) * 150/M - 75);
							glEnd();
						}
						if (k == 2) {
							glBegin(GL_LINES);
							glVertex2f(150 * (j+1)/N - 75, 150 * i/M - 75);
							glVertex2f((j+1)* 150/N - 75, (i + 1) * 150/M - 75);
							glEnd();
						}
				}
			}
		}
	}
}
int Maze::get_neighbours(Cell c)
{
	bool losowanie = true;				//ZMIENNA STERUJĄCA PĘTLĄ 
	int neigh = 0;
	int neigh_t[4] = { -1,-1,-1,-1 };	//WSTĘPNIE WSZYSTKIE SĄSIEDNIE KOMÓRKI SĄ JUŻ ODWIEDZONE
	int tab[4] = { 0,0,0,0 };			

	if ((c.x -1) >= 0) {			//PRZESZUKUJEMY LISTĘ NIEODWIEDZONYCH KOMÓREK I SPRAWDZAMY 
									//CZY ZNAJDUJE SIĘ NA NIEJ KTÓRAŚ Z SĄSIEDNICH KOMÓREK

		for (list<Cell>::iterator it = unvisited.begin(); it != unvisited.end(); ++it) {
			Cell ce = *it;
			
			if (ce.x == (c.x-1) && ce.y == c.y) {
				tab[1] = 1;
				neigh_t[1] = 0;
			}
		}
	}
	if ((c.x + 1) < N) {

		for (list<Cell>::iterator it = unvisited.begin(); it != unvisited.end(); ++it) {
			Cell ce = *it;

			if (ce.x == (c.x + 1) && ce.y == c.y) {
				tab[2] = 1;
				neigh_t[2] = 0;
			}
		}
	}
	if ((c.y - 1) >= 0) {

		for (list<Cell>::iterator it = unvisited.begin(); it != unvisited.end(); ++it) {
			Cell ce = *it;
			

			if (ce.x == c.x && ce.y == (c.y-1)) {
				tab[0] = 1;
				neigh_t[0] = 0;
			}
		}
	}
	if ((c.y + 1) < M) {

		for (list<Cell>::iterator it = unvisited.begin(); it != unvisited.end(); ++it) {
			Cell ce = *it;
			
			if (ce.x == c.x && ce.y == (c.y + 1)) {
				tab[3] = 1;
				neigh_t[3] = 0;
			}
		}
	}
	
	while (losowanie) {			//LOSUJEMY DOPÓKI WYLOSOWANY ZOSTANIE NEODWIEDZONY SĄSIAD 
								//GDY KOMÓRKA NIE POSIADA NIEODWIEDZONEGO SĄSIADA TO PRZERYWAMY PĘTLĘ 

		if (neigh_t[0] != -1 || neigh_t[1] != -1 || neigh_t[2] != -1 || neigh_t[3] != -1) {
			neigh = rand() % 4;
			if (tab[neigh] == 1) {
				losowanie = false;
			}
		}
		else {
			losowanie = false;
			neigh = -1;
		}
	}
	return neigh;				//ZWRÓĆ WYLOSOWANĄ STRONĘ Z KTÓREJ ZNAJDUJE SIĘ WYLOSOWANA KOMÓRKA
								//LUB INFORMACJĘ O BRAKU NIEODWIEDZONEGO SĄSIADA (neigh = -1)
}
/*************************************************************************************/

// Funkcaja określająca, co ma być rysowane 
// (zawsze wywoływana, gdy trzeba przerysować scenę)
void RenderScene(void){

	glClear(GL_COLOR_BUFFER_BIT);	// Czyszczenie okna aktualnym kolorem czyszczącym
	glColor3f(1.0f, 1.0f, 1.0f);	// Ustawienie aktualnego koloru rysowania na biały
	
	labirynt.generateMaze();		//URUCHOMIENIE FUNKCJI GENERUJĄCEJ LABIRYNT
	labirynt.drawMaze();			//URUCHOMIENIE FUNKCJI RYSUJĄCEJ LABIRYNT					

	glFlush();						// Przekazanie poleceń rysujących do wykonania
}
/*************************************************************************************/
// Funkcja ustalająca stan renderowania
void MyInit(void){
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Kolor okna wnętrza okna - ustawiono na szary
}
/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego

void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna
{
	GLfloat AspectRatio;	// Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna
	if (vertical == 0)	vertical = 1;	// Zabezpieczenie pzred dzieleniem przez 0
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna urządzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);		// Określenie układu współrzędnych obserwatora
	glLoadIdentity();					// Określenie przestrzeni ograniczającej
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;		// Wyznaczenie współczynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
	// określenie okna obserwatora. 
	// Pozwala to zachować właściwe proporcje rysowanego obiektu
	// Do określenia okna obserwatora służy funkcja glOrtho(...)

	if (horizontal <= vertical)		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);
	else    glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);		// Określenie układu współrzędnych    
	glLoadIdentity();
}
/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli

int main(){
	srand(time(NULL));							

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyświetlania
	// GLUT_SINGLE - pojedynczy bufor wyświetlania
	// GLUT_RGBA - model kolorów RGB

	glutInitWindowSize(500, 500);			//USTAWIENIE STAŁYCH WIELKOSCI OKNA

	glutCreateWindow("LABIRYNT");	// Utworzenie okna i określenie treści napisu w nagłówku okna

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
	// Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
	// trzeba będzie przerysować okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie  
	// inicjalizacje konieczne przed przystąpieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/

