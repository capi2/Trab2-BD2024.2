#include "../lib/BPTree1.hpp"
#include "../lib/Bloco.hpp"
#include "../lib/Registro.hpp"

#define ARQUIVO_DADOS "bd/ArquivoDados.bin"

class Seek1 {
    private:

        static Registro* lerRegistro(size_t indice, int id) {
            std::ifstream arquivo(ARQUIVO_DADOS, std::ios::in | std::ios::binary);
            if(!arquivo) {
                std::cout << "Não consigo ler bloco porque arquivo de dados não existe! Verifique se o diretório bd existe" << std::endl;
                exit(1);
            }
            arquivo.seekg(indice * sizeof(Bloco), std::ios::beg);
            Bloco bloco;

            arquivo.read(reinterpret_cast<char*>(&bloco), sizeof(Bloco));
            arquivo.close();

            while (true) {
                for (int i = 0; i < bloco.numRegistros; ++i) { // Busca no Bloco
                    if (bloco.registros[i].id == id) {
                        // Registro encontrado
                        Registro* resultado = new Registro(bloco.registros[i]);
                        metadados.blocosLidosUltimaConsultaHash = 1;
                        return resultado;
                    }
                }
                // Precisa encontrar no overflow
                break;
            }
            return nullptr;
        }

    public:
        static void buscaRegistro(int id) {
            std::cout << "\nBuscando Registro de ID: " << id << "..." << std::endl;
            size_t indiceBloco = BPTree1::buscaRegistro(id);
            
            if(indiceBloco == -1) {
                std::cout << "Registro não encontrado na árvore primária" << std::endl;
                return;
            } 

            Registro* reg = lerRegistro(indiceBloco, id);

            if(!reg) {
                std::cout << " ===== Registro não encontrado! =====\n" << std::endl;
            } else {
                std::cout << "===== Registro Encontrado | Blocos Lidos: " << metadados.blocosLidosUtimaConsultaIndicePrimario << " | Blocos Indice Primário: " << metadados.totalBlocosIndicePrimario << " =====" << std::endl;
                imprimeRegistro(*reg);
                std::cout << "===== Registro Encontrado | Blocos Lidos: " << metadados.blocosLidosUtimaConsultaIndicePrimario << " | Blocos Indice Primário: " << metadados.totalBlocosIndicePrimario << " =====" << std::endl;
            }


        }
};