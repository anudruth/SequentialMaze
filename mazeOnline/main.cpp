#include <iostream>
#include <sys\timeb.h>
#include "main.h"
#include "MyBitMap.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"



int main(int argc, char* argv[])
{
	struct timeb start_time, end_time;
	double total_time = 0;
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
			ftime(&start_time);
			mg.create(s);
			int sx, sy, ex, ey;
			sx = sy = 0;
			ex = ey = s - 1;			
			ms.solveIt(mg.getMaze(), s, sx, sy, ex, ey);
			ftime(&end_time);
			total_time = 1000 * (end_time.time - start_time.time) + (end_time.millitm - start_time.millitm);
			printf("Total execution time: %d\n", (int)total_time);
		}
		system("pause");
		system("cls");
	}
	return 0;
}
//--------------------------------------------------------------------------------------------------
