#ifndef BPTREE1_HPP
#define BPTREE1_HPP


#include <iostream>
#include <fstream>
#include "MetaDados.hpp"

#define ARQUIVO_INDICE_PRIMARIO "bd/ArquivoIndicePrimario.bin"
#define M  170 //2m x 4 + (2m+1) x 8 ≤ 4096 

struct No1 {
    int chaves[2*M];
    size_t ponteiro[2*M + 1];
    bool folha;
    unsigned short int numChaves;
    unsigned short int numPonteiros;

    No1() {
        folha = true;
    }

};

size_t proximoIndice = 0;

class BPTree1 {
    private:

        static No1 lerNo(size_t indice) {
            std::ifstream arquivo(ARQUIVO_INDICE_PRIMARIO, std::ios::in | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler No1 porque arquivo de indice primário não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }
            arquivo.seekg(indice * sizeof(No1), std::ios::beg);
            No1 no;

            arquivo.read(reinterpret_cast<char*>(&no), sizeof(No1));
            arquivo.close();
            return no;
        }

        static void escreverNo(size_t indice, const No1& no) {
            std::fstream arquivo(ARQUIVO_INDICE_PRIMARIO, std::ios::in | std::ios::out | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo escrever No1 porque arquivo de indice primário não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }

            arquivo.seekp(indice * sizeof(No1), std::ios::beg);
            arquivo.write(reinterpret_cast<const char*>(&no), sizeof(No1));
            arquivo.close();
        }

        // Função para encontrar o nó folha onde a chave deve ser inserida
        static size_t encontrarFolha(int chave, size_t indiceAtual) {
            No1 noAtual = lerNo(indiceAtual);

            if(noAtual.folha) {
                return indiceAtual;
            } else {
                // Percorrer as chaves para encontrar o caminho correto
                int i = 0;
                while(i < noAtual.numChaves && chave >= noAtual.chaves[i]) {
                    i++;
                }
                // Chamar recursivamente para o filho apropriado
                return encontrarFolha(chave, noAtual.ponteiro[i]);
            }
        }

        // Função para dividir o nó e ajustar a árvore
        static void dividirNo(size_t indiceNo, No1& no, size_t indicePai, int chaveMediana, size_t novoIndiceNo) {
            No1 novoNo;
            novoNo.folha = no.folha;
            novoNo.numChaves = M;
            novoNo.numPonteiros = no.folha ? M + 1 : M;

            // Copiar metade das chaves e ponteiros para o novo nó
            for(int i = 0; i < M; i++) {
                novoNo.chaves[i] = no.chaves[i + M];
            }
            for(int i = 0; i < novoNo.numPonteiros; i++) {
                novoNo.ponteiro[i] = no.ponteiro[i + M];
            }

            no.numChaves = M;
            no.numPonteiros = no.folha ? M + 1 : M;

            escreverNo(indiceNo, no);
            escreverNo(novoIndiceNo, novoNo);

            if(indicePai == (size_t)-1) {
                // Criar novo nó raiz
                No1 novaRaiz;
                novaRaiz.folha = false;
                novaRaiz.numChaves = 1;
                novaRaiz.chaves[0] = chaveMediana;
                novaRaiz.ponteiro[0] = indiceNo;
                novaRaiz.ponteiro[1] = novoIndiceNo;

                escreverNo(0, novaRaiz);
            } else {
                // Inserir chave mediana no nó pai
                inserirNoInterno(chaveMediana, indicePai, novoIndiceNo);
            }
        }

