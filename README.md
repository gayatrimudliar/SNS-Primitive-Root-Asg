# SNS-Primitive-Root-Asg

Initial Requirements:
1. GMP Library (sudo apt-get install libgmp3-dev)
2. g++ compiler (sudo apt install g++)

Commands for execution (Type in terminal):
1. compile the code: g++ snsasg.cpp -o sns -lgmp
2. run it: ./sns
3. you will be asked to input the number you want to calculate all the primitive roots of. Enter the number.

Expected Outputs:
1. If primitive roots do not exist, that will be printed directly.
2. Else, calculated number of primitive roots and Phi(Phi(number)) will be printed for verification along with comma separated primitive roots themselves.
