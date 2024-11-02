#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include <iostream>
#include <ctime>
#include <cstring>
#include <iomanip>

#define TOTAL_MAXIMO_REGISTROS 1549146

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

void imprimeRegistro(const Registro& reg) {
    // Converte a data de atualização para um formato legível
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&reg.atualizacao));

    // Imprime os detalhes do registro
    std::cout << "ID: " << reg.id << std::endl;
    std::cout << "Título: " << reg.titulo << std::endl;
    std::cout << "Ano: " << reg.ano << std::endl;
    std::cout << "Autores: " << reg.autores << std::endl;
    std::cout << "Citações: " << reg.citacoes << std::endl;
    std::cout << "Atualização: " << buffer << std::endl;
    std::cout << "Snippet: " << reg.snippet << std::endl;
}

#endif // REGISTRO_HPP
