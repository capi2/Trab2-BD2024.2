#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <ctime>
#include <cstring>

// Definição da estrutura Registro
struct Registro {
    int id;
    char titulo[301];
    int ano;
    char autores[151];
    int citacoes;
    time_t atualizacao; 
    char snippet[1025];
};

// Função para criar um novo registro
Registro criarRegistro(int id, const char* titulo, int ano, const char* autores, int citacoes, time_t atualizacao, const char* snippet) {
    Registro reg;
    reg.id = id;
    std::strncpy(reg.titulo, titulo, 300);
    reg.titulo[300] = '\0';  // Garante terminação nula
    reg.ano = ano;
    std::strncpy(reg.autores, autores, 150);
    reg.autores[150] = '\0';
    reg.citacoes = citacoes;
    reg.atualizacao = atualizacao;
    std::strncpy(reg.snippet, snippet, 1024);
    reg.snippet[1024] = '\0';
    return reg;
}

#endif // REGISTRO_HPP
