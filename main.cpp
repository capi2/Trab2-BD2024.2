#include <iostream>
#include "src/upload.hpp"
#include "src/seek1.hpp"
#include "src/seek2.hpp"
#include "src/findrec.hpp"
#include "lib/Bloco.hpp"
#include "lib/Registro.hpp"
 
#define CSV_PATH "data/artigomenor.csv"
#define COMPLETO_CSV_PATH "data/artigo.csv"

int main() {
    // Fazer o menu
    TabelaHash::inicializarArquivo();
    Reader::uploadCSV(CSV_PATH);

    FindRec::buscaRegistro(3);

    // std::cout << sizeof(Bloco) << std::endl;
    // std::cout << sizeof(Registro) << std::endl;

    return 0;
}