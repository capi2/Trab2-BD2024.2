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
    BPTree1::inicializarArquivo();

    Reader::uploadCSV(CSV_PATH);
    // FindRec::buscaRegistro(3);

    Seek1::buscaRegistro(1000006);

    MetaDadosManager::imprimeMetaDados();
    MetaDadosManager::escreverMetaDados();

    

    return 0;
}