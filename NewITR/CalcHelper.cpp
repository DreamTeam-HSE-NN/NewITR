#include "CalcHelper.h"
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include "Types.h"
#define border 1000

/**
*@brief: Calculates polar angle for using it in function 'quicksort';
*@param: 2 points, which type is Point2d
*@return: double arctg 
*/
double CalcHelper::polar_angle(Point2d p0, Point2d p1) 
{
	double x_span = p0.x - p1.x;
	double y_span = p0.y - p1.y;
	return atan2(y_span, x_span);
}

int CalcHelper::det(Point2d p1, Point2d p2, Point2d p3)
{
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

/**
*@brief: Calculates the (distance)^2 of two points;
*@param: 2 points, which type is Point2d
*@return: square of dustance, which type is double
*/
double CalcHelper::distance(Point2d p0, Point2d p1)
{
	double x_span = p0.x - p1.x;
	double y_span = p0.y - p1.y;
	return y_span * y_span + x_span * x_span;
}

/**
*@brief: places points in right order to build a convex hull;
*@param: vector of points, which type is Point2d
*@return: sorted vector of points with Point2d type
*/
std::vector<Point2d> CalcHelper::quicksort(std::vector<Point2d>& a)
{
	if (a.size() <= 1) {
		return a;
	}
	Point2d originPoint;
	originPoint.x = 0;
	originPoint.y = 0;
	std::vector<Point2d> smaller, equal, larger;
	double piv_ang = polar_angle(a[0], originPoint);
	for (auto pt : a) {
		double pt_ang = polar_angle(pt, originPoint);
		if (pt_ang < piv_ang) {
			smaller.push_back(pt);
		}
		else if (abs(pt_ang - piv_ang) < EPS) {
			equal.push_back(pt);
		}
		else {
			larger.push_back(pt);
		}
	}
	smaller = quicksort(smaller);
	larger = quicksort(larger);
	std::vector<Point2d> sorted_pts;
	sorted_pts.insert(sorted_pts.end(), smaller.begin(), smaller.end());
	sorted_pts.insert(sorted_pts.end(), equal.begin(), equal.end());
	sorted_pts.insert(sorted_pts.end(), larger.begin(), larger.end());
	return sorted_pts;
}

/**
*@brief: Calculates area of triangle for using it in Filter_points and checkIntersection functions;
*@param: 3 edges of triangle with double type
*@return: double area of triangle
*/
double CalcHelper::triangle_square(double A, double B, double C)
{
	double p = 0.5 * (A + B + C);
	return sqrt(p * (p - A) * (p - B) * (p - C));
}

/**
*@brief: Calculates area of polygon;
*@param: vector of polygon vertecies with type Point2d
*@return: double area of polygon
*/
double CalcHelper::calculatePolygonSquare(std::vector<Point2d> res_pts)// Nikolay's function
{
	res_pts.push_back(res_pts[0]);
	double p1 = 0;
	double p2 = 0;
	for (int i = 0; i < res_pts.size() - 1; i++)
	{
		p1 += res_pts[i].x * res_pts[i + 1].y;
		p2 += res_pts[i].y * res_pts[i + 1].x;
	}
	double a = abs(0.5 * (p1 - p2));
	return a;
}

/**
*@brief: Checks if two edges of different polygons intersect
*@param: 2 edges with Segment type
*@return: Logical, vector of intersection points
*/
bool CalcHelper::IntersectionOfSegments(Segment s1, Segment s2, std::vector <Point2d>& vertex)
{
	Line l1 = { s1.start.y - s1.finish.y, s1.finish.x - s1.start.x, l1.a * s1.start.x + l1.b * s1.start.y };
	Line l2 = { s2.start.y - s2.finish.y, s2.finish.x - s2.start.x, l2.a * s2.start.x + l2.b * s2.start.y };
	double check = l1.a * l2.b - l1.b * l2.a;
	if (abs(check) < EPS)
	{
		return false;
	}
	Point2d commonPoint = { (l1.c * l2.b - l2.c * l1.b) / check, (l1.a * l2.c - l1.c * l2.a) / check };
	if (sqrt(distance(commonPoint, s1.start)) + sqrt(distance(commonPoint, s1.finish)) - sqrt(distance(s1.start, s1.finish)) < EPS
		&& sqrt(distance(commonPoint, s2.start)) + sqrt(distance(commonPoint, s2.finish)) - sqrt(distance(s2.start, s2.finish)) < EPS)
	{
		vertex.push_back(commonPoint);
		return true;
	}
	return false;
}

/**
*@brief: finds intersection points of edges of each polygon with another one and assumes some points to be inner if edge of one polygon has only one intersection with another polygon
*@param: vector of vectors of each polygon edges
*@return: set with assumed inner points and vector tempAns which contain intersections of polygon edges
*/
void CalcHelper::checkIntersecton(std::vector <std::vector<Segment>> polygons, std::set<Segment> &InnerPoint, std::vector<Point2d> &tempAns)
{
	int i = 0;
	while (i < polygons.size())
	{
		for (int j = 0; j < polygons.at(i).size(); ++j)
		{
			bool checkPoint;
			int count = 0;
			int k = 0;
			while (k < polygons.size())
			{
				if (k != i)
				{
					for (int l = 0; l < polygons.at(k).size(); ++l)
					{
						checkPoint = IntersectionOfSegments(polygons.at(i).at(j), polygons.at(k).at(l), tempAns);
						if (checkPoint)
						{
							count++;
						}
					}
					if (count == 1 || count == 0)
					{
						InnerPoint.insert(polygons.at(i).at(j));
					}
				}
				k++;
			}
		}
		i++;
	}
}

/**
*@brief: Checkes inner points and intersection points if they belong to each polygon (edges or inner areas) and gets rid off extra points
*@param: inner points (set type), tempAns - vector of intersection points and vector of vectors of polygon edges
*@return: set of final points of polygon intersection
*/
std::set<Point2d> CalcHelper::Filter_points(std::set<Segment> InnerPoint, std::vector<Point2d> tempAns, std::vector<std::vector<Segment>> polygons)
{
	std::set<Point2d> ans(tempAns.begin(), tempAns.end());
	for (Segment const seg : InnerPoint)
	{
		int count_polygonsStart = 1;
		int count_polygonsFinish = 1;
		for (auto polygon : polygons)
		{
			if (std::find(polygon.begin(), polygon.end(), seg) == polygon.end())
			{
				double PolygonSquareStart = 0;
				double PolygonSquareFinish = 0;
				std::set<Point2d> coordinates;
				for (auto const edge : polygon)
				{
					coordinates.insert(edge.start);
					coordinates.insert(edge.finish);
					double A = sqrt(distance(seg.start, edge.start));
					double B = sqrt(distance(seg.start, edge.finish));
					double C = sqrt(distance(edge.start, edge.finish));
					PolygonSquareStart += triangle_square(A, B, C);
					A = sqrt(distance(seg.finish, edge.start));
					B = sqrt(distance(seg.finish, edge.finish));
					PolygonSquareFinish += triangle_square(A, B, C);
				}
				std::vector<Point2d> unique_coordinates(coordinates.begin(), coordinates.end());
				unique_coordinates = quicksort(unique_coordinates);

				if (abs(calculatePolygonSquare(unique_coordinates) - PolygonSquareStart) < EPS)
				{
					count_polygonsStart++;
				}
				if (abs(calculatePolygonSquare(unique_coordinates) - PolygonSquareFinish) < EPS)
				{
					count_polygonsFinish++;
				}
			}
		}
		if (count_polygonsFinish == polygons.size())
		{
			ans.insert(seg.finish);
		}
		if (count_polygonsStart == polygons.size())
		{
			ans.insert(seg.start);
		}
	}
	std::set<Point2d> final_ans;
	for (auto point : ans)
	{
		int count_polygons = 0;
		for (auto polygon : polygons)
		{
			double PolygonSquare = 0;
			std::set<Point2d> coordinates;
			for (auto const edge : polygon)
			{
				coordinates.insert(edge.start);
				coordinates.insert(edge.finish);
				double A = sqrt(distance(point, edge.start));
				double B = sqrt(distance(point, edge.finish));
				double C = sqrt(distance(edge.start, edge.finish));
				if (A + B - C < EPS)
				{
					PolygonSquare += 0;
				}
				else
				{
					PolygonSquare += triangle_square(A, B, C);
				}
			}
			std::vector<Point2d> unique_coordinates(coordinates.begin(), coordinates.end());
			unique_coordinates = quicksort(unique_coordinates);

			if (abs(calculatePolygonSquare(unique_coordinates) - PolygonSquare) < EPS)
			{
				count_polygons++;
			}
		}
		if (count_polygons == polygons.size())
		{
			final_ans.insert(point);
		}
	}
	return final_ans;
}

/**
*@brief: Generates convex polygons with different number of vertices
*@param: number of polygons, which type is uint32_t
*@return: vector of vectors of each polygon edges, the size of vector is equal to the number of polygons
*/
std::vector <std::vector <Segment>> CalcHelper::GeneratePolygons(uint32_t numOfPolygons)
{
	std::vector <std::vector <Segment>> Polygons;
	for (int i = 0; i < numOfPolygons; ++i)
	{
		std::vector <Point2d> tempList;
		uint32_t vertexes;
		vertexes = rand() % (5 - 4 + 1) + 4;
		//vertexes = 3;
		std::vector<Segment> segments;
		int x0 = 0;
		int y0 = 0;
		int radius = rand() % (30 - 20 + 1) + 20;
		for (int k = 1; k <= vertexes; ++k)
		{
			Point2d temp;
			//float phi = 2 * M_PI * rand();
			int phi = rand() % (180 - 45 + 1) + 45;
			temp.x = x0 + radius * cos(phi);
			temp.y = y0 + radius * sin(phi);
			tempList.push_back(temp);
		}
		std::vector <Point2d> sorted_tempList;
		sorted_tempList = quicksort(tempList);
		//sorted_tempList = tempList;
		for (int j = 0; j < sorted_tempList.size(); ++j)
		{
			Segment segment;
			segment.start = sorted_tempList.at(j);
			segment.finish = sorted_tempList.at((j + 1) % sorted_tempList.size());
			segments.push_back(segment);
		}
		Polygons.push_back(segments);
	}
	return Polygons;
}

/**
*@brief: implements functions: GeneratePolygons, checkIntersection, Filter_points
*@param: number of polygons
*@return: double area of intersectio area
*/
CalcHelperRetVal CalcHelper::GenerateAndCalcPolygons(int num_of_polygons)
{
	CalcHelperRetVal result;
	result.square = -1;
	if (num_of_polygons < 2)
	{
		return result;
	}
	std::srand(std::time(NULL));
	std::vector<Point2d> tempAns;
	std::set<Segment> InnerPoint;
	std::set<Point2d> final_ans;
	std::vector <std::vector <Segment>> polygons = GeneratePolygons(num_of_polygons);
	checkIntersecton(polygons, InnerPoint, tempAns);
	final_ans = Filter_points(InnerPoint, tempAns, polygons);

	// Фигуры и их вершины
	for (auto& figure : polygons)
	{
		QList<Point2d> resfig;
		for (auto& segment : figure)
		{
			resfig.append(segment.start);
		}
		result.figures.append(resfig);
	}

	// Область пересечения
	std::vector<Point2d> points_of_intersection(final_ans.begin(), final_ans.end());
	std::vector<Point2d> sorted_points_of_intersection = quicksort(points_of_intersection);
	for (auto pet : sorted_points_of_intersection)
	{
		result.intersection_area.append(pet);
	}

	// Площадь пересечения
	result.square = final_ans.size() > 2 ? calculatePolygonSquare(sorted_points_of_intersection) : 0;

	return result;
}
