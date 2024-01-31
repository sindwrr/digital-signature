#pragma once

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <regex>

// класс-интерфейс для операций по генерации подписи
// основан на системе RSA
class Signature
{
public:
    Signature() = default;
    Signature(int hash): fileHash(hash) {};
    uint32_t generateSignature();
    static bool isPrime(uint16_t&);
    static uint16_t generatePrime();
    static int modInverse(int, int);
    static void generateKeys(uint32_t&, uint32_t&, uint32_t&);
    static uint32_t modPow(uint32_t, uint32_t, uint32_t);
private:
    int fileHash;
};

// вспомогательные функции
int process(std::ifstream&, std::string);
uint16_t generateHash(std::ifstream&);