
#include <iostream>
#include <ap_int.h>

ap_uint<2> iris_mlp(
    ap_int<8> x0,
    ap_int<8> x1,
    ap_int<8> x2,
    ap_int<8> x3
);

int main()
{
    // Test input only.
    // These are arbitrary INT8 values, not a real Iris sample.
    ap_int<8> x0 = 10;
    ap_int<8> x1 = 20;
    ap_int<8> x2 = 30;
    ap_int<8> x3 = 40;

    ap_uint<2> predicted_class =
        iris_mlp(x0, x1, x2, x3);

    unsigned int class_id = predicted_class.to_uint();

    std::cout << "====================================\n";
    std::cout << "Iris MLP INT8 C-Simulation\n";
    std::cout << "====================================\n";

    std::cout << "Input INT8 features: "
              << int(x0) << ", "
              << int(x1) << ", "
              << int(x2) << ", "
              << int(x3) << "\n";

    std::cout << "Predicted class: "
              << class_id << "\n";

    if (class_id > 2) {
        std::cerr << "ERROR: Invalid class ID!\n";
        return 1;
    }

    const char* class_names[3] = {
        "Iris setosa",
        "Iris versicolor",
        "Iris virginica"
    };

    std::cout << "Predicted species: "
              << class_names[class_id] << "\n";

    std::cout << "C-Simulation test passed.\n";

    return 0;
}
