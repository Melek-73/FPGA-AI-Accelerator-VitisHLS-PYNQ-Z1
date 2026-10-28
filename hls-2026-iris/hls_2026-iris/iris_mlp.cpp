
#include <ap_int.h>

// ============================================================
// Iris MLP FPGA Accelerator: 4 -> 8 -> 3
// Inputs: 4 signed INT8 standardized and quantized features
// Weights: signed INT8
// Biases and accumulators: signed INT32
// Output: class ID 0, 1, or 2
// Interface: AXI4-Lite
// ============================================================

#define IRIS_INPUTS  4
#define IRIS_HIDDEN  8
#define IRIS_OUTPUTS 3

#define REQUANT_MULT  16306089
#define REQUANT_SHIFT 31

// First layer weights: 4 -> 8
static const ap_int<8> W1[IRIS_HIDDEN][IRIS_INPUTS] = {
    { 26, -24,  -8,  11},
    { 33, -24,  80,  79},
    {-11, -43, 115, 127},
    { 50, -33,  38, -15},
    {-53,  38, -94, -89},
    { -2,  -3,  12, -20},
    { -5,   1,  -6,   3},
    {-14,  26,  -1,  -9}
};

static const ap_int<32> B1[IRIS_HIDDEN] = {
    3961, -1731, -2723, 4498,
    925, -804, -645, 996
};

// Second layer weights: 8 -> 3
static const ap_int<8> W2[IRIS_OUTPUTS][IRIS_HIDDEN] = {
    {-74, -45, -28, -55,  53,  -3,  -1,   9},
    { 55, -66, -54,  63, -85, -15, -14,  15},
    {-38,  81, 127,  -3, -45, -15,  -3, -16}
};

static const ap_int<32> B2[IRIS_OUTPUTS] = {
    472, 1566, -1224
};

// ============================================================
// Q31 requantization
// ============================================================

static ap_int<32> requantize_q31(ap_int<32> value)
{
#pragma HLS INLINE

    ap_int<64> product =
        ap_int<64>(value) * ap_int<64>(REQUANT_MULT);

    ap_int<64> magnitude;
    if (product < 0)
        magnitude = -product;
    else
        magnitude = product;

    ap_int<64> rounded =
        (magnitude +
         (ap_int<64>(1) << (REQUANT_SHIFT - 1)))
        >> REQUANT_SHIFT;

    if (product < 0)
        rounded = -rounded;

    return ap_int<32>(rounded);
}

// ============================================================
// INT8 saturation
// ============================================================

static ap_int<8> saturate_int8(ap_int<32> value)
{
#pragma HLS INLINE

    if (value > 127)
        return ap_int<8>(127);

    if (value < -128)
        return ap_int<8>(-128);

    return ap_int<8>(value);
}

// ============================================================
// Top-level accelerator
// ============================================================

ap_uint<2> iris_mlp(
    ap_int<8> x0,
    ap_int<8> x1,
    ap_int<8> x2,
    ap_int<8> x3
) {
#pragma HLS INTERFACE s_axilite port=x0 bundle=CTRL
#pragma HLS INTERFACE s_axilite port=x1 bundle=CTRL
#pragma HLS INTERFACE s_axilite port=x2 bundle=CTRL
#pragma HLS INTERFACE s_axilite port=x3 bundle=CTRL
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    // Local input vector and output scores.
    ap_int<8> input[IRIS_INPUTS] = {x0, x1, x2, x3};
    ap_int<32> scores[IRIS_OUTPUTS];

#pragma HLS ARRAY_PARTITION variable=input complete dim=1
#pragma HLS ARRAY_PARTITION variable=scores complete dim=1

    ap_int<32> acc1[IRIS_HIDDEN];
    ap_int<8> hidden[IRIS_HIDDEN];

#pragma HLS ARRAY_PARTITION variable=acc1 complete dim=1
#pragma HLS ARRAY_PARTITION variable=hidden complete dim=1

    // --------------------------------------------------------
    // Layer 1: 4 inputs -> 8 hidden neurons
    // --------------------------------------------------------

    for (int i = 0; i < IRIS_HIDDEN; i++) {
#pragma HLS PIPELINE II=1

        ap_int<32> accumulator = B1[i];

        for (int j = 0; j < IRIS_INPUTS; j++) {
            ap_int<16> product = input[j] * W1[i][j];
            accumulator += ap_int<32>(product);
        }

        acc1[i] = accumulator;

        // Requantization and ReLU.
        ap_int<32> scaled = requantize_q31(acc1[i]);

        if (scaled < 0)
            scaled = 0;

        hidden[i] = saturate_int8(scaled);
    }

    // --------------------------------------------------------
    // Layer 2: 8 hidden neurons -> 3 class scores
    // --------------------------------------------------------

    for (int i = 0; i < IRIS_OUTPUTS; i++) {
#pragma HLS PIPELINE II=1

        ap_int<32> accumulator = B2[i];

        for (int j = 0; j < IRIS_HIDDEN; j++) {
            ap_int<16> product = hidden[j] * W2[i][j];
            accumulator += ap_int<32>(product);
        }

        scores[i] = accumulator;
    }

    // --------------------------------------------------------
    // Argmax classification
    // --------------------------------------------------------

    ap_uint<2> best_class = 0;
    ap_int<32> best_score = scores[0];

    for (int i = 1; i < IRIS_OUTPUTS; i++) {
        if (scores[i] > best_score) {
            best_score = scores[i];
            best_class = ap_uint<2>(i);
        }
    }

    // IMPORTANT: return the result; do not use predicted_class.
    return best_class;
}
