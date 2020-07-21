# Backwards Raycaster (CSCI 5607)
This project is a globally illuminated raycaster.
The raytracer is capable of anti-aliasing, texture rendering,
transparency, reflection, mesh rendering, different lighting
techniques and more. The entire project is written in C++,
and is capable of running multiple threads on a CPU for improved
performance. The compiled project takes two command line arguments:
an input .txt file and an output .ppm file.

#Input File
The renderer uses a very simple method for rendering that
makes it easy to incorporate .obj files in your render.
A single text file is required which describes all elements
of your scene, as well as camera data. 

##Required fields

- eye
    - x y z
- viewdir
    - x y z
- updir
    - x y z
- hfov
    - float (in degrees)
- imsize
    - width height
- bkgcolor
    - red green blue
- mtlcolor
    - red green blue spec_red spec_green spec_blue ka kd ks n opacity refraction_index
##Other fields
- sphere
    - x y z r
- cylinder
    - x y z orientation_x orientation_y orientation_z r 
- light
    - x y z directional_flag red green blue
- depthcueing
    - red green blue max min dist_max dist_min
- texture
    - filename
- viewdist
    - float
### Triangles
- v
    - x y z
- vn
    - x y z
- vt
    - x y
- f
    - v1 v2 v3
    - v1/vt1 v2/vt2 v3/vt3
    - v1//vn1 v2//vn2 v3//vn3
    - v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
 
 ## Command ordering
 the input file will be parsed according to chronological order. Meaning,
 that if you want to color a sphere with a certain material the material should come first:
 ```
mtlcolor 1 1 0 1 1 1 0.1 0.2 1 200 0.1 2.44
sphere 1 -0.39 -4.5 0.6
```
This would produce a yellow transparent sphere with the same refractive index as diamond.

Chronological ordering does not matter for the required fields, but its generally advised to put these first

## Comments
comments can be provided with a '#' prefix
```
# This is a comment
mtlcolor 1 1 0 1 1 1 0.1 0.2 1 200 0.1 2.44
sphere 1 -0.39 -4.5 0.6
```

## Sample file
```
viewdir 0 -0.3 -1
eye 0 2 2.5
imsize 144 144
updir 0 1 0
hfov 60
bkgcolor 0.1 0.1 0.15

mtlcolor 0.5 0.8 0.4 1 1 1 0.2 0.5 0.1 10 1 1
sphere 0.5 -0.69 -4.5 0.3
sphere -1.5 -0.69 -3.75 0.3
sphere 1 -0.69 -0.5 0.3


mtlcolor 0 1 0 1 1 1 0 0 1 500 1 200
sphere -0.3 -0.49 -3 0.5
sphere -1.25 -0.39 -0.8 0.6

mtlcolor 1 1 0 1 1 1 0.1 0.2 1 200 0.1 2.44
sphere 1 -0.39 -4.5 0.6
sphere -0.25 -0.39 -1 0.6

mtlcolor 1 1 1 1 1 1 0.01 0.1 1 200 0.05 2.44
sphere 0.75 -0.19 -2.25 0.8

v -2 -1 0
v -1 -1 0
v -1 -1 -6
v -2 -1 -6

v -0 -1 0
v -0 -1 -6

v 1 -1 0
v 1 -1 -6

v 2 -1 0
v 2 -1 -6
#11
v -2 3 0
#12
v -2 3 -6
#13
v 2 3 0
#14
v 2 3 -6
#15
v -0.5 3 -2.5
#16
v -0.5 3 0
#17
v 0.5 3 -2.5
#18
v 0.5 3 0
#19
v -0.5 3 -3.5
#20
v -0.5 3 -6
#21
v 0.5 3 -3.5
#22
v 0.5 3 -6

mtlcolor 0.8 0.8 0.8 1 1 1 0.3 0.7 0 20 1 1
f 1 2 3
f 1 3 4
mtlcolor 0.8 0 0.8 1 1 1 0.3 0.7 0 20 1 1
f 2 6 3
f 2 5 6
mtlcolor 0.8 0.8 0.8 1 1 1 0.3 0.7 0 20 1 1
f 5 8 6
f 5 7 8
mtlcolor 0.8 0 0.8 1 1 1 0.3 0.7 0 20 1 1
f 7 10 8
f 7 9 10

mtlcolor 1 0.6 0.6 1 1 1 0.1 0.9 0 20 1 1
f 1 4 11
f 11 4 12

f 9 13 14
f 9 14 10

# Ceiling
mtlcolor 0.3 1 0.4 1 1 1 0.1 0.7 0 20 1 1
f 20 11 12
f 16 11 20
f 16 15 17
f 18 16 17
f 21 19 22
f 19 20 22
f 18 22 14
f 13 18 14

# Back walls
texture io/test_art.ppm
vt 0 0
vt 1 0
vt 1 1
vt 0 1
mtlcolor 1 0 0 1 1 1 0.5 0.5 0.4 20 1 1
f 4/1 10/2 14/3
f 4/1 14/3 12/4

light -0.1 -0.1 5 1 0.3 0.3 0.3
light 0 3.3 -3 1 0.5 0.5 0.5
```

## Sample Images
