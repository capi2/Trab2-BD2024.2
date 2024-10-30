#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "../lib/Register.hpp"
#include "../lib/BPTree.hpp"
#include "../lib/Hash.hpp"


class Reader {

private:
    static Register stringToRegister(std::string line) {
        /*
        std::regex tokenPattern("\"(.*?)\"");
        std::smatch match;
        std::vector<std::string> tokens;

        // Procurando os valores na linha
        std::string::const_iterator searchStart(line.cbegin());
        while (std::regex_search(searchStart, line.cend(), match, tokenPattern)) {
            tokens.push_back(match[1].str());  // Adiciona o valor capturado à lista de tokens
            searchStart = match.suffix().first;  // Move o iterador para a próxima parte da string
        }
        */

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

        // Exibe os tokens
        if (tokens.size() != 7) {
            for (const auto& t : tokens) {
                std::cout << '[' << t << ']' << std::endl;
            }
            std::cout << "\n" << std::endl;
        }

        if(tokens.size() != 7) {
            std::cout << "\nlinha estranha com " << tokens.size() << " tokens\n" << line << "\n" << std::endl;
        } else {
            int id = std::stoi(tokens[0]);
            std::string titulo = tokens[1];
        }

        //for(int i = 0; i < tokens.size(); i++) {
        //    std::cout << tokens[i] << std::endl;
        //}


        return Register();
    }

public:
    static bool uploadCSV(std::string filePath) {
        std::ifstream file;
        std::string line;
        Register newRegister;

        line.reserve(sizeof(Register) + 500);

        file.open(filePath);

        if (file.fail()) {
            std::cout << "Erro ao abrir o arquivo!" << std::endl;
            return 1;
        }

        int i = 0;
        while(getline(file, line)) {
            //std::cout << "LINE\n" << line << "\nLINE\n" << std::endl;
            newRegister = stringToRegister(line);


            //colocar nas estruturas
            if( i == 5 ) {
                //exit(0);
            }
            i++;
        }

        return 0;
    }
};

#endif
