#pragma once

#include <cmath>
#include <QList>
#define EPS 1E-8

struct Point2d
{
	double x, y;

	bool operator== (const Point2d b) const
	{
		return (abs(this->x - b.x) < EPS && abs(this->y - b.y) < EPS);
	}
	bool operator<(const Point2d& rhs) const
	{
		return this->x < rhs.x;
	}
};

struct Line
{
	double a, b, c;
};

struct Segment
{
	Point2d start, finish;
	bool operator==(const Segment& ref) const
	{
		return (this->start == ref.start) && (this->finish == ref.finish);
	}
	bool operator<(const Segment& rhs) const
	{
		return this->start < rhs.start;
	}
};

struct CalcHelperRetVal
{
	/// \brief Ћист содержащий листы=фигуры, в которых наход€тс€ их вершины
	QList<QList<Point2d> > figures;

	/// \brief “очки, образующие фигуру области пересечени€
	QList<Point2d> intersection_area;

	/// \brief ѕлощадь пересечени€
	double square;
};
