#3D\_Trees

## Growing trees with OpenGL

###John Novak

john (period) franc (period) novak (at) gmail (dot) com <br />

####What it is:<br />
  This is an algorithmic tree growth program written in C++ and rendered with OpenGL. Trees are treated as a collection of branches. When the tree "buds" new branches are initialized at the ends of the previous generation of branches. Branches are initialized in pairs, with a fixed angle between them. When the tree "grows", all branches expand in length and radius.

####Note on OS:<br />
  This has been written and tested on OS X, using OpenGL and GLUT. It should be easy to get this to work on other systems, I just haven't tried yet.


####Requires:<br />
  g++<br />
  OpenGL


####To use:<br />
  $ make<br />
  $ ./branch


####Key bindings:<br />
  ESC - quit<br />
  Space - play/pause<br />
  . - bud. Init new branches<br />
  x,X - pan camera in phi<br />
  c,C - pan camera in theta<br />
  z,Z - zoom in/out<br />
  a,A,s,S,d,D - move location of camera focus<br />
  , - turn on/off point of camera focus<br />
  [,] - increase/decrease autozoom (camera will zoom out (or in) when growth is not paused)
