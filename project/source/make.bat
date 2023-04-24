::chcp 866
del HaffmanCoder.exe
g++ -std=c++20 main.cpp StreamType.cpp Binfstream.cpp HaffCoder.cpp HaffNode.cpp IntermediaData.cpp -o HaffmanCoder.exe
HaffmanCoder.exe --help