        // Função para inserir chave em nó interno
        static void inserirNoInterno(int chave, size_t indiceNo, size_t indiceDireita) {
            No1 no = lerNo(indiceNo);

            // Encontrar posição para inserir a chave
            int i = no.numChaves - 1;
            while(i >= 0 && chave < no.chaves[i]) {
                no.chaves[i + 1] = no.chaves[i];
                no.ponteiro[i + 2] = no.ponteiro[i + 1];
                i--;
            }
            no.chaves[i + 1] = chave;
            no.ponteiro[i + 2] = indiceDireita;
            no.numChaves++;

            if(no.numChaves > 2 * M) {
                // Dividir nó interno
                size_t novoIndiceNo = proximoIndice++;
                int chaveMediana = no.chaves[M];

                No1 novoNo;
                novoNo.folha = false;
                novoNo.numChaves = M - 1;

                // Copiar metade das chaves e ponteiros para o novo nó
                for(int j = 0; j < M - 1; j++) {
                    novoNo.chaves[j] = no.chaves[j + M + 1];
                }
                for(int j = 0; j < M; j++) {
                    novoNo.ponteiro[j] = no.ponteiro[j + M + 1];
                }

                no.numChaves = M;

                escreverNo(indiceNo, no);
                escreverNo(novoIndiceNo, novoNo);

                if(indiceNo == 0) {
                    // Criar novo nó raiz
                    No1 novaRaiz;
                    novaRaiz.folha = false;
                    novaRaiz.numChaves = 1;
                    novaRaiz.chaves[0] = chaveMediana;
                    novaRaiz.ponteiro[0] = indiceNo;
                    novaRaiz.ponteiro[1] = novoIndiceNo;

                    escreverNo(0, novaRaiz);
                } else {
                    // Obter índice do pai (precisamos de uma forma de rastrear o pai)
                    // Neste exemplo simplificado, assumimos que o pai é o nó raiz
                    inserirNoInterno(chaveMediana, 0, novoIndiceNo);
                }
            } else {
                escreverNo(indiceNo, no);
            }
        }

        // Função recursiva para buscar a chave
        static size_t buscarRecursivo(int chave, size_t indiceAtual) {
            No1 noAtual = lerNo(indiceAtual);

            metadados.blocosLidosUtimaConsultaIndicePrimario++;

            if(noAtual.folha) {
                // Procurar a chave no nó folha
                for(int i = 0; i < noAtual.numChaves; i++) {
                    if(noAtual.chaves[i] == chave) {
                        return noAtual.ponteiro[i]; // Retorna o ponteiro associado
                    }
                }
                // Chave não encontrada no nó folha
                return size_t(-1);
            } else {
                // Nó interno: determinar qual filho seguir
                int i = 0;
                while(i < noAtual.numChaves && chave >= noAtual.chaves[i]) {
                    i++;
                }
                // Chamar recursivamente para o filho apropriado
                return buscarRecursivo(chave, noAtual.ponteiro[i]);
            }
        }


    public:

        static void inicializarArquivo() {
            std::fstream arquivo(ARQUIVO_INDICE_PRIMARIO, std::ios::in | std::ios::binary);

            if (!arquivo) {
                std::cout << "Criando Arquivo de Indice Primário!" << std::endl;
                // Arquivo não existe, criar e pré-alocar espaço
                arquivo.open(ARQUIVO_INDICE_PRIMARIO, std::ios::out | std::ios::binary);

                No1 noVazio;

                arquivo.write(reinterpret_cast<const char*>(&noVazio), sizeof(No1));

                arquivo.close();

            }
        }

        static void inserir(int chave, size_t indiceBloco) {
            size_t indiceFolha = encontrarFolha(chave, 0);
            No1 folha = lerNo(indiceFolha);

            // Inserir chave na posição correta
            int i = folha.numChaves - 1;
            while(i >= 0 && chave < folha.chaves[i]) {
                folha.chaves[i + 1] = folha.chaves[i];
                folha.ponteiro[i + 1] = folha.ponteiro[i];
                i--;
            }
            folha.chaves[i + 1] = chave;
            folha.ponteiro[i + 1] = indiceBloco;
            folha.numChaves++;
            folha.numPonteiros = folha.numChaves;

            if(folha.numChaves > 2 * M) {
                // Dividir o nó folha
                size_t novoIndiceFolha = proximoIndice++;
                int chaveMediana = folha.chaves[M];

                dividirNo(indiceFolha, folha, (size_t)-1, chaveMediana, novoIndiceFolha);
            } else {
                escreverNo(indiceFolha, folha);
            }

            metadados.totalBlocosIndicePrimario++;
        }


        static size_t buscar(int chave) {
            metadados.blocosLidosUtimaConsultaIndicePrimario = 0;
            return buscarRecursivo(chave, 0);
        }

};


#endif // BPTRER1_HPP