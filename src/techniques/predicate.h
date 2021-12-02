#ifndef PREDICATE_H
#define PREDICATE_H

#include "utils/point.h"

/* Prédicats géométriques */

// Retourne une valeur > à 0 si abc est orienté dans le sens trigonométrique
float testOrientation(const Point &a, const Point &b, const Point &c);
int isInTriangle(const Point &a, const Point &b, const Point &c, const Point &d);
// d est dans le cercle circonscrit au triangle (a,b,c) ? (a,b,c) orienté dans le sens trigonométrique
int etreDansCercle(const Point &a, const Point &b, const Point &c, const Point &d);

#endif // PREDICATE_H
