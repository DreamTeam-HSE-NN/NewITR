#pragma once
#include "Types.h"
#include <set>

class CalcHelper
{
private:
	// ¬се вопросы по ниженаписанному можете задавать команде разработки алгоритмов
	// мы, юайщики, вообще не понимаем что тут происходит
	static double PolarAngle(Point2d p0, Point2d p1);

	static int Det(Point2d p1, Point2d p2, Point2d p3);

	static double Distance(Point2d p0, Point2d p1);

	static std::vector<Point2d> Quicksort(std::vector<Point2d>& a);

	static double TriangleSquare(double a, double b, double c);

	static double CalculatePolygonSquare(std::vector<Point2d> res_pts);

	static std::vector <std::vector <Segment>> GeneratePolygons(uint32_t num_of_polygons);

	static bool IntersectionOfSegments(const Segment& s1, const Segment& s2, std::vector <Point2d>& vertex);

	static void CheckIntersection(const std::vector <std::vector<Segment>>& polygons, std::set<Segment>& inner_point, std::vector<Point2d>& temp_ans);

	static std::set<Point2d> FilterPoints(const std::set<Segment>& inner_point, std::vector<Point2d> temp_ans, const std::vector<std::vector<Segment>>
	                                      & polygons);
public:
	// *ui-team* retval мы выкинули, остальное не понимаем))
	static CalcHelperRetVal GenerateAndCalcPolygons(int num_of_polygons);
};

