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

    No1() {
        folha = true;
        numChaves = 0;
    }
};

struct No1Caminho {
    size_t indiceNo;
    No1 no;

    No1Caminho() {
        indiceNo = -1;
    }

};

size_t proximoIndice1 = 1; // Começa em 1 porque 0 é usado para a posição inicial da raiz

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

        static void novaRaiz(size_t indiceNo, size_t novoIndiceNo, int chaveMediana) {
            No1 novaRaiz;
            novaRaiz.folha = false;
            novaRaiz.numChaves = 1;
            novaRaiz.chaves[0] = chaveMediana;
            novaRaiz.ponteiro[0] = indiceNo;
            novaRaiz.ponteiro[1] = novoIndiceNo;

            metadados.indiceRaizPrimario = proximoIndice1;
            escreverNo(proximoIndice1++, novaRaiz);
            metadados.totalBlocosIndicePrimario++;

        }

        // Função para encontrar o nó folha onde a chave deve ser inserida
        static size_t encontrarFolha(int chave, size_t indiceAtual, std::vector<No1Caminho>& caminho) {
            No1 noAtual = lerNo(indiceAtual);
            No1Caminho noCaminho;
            noCaminho.indiceNo = indiceAtual;
            noCaminho.no = noAtual;
            caminho.push_back(noCaminho);

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
        static void inserirNaArvore(int chave, size_t indiceBloco, std::vector<No1Caminho>& caminho) {
            // Obter o nó folha
            No1Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No1 no = noCaminho.no;

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
                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoFolha(int chave, size_t indiceBloco, std::vector<No1Caminho>& caminho) {
            No1Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No1 no = noCaminho.no;

            // Vetor auxiliar
            int chavesAuxiliar[2*M + 1];
            size_t ponteirosAuxiliar[2*M + 1];

            int i = 0;
            while(i < 2*M && no.chaves[i] < chave) {
                chavesAuxiliar[i] = no.chaves[i];
                ponteirosAuxiliar[i] = no.ponteiro[i];
                i++;
            }

            chavesAuxiliar[i] = chave;
            ponteirosAuxiliar[i] = indiceBloco;
            i++;

            while(i < 2*M + 1) {
                chavesAuxiliar[i] = no.chaves[i - 1]; 
                ponteirosAuxiliar[i] = no.ponteiro[i - 1];
                i++;
            }

            // Cria novo Nó
            No1 novoNo;
            novoNo.folha = true;
            novoNo.numChaves = M + 1; // Metade + 1 será preenchido

            // Copiar metade + 1 das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                novoNo.chaves[i] = chavesAuxiliar[M + i];
                novoNo.ponteiro[i] = ponteirosAuxiliar[M + i];
            }

            no.numChaves = M;
            // Copiar primeira metade das chaves e ponteiros para o nó atual
            for(int i = 0; i < no.numChaves; i++) {
                no.chaves[i] = chavesAuxiliar[i];
                no.ponteiro[i] = ponteirosAuxiliar[i];
            }

            // Ajustando ponteiros das proximas folhas  
            novoNo.ponteiro[2*M] = no.ponteiro[2*M];
            size_t novoIndiceNo = proximoIndice1++;
            no.ponteiro[2*M] = novoIndiceNo;

            escreverNo(indiceNo, no);
            escreverNo(novoIndiceNo, novoNo);
            metadados.totalBlocosIndicePrimario++;

            int chaveMediana = novoNo.chaves[0];

            caminho.pop_back(); // Remover o nó folha do caminho

            if(caminho.empty()) {
                // Não tem nó anterior criar novo nó raiz
                novaRaiz(indiceNo, novoIndiceNo, chaveMediana);
            } else {
                // Inserir chave mediana no nó pai
                inserirNoInterno(chaveMediana, caminho, novoIndiceNo);
            }
        }

        // Função para inserir chave em nó interno
        static void inserirNoInterno(int chave, std::vector<No1Caminho>& caminho, size_t indiceDireita) {
            No1Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No1 no = noCaminho.no;

            // Encontrar posição para inserir a chave
            if(no.numChaves == 2 * M) {
                // Nó excedeu a capacidade, precisa dividir
                dividirNoInterno(chave, indiceDireita, caminho);
            } else {

                int i = no.numChaves - 1;
                while(i >= 0 && chave < no.chaves[i]) {
                    no.chaves[i + 1] = no.chaves[i];
                    no.ponteiro[i + 2] = no.ponteiro[i + 1];
                    i--;
                }
                no.chaves[i + 1] = chave;
                no.ponteiro[i + 2] = indiceDireita;
                no.numChaves++; 

                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoInterno(int chave, size_t indiceNo, std::vector<No1Caminho>& caminho) {
            No1Caminho noCaminho = caminho.back();
            size_t indiceNoAtual = noCaminho.indiceNo;
            No1 no = noCaminho.no;

            caminho.pop_back();

            // Vetor auxiliar
            int chavesAuxiliar[2*M + 1];
            size_t ponteirosAuxiliar[2*M + 2];

            int i = 0;
            while(i < 2*M && no.chaves[i] < chave) {
                chavesAuxiliar[i] = no.chaves[i];
                ponteirosAuxiliar[i] = no.ponteiro[i];
                i++;
            }

            ponteirosAuxiliar[i] = no.ponteiro[i];

            chavesAuxiliar[i] = chave;
            ponteirosAuxiliar[i + 1] = indiceNo;
            i++;

            while(i < 2*M + 1) {
                chavesAuxiliar[i] = no.chaves[i - 1]; 
                ponteirosAuxiliar[i + 1] = no.ponteiro[i];
                i++;
            }

            // Criar novo Nó
            No1 novoNo;
            novoNo.folha = false;
            novoNo.numChaves = M; // Pega metade

            // Copiar segunda metade + 1 das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                novoNo.chaves[i] = chavesAuxiliar[M + i + 1];
                novoNo.ponteiro[i] = ponteirosAuxiliar[M + i + 1];
            }
            novoNo.ponteiro[novoNo.numChaves] = ponteirosAuxiliar[2*M + 1];
            
            // Copiar primeira metade das chaves e ponteiros para o nó atual
            no.numChaves = M;
            for(int i = 0; i < no.numChaves; i++) {
                no.chaves[i] = chavesAuxiliar[i];
                no.ponteiro[i] = ponteirosAuxiliar[i];
            }
            no.ponteiro[no.numChaves] = ponteirosAuxiliar[M];

            size_t chaveMediana = chavesAuxiliar[M];

            size_t novoIndiceNo = proximoIndice1++;

            escreverNo(indiceNoAtual, no);
            escreverNo(novoIndiceNo, novoNo);
            metadados.totalBlocosIndicePrimario++;


            if(caminho.empty()) {
                // Criar novo nó raiz
                novaRaiz(indiceNoAtual, novoIndiceNo, chaveMediana);
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

                // Escrever nó raiz vazio
                arquivo.write(reinterpret_cast<const char*>(&noVazio), sizeof(No1));

                arquivo.close();

            }
        }

        // Inserir registro na árvore
        static void inserir(int chave, size_t indiceBloco) {
            std::vector<No1Caminho> caminho;
            size_t indiceFolha = encontrarFolha(chave, metadados.indiceRaizPrimario, caminho);
            
            inserirNaArvore(chave, indiceBloco, caminho);
        }

        // Buscar registro recursivamente na árvore
        static size_t buscaRegistro(int chave) {
            metadados.blocosLidosUtimaConsultaIndicePrimario = 0;
            return buscarRecursivo(chave, metadados.indiceRaizPrimario);
        }

};

#endif // BPTRER1_HPP