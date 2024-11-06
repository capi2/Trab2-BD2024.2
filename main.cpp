#include <iostream>
#include "src/upload.hpp"
#include "src/seek1.hpp"
#include "src/seek2.hpp"
#include "src/findrec.hpp"
#include "lib/MetaDados.hpp"
 
#define CSV_PATH "data/artigomenor.csv"
#define COMPLETO_CSV_PATH "data/artigo.csv"

void test1() {
    No1 noAtual = BPTree1::lerNoTeste(3);


    for(int i = 0; i < noAtual.numChaves; i++) {
        std::cout << noAtual.chaves[i] << std::endl;
    }
    std::cout << noAtual.folha << std:: endl;
    std::cout << noAtual.ponteiro[2*M2] << std::endl;

}

void test2() {
    size_t indiceAtual = 0;
    No2 noAtual = BPTree2::lerNoTeste(indiceAtual);


    for(int i = 0; i < noAtual.numChaves; i++) {
        std::cout << indiceAtual <<" Folha[" << noAtual.folha << "] [" << i + 1 << "/" << noAtual.numChaves  << "] | " << noAtual.chaves[i] << " | " << noAtual.ponteiro[i] << std::endl;
    } std::cout << noAtual.ponteiro[noAtual.numChaves] << std::endl;

}





int main() {

    // Fazer o menu
    TabelaHash::inicializarArquivo();
    MetaDadosManager::inicializarArquivo();
    BPTree1::inicializarArquivo();
    BPTree2::inicializarArquivo();

    Reader::uploadCSV(CSV_PATH);

    MetaDadosManager::imprimeMetaDados();
    MetaDadosManager::escreverMetaDados();

    

    return 0;
}