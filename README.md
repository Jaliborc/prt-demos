# PRT Demos
A set of C++ applications showcasing the use of Precomputed Radiance Transfer for Geometry with Pose Varying Subsurface Properties:
* **Glove.cpp** - Allows a user to interactively control a virtual hand using a [ShapeHand](http://handmotioncapture.com/shapehand.html) glove.
* **Recording.cpp** - Plays a recording of human hand motion capture on the virtual hand.
* **Pdt.cpp** - Plays trough all the poses stored in a .pdt model on the virtual hand.

Supported models can be created using [prt generator](https://github.com/jaliborc/prt-generator) and tested in [prt analyser](https://github.com/jaliborc/prt-analyser). For already generated models and example media files, see [releases](https://github.com/Jaliborc/prt-demo/releases).

[![Preview](http://jaliborc.com/images/research/prt-demos.png)](https://goo.gl/bxt1rq)
[Preview](https://goo.gl/bxt1rq)

### Controls
* Up/Down Arrows - change environment
* Left/Right Arrows - rotate hand
* Q/A - increase/reduce diffuse
* W/S - increase/reduce specular
