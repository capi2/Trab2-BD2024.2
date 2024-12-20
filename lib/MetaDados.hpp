#ifndef METADADOS_HPP
#define METADADOS_HPP

#include <iostream>
#include <fstream>

#define ARQUIVO_METADADOS "bd/ArquivoMetaDados.bin"

struct MetaDados { // Informações sobre o metadados
    size_t totalBlocos;
    size_t totalRegistros;
    size_t blocosUsados;
    size_t blocosLidosUltimaConsultaHash;
    size_t totalBlocosIndicePrimario;
    size_t blocosLidosUtimaConsultaIndicePrimario;
    size_t totalBlocosIndiceSecundario;
    size_t blocosLidosUtimaConsultaIndiceSecundario;
    size_t indiceRaizPrimario;
    size_t indiceRaizSecundario;
    bool arquivoCSVLido;

    MetaDados() {
        totalBlocos = 0;
        totalRegistros = 0;
        blocosUsados = 0;
        blocosLidosUltimaConsultaHash = 0;
        totalBlocosIndicePrimario = 0;
        blocosLidosUtimaConsultaIndicePrimario = 0;
        totalBlocosIndiceSecundario = 0;
        blocosLidosUtimaConsultaIndiceSecundario = 0;
        indiceRaizPrimario = 0;
        indiceRaizSecundario = 0;
        arquivoCSVLido = false;
    }
};

MetaDados metadados = MetaDados();

class MetaDadosManager {
    private:
    
    public:

        static void lerMetaDados() {
            std::ifstream arquivo(ARQUIVO_METADADOS, std::ios::in | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler metadados porque arquivo de dados não existe! Verifique se o diretório bd existe ou se não fez upload ainda!" << std::endl;
            }

            arquivo.read(reinterpret_cast<char*>(&metadados), sizeof(MetaDados));
            arquivo.close();
        }

        static void inicializarArquivo() {
            std::fstream arquivo(ARQUIVO_METADADOS, std::ios::in | std::ios::binary);

            if (!arquivo) {
                std::cout << "Criando Arquivo de metadados!" << std::endl;
                arquivo.open(ARQUIVO_METADADOS, std::ios::out | std::ios::binary);
                MetaDados metaDadosVazio;

                // Escrever MetaDados vazio
                arquivo.write(reinterpret_cast<const char*>(&metaDadosVazio), sizeof(MetaDados));
                arquivo.close();
            } else {
                lerMetaDados();
            }
        }

        static void escreverMetaDados() {
            std::fstream arquivo(ARQUIVO_METADADOS, std::ios::in | std::ios::out | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler metadados porque arquivo de dados não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }

            arquivo.write(reinterpret_cast<const char*>(&metadados), sizeof(MetaDados));
            arquivo.close();
        }

        static void imprimeMetaDados() {

            std::cout << "\n ===== METADADOS ===== " << std::endl;
            std::cout << "Total de blocos no arquivo de dados: " << metadados.totalBlocos << std::endl;
            std::cout << "Total de Registros: " << metadados.totalRegistros << std::endl;
            std::cout << "Total de Blocos Ocupados (pelo menos 1 registro): " << metadados.blocosUsados << std::endl;
            std::cout << "Blocos Lidos na última consulta ao Índice: " << metadados.blocosLidosUltimaConsultaHash << std::endl;
            std::cout << "Total de Blocos Índice Primário: " << metadados.totalBlocosIndicePrimario << std::endl;
            std::cout << "Blocos Lidos na última consulta ao Índice Primário: " << metadados.blocosLidosUtimaConsultaIndicePrimario << std::endl;
            std::cout << "Total de Blocos Índice Secundário: " << metadados.totalBlocosIndiceSecundario << std::endl;
            std::cout << "Blocos Lidos na última consulta ao Índice Secundário: " << metadados.blocosLidosUtimaConsultaIndiceSecundario << std::endl;
            std::cout << "Indice Raiz Primário: " << metadados.indiceRaizPrimario << std::endl;
            std::cout << "Indice Raiz Secundário: " << metadados.indiceRaizSecundario << std::endl;
            std::cout << "Arquivo csv lido: " << metadados.arquivoCSVLido << std::endl;
            std::cout << " ===== METADADOS ===== \n" << std::endl;

        }
};




#endif // METADADOS_HPP