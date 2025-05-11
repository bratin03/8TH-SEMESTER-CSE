#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

using namespace std;

struct Point
{
    double x, y;

    bool operator<(const Point &other) const
    {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Segment
{
    Point p1, p2;

    Segment(Point p1, Point p2) : p1(p1), p2(p2)
    {
        if (p2 < p1)
            swap(this->p1, this->p2); // Ensure p1 is always the smaller point
    }

    bool contains(const Point &p) const
    {
        return (p.x >= min(p1.x, p2.x) && p.x <= max(p1.x, p2.x) &&
                p.y >= min(p1.y, p2.y) && p.y <= max(p1.y, p2.y));
    }
};

class SweepLine
{
    struct Event
    {
        Point point;
        int segmentIndex;
        bool isStart;

        bool operator<(const Event &other) const
        {
            if (point.x != other.point.x)
                return point.x < other.point.x;
            return isStart > other.isStart;
        }
    };

    vector<Segment> segments;
    set<int> activeSegments;

    static double crossProduct(const Point &a, const Point &b, const Point &c)
    {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    static bool doIntersect(const Segment &s1, const Segment &s2, Point &intersection)
    {
        double d1 = crossProduct(s1.p1, s1.p2, s2.p1);
        double d2 = crossProduct(s1.p1, s1.p2, s2.p2);
        double d3 = crossProduct(s2.p1, s2.p2, s1.p1);
        double d4 = crossProduct(s2.p1, s2.p2, s1.p2);

        if (d1 * d2 < 0 && d3 * d4 < 0)
        {
            // Compute intersection point
            double a1 = s1.p2.y - s1.p1.y;
            double b1 = s1.p1.x - s1.p2.x;
            double c1 = a1 * s1.p1.x + b1 * s1.p1.y;

            double a2 = s2.p2.y - s2.p1.y;
            double b2 = s2.p1.x - s2.p2.x;
            double c2 = a2 * s2.p1.x + b2 * s2.p1.y;

            double det = a1 * b2 - a2 * b1;
            if (abs(det) < 1e-9)
                return false; // Parallel

            intersection.x = (b2 * c1 - b1 * c2) / det;
            intersection.y = (a1 * c2 - a2 * c1) / det;
            return s1.contains(intersection) && s2.contains(intersection);
        }
        return false;
    }

public:
    SweepLine(const vector<Segment> &segments) : segments(segments) {}

    vector<Point> findIntersections()
    {
        vector<Event> events;
        for (int i = 0; i < segments.size(); ++i)
        {
            events.push_back({segments[i].p1, i, true});
            events.push_back({segments[i].p2, i, false});
        }
        sort(events.begin(), events.end());

        vector<Point> intersections;
        for (const auto &event : events)
        {
            int idx = event.segmentIndex;
            if (event.isStart)
            {
                for (int activeIdx : activeSegments)
                {
                    Point intersection;
                    if (doIntersect(segments[idx], segments[activeIdx], intersection))
                    {
                        intersections.push_back(intersection);
                    }
                }
                activeSegments.insert(idx);
            }
            else
            {
                activeSegments.erase(idx);
            }
        }

        sort(intersections.begin(), intersections.end());
        intersections.erase(unique(intersections.begin(), intersections.end()), intersections.end());
        return intersections;
    }
};

int main()
{
    vector<Segment> segments = {
        {{0, 0}, {4, 4}},
        {{0, 4}, {4, 0}},
        {{2, 2}, {6, 2}},
        {{2, 0}, {2, 4}}};

    SweepLine sweepLine(segments);
    vector<Point> intersections = sweepLine.findIntersections();

    cout << "Intersections found: " << intersections.size() << endl;
    for (const auto &p : intersections)
    {
        cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}
