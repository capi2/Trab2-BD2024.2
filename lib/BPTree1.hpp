#ifndef BPTREE1_HPP
#define BPTREE1_HPP

#include <iostream>
#include <fstream>
#include <vector>
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
        numChaves = 0;
        numPonteiros = 0;
    }
};

size_t proximoIndice = 1; // Começa em 1 porque 0 é usado para a raiz

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
        static size_t encontrarFolha(int chave, size_t indiceAtual, std::vector<size_t>& caminho) {
            No1 noAtual = lerNo(indiceAtual);
            caminho.push_back(indiceAtual);

            if(noAtual.folha) {
                return indiceAtual;
            } else {
                // Percorrer as chaves para encontrar o caminho correto
                int i = 0;
                while(i < noAtual.numChaves && chave >= noAtual.chaves[i]) {
                    i++;
                }
                // Chamar recursivamente para o filho apropriado
                return encontrarFolha(chave, noAtual.ponteiro[i], caminho);
            }
        }

        // Função para inserir chave na árvore e ajustar a árvore
        static void inserirNaArvore(int chave, size_t indiceBloco, std::vector<size_t>& caminho) {
            // Obter o nó folha
            size_t indiceNo = caminho.back();
            No1 no = lerNo(indiceNo);

            // Inserir chave no nó

            if(no.numChaves == 2 * M) {
                // Nó excedeu a capacidade, precisa dividir
                dividirNoFolha(chave, indiceBloco, caminho);
            } else {

                int i = no.numChaves - 1;
                while(i >= 0 && chave < no.chaves[i]) {
                    no.chaves[i + 1] = no.chaves[i];
                    no.ponteiro[i + 1] = no.ponteiro[i];
                    i--;
                }
                no.chaves[i + 1] = chave;
                no.ponteiro[i + 1] = indiceBloco;
                no.numChaves++;
                no.numPonteiros = no.numChaves;


                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoFolha(int chave, size_t indiceBloco, std::vector<size_t>& caminho) {
            size_t indiceNo = caminho.back();
            No1 no = lerNo(indiceNo);

            int mid = M; // Divisão no meio
            No1 novoNo;
            novoNo.folha = true;
            novoNo.numChaves = no.numChaves - mid;
            novoNo.numPonteiros = novoNo.numChaves;

            // Copiar metade das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                novoNo.chaves[i] = no.chaves[mid + i];
                novoNo.ponteiro[i] = no.ponteiro[mid + i];
            }

            no.numChaves = mid;
            no.numPonteiros = no.numChaves;

            escreverNo(indiceNo, no);

            size_t novoIndiceNo = proximoIndice++;
            escreverNo(novoIndiceNo, novoNo);

            int chaveMediana = novoNo.chaves[0];

            caminho.pop_back(); // Remover o nó folha do caminho

            if(caminho.empty()) {
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
                inserirNoInterno(chaveMediana, caminho, novoIndiceNo);
            }
        }

        // Função para inserir chave em nó interno
        static void inserirNoInterno(int chave, std::vector<size_t>& caminho, size_t indiceDireita) {
            size_t indiceNo = caminho.back();
            caminho.pop_back();

            No1 no = lerNo(indiceNo);

            // Encontrar posição para inserir a chave
            if(no.numChaves == 2 * M) {
                // Nó excedeu a capacidade, precisa dividir
                dividirNoFolha(chave, indiceDireita, caminho);
            } else {

                int i = no.numChaves - 1;
                while(i >= 0 && chave < no.chaves[i]) {
                    no.chaves[i + 1] = no.chaves[i];
                    no.ponteiro[i + 1] = no.ponteiro[i];
                    i--;
                }
                no.chaves[i + 1] = chave;
                no.ponteiro[i + 1] = indiceDireita;
                no.numChaves++;
                no.numPonteiros = no.numChaves;


                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoInterno(No1& no, size_t indiceNo, std::vector<size_t>& caminho) {
            int mid = M; // Divisão no meio
            int chaveMediana = no.chaves[mid];

            No1 novoNo;
            novoNo.folha = false;
            novoNo.numChaves = no.numChaves - mid - 1;

            // Copiar metade das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                novoNo.chaves[i] = no.chaves[mid + 1 + i];
            }
            for(int i = 0; i <= novoNo.numChaves; i++) {
                novoNo.ponteiro[i] = no.ponteiro[mid + 1 + i];
            }

            no.numChaves = mid;

            escreverNo(indiceNo, no);

            size_t novoIndiceNo = proximoIndice++;
            escreverNo(novoIndiceNo, novoNo);

            if(caminho.empty()) {
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
                inserirNoInterno(chaveMediana, caminho, novoIndiceNo);
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
                noVazio.folha = true;
                noVazio.numChaves = 0;
                noVazio.numPonteiros = 0;

                // Escrever nó raiz vazio
                arquivo.write(reinterpret_cast<const char*>(&noVazio), sizeof(No1));

                arquivo.close();

            }
        }

        static void inserir(int chave, size_t indiceBloco) {
            std::vector<size_t> caminho;
            size_t indiceFolha = encontrarFolha(chave, 0, caminho);

            inserirNaArvore(chave, indiceBloco, caminho);
        }

        static size_t buscar(int chave) {
            metadados.blocosLidosUtimaConsultaIndicePrimario = 0;
            return buscarRecursivo(chave, 0);
        }

};

#endif // BPTRER1_HPP
