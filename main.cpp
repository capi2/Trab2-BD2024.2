#include <iostream>
#include "src/upload.hpp"
#include "src/seek1.hpp"
#include "src/seek2.hpp"
#include "src/findrec.hpp"
#include "lib/MetaDados.hpp"
 
//#define CSV_PATH "data/artigomenor.csv" // Arquivo contendo 1000 registros para teste
#define COMPLETO_CSV_PATH "data/artigo.csv"

int main() {

    int comando;
    MetaDadosManager::inicializarArquivo();

    std::cout << " ===== BD2024.2 trabalho-pratico-2 ===== \n" << std::endl;
    std::cout << "O programa implementa o comportamento de um banco de dados utilizando arquivo de dados por hash e arquivo de indice com árvore B+." << std::endl;
    std::cout << "Os arquivos criados no programa ficam salvos para poderem ser executados novamente após finalizar o programa. \n" << std::endl;

    while(true) {

        std::cout << "Digite o número da função que você quer executar: " << std::endl;
        std::cout << "0) Upload - Lê o arquivo data/artigo.csv e cria os arquivos de dados, indice primario, secundario e o arquivo de metadados." << std::endl;
        std::cout << "1) Findrec - Procura um registro pelo ID no arquivo de dados estruturado por tabela hash." << std::endl;
        std::cout << "2) Seek1 - Procura um registro pelo ID no arquivo de indice primário." << std::endl;
        std::cout << "3) Seek2 - Procura um registro pelo Título no arquivo de indice secundário." << std::endl;
        std::cout << "4) MetaDados - Mostra os metadados do programa." << std::endl;
        std::cout << "-1) Exit - Finaliza o programa.\n"<< std::endl;

        std::cin >> comando;

        switch (comando)
        {
        case 0:
            TabelaHash::inicializarArquivo();
            BPTree1::inicializarArquivo();
            BPTree2::inicializarArquivo();

            std::cout << std::endl;
            Reader::uploadCSV(COMPLETO_CSV_PATH);
            std::cout << std::endl;

            break;

        case 1:
            int idFindrec;
            std::cout << "\nDigite o ID para buscar: ";
            std::cin >> idFindrec;

            std::cout << std::endl;
            FindRec::buscaRegistro(idFindrec);
            std::cout << std::endl;

            break;
        case 2:
            int idSeek1;
            std::cout << "\nDigite o ID para buscar: ";
            std::cin >> idSeek1;

            std::cout << std::endl;
            Seek1::buscaRegistro(idSeek1);
            std::cout << std::endl;

            break;
        case 3:
            char idSeek2[301];
            std::cout << "\nDigite o Título para buscar: ";

            std::cin.ignore();
            std::cin.getline(idSeek2, 301);

            std::cout << std::endl;
            Seek2::buscaRegistro(idSeek2);
            std::cout << std::endl;
            break;
        case 4:
            std::cout << std::endl;
            MetaDadosManager::imprimeMetaDados();
            std::cout << std::endl;

            break;
        case -1:
            MetaDadosManager::escreverMetaDados();
            exit(0);

            break;
        default:
            std::cout << "Entrada inválida!" << std::endl;
            break;
        }

    }

    MetaDadosManager::escreverMetaDados();


    return 0;
}