::chcp 866
del HaffmanCoder.exe
g++ -std=c++20 source\main.cpp source\StreamType.cpp source\Binfstream.cpp source\HaffCoder.cpp source\HaffNode.cpp source\IntermediaData.cpp -o HaffmanCoder.exe
HaffmanCoder.exe --help