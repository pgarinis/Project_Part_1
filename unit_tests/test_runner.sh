rm ./testexec

g++ -std=c++11 -isystem ./gtest -I . -pthread tests/sample_test.cc ../src/JoinEngine.cpp ../src/outputList.cpp libgtest.a -o testexec

./testexec
