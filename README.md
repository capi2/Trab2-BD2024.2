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
- ```lib```:    pasta onde as estruturas de dados e classes serão utilizadas no trabalho.
- ```src```:    pasta onde as funções findrec, seek1, seek2 e upload estão localizadas.

### Estruturas e Decisões

O arquivo de dados feito em hash, fizemos 1 bloco por bucket pois facilita a implementação devido a dificuldade que tivemos durante o trabalho, outra coisa é que não foi implementado os buckets de overflow

### Arquivos

#### Bloco.hpp

Define a estrutura básica do bloco. O tamanho do bloco será de 4096 bytes com 2 registros por bloco.

#### BPTree1.hpp

Define a estrutura da árvore B+ a ser utilizada como arquivo de índice primário com um M = 170.

#### Hash.hpp

Define a classe da tabela Hash e seus métodos.

#### MetaDados.hpp

Define a estrutura dos metadados e a classe MetaDadosManager que fará o controle de leitura e escrita de arquivos.

#### Registro.hpp

Define a estrutura do registro.

#### upload.hpp

Define a classe Reader que fará a leitura do arquivo ```artigo.csv``` inicial.

### Lista de funções/ métodos

- Bloco.hpp

| Função       | O que faz?                                                                | Autores                               |
| -------------- | ------------------------------------------------------------------------- | ------------------------------------- |
| imprimeBloco() | Mostra o número de registros dentro do bloco e o conteúdo dos registros | Hugo, André Valente, André Hirosaki |

- BPTree1.hpp

| Função             | O que faz?                                                                      | Autores                               |
| -------------------- | ------------------------------------------------------------------------------- | ------------------------------------- |
| lerNo()              | Abre o arquivo de índice, procura o nó e retorna o mesmo                      | Hugo, André Valente, André Hirosaki |
| escreverNo()         | Abre o arquivo de índice, procura o nó e escreve o mesmo                      | Hugo, André Valente, André Hirosaki |
| novaRaiz()           | Aloca um novo nó raiz e configura os ponteiros das subárvores                 | Hugo, André Valente, André Hirosaki |
| encontrarFolha()     | Função para encontrar o nó folha onde a chave deve ser inserida              | Hugo, André Valente, André Hirosaki |
| inserirNaArvore()    | Função para inserir chave na árvore e ajustar a árvore                      | Hugo, André Valente, André Hirosaki |
| dividirNoFolha()     | Caso dê um overflow em um nó folha, divide a mesma e configura os ponteiros   | Hugo, André Valente, André Hirosaki |
| inserirNoInterno()   | Função para inserir chave em nó interno                                      | Hugo, André Valente, André Hirosaki |
| dividirNoInterno()   | Caso dê um overflow em um nó interno, divide o mesmo e configura os ponteiros | Hugo, André Valente, André Hirosaki |
| buscarRecursivo()    | Função recursiva para buscar a chave                                          | Hugo, André Valente, André Hirosaki |
| inicializarArquivo() | Cria o arquivo de índice primário                                             | Hugo, André Valente, André Hirosaki |
| inserir()            | Interface para inserir uma chave na árvore                                     | Hugo, André Valente, André Hirosaki |
| buscaRegistro()      | Interface para buscar um registro na árvore                                    | Hugo, André Valente, André Hirosaki |

- BPTree2.hpp

| Função             | O que faz?                                                                      | Autores                               |
| -------------------- | ------------------------------------------------------------------------------- | ------------------------------------- |
| lerNo()              | Abre o arquivo de índice, procura o nó e retorna o mesmo                      | Hugo, André Valente, André Hirosaki |
| escreverNo()         | Abre o arquivo de índice, procura o nó e escreve o mesmo                      | Hugo, André Valente, André Hirosaki |
| novaRaiz()           | Aloca um novo nó raiz e configura os ponteiros das subárvores                 | Hugo, André Valente, André Hirosaki |
| encontrarFolha()     | Função para encontrar o nó folha onde a chave deve ser inserida              | Hugo, André Valente, André Hirosaki |
| inserirNaArvore()    | Função para inserir chave na árvore e ajustar a árvore                      | Hugo, André Valente, André Hirosaki |
| dividirNoFolha()     | Caso dê um overflow em um nó folha, divide a mesma e configura os ponteiros   | Hugo, André Valente, André Hirosaki |
| inserirNoInterno()   | Função para inserir chave em nó interno                                      | Hugo, André Valente, André Hirosaki |
| dividirNoInterno()   | Caso dê um overflow em um nó interno, divide o mesmo e configura os ponteiros | Hugo, André Valente, André Hirosaki |
| buscarRecursivo()    | Função recursiva para buscar a chave                                          | Hugo, André Valente, André Hirosaki |
| inicializarArquivo() | Cria o arquivo de índice primário                                             | Hugo, André Valente, André Hirosaki |
| inserir()            | Interface para inserir uma chave na árvore                                     | Hugo, André Valente, André Hirosaki |
| buscaRegistro()      | Interface para buscar um registro na árvore                                    | Hugo, André Valente, André Hirosaki |

- Hash.hpp

| Método              | O que faz?                                                          | Autores                               |
| -------------------- | ------------------------------------------------------------------- | ------------------------------------- |
| funcaoHash()         | Função para espalhar os ids pela tabela                           | Hugo, André Valente, André Hirosaki |
| lerBloco()           | Procura um bloco pelo índice no arquivo de dados e retorna o bloco | Hugo, André Valente, André Hirosaki |
| escreverBloco()      | Abre o arquivo de dados e escreve o bloco no índice informado      | Hugo, André Valente, André Hirosaki |
| inicializarArquivo() | Inicializa o arquivo de dados                                       | Hugo, André Valente, André Hirosaki |
| inserir()            | Insere um registro em um bloco                                      | Hugo, André Valente, André Hirosaki |
| buscar()             | Busca um registro pelo seu id                                       | Hugo, André Valente, André Hirosaki |

- MetaDados.hpp

| Método              | O que faz?                                 | Autores                               |
| -------------------- | ------------------------------------------ | ------------------------------------- |
| lerMetaDados()       | Abre o arquivo de metadados e lê os dados | Hugo, André Valente, André Hirosaki |
| inicializarArquivo() | Cria o arquivo de metadados                | Hugo, André Valente, André Hirosaki |
| escreverMetaDados()  | Escreve os metadados no arquivo            | Hugo, André Valente, André Hirosaki |
| imprimeMetaDados()   | Mostra os metadados                        | Hugo, André Valente, André Hirosaki |

- Registro.hpp

| Método           | O que faz?                                          | Autores                               |
| ----------------- | --------------------------------------------------- | ------------------------------------- |
| criarRegistro()   | Recebe os parâmetros do registro e devolve o mesmo | Hugo, André Valente, André Hirosaki |
| sanitizeString()  | Transforma caracteres especiais da string em 0      | Hugo                                  |
| imprimeRegistro() | Mostra o conteúdo do registro                      | Hugo, André Valente, André Hirosaki |

- upload.hpp

| Método               | O que faz?                                                                                        | Autores |
| --------------------- | ------------------------------------------------------------------------------------------------- | ------- |
| extractTokens()       | Extrai os campos do registro                                                                      | Hugo    |
| obterTamanhoArquivo() | Vai para o final do arquivo, obtém a posição (em bytes) do final e volta ao início do arquivo | Hugo    |
| mostrarProgresso()    | Mostra uma barra de progresso do upload do arquivo```artigo.csv```                                | Hugo    |
| uploadCSV()           | Faz o upload do```artigo.csv``` através do path do mesmo                                         | Hugo    |

