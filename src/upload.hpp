#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "../lib/Hash.hpp"
#include "../lib/BPTree1.hpp"
#include "../lib/BPTree2.hpp"

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

    static std::streampos obterTamanhoArquivo(std::ifstream& file) {
        file.seekg(0, std::ios::end);   // Vai para o final do arquivo
        std::streampos tamanho = file.tellg(); // Obtém a posição (em bytes) do final
        file.seekg(0, std::ios::beg);   // Volta ao início do arquivo
        return tamanho;
    }

    static void mostrarProgresso(std::streampos atual, std::streampos total) {
        int largura = 50; // Largura da barra de progresso
        float progresso = static_cast<float>(atual) / total;
        int pos = largura * progresso;

        std::cout << "[";
        for (int i = 0; i < largura; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progresso * 100.0) << " %\r";
        std::cout.flush();
    }

    static void insereRegistroNosArquivos(std::vector<std::string> tokens) {
        int id = std::stoi(tokens[0]);
        const char* titulo = tokens[1].c_str();
        int ano = std::stoi(tokens[2]);
        const char* autores = tokens[3].c_str();
        int citacoes = std::stoi(tokens[4]);
        time_t atualizacao = static_cast<time_t>(std::stol(tokens[5]));
        const char* snippet = tokens[6].c_str();


        Registro newRegistro = criarRegistro(id, titulo, ano, autores, citacoes, atualizacao, snippet);

        size_t indiceBlocoEscrito;

        indiceBlocoEscrito = TabelaHash::inserir(newRegistro);
        BPTree1::inserir(id, indiceBlocoEscrito);

        char tempTitulo[301];
        strcpy(tempTitulo, titulo);
        sanitizeString(tempTitulo);
        BPTree2::inserir(tempTitulo, indiceBlocoEscrito);
    }

public:
    static bool uploadCSV(std::string filePath) {

        if (metadados.arquivoCSVLido) {
            std::cout << "Arquivo CSV já foi lido!" << std::endl;
            return 1;
        }

        std::ifstream file;
        std::string line;
        Registro newRegister;

        line.reserve(sizeof(Registro) + 500); // Buffer para armazenar informações da linha

        file.open(filePath);
        if (file.fail()) { std::cout << "Erro ao abrir o arquivo!" << std::endl; return 1; }


        std::cout << "Lendo arquivo CSV!" << std::endl;

        std::streampos totalBytes = obterTamanhoArquivo(file);

        std::vector<std::string> tokens;
        int i = 0;
        while(true) {
            // Obtém a posição atual no arquivo em bytes
            std::streampos posAtual = file.tellg();
            
            // Mostra a barra de progresso
            mostrarProgresso(posAtual, totalBytes);

            if (!getline(file, line)) {

                if (file.peek() != EOF) {
                    std::stringstream buffer;
                    buffer << file.rdbuf();  // Lê apenas o conteúdo restante
                    line = buffer.str();
                    tokens = extractTokens(line);
                    insereRegistroNosArquivos(tokens);
                    break;
                }

                break;
            }

            tokens = extractTokens(line);

            if(tokens.size() != 7) { // Inconsistência no dataset
                std::string missingLine;
                if(!getline(file, missingLine)) {break;}

                line = line + missingLine;

                tokens = extractTokens(line);
            }

            //insereRegistroNosArquivos(tokens);

            int id = std::stoi(tokens[0]);
            if(!BPTree1::buscaRegistro(id)) {
                std::cout << "Não achei na avore BPTree1: " << id << std::endl;
            }

             const char* titulo = tokens[1].c_str();
             char tempTitulo[301];
             strcpy(tempTitulo, titulo);
             if(!BPTree2::buscaRegistro(tempTitulo)) {
                 std::cout << "Não achei na avore BPTree2: " << tempTitulo << std::endl;
             }



        }

        std::cout << std::endl << "Leitura arquivo CSV completo!" << std::endl;
        metadados.arquivoCSVLido = true;

        return 0;
    }
};

#endif
