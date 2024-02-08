#include "signature-aux.h"

int main(int argc, char** argv)
{
    // сохраняем имя файла (если не введено, то по умолчанию)
    std::string fileName = argc == 1 ? "test.txt" : argv[1];

    // открываем файл
    std::ifstream input(fileName);
    if (!input.is_open())
    {
        std::cerr << "Error: cannot open the file!" << std::endl;
        return 1;
    }

    // совершаем основную обработку
    int exitCode = process(input, fileName);

    input.close();
    return exitCode;
}