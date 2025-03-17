#pragma once

#include <image.hpp>
#include <algorithm>
#include <queue>
#include <cstdio>

class Element
{
public:
    virtual void draw(Image &img) = 0;
    virtual ~Element() = default;
};

class Line : public Element
{

public:
    int xA, yA;
    int xB, yB;
    Vector3f color;
    void draw(Image &img) override
    {
        int x0 = xA, y0 = yA;
        int x1 = xB, y1 = yB;
        int dx = abs(x1 - x0);
        int dy = -abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;
        while (true)
        {
            img.SetPixel(x0, y0, color);
            if (x0 == x1 && y0 == y1)
                break;
            int e2 = 2 * err;
            if (e2 >= dy)
            { 
                if (x0 == x1)
                    break;
                err += dy; 
                x0 += sx; 
            }
            if (e2 <= dx)
            {
                if (y0 == y1)
                    break;
                err += dx; 
                y0 += sy;  
            }
        }
        printf("Draw a line from (%d, %d) to (%d, %d) using color (%f, %f, %f)\n",
               xA, yA, xB, yB, color.x(), color.y(), color.z());
    }
};

class Circle : public Element
{

public:
    int cx, cy;
    int radius;
    Vector3f color;
    void draw(Image &img) override
    {
        int x = 0;
        int y = radius;
        float d = 1.25 - radius;

        auto drawCirclePoints = [&](int x, int y)
        {
            img.SetPixel(cx + x, cy + y, color);
            img.SetPixel(cx - x, cy + y, color);
            img.SetPixel(cx + x, cy - y, color);
            img.SetPixel(cx - x, cy - y, color);
            img.SetPixel(cx + y, cy + x, color);
            img.SetPixel(cx - y, cy + x, color);
            img.SetPixel(cx + y, cy - x, color);
            img.SetPixel(cx - y, cy - x, color);
        };

        drawCirclePoints(x, y);

        while (x <= y)
        {
            if (d < 0)
            {
                d += 2 * x + 3;
            }
            else
            {
                d += 2 * (x - y) + 5;
                y--;
            }
            x++;
            drawCirclePoints(x, y);
        }

        printf("Draw a circle with center (%d, %d) and radius %d using color (%f, %f, %f)\n",
               cx, cy, radius, color.x(), color.y(), color.z());
    }
};

class Fill : public Element
{

public:
    int cx, cy;
    Vector3f color;
    void draw(Image &img) override
    {
        // TODO: Flood fill alorithm
        Vector3f targetColor = img.GetPixel(cx, cy);
        if (targetColor == color)
        {
            return;
        }
        std::queue<std::pair<int, int>> qupixels;
        qupixels.push(std::make_pair(cx, cy));
        int width = img.Width();
        int height = img.Height();
        while (!qupixels.empty())
        {
            std::pair<int, int> pixel = qupixels.front();
            qupixels.pop();
            if (pixel.first < 0 || pixel.first >= width || pixel.second < 0 || pixel.second >= height)
            {
                continue;
            }
            if (img.GetPixel(pixel.first, pixel.second) != targetColor)
            {
                continue;
            }
            img.SetPixel(pixel.first, pixel.second, color);
            qupixels.push(std::make_pair(pixel.first + 1, pixel.second));
            qupixels.push(std::make_pair(pixel.first - 1, pixel.second));
            qupixels.push(std::make_pair(pixel.first, pixel.second + 1));
            qupixels.push(std::make_pair(pixel.first, pixel.second - 1));
        }

        printf("Flood fill source point = (%d, %d) using color (%f, %f, %f)\n", cx, cy,
               color.x(), color.y(), color.z());
    }
};
