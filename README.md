# converter
highly customizable converter from mesh files to other mesh files (expl: .obj ->.stl)

Purpose:
Create a framework to convert 3D mesh data from a file format to another file format (expl: OBJ -> STL)

Implementation:
Instead of creating a class that could be subclassed and overload its API methods to handle different File Format,
I preferred creating two helper classes that can handle many 3DMesh file format just by defining a 'JSON' file.

please check: Serialize and Deserialize classes for the implementation detail.

All other classes are boiler plate and supporting source code.
