#include "../lib/Hash.hpp"
#include "../lib/Registro.hpp"

class FindRec {
    public:
        static void buscaRegistro(int id) {
            Registro* reg = TabelaHash::buscar(id);

            if(!reg) {
                std::cout << "Registro não encontrado!" << std::endl;
            } else {
                std::cout << "===== Registro Encontrado =====" << std::endl;
                imprimeRegistro(*reg);
                std::cout << "===== Registro Encontrado =====" << std::endl;
            }


        }
};