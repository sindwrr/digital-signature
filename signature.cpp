#include "signature-aux.h"

// генерация хеша файла
// (сумма кодов всех символов по модулю 2^16 - 1)
unsigned short generateHash(std::ifstream& file)
{
    unsigned short res = 0;
    for (std::string line; getline(file, line);)
        for (int i = 0; i < line.size(); i++)
        {
            res += line[i];
            res %= 65535;
        }

    return res;
}

// проверка числа на простоту
bool Signature::isPrime(int& n)
{
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;

    return true;
}

// генерация случайного простого числа 
int Signature::generatePrime()
{
    const int min = 1;
    const int max = 2048;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(min, max);

    int res = dist(rng);
    if (res % 2 == 0)
        res++;
    
    while (!isPrime(res))
        res += 2;

    return res;
}

// вычисление обратного по модулю
// (секретная экспонента)
int Signature::modInverse(int a, int m) 
{
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1) 
    {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) 
        x1 += m0;

    return x1;
}

// генерация закрытого и открытого ключей
// (на основе RSA)
void Signature::generateKeys(int& priv, int& pub, int& n)
{
    pub = 257;

    int p, q, f;
    while (priv == 0)
    {
        p = generatePrime();
        q = generatePrime();
        n = p * q;
        f = (p - 1) * (q - 1);
        priv = modInverse(pub, f);
    }
}

// возведение в степень по модулю
int Signature::modPow(int base, int exponent, int mod)
{
    long long result = 1;
    base = base % mod;

    while (exponent > 0) 
    {
        if (exponent & 1)
        {
            result = (result * base) % mod;

            if (result < 0)
                result += mod;
        }

        exponent >>= 1;
        base = (1LL * base * base) % mod;

        if (base < 0)
            base += mod;
    }

    return int(result);
}

// генерация подписи
int Signature::generateSignature()
{
    int privateKey{}, publicKey{}, n{};
    generateKeys(privateKey, publicKey, n);

    return modPow(fileHash, privateKey, n);
}