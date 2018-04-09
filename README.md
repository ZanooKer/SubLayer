# SubLayer

How to run:

from folder build-API

run ./API [cmd] [millimeter in width] [millimeter in height] [resolution mode] [input text file] [output file]

[cmd] can be help(to see all functions), visualize and exportO3DP

[millimeter in width] is the number of millimeters in the width of the based layer

[millimeter in height] is the number of millimeters in the height of the based layer

(based layer is the #0 layer)

[resolution mode] is the mode of resolution (pixel per millimeters) or '--auto-scale'. The program will find the resolution from the based layer pixels and millimeter in width&height

[input text file] is the configuration of input images and layer of that image

[output file] is the O3DP file as the output of the program

Input text file format:
_____________________________
| #layers,dl0,dl1,d2,...    |

| #l0,image1.png            |

| #l1,image2.png            |

|.                          |

|.                          |

|.                          |
_____________________________
#layers is how many layers image as input

dl0,dl1,dl2....dl(n) is the difference between layer(n) and layer(n+1)

#l0,#l1,#l2,...#l(n) is the number of layers related to dl(n)

image1.png,image2.png is the input image path

Suggestion:

#l0,#l1,... can be non-consecutive, but each one have to related to dl(n)
