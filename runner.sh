rm bin/joinEngine

g++ -std=c++11 src/*.cpp -o bin/joinEngine

bin/joinEngine datasets/r0 0 datasets/r1 1

