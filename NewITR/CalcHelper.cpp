#include "CalcHelper.h"
#include <iostream>
#include <vector>
#define USE_MATH_DEFINES
#include "Types.h"


/**
*@brief: Calculates polar angle for using it in function 'quicksort';
*@param: 2 points, which type is Point2d
*@return: double arctg 
*/
double CalcHelper::PolarAngle(const Point2d p0, const Point2d p1) 
{
	const double x_span = p0.x - p1.x;
	const double y_span = p0.y - p1.y;
	return atan2(y_span, x_span);
}

int CalcHelper::Det(const Point2d p1, const Point2d p2, const Point2d p3)
{
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

/**
*@brief: Calculates the (distance)^2 of two points;
*@param: 2 points, which type is Point2d
*@return: square of dustance, which type is double
*/
double CalcHelper::Distance(const Point2d p0, const Point2d p1)
{
	const double x_span = p0.x - p1.x;
	const double y_span = p0.y - p1.y;
	return y_span * y_span + x_span * x_span;
}

/**
*@brief: places points in right order to build a convex hull;
*@param: vector of points, which type is Point2d
*@return: sorted vector of points with Point2d type
*/
std::vector<Point2d> CalcHelper::Quicksort(std::vector<Point2d>& a)
{
	if (a.size() <= 1) {
		return a;
	}
	constexpr Point2d origin_point = {0, 0};
	std::vector<Point2d> smaller, equal, larger;
	const double piv_ang = PolarAngle(a[0], origin_point);
	for (auto& pt : a) {
		if (const double pt_ang = PolarAngle(pt, origin_point); pt_ang < piv_ang) {
			smaller.push_back(pt);
		}
		else if (abs(pt_ang - piv_ang) < EPS) {
			equal.push_back(pt);
		}
		else {
			larger.push_back(pt);
		}
	}
	smaller = Quicksort(smaller);
	larger = Quicksort(larger);
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
double CalcHelper::TriangleSquare(const double a, const double b, const double c)
{
	const double p = 0.5 * (a + b + c);
	return sqrt(p * (p - a) * (p - b) * (p - c));
}

/**
*@brief: Calculates area of polygon;
*@param: vector of polygon vertecies with type Point2d
*@return: double area of polygon
*/
double CalcHelper::CalculatePolygonSquare(std::vector<Point2d> res_pts)// Nikolay's function
{
	res_pts.push_back(res_pts[0]);
	double p1 = 0;
	double p2 = 0;
	for (int i = 0; i < res_pts.size() - 1; i++)
	{
		p1 += res_pts[i].x * res_pts[i + 1].y;
		p2 += res_pts[i].y * res_pts[i + 1].x;
	}
	const double a = abs(0.5 * (p1 - p2));
	return a;
}

/**
*@brief: Checks if two edges of different polygons intersect
*@param: 2 edges with Segment type
*@return: Logical, vector of intersection points
*/
bool CalcHelper::IntersectionOfSegments(const Segment& s1, const Segment& s2, std::vector <Point2d>& vertex)
{
	const Line l1 = { s1.start.y - s1.finish.y, s1.finish.x - s1.start.x, l1.a * s1.start.x + l1.b * s1.start.y };
	const Line l2 = { s2.start.y - s2.finish.y, s2.finish.x - s2.start.x, l2.a * s2.start.x + l2.b * s2.start.y };
	const double check = l1.a * l2.b - l1.b * l2.a;
	if (abs(check) < EPS)
	{
		return false;
	}
	if (const Point2d common_point = {(l1.c * l2.b - l2.c * l1.b) / check, (l1.a * l2.c - l1.c * l2.a) / check};
		sqrt(Distance(common_point, s1.start)) + sqrt(Distance(common_point, s1.finish)) - sqrt(
			Distance(s1.start, s1.finish)) < EPS
		&& sqrt(Distance(common_point, s2.start)) + sqrt(Distance(common_point, s2.finish)) - sqrt(Distance(s2.start, s2.finish)) < EPS)
	{
		vertex.push_back(common_point);
		return true;
	}
	return false;
}

/**
*@brief: finds intersection points of edges of each polygon with another one and assumes some points to be inner if edge of one polygon has only one intersection with another polygon
*@param: vector of vectors of each polygon edges
*@return: set with assumed inner points and vector tempAns which contain intersections of polygon edges
*/
void CalcHelper::CheckIntersection(const std::vector <std::vector<Segment>>& polygons, std::set<Segment> &inner_point, std::vector<Point2d> &temp_ans)
{
	int i = 0;
	while (i < polygons.size())
	{
		for (int j = 0; j < polygons.at(i).size(); ++j)
		{
			int count = 0;
			int k = 0;
			while (k < polygons.size())
			{
				if (k != i)
				{
					for (int l = 0; l < polygons.at(k).size(); ++l)
					{
						if (IntersectionOfSegments(polygons.at(i).at(j), polygons.at(k).at(l), temp_ans))
						{
							count++;
						}
					}
					if (count == 1 || count == 0)
					{
						inner_point.insert(polygons.at(i).at(j));
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
std::set<Point2d> CalcHelper::FilterPoints(const std::set<Segment>& inner_point, std::vector<Point2d> temp_ans,
                                           const std::vector<std::vector<Segment>>& polygons)
{
	std::set<Point2d> ans(temp_ans.begin(), temp_ans.end());
	for (Segment const seg : inner_point)
	{
		int count_polygons_start = 1;
		int count_polygons_finish = 1;
		for (auto& polygon : polygons)
		{
			if (std::find(polygon.begin(), polygon.end(), seg) == polygon.end())
			{
				double polygon_square_start = 0;
				double polygon_square_finish = 0;
				std::set<Point2d> coordinates;
				for (auto const& edge : polygon)
				{
					coordinates.insert(edge.start);
					coordinates.insert(edge.finish);
					double a = sqrt(Distance(seg.start, edge.start));
					double b = sqrt(Distance(seg.start, edge.finish));
					double c = sqrt(Distance(edge.start, edge.finish));
					polygon_square_start += TriangleSquare(a, b, c);
					a = sqrt(Distance(seg.finish, edge.start));
					b = sqrt(Distance(seg.finish, edge.finish));
					polygon_square_finish += TriangleSquare(a, b, c);
				}
				std::vector<Point2d> unique_coordinates(coordinates.begin(), coordinates.end());
				unique_coordinates = Quicksort(unique_coordinates);

				if (abs(CalculatePolygonSquare(unique_coordinates) - polygon_square_start) < EPS)
				{
					count_polygons_start++;
				}
				if (abs(CalculatePolygonSquare(unique_coordinates) - polygon_square_finish) < EPS)
				{
					count_polygons_finish++;
				}
			}
		}
		if (count_polygons_finish == polygons.size())
		{
			ans.insert(seg.finish);
		}
		if (count_polygons_start == polygons.size())
		{
			ans.insert(seg.start);
		}
	}
	std::set<Point2d> final_ans;
	for (auto& point : ans)
	{
		int count_polygons = 0;
		for (const auto& polygon : polygons)
		{
			double polygon_square = 0;
			std::set<Point2d> coordinates;
			for (auto const& edge : polygon)
			{
				coordinates.insert(edge.start);
				coordinates.insert(edge.finish);
				const double a = sqrt(Distance(point, edge.start));
				const double b = sqrt(Distance(point, edge.finish));
				const double c = sqrt(Distance(edge.start, edge.finish));
				if (a + b - c < EPS)
				{
					polygon_square += 0;
				}
				else
				{
					polygon_square += TriangleSquare(a, b, c);
				}
			}
			std::vector<Point2d> unique_coordinates(coordinates.begin(), coordinates.end());
			unique_coordinates = Quicksort(unique_coordinates);

			if (abs(CalculatePolygonSquare(unique_coordinates) - polygon_square) < EPS)
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
std::vector <std::vector <Segment>> CalcHelper::GeneratePolygons(const uint32_t num_of_polygons)
{
	std::vector <std::vector <Segment>> polygons;
	for (int i = 0; i < num_of_polygons; ++i)
	{
		std::vector <Point2d> temp_list;
		const uint32_t vertexes = rand() % (5 - 4 + 1) + 4;
		//vertexes = 3;
		std::vector<Segment> segments;

		const int radius = rand() % (30 - 20 + 1) + 20;
		const int y0 = 0;
		const int x0 = 0;

		for (int k = 1; k <= vertexes; ++k)
		{
			const int phi = rand() % (180 - 45 + 1) + 45;
			Point2d temp = { x0 + radius * cos(phi) , y0 + radius * sin(phi) };
			temp_list.push_back(temp);
		}
		std::vector <Point2d> sorted_temp_list;
		sorted_temp_list = Quicksort(temp_list);
		//sorted_tempList = tempList;
		for (int j = 0; j < sorted_temp_list.size(); ++j)
		{
			Segment segment = { sorted_temp_list.at(j), sorted_temp_list.at((j + 1) % sorted_temp_list.size()) };
			segments.push_back(segment);
		}
		polygons.push_back(segments);
	}
	return polygons;
}

/**
*@brief: implements functions: GeneratePolygons, checkIntersection, Filter_points
*@param: number of polygons
*@return: double area of intersectio area
*/
CalcHelperRetVal CalcHelper::GenerateAndCalcPolygons(const int num_of_polygons)
{
	CalcHelperRetVal result;
	result.square = -1;
	if (num_of_polygons < 2)
	{
		return result;
	}
	std::srand(std::time(nullptr));
	std::vector<Point2d> temp_ans;
	std::set<Segment> inner_point;
	const std::vector <std::vector <Segment>> polygons = GeneratePolygons(num_of_polygons);
	CheckIntersection(polygons, inner_point, temp_ans);
	std::set<Point2d> final_ans = FilterPoints(inner_point, temp_ans, polygons);

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
	const std::vector<Point2d> sorted_points_of_intersection = Quicksort(points_of_intersection);
	for (auto& pet : sorted_points_of_intersection)
	{
		result.intersection_area.append(pet);
	}

	// Площадь пересечения
	result.square = final_ans.size() > 2 ? CalculatePolygonSquare(sorted_points_of_intersection) : 0;

	return result;
}
