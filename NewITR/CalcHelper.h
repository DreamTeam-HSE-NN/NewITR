#pragma once
#include "Types.h"

class CalcHelper
{
private:
	// ¬се вопросы по ниженаписанному можете задавать команде разработки алгоритмов
	// мы, юайщики, вообще не понимаем что тут происходит
	double polar_angle(Point2d p0, Point2d p1);
	int det(Point2d p1, Point2d p2, Point2d p3);
	double distance(Point2d p0, Point2d p1);
	std::vector<Point2d> quicksort(std::vector<Point2d>& a);
	double triangle_square(double A, double B, double C);
	double calculatePolygonSquare(std::vector<Point2d> res_pts);
	std::vector <std::vector <Segment>> GeneratePolygons(uint32_t numOfPolygons);
	bool IntersectionOfSegments(Segment s1, Segment s2, std::vector <Point2d>& vertex);

public:
	// *ui-team* retval мы выкинули, остальное не понимаем))
	CalcHelperRetVal GenerateAndCalcPolygons(int num_of_polygons);
};

