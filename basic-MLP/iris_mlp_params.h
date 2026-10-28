
#ifndef IRIS_MLP_PARAMS_H
#define IRIS_MLP_PARAMS_H

#include <ap_int.h>
#include <stdint.h>

#define IRIS_INPUTS 4
#define IRIS_HIDDEN 8
#define IRIS_OUTPUTS 3

#define IRIS_INPUT_SCALE 0.02405509166
#define IRIS_WEIGHT1_SCALE 0.01734972745
#define IRIS_HIDDEN_SCALE 0.05496417731
#define IRIS_WEIGHT2_SCALE 0.01505700406

#define IRIS_REQUANT_MULT 16306089
#define IRIS_REQUANT_SHIFT 31

static const ap_int<8> W1[8][4] = {{ 26, -24,  -8,  11},
 { 33, -24,  80,  79},
 {-11, -43, 115, 127},
 { 50, -33,  38, -15},
 {-53,  38, -94, -89},
 { -2,  -3,  12, -20},
 { -5,   1,  -6,   3},
 {-14,  26,  -1,  -9}};
static const ap_int<32> B1[8] = { 3961, -1731, -2723,  4498,   925,  -804,  -645,   996};

static const ap_int<8> W2[3][8] = {{-74, -45, -28, -55,  53,  -3,  -1,   9},
 { 55, -66, -54,  63, -85, -15, -14,  15},
 {-38,  81, 127,  -3, -45, -15,  -3, -16}};
static const ap_int<32> B2[3] = {  472,  1566, -1224};

#endif
