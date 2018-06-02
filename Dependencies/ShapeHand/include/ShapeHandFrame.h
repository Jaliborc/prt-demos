#ifndef _ShapeHandFrame_H_
#define _ShapeHandFrame_H_

struct ShapeHandFrame
{

    static const int sNumFingers = 5;
    static const int sJointsPerFinger = 3;

    ShapeHandFrame();
    ShapeHandFrame(const ShapeHandFrame& other);

    float hand[4];                                      ///< quaternion for the wrist
    float finger[sNumFingers][sJointsPerFinger][4];     ///< quaternion for each finger joint, with finger index thumb = 0 ... pinky = 4, and joint index proximal = 0 ... distal = 2
    unsigned int timestamp;

    bool fromBuf(char* buf);

    void print();

};

#endif  // _ShapeHandFrame_H_
