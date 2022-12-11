#include <qmath.h>
#ifndef UTILS_H
#define UTILS_H
struct DoubleFullTime{

    int srHour;
    int srMin;

    int ssHour;
    int ssMin;
};
static float DegreesToRadians(float degrees)
{
    return degrees*(M_PI/180);
}
static float RadiansToDegrees(float degrees)
{
    return degrees*(180/M_PI);
}

#endif // UTILS_H
