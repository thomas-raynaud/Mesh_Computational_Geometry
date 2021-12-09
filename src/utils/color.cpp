#include "color.h"

glm::vec3 hsv2rgb(int H, double S, double V) {
    double              hh, p, q, t, ff;
    long                i;
    glm::vec3           out;

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
            out.r = V;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = V;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = V;
            out.b = t;
            break;
        case 3:
            out.r = p;
            out.g = q;
            out.b = V;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = V;
            break;
        case 5:
        default:
            out.r = V;
            out.g = p;
            out.b = q;
            break;
    }
    return out;
}
