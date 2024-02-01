#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <regex>

// класс-интерфейс для операций по генерации подписи
class Signature
{
public:
    Signature(int hash): fileHash(hash) {};
    int generateSignature();
    static bool isPrime(int&);
    static int generatePrime();
    static int modInverse(int, int);
    static void generateKeys(int&, int&, int&);
    static int modPow(int, int, int);
private:
    unsigned short fileHash;
};

// вспомогательные функции
int process(std::ifstream&, std::string);
unsigned short generateHash(std::ifstream&);