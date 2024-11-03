#include <iostream>
#include "src/upload.hpp"
#include "src/seek1.hpp"
#include "src/seek2.hpp"
#include "src/findrec.hpp"
#include "lib/MetaDados.hpp"
 
#define CSV_PATH "data/artigomenor.csv"
#define COMPLETO_CSV_PATH "data/artigo.csv"

int main() {
    // Fazer o menu
    TabelaHash::inicializarArquivo();
    MetaDadosManager::inicializarArquivo();
    
    Reader::uploadCSV(COMPLETO_CSV_PATH);
    FindRec::buscaRegistro(3);

    MetaDadosManager::imprimeMetaDados();

    MetaDadosManager::escreverMetaDados();

    return 0;
}