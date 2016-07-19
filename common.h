#ifndef _COMMON_H_HEADER_
#define _COMMON_H_HEADER_

struct Point
{
    Point(int a, int b) : x(a), y(b)
    {
    }

    int x, y;
};

struct Rect
{
    Rect(int left, int top, int right, int bottom) : start(left, top), end(right, bottom)
    {
    }

    Rect(const Point& a, const Point& b) : start(a), end(b)
    {
    }

    Point dimensions() const
    {
        return Point(end.x - start.x, end.y - start.y);
    }

    Point start;
    Point end;
};

#endif