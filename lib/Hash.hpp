#ifndef TABELA_HASH_HPP
#define TABELA_HASH_HPP

#include <string>
#include <fstream>
#include <iostream>
#include "Registro.hpp"
#include "Bloco.hpp"
#include "MetaDados.hpp"

const size_t TAMANHO_TABELA_HASH = (TOTAL_MAXIMO_REGISTROS + 1)/MAX_REG_BLOCO; // Usar define aqui dá erro devido overflow, por isso é necessário declarar um size_t

#define ARQUIVO_DADOS "bd/ArquivoDados.bin"

class TabelaHash {
private:

    static size_t funcaoHash(int id) {
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

    static size_t inserir(const Registro& reg) {
        size_t indice = funcaoHash(reg.id);
        Bloco bloco = lerBloco(indice);

        if (bloco.numRegistros < MAX_REG_BLOCO) {
            // Espaço disponível no bloco principal
            bloco.registros[bloco.numRegistros++] = reg;
            escreverBloco(indice, bloco);

            if(bloco.numRegistros == 1) {
                metadados.blocosUsados++;
            }
            metadados.totalRegistros++;

            return indice;

        } else {
            // Colisão
            std::cout << "Aconteceu Colisão no Hash, era pra ser perfeito :( !\nID: " << reg.id << "\nindice: " << indice << "\n" << std::endl;
            exit(1);
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
