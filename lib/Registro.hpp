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

void sanitizeString(char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!std::isprint(static_cast<unsigned char>(str[i]))) { // Faz caracter especial virar 0
            str[i] = ' ';
        }
    }
}

void imprimeRegistro(const Registro& reg) {
    // Converte a data de atualização para um formato legível
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&reg.atualizacao));

     char titulo[301];
    char autores[151];
    char snippet[1025];
    
    std::strncpy(titulo, reg.titulo, 300);
    titulo[300] = '\0';
    std::strncpy(autores, reg.autores, 150);
    autores[150] = '\0';
    std::strncpy(snippet, reg.snippet, 1024);
    snippet[1024] = '\0';

    sanitizeString(titulo);
    sanitizeString(autores);
    sanitizeString(snippet);    

    // Imprime os detalhes do registro
    std::cout << "ID: " << reg.id << std::endl;
    std::cout << "Título: " << titulo << std::endl;
    std::cout << "Ano: " << reg.ano << std::endl;
    std::cout << "Autores: " << autores << std::endl;
    std::cout << "Citações: " << reg.citacoes << std::endl;
    std::cout << "Atualização: " << buffer << std::endl;
    std::cout << "Snippet: " << snippet << std::endl;
}

#endif // REGISTRO_HPP
