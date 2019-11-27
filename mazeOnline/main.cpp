#include <iostream>
#include "main.h"
#include "MyBitMap.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"


int main(int argc, char* argv[])
{
	//ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	srand(GetTickCount());

	mazeGenerator mg;
	mazeSolver ms;
	int s;
	while (true)
	{
		cout << "Enter the maze size, an odd number bigger than 2 ( 0 to QUIT ): "; cin >> s;
		if (!s) return 0;
		if (!(s & 1)) s++;
		if (s >= 3)
		{
			mg.create(s);
			int sx, sy, ex, ey;
			while (true)
			{
				sx = rand() % s; sy = rand() % s;
				ex = rand() % s; ey = rand() % s;
				if (ex != sx || ey != sy) break;
			}
			ms.solveIt(mg.getMaze(), s, sx, sy, ex, ey);
			cout << endl;
		}
		system("pause");
		system("cls");
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
