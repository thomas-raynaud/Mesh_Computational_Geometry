#include "color.h"

std::array<int, 3> hsv2rgb(double h, double s, double v) {
    double      hh, p, q, t, ff;
    long        i;
    std::array<int, 3>         out;

    if(s <= 0.0) {
        out[0] = v;
        out[1] = v;
        out[2] = v;
        return out;
    }
    hh = h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
    case 0:
        out[0] = v;
        out[1] = t;
        out[2] = p;
        break;
    case 1:
        out[0] = q;
        out[1] = v;
        out[2] = p;
        break;
    case 2:
        out[0] = p;
        out[1] = v;
        out[2] = t;
        break;

    case 3:
        out[0] = p;
        out[1] = q;
        out[2] = v;
        break;
    case 4:
        out[0] = t;
        out[1] = p;
        out[2] = v;
        break;
    case 5:
    default:
        out[0] = v;
        out[1] = p;
        out[2] = q;
        break;
    }
    return out;
}
