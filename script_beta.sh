#!/bin/bash

compilar() {
    g++ main.cpp -o main
}

# Listas de valores
#valores de beta para o SA
lista1=(0.999 0.995 0.975)

#valores de temperatura inicial para o SA
lista2=(500 750 1000)


# Nomes dos arquivos de cada instância

instancias=("i01.txt" "i02.txt" "i03.txt" "i04.txt" "i05.txt")
COUNTER=0
teste() {
for l1 in ${lista1[@]}; do

    for l2 in ${lista2[@]}; do

            for arquivo in ${instancias[@]}; do

              let COUNTER++
              # echo "$l1 $l2 $arquivo"

              ./main $l1 $l2 $arquivo

            done

    done

done
echo $COUNTER   
}
compilar
teste

# Executa o programa em C++ com os valores e arquivo da instância atual

