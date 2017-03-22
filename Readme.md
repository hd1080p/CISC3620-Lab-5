# Lab 5: Complete a lighting model

## Starting point

The initial code for this lab renders a rotating cube with diffuse shading implemented in the fragment shader. If, after building and running, you see a rotating cube with each side a different bright color, you are in the wrong branch. Type `git checkout lighting_hw` at the command line.

## Modification

Only diffuse shading is implemented in the fragment shader. Add code to the fragment shader to implement the specular and ambient components as well.

## Extra credit

Use an element buffer to store the vertices and give each vertex a normal that is the average of the three planes intersecting there (e.g. the front, top, left vertex would be the average of the front, top, and left directions: normalize(-0.33, 0.33, 0.33))

## Due date

Thursday, March 29 11:59pm
