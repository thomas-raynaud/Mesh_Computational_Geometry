# TP1 - maillage

# Étudiants
* Jan AALMOES 11804741
* Thomas RAYNAUD 11402517

# Courbures

Nous avons analysé la courbure de trois maillages 3D différents : le tetrahèdre, la pyramide, la boîte de collision 2D et la statue (*queen.off*). La boîte de collision possède un sommet infini qui n'est pas affiché. Implémentation dans *meshcurvature.cpp*.

# Delaunay & Voronoi

## Triangulation naïve

- Implémentation de la division d'un triangle en 3, et du retournement d'arêtes (*meshdelaunay.cpp*).

- Prédicat d'orientation : tester si trois points d'un triangle sont orientés trigonomiquement ou non (*testOrientation, predicate.cpp*).

- Ajout d'un sommet aléatoirement, à l'intérieur ou à l'extérieur de l'enveloppe convexe. Si le sommet se trouve à l'extérieur de l'enveloppe convexe, on divise un triangle infini qui voit le point, puis on retourne les arêtes des autres triangles infinis qui voient le point. S'il est à l'intérieur, on divise un triangle pour ajouter le nouveau point.

## Insertion incrémentale de Delaunay

- Prédicat pour tester si un point est dans le cercle circonscrit d'un triangle (*etreDansCercle, predicate.cpp*), utilisé pour faire les flips potentiels d'arêtes.

- On ajoute un sommet aléatoirement dans le maillage. on crée avec les arêtes voyant le nouveau sommet inséré une file contenant les arêtes à retourner potentiellement.

## Voronoi

Le diagramme de Voronoi (*voronoi.cpp*) dual à la triangulation de Delaunay est affiché avec les traits rouges dans l'affichage wireframe. En vert : la triangulation de Delaunay.

## La marche de visibilité

on utilise cet algorithme (implémenté dans *visibilitymarch.cpp*) pour trouver efficacement dans quel triangle se trouve un point. Il est utilisé au moment de l'insertion d'un point aléatoire dans un maillage 2D.

# Paraboles

Les paraboles (*meshparabola.cpp*) sont des maillages 2D au départ, mais on a modifié leur composante z en fonction de la valeur de x et y. Deux fonctions peuvent être affichées : z=x\*x + y\*y et la fonction de Rosenbrock qui possède une propriété intéressante : c'est une fonction non-convexe.

Nous avons utilisé ces fonctions pour tester si notre application génère bien des triangulations de Delaunay. On insère 2000 points à des endroits aléatoires dans le maillage, et nous donnons à chaque point une couleur en fonction de la valeur de la composante z.

# Todo

- Show number of vertices + faces in UI
- Pbrt-like rendering
- Rendering with Vulkan
- Rendering with OpenGL: use vertex array objects, shaders, ...
- flat shading
- implicit surfaces