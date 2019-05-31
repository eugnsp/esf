# Finite elements library

## Synopsis

This is a finite elements library that is being developed mostly
for educational purposes. It was mainly written for a
Boltzmann-Poisson solver based on spherical harmonics expansion.

## Main features

* 1D meshes
* 2D meshed based on half-edge datastructure
* Vertex, edge and face iterators and views
* 1D and 2D Lagrange elements of variable degree that is fixed at compile-time
* 2D&otimes;1D Lagrange elements
* Guass and Dunavant rules for itegration
* Matrix-based linear, non-linear and eigensolvers
* Mesh faces colouring and multi-threaded matrix assembly
* Import from Gmsh mash files, export into VTK `.vtk` and Matlab `.mat` files

## Dependencies

* [`es_la` library](https://github.com/eugnsp/es_la)
* [`es_util` library](https://github.com/eugnsp/es_util)

Requires C++11/14/17 compiler.
