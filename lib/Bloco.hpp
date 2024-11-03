#ifndef BLOCO_HPP
#define BLOCO_HPP

#include "Registro.hpp"
#include <cstdlib>
#include <cstdio>

#define MAX_REG_BLOCO 2  // Número de registros por bloco
#define TAMANHO_BLOCO 4096

struct Bloco {
    Registro registros[MAX_REG_BLOCO];
    int numRegistros;
    int proxBloco; // Índice do próximo bloco de overflow (-1 se não houver)
    char padding[4096 - (sizeof(Registro) * MAX_REG_BLOCO) - sizeof(int)*2];

    Bloco() {
        numRegistros = 0;
        proxBloco = -1; // -1 indica que não há bloco de overflow
    }
};

void imprimeBloco(const Bloco& bloco) {
    std::cout << "numRegistros: " << bloco.numRegistros << std::endl;

    for(int i = 0; i < bloco.numRegistros; i++) {
        std::cout << "Registro [" << i << "]:" << std::endl;
        imprimeRegistro(bloco.registros[i]);
    }
}

#endif // BLOCO_HPP
