//
// Created by ajlou on 2/11/2023.
//

#include <cmath>

bool cmpd(double A, double B, double epsilon = 0.001)
{
    return (fabs(A - B) < epsilon);
}