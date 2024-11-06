#include "../lib/BPTree2.hpp"
#include "../lib/Bloco.hpp"
#include "../lib/Registro.hpp"

#define ARQUIVO_DADOS "bd/ArquivoDados.bin"

class Seek2 {
    private:

        static Registro* lerRegistro(size_t indice, char* titulo) {
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
                    if (strcmp(bloco.registros[i].titulo, titulo) == 0) {
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
        static void buscaRegistro(const char* tituloConst) {
            char titulo[301];
            strcpy(titulo, tituloConst);

            std::cout << "\nBuscando Registro de Título: \'" ;
            std::cout.write(titulo, 20);
            std::cout << "...\'" << std::endl;

            size_t indiceBloco = BPTree2::buscaRegistro(titulo);
            
            if(indiceBloco == -1) {
                std::cout << "Registro não encontrado na árvore secundária" << std::endl;
                return;
            } 

            Registro* reg = lerRegistro(indiceBloco, titulo);

            if(!reg) {
                std::cout << " ===== Registro não encontrado! =====\n" << std::endl;
            } else {
                std::cout << "===== Registro Encontrado | Blocos Lidos: " << metadados.blocosLidosUtimaConsultaIndiceSecundario << " | Blocos Indice Secundário: " << metadados.totalBlocosIndiceSecundario << " =====" << std::endl;
                imprimeRegistro(*reg);
                std::cout << "===== Registro Encontrado | Blocos Lidos: " << metadados.blocosLidosUtimaConsultaIndiceSecundario << " | Blocos Indice Secundário: " << metadados.totalBlocosIndiceSecundario << " =====" << std::endl;
            }


        }
};