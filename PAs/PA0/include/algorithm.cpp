void BresenhamLine(int x0, int y0, int x1, int y1, Image *image, const Vector3f &color)
{
    int x, y, dx, dy;
    float k, e;
    dx = x1- x0;
    dy = y1- y0;
    k = dy / dx;
    e = -0.5;
    x = x0;
    y = y0;
    for (int i = 0; i <= dx; i++)
    {
        image->SetPixel(x, y, color);
        x++;
        e += k;
        if (e >= 0)
        {
            y++;
            e -= 1;
        }
    }
}

void MidPointCircle(int r, Image *image, const Vector3f &color)
{
    int x, y;
    float d;
    x = 0;
    y = r;
    d = 1.25 - r;
    image->SetPixel(x, y, color);
    while (x <= y)
    {
        if(d < 0)
        {
            d += 2 * x + 3;
        }
        else
        {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
        image->SetPixel(x, y, color);
    }
}