
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

	Cell() {}									//DOMYŚLNY KONSTRUKTOR
	Cell(int x, int y, int k) {					//KONSTRUKTOR TWORZĄCY OBIEKT STRUKTURY CELL GDZIE X, Y - WSPÓŁRZĘDNE, K - NUMER KOMÓRKI
		this->x = x;
		this->y = y;
		this->k = k;

	}
	bool operator== (const Cell &q) {			//PRZEŁADOWANIE OPERTORA == KONIECZNE BY MÓC USUWAĆ Z LISTY KONKRETNY ELEMENT
		return k == q.k;
	}
};
/*************************************************************************************/
class Maze {

	stack<Cell> stack;
	list<Cell> visited;
	list<Cell> unvisited;
	Cell current; 
	vector<vector<vector<bool>> > wall;
	vector<Cell> cell;
	int N;
	int M;

public:

	Maze(int n, int m) {

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

	void generateMaze(); 
	void drawMaze();
	int get_neighbours(Cell c);

	

};
/*************************************************************************************/

Maze labirynt(20, 20);

/*************************************************************************************/
void Maze::generateMaze()
{
	
	current = cell[rand() % (N + M)];
	visited.push_back(current);

	unvisited.remove(current);			

	int neighbours;
	Cell help;
	int x = 0;

	while (!unvisited.empty()) {

		x++;

		neighbours = get_neighbours(current);
		
		help = current;

		if (neighbours == 0) {

			wall[current.x][current.y][0] = false;

			stack.push(current);

			current = cell[current.k - N];

			visited.push_back(current);

			unvisited.remove(current);

		}
		
		else if (neighbours == 1) {

			wall[current.x][current.y][1] = false;

			stack.push(current);

			current = cell[current.k - 1];

			visited.push_back(current);

			unvisited.remove(current);
		
		}
		
		else if (neighbours == 2) {

			wall[current.x][current.y][2] = false;

			stack.push(current);

			current = cell[current.k + 1];

			visited.push_back(current);

			unvisited.remove(current);
			
		}
		
		else if (neighbours == 3) {

			wall[current.x][current.y][3] = false;

			stack.push(current);

			current = cell[current.k + N];

			visited.push_back(current);

			unvisited.remove(current);

		}
		
		else if (neighbours == -1) {
			if (!stack.empty()) {
				Cell pomoc;
				pomoc = stack.top();
				stack.pop();
				current = pomoc;
			}
		}
		else {
			current = unvisited.front();
			visited.push_back(current);
			unvisited.remove(current);
		
		}
	}
}
void Maze::drawMaze()
{
	
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 4; k++) {

				if (i > 0 && j > 0 && j < N - 1 && i < M - 1) {

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


	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < 4; k++) {

				if (wall[j][i][k] == true) {			

					if (k == 0 || k == 3) {

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

					}
					else if (k == 1 || k == 2) {

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

	

}
int Maze::get_neighbours(Cell c)
{
	bool losowanie = true;
	int neigh = 0;
	int neigh_t[4] = { -1,-1,-1,-1 };
	int tab[4] = { 0,0,0,0 };
	bool a;


	if ((c.x -1) >= 0) {

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
	
	

	while (losowanie) {

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
	return neigh;
}
/*************************************************************************************/
// Funkcaja określająca, co ma być rysowane 
// (zawsze wywoływana, gdy trzeba przerysować scenę)




void RenderScene(void)

{

	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym



	glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie aktualnego koloru rysowania na biały
	
	labirynt.generateMaze();
	labirynt.drawMaze();								


	glFlush();
	// Przekazanie poleceń rysujących do wykonania

}

/*************************************************************************************/

// Funkcja ustalająca stan renderowania



void MyInit(void)

{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Kolor okna wnętrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;


	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna urządzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)


	glMatrixMode(GL_PROJECTION);
	// Określenie układu współrzędnych obserwatora

	glLoadIdentity();
	// Określenie przestrzeni ograniczającej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie współczynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
	// określenie okna obserwatora. 
	// Pozwala to zachować właściwe proporcje rysowanego obiektu
	// Do określenia okna obserwatora służy funkcja glOrtho(...)



	if (horizontal <= vertical)

		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Określenie układu współrzędnych    

	glLoadIdentity();

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



int main()

{
	srand(time(NULL));

	

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyświetlania
	// GLUT_SINGLE - pojedynczy bufor wyświetlania
	// GLUT_RGBA - model kolorów RGB

	glutInitWindowSize(500, 500);


	glutCreateWindow("LABIRYNT");
	// Utworzenie okna i określenie treści napisu w nagłówku okna


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

