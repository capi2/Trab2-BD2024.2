#ifndef BLOCO_HPP
#define BLOCO_HPP

#include "registro.hpp"
#include <cstdlib>
#include <cstdio>

#define TAMANHO_BLOCO 5  // Defina o tamanho apropriado do bloco

// Estrutura do bloco de overflow
struct BlocoOverflow {
    Registro registros[TAMANHO_BLOCO];
    int count;
    int proximoRRN;  // RRN do próximo bloco de overflow
};

// Estrutura do bloco principal
struct Bloco {
    Registro registros[TAMANHO_BLOCO];
    int count;
    int overflowRRN;  // RRN do primeiro bloco de overflow
};

// Função para inicializar um bloco principal
void inicializarBloco(Bloco* bloco) {
    bloco->count = 0;
    bloco->overflowRRN = -1;  // -1 indica que não há overflow
}

// Função para inicializar um bloco de overflow
void inicializarBlocoOverflow(BlocoOverflow* blocoOverflow) {
    blocoOverflow->count = 0;
    blocoOverflow->proximoRRN = -1;  // -1 indica que não há próximo bloco
}

#endif // BLOCO_HPP
