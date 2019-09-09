#include "color.h"



std::array<double, 3> hsv2rgb(int H, double S, double V) {
    double              hh, p, q, t, ff;
    long                i;
    std::array<double, 3>  out;

    if(S <= 0.0) {
        out[0] = V;
        out[1] = V;
        out[2] = V;
        return out;
    }
    hh = H;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = V * (1.0 - S);
    q = V * (1.0 - (S * ff));
    t = V * (1.0 - (S * (1.0 - ff)));

    switch(i) {
    case 0:
        out[0] = V;
        out[1] = t;
        out[2] = p;
        break;
    case 1:
        out[0] = q;
        out[1] = V;
        out[2] = p;
        break;
    case 2:
        out[0] = p;
        out[1] = V;
        out[2] = t;
        break;

    case 3:
        out[0] = p;
        out[1] = q;
        out[2] = V;
        break;
    case 4:
        out[0] = t;
        out[1] = p;
        out[2] = V;
        break;
    case 5:
    default:
        out[0] = V;
        out[1] = p;
        out[2] = q;
        break;
    }
    return out;
}
