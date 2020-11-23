# converter
highly customizable mesh files format converter(expl: .obj ->.stl)

Purpose:
Create a framework to convert 3D mesh data from a file format to another file format (expl: OBJ -> STL)

Implementation:
Instead of creating a class that could be subclassed and overload its API methods to handle different File Format,
I preferred creating two helper classes that can handle many 3DMesh file format just by defining a 'JSON' file.

please check: Serialize and Deserialize classes for the implementation detail.

All other classes are boiler plate and supporting source code.

how it works?

1- create a json file template format, according to some jargon defined in the documentation
(check the fileTemplate.json for explanation)

2- in this json file you will define how an Objet Mesh file Format is laid out,
how the vertex are formatted, the faces, the normal...etc! also you can add some meta info
about the extension of the file format and its version and backward compatibility

3- using this json template, instantiate Serialize class and a Deserialize class, give them 
this json tamplate as input

4- in a separate Header/Source file, create callbacks; which are function or lambdas or functor;
with a signature: bool (string, string, mesh) for Deserialize, and void (mesh, string ,outstream)
for Serializer, and add them to each instance accordingly.
this call backs takes an input line from a Mesh file and do theright thing;
either print it into the stream or read it into a local mesh object structure.
For each tag that define a mesh feature (like a vertex or face...etc) that you want to 
read or write create a callback and push it into the correct instance (Ser/Des)

5- instanciate a Converter class with a Serializer and Deserializer or only one of them. Et Voila!

This design is highly modularizable, since you can read and write any format, plus you can augment
your Read/Writer with any Mesh feature you want to handle (parametric surfaces like NURBS...etc)
and you don't need to worry about version conflicts between the same Mesh file formats, since All what 
you have to do ,is to create a callback that handle that format line, and it's done!
at the end you will have a library of functions that handle the Mesh feature you are interested in
reading or writing.

TDD is the center of this project, the coverage is almost 86% roughly.


BUGS and SUGGESTIONS are WELCOME!
