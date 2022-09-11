# Mesh and Computational Geometry

This repository regroups the practical works related to the subject *Mesh and Computational Geometry* I've been following during my studies at Université Lyon 1.

## Installation

- Install cmake
- Install glm, Catch2, Qt5 and OpenGL
- Create a script to populate the cmake cache when building the project. The one I use contains these lines :
```
set(CMAKE_INSTALL_PREFIX "[PROJECT_LOCATION]/build/install" CACHE PATH "Install directory")
set(glm_DIR "[GLM_LOCATION]/build/install/lib/cmake/glm" CACHE PATH "Path to the GLM library's cmake files")
set(Catch2_DIR "[CATCH2_LOCATION]/build/install/lib/cmake/Catch2" CACHE PATH "Path to the Catch2 library's cmake files")
set(CMAKE_BUILD_TYPE "Debug")
```
- Build the project : `cmake -C [PROJECT_LOCATION]/user_init_cache.txt -S [PROJECT_LOCATION] -B [PROJECT_LOCATION]/build`
- `cd [PROJECT_LOCATION]/build`
- Run the program: `./Mesh_Computational_Geometry`

**Contributors:**
* Jan Aalmoes
* Thomas Raynaud

**Teacher:**
* Raphaëlle Chaine

## 2D

### Delaunay
...

### Voronoi
...

### Crust

The [Crust algorithm](https://web.mit.edu/manoli/crust/www/crust.html) is used to reconstruct the surface of an object from a set of 2D/3D points.

**Idea :**
The skeleton of a shape regroups the centers of maximal balls contained in the object. The Crust algorithm consists in destroying any Delanay edge crossing the skeleton. The points of the samples are completed with new points whose insertion in Delaunay will kill the target edges. Those new points are the Voronoi centers. Edges of the curve to be reconstructed should be preserved by this insertion. In the new Delaunay triangulation, retain the edges joining two points from the entry sample only.

**Algorithm :**
- Construct the Delaunay triangulation with the points from the entry sample.
- Add the Voronoi vertices inside the Delaunay triangulation.
- Draw edges between two points in the Delaunay triangle only if they belong to the entry sample.

### Ruppert

...

## 3D

### Curvatures

Nous avons analysé la courbure de trois maillages 3D différents : le tetrahèdre, la pyramide, la boîte de collision 2D et la statue (*queen.off*). La boîte de collision possède un sommet infini qui n'est pas affiché. Implémentation dans *meshcurvature.cpp*.

### Mesh simplificaton
...

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

- Restore stable state (remove crashes, bugs)
- Put algorithms in different Qt Windows
- Rendering with OpenGL: use vertex array objects, shaders, ...
- Use QOpenGLWidget instead of QWidget
- Show number of vertices + faces in UI
- Pbrt-like rendering
- Rendering with Vulkan
- flat shading
- implicit surfaces