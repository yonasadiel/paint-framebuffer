# IF3270 Grafika Komputer

## Setup

    sudo apt-get install libncurses5-dev

## Compile and Run

Use tty1 for compile and run

    ./compile
    sudo ./main

## Images file

### Composite

Example, for drawing cursor:

    4                       // number of polygons
    4       // first polygon, consist of 4 points
    9 0                        // list of points,
    11 0                    // written as x and y
    11 7
    9 7
    4      // second polygon, consist of 4 points
    0 9
    7 9
    7 11
    0 11
    4       // third polygon, consist of 4 points
    13 9
    20 9
    20 11
    13 11
    4      // fourth polygon, consist of 4 points
    9 13
    11 13
    11 20
    9 20
