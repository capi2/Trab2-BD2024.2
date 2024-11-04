# Trab2-BD2024.2 

Repositório para a disciplina Banco de Dados I do segundo trabalho pŕatico.

## Como executar?

Para executar este código, coloque o arquivo ```arquivo.csv``` na pasta ```data``` e execute:

```cpp
g++ main.cpp -o main
./main
```

## Documentação

### Pastas

- ```bd```:      pasta onde os arquivos binários serão criados após a leitura do ```arquivo.csv```.
- ```data```:    pasta onde o ```arquivo.csv``` deve estar para ser lido!
-  ```lib```:    pasta onde as estruturas de dados e classes serão utilizadas no trabalho.
-  ```src```:    pasta onde as funções findrec, seek1, seek2 e upload estão localizadas.

### Arquivos

####  Bloco.hpp

Define a estrutura básica do bloco. O tamanho do bloco será de 4096 bytes com 2 registros por bloco.

#### BPTree1.hpp

Define a estrutura da árvore B+ a ser utilizada como arquivo de índice primário. A árvore terá um M = 170.

### Lista de funções

- Bloco.hpp

  | Função         | O que faz?                                                              | Autores                             |
|----------------|-------------------------------------------------------------------------|-------------------------------------|
| imprimeBloco() | Mostra o número de registros dentro do bloco e o conteúdo dos registros | Hugo, André Valente, André Hirosaki |

- BPTree1.hpp

| Função               | O que faz?                                                                    | Autores                             |
|----------------------|-------------------------------------------------------------------------------|-------------------------------------|
| lerNo()              | Abre o arquivo de índice, procura o nó e retorna o mesmo                      | Hugo, André Valente, André Hirosaki |
| escreverNo()         | Abre o arquivo de índice, procura o nó e escreve o mesmo                      | Hugo, André Valente, André Hirosaki |
| novaRaiz()           | Aloca um novo nó raiz e configura os ponteiros das subárvores                 | Hugo, André Valente, André Hirosaki |
| encontrarFolha()     | Função para encontrar o nó folha onde a chave deve ser inserida               | Hugo, André Valente, André Hirosaki |
| inserirNaArvore()    | Função para inserir chave na árvore e ajustar a árvore                        | Hugo, André Valente, André Hirosaki |
| dividirNoFolha()     | Caso dê um overflow em um nó folha, divide a mesma e configura os ponteiros   | Hugo, André Valente, André Hirosaki |
| inserirNoInterno()   | Função para inserir chave em nó interno                                       | Hugo, André Valente, André Hirosaki |
| dividirNoInterno()   | Caso dê um overflow em um nó interno, divide o mesmo e configura os ponteiros | Hugo, André Valente, André Hirosaki |
| buscarRecursivo()    | Função recursiva para buscar a chave                                          | Hugo, André Valente, André Hirosaki |
| inicializarArquivo() | Cria o arquivo de índice primário                                             | Hugo, André Valente, André Hirosaki |
| inserir()            | Interface para inserir uma chave na árvore                                    | Hugo, André Valente, André Hirosaki |
| buscaRegistro()      | Interface para buscar um registro na árvore                                   | Hugo, André Valente, André Hirosaki |
