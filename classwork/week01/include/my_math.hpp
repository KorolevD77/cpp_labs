#pragma once
#include "ans_handler.hpp"

#ifndef PI
#define PI 3.14159265358979323
#endif

#ifndef EPSILON_FOR_MY_SIN
#define EPSILON_FOR_MY_SIN 1e-10
#endif

#ifndef MAX_ITS_FOR_MY_SIN
#define MAX_ITS_FOR_MY_SIN 20
#endif

AnswerHandler my_sin(double x);