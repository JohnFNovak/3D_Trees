#HANDY

## Growing trees with OpenGL

###John Novak

john (period) franc (period) novak (at) gmail (dot) com <br />

####What it is:<br />
    This is an algorithmic tree growth program written in C++ and rendered with OpenGL. Trees are treated as a collection of branches. When the tree "buds" new branches are initialized at the ends of the previous generation of branches. Branches are initialized in pairs, with a fixed angle between them. When the tree "grows", all branches expand in length and radius.


####Requires:<br />
    g++
    OpenGL

####Note on OS:<br />
    This has been written and tested on OS X, using OpenGL and GLUT. It should be easy to get this to work on other systems, I just haven't tried yet.


####To use:<br />
    $ make
    $ ./branch


####Key bindings:<br />
    ESC - quit
    Space - play/pause
    . - bud. Init new branches
    x,X - pan camera in phi
    c,C - pan camera in theta
    z,Z - zoom in/out
    a,A,s,S,d,D - move location of camera focus
    , - turn on/off point of camera focus
    [,] - increase/decrease autozoom (camera will zoom out (or in) when growth is not paused)