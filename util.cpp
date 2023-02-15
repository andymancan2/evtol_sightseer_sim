//
// Created by ajlou on 2/11/2023.
//

#include <cmath>

/// \brief Compare two doubles are nearly the same.
/// \param A
/// \param B
/// \param epsilon
/// \return
bool cmpZero(double A, double B, double epsilon = 0.001)
{
    return (fabs(A - B) < epsilon);
}

/// \brief Compare two doubles are Greater or Equal.
/// \param A
/// \param B
/// \param epsilon
/// \return
bool cmpGreaterOrEqual(double A, double B, double epsilon = 0.001)
{
    bool RV = false;
    if (A > B)
        RV = true;
    else
        RV = (fabs(A - B) < epsilon);
    return RV;
}

/// \brief Compare two doubles to less or equal.
/// \param A
/// \param B
/// \param epsilon
/// \return
bool cmpLessOrEqual(double A, double B, double epsilon = 0.001)
{
    bool RV = false;
    if (A < B)
        RV = true;
    else
        RV = (fabs(A - B) < epsilon);
    return RV;
}