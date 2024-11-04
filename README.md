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


## Lista de funções e métodos

| a | b     | c     |
|---|-------|-------|
| 1 | asdas | 34345 |
| 3 | gfebt | 1432  |

- ```imprimeBloco(const Bloco& bloco)```: Mostra o número de registros dentro do bloco e o conteúdo dos registros. 
