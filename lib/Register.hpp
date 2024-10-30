#include <iostream>
#include <cstring>
#include <ctime>

class Register {
private:
    int id;
    char titulo[301];
    int ano;
    char autores[151];
    int citacoes;
    time_t atualizacao; 
    char snippet[1025];

public:

    Register() : id(0), ano(0), citacoes(0), atualizacao(time(nullptr)) {
        std::memset(titulo, 0, sizeof(titulo));
        std::memset(autores, 0, sizeof(autores));
        std::memset(snippet, 0, sizeof(snippet));
    }

    int getId() const {
        return id;
    }

    void setId(int newId) {
        if (newId > 0) {
            id = newId;
        }
    }

    const char* getTitulo() const {
        return titulo;
    }

    void setTitulo(const char* newTitulo) {
        if (std::strlen(newTitulo) <= 300) {
            std::strncpy(titulo, newTitulo, sizeof(titulo) - 1);
        } else {
            std::cout << "Erro: Título excede o limite de 300 caracteres." << std::endl;
        }
    }

    int getAno() const {
        return ano;
    }

    void setAno(int newAno) {
        if (newAno > 0) {
            ano = newAno;
        }
    }

    const char* getAutores() const {
        return autores;
    }

    void setAutores(const char* newAutores) {
        if (std::strlen(newAutores) <= 150) {
            std::strncpy(autores, newAutores, sizeof(autores) - 1);
        } else {
            std::cout << "Erro: Lista de autores excede o limite de 150 caracteres." << std::endl;
        }
    }

    int getCitacoes() const {
        return citacoes;
    }

    void setCitacoes(int newCitacoes) {
        if (newCitacoes >= 0) {
            citacoes = newCitacoes;
        }
    }

    time_t getAtualizacao() const {
        return atualizacao;
    }

    void setAtualizacao(time_t newAtualizacao) {
        atualizacao = newAtualizacao;
    }

    const char* getSnippet() const {
        return snippet;
    }

    void setSnippet(const char* newSnippet) {
        if (std::strlen(newSnippet) >= 100 && std::strlen(newSnippet) <= 1024) {
            std::strncpy(snippet, newSnippet, sizeof(snippet) - 1);
        } else {
            std::cout << "Erro: Snippet deve ter entre 100 e 1024 caracteres." << std::endl;
        }
    }
};
