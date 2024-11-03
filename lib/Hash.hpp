#ifndef TABELA_HASH_HPP
#define TABELA_HASH_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "Registro.hpp"
#include "Bloco.hpp"
#include "MetaDados.hpp"

const size_t TAMANHO_TABELA_HASH = (TOTAL_MAXIMO_REGISTROS + 1)/MAX_REG_BLOCO; // O define da erro! overflow

#define ARQUIVO_DADOS "bd/ArquivoDados.bin"

class TabelaHash {
private:

    static size_t funcaoHash(int id) {
        //std::cout << id << " % " << TAMANHO_TABELA_HASH << " = " << id % TAMANHO_TABELA_HASH << std::endl;
        return id % TAMANHO_TABELA_HASH;
    }

    static Bloco lerBloco(size_t indice) {
        std::ifstream arquivo(ARQUIVO_DADOS, std::ios::in | std::ios::binary);
        if(!arquivo) {
            std::cout << "Não consigo ler bloco porque arquivo de dados não existe! Verifique se o diretório bd existe" << std::endl;
            exit(1);
        }
        arquivo.seekg(indice * sizeof(Bloco), std::ios::beg);
        Bloco bloco;

        arquivo.read(reinterpret_cast<char*>(&bloco), sizeof(Bloco));
        arquivo.close();
        return bloco;
    }

    static void escreverBloco(size_t indice, const Bloco& bloco) {
        std::fstream arquivo(ARQUIVO_DADOS, std::ios::in | std::ios::out | std::ios::binary);
        if(!arquivo) {
            std::cout << "Não consigo escrever bloco porque arquivo de dados não existe! Verifique se o diretório bd existe" << std::endl;
            exit(1);
        }

        arquivo.seekp(indice * sizeof(Bloco), std::ios::beg);
        arquivo.write(reinterpret_cast<const char*>(&bloco), sizeof(Bloco));
        arquivo.close();
    }

    std::streampos obterTamanhoArquivo(std::ifstream& file) {
        file.seekg(0, std::ios::end);   // Vai para o final do arquivo
        std::streampos tamanho = file.tellg(); // Obtém a posição (em bytes) do final
        file.seekg(0, std::ios::beg);   // Volta ao início do arquivo
        return tamanho;
    }

    void mostrarProgresso(std::streampos atual, std::streampos total) {
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


public:
    static void inicializarArquivo() {
        std::fstream arquivo(ARQUIVO_DADOS, std::ios::in | std::ios::binary);

        if (!arquivo) {
            std::cout << "Criando Arquivo de dados!" << std::endl;
            // Arquivo não existe, criar e pré-alocar espaço
            arquivo.open(ARQUIVO_DADOS, std::ios::out | std::ios::binary);
            Bloco blocoVazio;

            // Escrever blocos vazios
            for (size_t i = 0; i < TAMANHO_TABELA_HASH; ++i) {
                arquivo.write(reinterpret_cast<const char*>(&blocoVazio), sizeof(Bloco));
            }
            arquivo.close();

            metadados.totalBlocos = TAMANHO_TABELA_HASH;
        }
    }

    static void inserir(const Registro& reg) {
        size_t indice = funcaoHash(reg.id);
        Bloco bloco = lerBloco(indice);

        //std::cout << "ID: " << reg.id << "\nindice: " << indice << "\n" << std::endl;

        if (bloco.numRegistros < MAX_REG_BLOCO) {
            // Espaço disponível no bloco principal
            bloco.registros[bloco.numRegistros++] = reg;
            escreverBloco(indice, bloco);

            if(bloco.numRegistros == 1) {
                metadados.blocosUsados++;
            }
            metadados.totalRegistros++;

        } else {
            // Colisão
            std::cout << "Aconteceu Colisão no Hash!\nID: " << reg.id << "\nindice: " << indice << "\n" << std::endl;
        }
    }

    static Registro* buscar(int id) {
        size_t indice = funcaoHash(id);
        Bloco blocoAtual = lerBloco(indice);

        while (true) {
            for (int i = 0; i < blocoAtual.numRegistros; ++i) { // Busca no Bloco
                if (blocoAtual.registros[i].id == id) {
                    // Registro encontrado
                    Registro* resultado = new Registro(blocoAtual.registros[i]);
                    metadados.blocosLidosUltimaConsultaHash = 1;
                    return resultado;
                }
            }
            // Precisa encontrar no overflow
            break;
        }
        return nullptr;
    }
};

#endif // TABELA_HASH_HPP
