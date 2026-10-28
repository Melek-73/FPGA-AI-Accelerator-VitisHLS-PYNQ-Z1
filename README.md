\# INT8 Iris MLP FPGA Accelerator



An end-to-end FPGA implementation of a quantized neural network for Iris flower classification, using AMD Vitis HLS, Vivado, and the PYNQ-Z1 development board.



The project explores neural network quantization, fixed-point arithmetic, FPGA hardware acceleration, AXI4-Lite communication, and on-board inference validation.



\## Project Overview



The objective is to deploy a compact multilayer perceptron (MLP) on the programmable logic of a Zynq-7000 FPGA and execute inference on hardware.



The workflow includes:



1\. Training an MLP for Iris flower classification.

2\. Normalizing input features and quantizing the model to INT8.

3\. Implementing fixed-point inference using Vitis HLS.

4\. Integrating the generated accelerator IP into a Vivado block design.

5\. Generating a bitstream for the PYNQ-Z1 board.

6\. Running inference from Python and validating the FPGA predictions against the expected labels.



\## Neural Network Architecture



| Component | Configuration |

|---|---|

| Input features | 4 |

| Hidden layer | 8 neurons |

| Hidden activation | ReLU |

| Output classes | 3 |

| Input representation | Signed INT8 |

| Weight representation | Signed INT8 |

| Accumulators and biases | INT32 |

| Hardware interface | AXI4-Lite |

| Output | Predicted class ID: 0, 1, or 2 |



The network classifies samples into three Iris species:



\- Class 0: Iris-setosa

\- Class 1: Iris-versicolor

\- Class 2: Iris-virginica



The input features are standardized using a fitted training-set scaler before quantization. The resulting quantized inputs are transferred to the FPGA accelerator.



\## Hardware Architecture



The accelerator is integrated into a Vivado block design containing the Zynq Processing System, AXI interconnect, reset logic, and custom MLP IP.



The processing system communicates with the accelerator through AXI4-Lite registers. The Python test application writes the four input features, starts inference, polls for completion, and reads the predicted class.



The hardware implementation includes:



\- INT8 multiply-accumulate operations

\- INT32 accumulation

\- Fixed-point requantization

\- ReLU activation

\- INT8 saturation

\- Argmax classification



\## Development Environment



| Component | Configuration |

|---|---|

| FPGA platform | PYNQ-Z1 |

| FPGA part | `xc7z020clg400-1` |

| HLS tool | AMD Vitis HLS 2026.1 |

| Hardware design tool | AMD Vivado 2026.1 |

| Board software | PYNQ 3.1.1 |

| Accelerator interface | AXI4-Lite |



\## Verification and Results



\### HLS verification



The following verification stages passed:



\- C simulation

\- C/RTL co-simulation



The reported co-simulation latency is \*\*32 clock cycles\*\*, corresponding to approximately \*\*320 ns at the 10 ns HLS clock target\*\*.



This is the reported HLS simulation latency, not the total end-to-end Python-to-FPGA execution time.



\### FPGA hardware validation



The generated bitstream was loaded onto the physical PYNQ-Z1 board, and a Python test script executed inference on 30 held-out test samples.



\*\*Measured accuracy: 100% (30/30 samples).\*\*



All 30 FPGA predictions matched their expected class labels for this test set.



This result applies to the tested subset and does not guarantee 100% accuracy on every unseen sample.



\### Reported implementation resources



| Resource or metric | Reported value |

|---|---:|

| LUT | 1,334 |

| Flip-flops | 1,211 |

| BRAM | 0 |

| DSP | 1 |

| Worst Negative Slack (WNS) | +10.888 ns |



These values are from the current Vivado implementation reports. The timing result applies to the constraints used for that implementation.



\## Repository Structure



```text

FPGA-Acclerator-project/

├── README.md

├── .gitignore

├── basic-MLP/

├── hls-2026-iris/

├── deployment/

│   ├── design\_1\_wrapper.bit

│   └── design\_1\_wrapper.hwh

├── tests/

│   ├── iris\_fpga\_test\_samples.npz

│   └── results.txt

└── docs/

&#x20;   └── images/

&#x20;       ├── 01\_hls\_reports.png

&#x20;       ├── 02\_vivado\_block\_design.png

&#x20;       ├── 03\_vivado\_timing\_resources.png

&#x20;       ├── 04\_pynq\_100\_percent\_accuracy.png

&#x20;       └── 05\_int8\_model\_implementation.png
