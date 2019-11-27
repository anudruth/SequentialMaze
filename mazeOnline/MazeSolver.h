#ifndef __MAZE_SOLVER_H
#define __MAZE_SOLVER_H

#include "main.h"

class mazeSolver
{
public:
	mazeSolver()
	{
		_bmp.create(BMP_SIZE, BMP_SIZE);
		_pts = 0;
	}

	~mazeSolver() { killPoints(); }

	void solveIt(BYTE* maze, int size, int sX, int sY, int eX, int eY)
	{
		_lastDir = NONE;
		_world = maze; _s = size; _sx = sX; _sy = sY; _ex = eX; _ey = eY;

		for (int y = 0; y < _s; y++)
			for (int x = 0; x < _s; x++)
				_world[x + _s * y] &= 0x0f;

		_world[_sx + _s * _sy] |= NOR << 4;

		killPoints();
		_pts = new BYTE[_s * _s];
		ZeroMemory(_pts, _s * _s);

		findTheWay();

		_sx = sX; _sy = sY;
		display();
	}

private:
	int invert(int d)
	{
		switch (d)
		{
		case NOR: return SOU;
		case SOU: return NOR;
		case WES: return EAS;
		case EAS: return WES;
		}
		return NONE;
	}

	void updatePosition(int d)
	{
		switch (d)
		{
		case NOR: _sy--; break;
		case EAS: _sx++; break;
		case SOU: _sy++; break;
		case WES: _sx--;
		}
	}

	void findTheWay()
	{
		while (true)
		{
			int d = getDirection();
			if (d < NOR) return;
			_lastDir = invert(d);
			_world[_sx + _s * _sy] |= d;
			_pts[_sx + _s * _sy] = d;
			updatePosition(d);
			if (_sx == _ex && _sy == _ey) return;
			_world[_sx + _s * _sy] |= _lastDir << 4;
		}
	}

	int getDirection()
	{
		int d = 1 << rand() % 4;
		while (true)
		{
			for (int x = 0; x < 4; x++)
			{
				if (testDirection(d)) return d;
				d <<= 1;
				if (d > 8) d = 1;
			}

			d = (_world[_sx + _s * _sy] & 0xf0) >> 4;
			if (!d) return -1;
			_pts[_sx + _s * _sy] = 0;
			updatePosition(d);
			_lastDir = invert(d);
			d = 1 << rand() % 4;
		}
	}

	bool testDirection(int d)
	{
		if (d == _lastDir || !(_world[_sx + _s * _sy] & d)) return false;
		switch (d)
		{
		case NOR:
			return _sy - 1 > -1 && !(_world[_sx + _s * (_sy - 1)] & 0xf0);
		case EAS:
			return _sx + 1 < _s && !(_world[_sx + 1 + _s * _sy] & 0xf0);
		case SOU:
			return _sy + 1 < _s && !(_world[_sx + _s * (_sy + 1)] & 0xf0);
		case WES:
			return _sx - 1 > -1 && !(_world[_sx - 1 + _s * _sy] & 0xf0);
		}
		return false;
	}

	void display()
	{
		_bmp.setPenColor(RGB(255, 255, 255));
		_bmp.clear();
		HDC dc = _bmp.getDC();
		for (int y = 0; y < _s; y++)
		{
			int yy = y * _s;
			for (int x = 0; x < _s; x++)
			{
				BYTE b = _world[x + yy];
				int nx = x * CELL_SIZE,
					ny = y * CELL_SIZE;

				if (!(b & NOR))
				{
					MoveToEx(dc, nx, ny, NULL);
					LineTo(dc, nx + CELL_SIZE + 1, ny);
				}
				if (!(b & EAS))
				{
					MoveToEx(dc, nx + CELL_SIZE, ny, NULL);
					LineTo(dc, nx + CELL_SIZE, ny + CELL_SIZE + 1);
				}
				if (!(b & SOU))
				{
					MoveToEx(dc, nx, ny + CELL_SIZE, NULL);
					LineTo(dc, nx + CELL_SIZE + 1, ny + CELL_SIZE);
				}
				if (!(b & WES))
				{
					MoveToEx(dc, nx, ny, NULL);
					LineTo(dc, nx, ny + CELL_SIZE + 1);
				}
			}
		}
		_bmp.saveBitmap("maze.png");
		drawEndPoints(dc);
		_bmp.setPenColor(RGB(255, 0, 0));

		for (int y = 0; y < _s; y++)
		{
			int yy = y * _s;
			for (int x = 0; x < _s; x++)
			{
				BYTE d = _pts[x + yy];
				if (!d) continue;

				int nx = x * CELL_SIZE + 1,
					ny = y * CELL_SIZE + 1;

				MoveToEx(dc, nx, ny, NULL);
				switch (d)
				{
				case NOR: LineTo(dc, nx, ny - CELL_SIZE - 1); break;
				case EAS: LineTo(dc, nx + CELL_SIZE + 1, ny); break;
				case SOU: LineTo(dc, nx, ny + CELL_SIZE + 1); break;
				case WES: LineTo(dc, nx - CELL_SIZE - 1, ny); break;
				}
			}
		}

		_bmp.saveBitmap("maze_s.png");
		BitBlt(GetDC(GetConsoleWindow()), 10, 60, BMP_SIZE, BMP_SIZE, _bmp.getDC(), 0, 0, SRCCOPY);
	}

	void drawEndPoints(HDC dc)
	{
		RECT rc;
		int x = _sx , y = _sy ;
		SetRect(&rc, x, y, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
		FillRect(dc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
		x = 1 + _ex * CELL_SIZE, y = 1 + _ey * CELL_SIZE;
		SetRect(&rc, x, y, x + CELL_SIZE - 1, y + CELL_SIZE - 1);
		FillRect(dc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}

	void killPoints() { if (_pts) delete[] _pts; }

	BYTE* _world, * _pts;
	int      _s, _sx, _sy, _ex, _ey, _lastDir;
	myBitmap _bmp;
};

#endif