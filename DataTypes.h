//
// Created by aspendlove on 3/30/24.
//

#ifndef TERRAQT_DATATYPES_H
#define TERRAQT_DATATYPES_H

enum class MouthShape {
    A,
    B,
    C,
    D,
    E,
    F,
    X
};

struct ShapeTime {
    int delay;
    MouthShape shape;
};


#endif //TERRAQT_DATATYPES_H
