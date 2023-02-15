//
// Created by ajlou on 2/11/2023.
//

#include <cmath>

bool cmpZero(double A, double B, double epsilon = 0.001)
{
    return (fabs(A - B) < epsilon);
}

bool cmpGreaterOrEqual(double A, double B, double epsilon = 0.001)
{
    bool RV = false;
    if (A > B)
        RV = true;
    else
        RV = (fabs(A - B) < epsilon);
    return RV;
}
bool cmpLessOrEqual(double A, double B, double epsilon = 0.001)
{
    bool RV = false;
    if (A < B)
        RV = true;
    else
        RV = (fabs(A - B) < epsilon);
    return RV;
}