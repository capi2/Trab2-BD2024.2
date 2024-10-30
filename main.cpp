#include <iostream>
#include "src/upload.hpp"
#include "src/seek1.hpp"
#include "src/seek2.hpp"
#include "src/findrec.hpp"

#define CSV_PATH "data/artigomenor.csv"
#define COMPLETE_CSV_PATH "data/artigo.csv"
 
int main() {
    // Fazer o menu
    Reader::uploadCSV(COMPLETE_CSV_PATH);

    return 0;
}