#ifndef __MAZE_GENERATOR_H
#define __MAZE_GENERATOR_H

#include "main.h"

class mazeGenerator
{
public:
	mazeGenerator()
	{
		_world = 0;
		_bmp.create(BMP_SIZE, BMP_SIZE);
		_bmp.setPenColor(RGB(0, 255, 0));
	}

	~mazeGenerator() { killArray(); }

	BYTE* getMaze() const { return _world; }

	void create(int side)
	{
		_s = side;
		generate();
	}

private:
	void generate()
	{
		killArray();
		_world = new BYTE[_s * _s];
		ZeroMemory(_world, _s * _s);
		_ptX = rand() % _s; _ptY = rand() % _s;
		carve();
	}

	void carve()
	{
		while (true)
		{
			int d = getDirection();
			if (d < NOR) return;

			switch (d)
			{
			case NOR:
				_world[_ptX + _s * _ptY] |= NOR; _ptY--;
				_world[_ptX + _s * _ptY] = SOU | SOU << 4;
				break;
			case EAS:
				_world[_ptX + _s * _ptY] |= EAS; _ptX++;
				_world[_ptX + _s * _ptY] = WES | WES << 4;
				break;
			case SOU:
				_world[_ptX + _s * _ptY] |= SOU; _ptY++;
				_world[_ptX + _s * _ptY] = NOR | NOR << 4;
				break;
			case WES:
				_world[_ptX + _s * _ptY] |= WES; _ptX--;
				_world[_ptX + _s * _ptY] = EAS | EAS << 4;
			}
		}
	}

	int getDirection()
	{
		int d = 1 << rand() % 4;
		while (true)
		{
			for (int x = 0; x < 4; x++)
			{
				if (testDir(d)) return d;
				d <<= 1;
				if (d > 8) d = 1;
			}
			d = (_world[_ptX + _s * _ptY] & 0xf0) >> 4;
			if (!d) return -1;
			switch (d)
			{
			case NOR: _ptY--; break;
			case EAS: _ptX++; break;
			case SOU: _ptY++; break;
			case WES: _ptX--; break;
			}

			d = 1 << rand() % 4;
		}
	}

	bool testDir(int d)
	{
		switch (d)
		{
		case NOR: return (_ptY - 1 > -1 && !_world[_ptX + _s * (_ptY - 1)]);
		case EAS: return (_ptX + 1 < _s && !_world[_ptX + 1 + _s * _ptY]);
		case SOU: return (_ptY + 1 < _s && !_world[_ptX + _s * (_ptY + 1)]);
		case WES: return (_ptX - 1 > -1 && !_world[_ptX - 1 + _s * _ptY]);
		}
		return false;
	}

	void killArray() { if (_world) delete[] _world; }

	BYTE* _world;
	int      _s, _ptX, _ptY;
	myBitmap _bmp;
};

#endif