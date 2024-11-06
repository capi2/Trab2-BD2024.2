#ifndef BPTREE2_HPP
#define BPTREE2_HPP

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "MetaDados.hpp"

#define ARQUIVO_INDICE_SECUNDARIO "bd/ArquivoIndiceSecundario.bin"
#define M2  6 //2m x 301 + (2m+1) x 8 ≤ 4096

struct No2 {
    char chaves[2*M2][301];
    size_t ponteiro[2*M2 + 1];
    bool folha;
    unsigned short int numChaves;
    char padding[4096 - ((2*M2*301) + 8*(2*M2 + 1) + 1 + 2 + 8)];// 8?

    No2() {
        folha = true;
        numChaves = 0;
        memset(chaves, 0, sizeof(chaves));
        memset(ponteiro, 0, sizeof(ponteiro));
    }
};

struct No2Caminho {
    size_t indiceNo;
    No2 no;

    No2Caminho() {
        indiceNo = -1;
    }

};

size_t proximoIndice2 = 1; // Começa em 1 porque 0 é usado para a raiz

class BPTree2 {
    private:

        static No2 lerNo(size_t indice) {
            std::ifstream arquivo(ARQUIVO_INDICE_SECUNDARIO, std::ios::in | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler No2 porque arquivo de indice primário não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }
            arquivo.seekg(indice * sizeof(No2), std::ios::beg);
            No2 no;

            arquivo.read(reinterpret_cast<char*>(&no), sizeof(No2));
            arquivo.close();
            return no;
        }

        static void escreverNo(size_t indice, const No2& no) {
            std::fstream arquivo(ARQUIVO_INDICE_SECUNDARIO, std::ios::in | std::ios::out | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo escrever No2 porque arquivo de indice primário não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }

            arquivo.seekp(indice * sizeof(No2), std::ios::beg);
            arquivo.write(reinterpret_cast<const char*>(&no), sizeof(No2));
            arquivo.close();
        }

        static void novaRaiz(size_t indiceNo, size_t novoIndiceNo, char* chaveMediana) {
            No2 novaRaiz;
            novaRaiz.folha = false;
            novaRaiz.numChaves = 1;
            // Zerar os caracteres

            strcpy(novaRaiz.chaves[0], chaveMediana);


            novaRaiz.ponteiro[0] = indiceNo;
            novaRaiz.ponteiro[1] = novoIndiceNo;

            metadados.indiceRaizSecundario = proximoIndice2;
            //std::cout << "Escrevi Nova Raiz no indice: " << proximoIndice2 << std::endl;
            escreverNo(proximoIndice2++, novaRaiz);
            metadados.totalBlocosIndiceSecundario++;

        }

        // Função para encontrar o nó folha onde a chave deve ser inserida
        static size_t encontrarFolha(char* chave, size_t indiceAtual, std::vector<No2Caminho>& caminho) {
            No2 noAtual = lerNo(indiceAtual);
            No2Caminho noCaminho;
            noCaminho.indiceNo = indiceAtual;
            noCaminho.no = noAtual;
            caminho.push_back(noCaminho);

            if(noAtual.folha) {
                return indiceAtual;
            } else {
                // Percorrer as chaves para encontrar o caminho correto
                int i = 0;
                while(i < noAtual.numChaves && strcmp(chave, noAtual.chaves[i]) >= 0) {
                    i++;
                }
                // Chamar recursivamente para o filho apropriado
                return encontrarFolha(chave, noAtual.ponteiro[i], caminho);
            }
        }

