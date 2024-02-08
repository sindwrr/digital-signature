#include "signature-aux.h"

// функция для получения последней строки в файле
std::string getLastLine(std::ifstream& input)
{
    // ставим позицию в конец файла
    input.seekg(0, std::ios::end);
    std::streampos fileSize = input.tellg();

    std::vector<char> lineBuffer;
    lineBuffer.reserve(16);

    // поочередно заносим символы строки в вектор
    for (std::streampos pos = fileSize - std::streampos(1); pos >= 0; pos -= 1) 
    {
        input.seekg(pos);

        char currentChar;
        input.get(currentChar);

        if (currentChar == '\n')
            break;

        lineBuffer.insert(lineBuffer.begin(), currentChar);
    }

    // ставим позицию обратно в начало
    input.clear();
    input.seekg(0);

    return std::string(lineBuffer.begin(), lineBuffer.end());
}

int process(std::ifstream& input, std::string inputName)
{
    std::string lastLine = getLastLine(input);

    std::regex format("sig[0-9]+");

    // проверяем последнюю строку на соответствие шаблону подписи
    // спрашиваем пользователя об ее удалении/добавлении
    bool foundSig = false;
    if (std::regex_match(lastLine, format)) 
    {
        foundSig = true;
        std::cout << "A signature is found. Do you want to delete it? [y/n]" << std::endl;
    }
    else
        std::cout << "No signature is found. Do you want to add it? [y/n]" << std::endl;

    char ans;
    std::cin >> ans;
    std::cin.ignore();

    // спрашиваем имя нового файла
    if (ans == 'n')
        return 0;
    else if (!foundSig)
        std::cout << "Write the name for the new file (signed-" 
                  << inputName << " by default): ";
    else
        std::cout << "Write the name for the new file (unsigned-" 
                  << inputName << " by default): ";

    std::string newFile;
    std::getline(std::cin, newFile);

    // если не введено, ставим по умолчанию
    if (newFile.empty())
        newFile = foundSig ? "unsigned-" + (std::string)inputName 
                           : "signed-" + (std::string)inputName;


    // открываем новый файл
    std::ofstream output(newFile);
    if (!output.is_open())
    {
        std::cerr << "Error: cannot open the new file!" << std::endl;
        return 1;
    }

    // сохраняем строки в вектор
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input, line))
        lines.push_back(line);

    // если найдена подпись, переносим все строки, кроме последней
    // иначе заносим все и генерируем подпись
    if (foundSig)
    {
        for (int i = 0; i < lines.size() - 1; ++i)
        {
            output << lines[i];
            if (i != lines.size() - 2)
                output << '\n';
        }

        std::cout << "Signature deleted! Result is written in chosen file." << std::endl;            
    }
    else
    {
        input.clear();
        input.seekg(0);
        int fileHash = generateHash(input); // генерируем хеш файла

        input.clear();
        input.seekg(0);
        output << input.rdbuf(); // заносим все в новый файл

        // генерируем подпись и заносим ее в конец файла
        Signature sigGen(fileHash);
        int sig = sigGen.generateSignature();
        output << "\nsig" << sig;
        std::cout << "Signature added! Result is written in chosen file." << std::endl;
    }

    output.close();
    return 0;
}