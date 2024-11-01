#ifndef TABELA_HASH_HPP
#define TABELA_HASH_HPP

#include "registro.hpp"
#include "bloco.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define NUM_BUCKETS 10

int funcaoHash(int id) {
    return id % NUM_BUCKETS;
}

void inicializarTabelaHash(const char* nomeArquivo) {
    FILE* arquivo = std::fopen(nomeArquivo, "wb");
    if (!arquivo) {
        std::perror("Erro ao criar arquivo de dados");
        exit(EXIT_FAILURE);
    }

    // Inicializa os baldes com blocos vazios
    Bloco blocoVazio;
    inicializarBloco(&blocoVazio);

    for (int i = 0; i < NUM_BUCKETS; i++) {
        std::fwrite(&blocoVazio, sizeof(Bloco), 1, arquivo);
    }

    std::fclose(arquivo);
}

// Função para inserir um registro na tabela hash
void inserirRegistroHash(const char* nomeArquivo, const Registro& registro) {
    int hash = funcaoHash(registro.id);

    FILE* arquivo = std::fopen(nomeArquivo, "rb+");
    if (!arquivo) {
        std::perror("Erro ao abrir arquivo de dados");
        exit(EXIT_FAILURE);
    }

    // Calcula a posição do bloco no arquivo
    long offset = hash * sizeof(Bloco);

    // Move o ponteiro do arquivo para o bloco correto
    std::fseek(arquivo, offset, SEEK_SET);

    // Lê o bloco do arquivo
    Bloco bloco;
    std::fread(&bloco, sizeof(Bloco), 1, arquivo);

    // Insere o registro no bloco (em memória)
    inserirRegistroNoBloco(&bloco, registro, arquivo);

    // Move o ponteiro de volta para escrever o bloco atualizado
    std::fseek(arquivo, offset, SEEK_SET);

    // Escreve o bloco atualizado de volta no arquivo
    std::fwrite(&bloco, sizeof(Bloco), 1, arquivo);

    std::fclose(arquivo);
}

// Função modificada para inserir registro no bloco e gerenciar overflow em arquivo
void inserirRegistroNoBloco(Bloco* bloco, const Registro& registro, FILE* arquivo) {
    if (bloco->count < TAMANHO_BLOCO) {
        bloco->registros[bloco->count++] = registro;
    } else {
        // Gerencia os blocos de overflow
        BlocoOverflow overflowBloco;

        // Se não há overflow ainda, inicializa e atualiza o ponteiro no bloco principal
        if (bloco->overflowRRN == -1) {
            bloco->overflowRRN = obterRRNOverflow(arquivo);

            inicializarBlocoOverflow(&overflowBloco);
            overflowBloco.registros[overflowBloco.count++] = registro;

            escreverBlocoOverflow(arquivo, bloco->overflowRRN, &overflowBloco);
        } else {
            // Carrega o bloco de overflow existente
            lerBlocoOverflow(arquivo, bloco->overflowRRN, &overflowBloco);

            // Procura o último bloco de overflow disponível
            while (overflowBloco.count == TAMANHO_BLOCO) {
                if (overflowBloco.proximoRRN == -1) {
                    // Cria um novo bloco de overflow
                    int novoRRN = obterRRNOverflow(arquivo);
                    overflowBloco.proximoRRN = novoRRN;
                    escreverBlocoOverflow(arquivo, bloco->overflowRRN, &overflowBloco);

                    inicializarBlocoOverflow(&overflowBloco);
                    overflowBloco.registros[overflowBloco.count++] = registro;

                    escreverBlocoOverflow(arquivo, novoRRN, &overflowBloco);
                    return;
                } else {
                    // Carrega o próximo bloco de overflow
                    int proximoRRN = overflowBloco.proximoRRN;
                    lerBlocoOverflow(arquivo, proximoRRN, &overflowBloco);
                    bloco->overflowRRN = proximoRRN;
                }
            }

            // Insere o registro no bloco de overflow disponível
            overflowBloco.registros[overflowBloco.count++] = registro;
            escreverBlocoOverflow(arquivo, bloco->overflowRRN, &overflowBloco);
        }
    }
}

// Função para obter o RRN (Relative Record Number) para um novo bloco de overflow
int obterRRNOverflow(FILE* arquivo) {
    // Move para o final do arquivo para adicionar um novo bloco
    std::fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = std::ftell(arquivo);
    return (tamanhoArquivo - NUM_BUCKETS * sizeof(Bloco)) / sizeof(BlocoOverflow);
}

// Funções para ler e escrever blocos de overflow
void lerBlocoOverflow(FILE* arquivo, int rrn, BlocoOverflow* blocoOverflow) {
    long offset = NUM_BUCKETS * sizeof(Bloco) + rrn * sizeof(BlocoOverflow);
    std::fseek(arquivo, offset, SEEK_SET);
    std::fread(blocoOverflow, sizeof(BlocoOverflow), 1, arquivo);
}

void escreverBlocoOverflow(FILE* arquivo, int rrn, BlocoOverflow* blocoOverflow) {
    long offset = NUM_BUCKETS * sizeof(Bloco) + rrn * sizeof(BlocoOverflow);
    std::fseek(arquivo, offset, SEEK_SET);
    std::fwrite(blocoOverflow, sizeof(BlocoOverflow), 1, arquivo);
}

#endif // TABELA_HASH_HPP