        // Função para inserir chave na árvore e ajustar a árvore
        static void inserirNaArvore(char* chave, size_t indiceBloco, std::vector<No2Caminho>& caminho) {
            // Obter o nó folha
            No2Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No2 no = noCaminho.no;



            // Inserir chave no nó

            if(no.numChaves == 2 * M2) {
                // Nó excedeu a capacidade, precisa dividir
                //std::cout << "Folha " << indiceNo << " lotada!";
                dividirNoFolha(chave, indiceBloco, caminho);
            } else {

                int i = no.numChaves - 1; //erro ta aqui
                while(i >= 0 && strcmp(chave, no.chaves[i]) < 0) {
                    strcpy(no.chaves[i + 1], no.chaves[i]);
                    no.ponteiro[i + 1] = no.ponteiro[i];
                    i--;
                }
                strcpy(no.chaves[i + 1], chave);
                no.ponteiro[i + 1] = indiceBloco;
                no.numChaves++;

                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoFolha(char* chave, size_t indiceBloco, std::vector<No2Caminho>& caminho) {
            No2Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No2 no = noCaminho.no;

            // Vetor auxiliar
            char chavesAuxiliar[2*M2 + 1][301];
            size_t ponteirosAuxiliar[2*M2 + 1];

            int i = 0;
            while(i < 2*M2 && strcmp(no.chaves[i], chave) < 0) {
                strcpy(chavesAuxiliar[i], no.chaves[i]);
                ponteirosAuxiliar[i] = no.ponteiro[i];
                i++;
            }

            strcpy(chavesAuxiliar[i], chave);
            ponteirosAuxiliar[i] = indiceBloco;
            i++;

            while(i < 2*M2 + 1) {
                strcpy(chavesAuxiliar[i], no.chaves[i - 1]);
                ponteirosAuxiliar[i] = no.ponteiro[i - 1];
                i++;
            }
            // Vetor auxiliar


            No2 novoNo;
            novoNo.folha = true;
            novoNo.numChaves = M2 + 1; // Pega metade

            // Copiar metade + 1 das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                strcpy(novoNo.chaves[i], chavesAuxiliar[M2 + i]);
                novoNo.ponteiro[i] = ponteirosAuxiliar[M2 + i];
            }

            no.numChaves = M2;
            // Copiar metade das chaves e ponteiros para o nó atual
            for(int i = 0; i < no.numChaves; i++) {
                strcpy(no.chaves[i], chavesAuxiliar[i]);
                no.ponteiro[i] = ponteirosAuxiliar[i];
            }

            // Ajustando ponteiros das proximas folhas  
            novoNo.ponteiro[2*M2] = no.ponteiro[2*M2];
            size_t novoIndiceNo = proximoIndice2++;
            no.ponteiro[2*M2] = novoIndiceNo;

            escreverNo(indiceNo, no);
            escreverNo(novoIndiceNo, novoNo);
            metadados.totalBlocosIndiceSecundario++;

            char chaveMediana[301]; 
            strcpy(chaveMediana, novoNo.chaves[0]);

            caminho.pop_back(); // Remover o nó folha do caminho

            if(caminho.empty()) {
                // Criar novo nó raiz
                novaRaiz(indiceNo, novoIndiceNo, chaveMediana);
            } else {
                // Inserir chave mediana no nó pai
                //std::cout << "Folha dividida joguei chave: " << chaveMediana << std::endl; 
                inserirNoInterno(chaveMediana, caminho, novoIndiceNo);
            }
        }

        // Função para inserir chave em nó interno
        static void inserirNoInterno(char* chave, std::vector<No2Caminho>& caminho, size_t indiceDireita) {
            No2Caminho noCaminho = caminho.back();
            size_t indiceNo = noCaminho.indiceNo;
            No2 no = noCaminho.no;

            // Encontrar posição para inserir a chave
            if(no.numChaves == 2 * M2) {
                // Nó excedeu a capacidade, precisa dividir
                dividirNoInterno(chave, indiceDireita, caminho);
            } else {

                int i = no.numChaves - 1;
                while(i >= 0 && strcmp(chave, no.chaves[i]) < 0) {
                    strcpy(no.chaves[i + 1], no.chaves[i]);
                    no.ponteiro[i + 2] = no.ponteiro[i + 1];
                    i--;
                } // se for o ultimo?

                strcpy(no.chaves[i + 1], chave);
                no.ponteiro[i + 2] = indiceDireita;
                no.numChaves++; 

                //std::cout << "Inseri chave" << std::endl;
                escreverNo(indiceNo, no);
                return;
            
            }
        }

        static void dividirNoInterno(char* chave, size_t indiceNo, std::vector<No2Caminho>& caminho) {
            No2Caminho noCaminho = caminho.back();
            size_t indiceNoAtual = noCaminho.indiceNo;
            No2 no = noCaminho.no;

            
            // Vetor auxiliar
            char chavesAuxiliar[2*M2 + 1][301];
            size_t ponteirosAuxiliar[2*M2 + 2];

            int i = 0;
            while(i < 2*M2 && strcmp(no.chaves[i], chave) < 0) {
                strcpy(chavesAuxiliar[i], no.chaves[i]);
                ponteirosAuxiliar[i] = no.ponteiro[i];
                i++;
            }

            ponteirosAuxiliar[i] = no.ponteiro[i];

            strcpy(chavesAuxiliar[i], chave);
            ponteirosAuxiliar[i + 1] = indiceNo;
            i++;

            while(i < 2*M2 + 1) {
                strcpy(chavesAuxiliar[i], no.chaves[i - 1]); 
                ponteirosAuxiliar[i + 1] = no.ponteiro[i];
                i++;
            }

            

            // Vetor auxiliar

            No2 novoNo;
            novoNo.folha = false;
            novoNo.numChaves = M2; // Pega metade

            // Copiar metade das chaves e ponteiros para o novo nó
            for(int i = 0; i < novoNo.numChaves; i++) {
                strcpy(novoNo.chaves[i], chavesAuxiliar[M2 + i + 1]);
                novoNo.ponteiro[i] = ponteirosAuxiliar[M2 + i + 1];
            }

            novoNo.ponteiro[novoNo.numChaves] = ponteirosAuxiliar[2*M2 + 1];


            no.numChaves = M2;

            for(int i = 0; i < no.numChaves; i++) {
                strcpy(no.chaves[i], chavesAuxiliar[i]);
                no.ponteiro[i] = ponteirosAuxiliar[i];
            }
            no.ponteiro[no.numChaves] = ponteirosAuxiliar[M2];

            char chaveMediana[301];

            strcpy(chaveMediana, chavesAuxiliar[M2]);

            size_t novoIndiceNo = proximoIndice2++;

        



            escreverNo(indiceNoAtual, no);
            escreverNo(novoIndiceNo, novoNo);
            metadados.totalBlocosIndiceSecundario++;

            caminho.pop_back();

            if(caminho.empty()) {
                // Criar novo nó raiz
                novaRaiz(indiceNoAtual, novoIndiceNo, chaveMediana);
            } else {
                // Inserir chave mediana no nó pai
                //std::cout << "Nó dividido joguei chave: " << chaveMediana << std::endl;
                inserirNoInterno(chaveMediana, caminho, novoIndiceNo);
            }
        }

        // Função recursiva para buscar a chave
        static size_t buscarRecursivo(char* chave, size_t indiceAtual) {
            No2 noAtual = lerNo(indiceAtual);

            //std::cout << "Lendo No de indice " << indiceAtual << std::endl;

            metadados.blocosLidosUtimaConsultaIndiceSecundario++;

            if(noAtual.folha) {
                //std::cout << "Indice folha:  " << indiceAtual << std::endl;
                // Procurar a chave no nó folha
                for(int i = 0; i < noAtual.numChaves; i++) {
                    //std::cout << indiceAtual << " Folha [" << i + 1 << "/" << noAtual.numChaves  << "] | " << noAtual.chaves[i] << "==" << chave << std::endl;
                    if(strcmp(noAtual.chaves[i], chave) == 0) {
                        return noAtual.ponteiro[i]; // Retorna o ponteiro associado
                    }
                }
                // Chave não encontrada no nó folha
                return size_t(-1);
            } else {
                // Nó interno: determinar qual filho seguir
                //std::cout << indiceAtual <<" | ";
                int i = 0;
                while(i < noAtual.numChaves && strcmp(chave, noAtual.chaves[i]) >= 0) {
                    i++;
                }

                //std::cout << noAtual.chaves[0] << " | " << noAtual.ponteiro[i] << std::endl;

                // std::cout << "Lendo No de indice " << indiceAtual << std::endl;
                // Chamar recursivamente para o filho apropriado
                return buscarRecursivo(chave, noAtual.ponteiro[i]);
            }
        }

    public:

        static void inicializarArquivo() {
            std::fstream arquivo(ARQUIVO_INDICE_SECUNDARIO, std::ios::in | std::ios::binary);

            if (!arquivo) {
                std::cout << "Criando Arquivo de Indice Secundário!" << std::endl;
                // Arquivo não existe, criar e pré-alocar espaço
                arquivo.open(ARQUIVO_INDICE_SECUNDARIO, std::ios::out | std::ios::binary);

                No2 noVazio;
                noVazio.folha = true;
                noVazio.numChaves = 0;

                // Escrever nó raiz vazio
                arquivo.write(reinterpret_cast<const char*>(&noVazio), sizeof(No2));

                arquivo.close();

            }
        }

        static void inserir(char* chave, size_t indiceBloco) {
            std::vector<No2Caminho> caminho;
            size_t indiceFolha = encontrarFolha(chave, metadados.indiceRaizSecundario, caminho);

            inserirNaArvore(chave, indiceBloco, caminho);
        }

        static size_t buscaRegistro(char* chave) {
            metadados.blocosLidosUtimaConsultaIndiceSecundario = 0;
            return buscarRecursivo(chave, metadados.indiceRaizSecundario);
        }

        static No2 lerNoTeste(size_t indice) {
            std::ifstream arquivo(ARQUIVO_INDICE_SECUNDARIO, std::ios::in | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler No2 porque arquivo de indice primário não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }
            arquivo.seekg(indice * sizeof(No2), std::ios::beg);
            No2 no;

            arquivo.read(reinterpret_cast<char*>(&no), sizeof(No2));
            arquivo.close();
            return no;
        }

};

#endif // BPTRER2_HPP