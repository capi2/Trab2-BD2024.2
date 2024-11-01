#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "Registro.hpp"
#include "Bloco.hpp"
#include "Hash.hpp"

class Reader {

private:
    static std::vector<std::string> extractTokens(std::string line) {
        std::vector<std::string> tokens;
        std::string token;
        bool in_quotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                in_quotes = !in_quotes;
                continue; // Ignora as aspas
            }
            if (c == ';' && !in_quotes) {
                tokens.push_back(token);
                token.clear();
            } else {
                token += c;
            }
        }
        // Adiciona o último token
        tokens.push_back(token);

        return tokens;
    }

public:
    static bool uploadCSV(std::string filePath) {
        std::ifstream file;
        std::string line;
        Registro newRegister;

        line.reserve(sizeof(Registro) + 500); // Buffer para armazenar informações da linha

        file.open(filePath);
        if (file.fail()) { std::cout << "Erro ao abrir o arquivo!" << std::endl; return 1; }

        int i = 0;
        while(true) {

            if(!getline(file, line)) {break;}

            std::vector<std::string> tokens = extractTokens(line);

            if(tokens.size() != 7) { // Inconsistência no dataset
                std::string missingLine;
                if(!getline(file, missingLine)) {break;}

                std::cout << "Inconsistence found\n" << std::endl;

                std::cout << "currentLine: " << line << "\n" << "missingLine: " << missingLine << "\n" << "completeLine: " << line + missingLine << "\n" << std::endl;

                line = line + missingLine;

                tokens = extractTokens(line);
            }





            

            /*
            // Exibe os tokens
            if (tokens.size() != 7) {
                for (const auto& t : tokens) {
                    std::cout << '[' << t << ']' << std::endl;
                }
                std::cout << "\n" << std::endl;
            }*/


            if( i == 5 ) {
                //exit(0);
            }
            i++;
        }

        return 0;
    }
};

#endif
