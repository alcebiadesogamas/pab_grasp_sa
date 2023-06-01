#!/bin/bash

compilar() {
    g++ main.cpp -o main
}

rodar() {
    local alpha=$1;
    local instancia=$2;
    ./main "$alpha" "$instancia"
}

executar() {
    compilar
    for((j=1; j <=5; j++))
    do
        for((i=1;i<=5; i++))
        do 
            case "$i" in
                1) 
                    echo "instance: i0"$j".txt"               
                    echo "alpha equals 0"
                    rodar 0 "i0"$j".txt"
                    ;;
                2)
                    echo "instance: i0"$j".txt"               
                    echo "alpha equals 0.2"
                    rodar 0.2 "i0"$j".txt"
                    ;;
                3)
                    echo "instance: i0"$j".txt"               
                    echo "alpha equals 0.5"
                    rodar 0.5 "i0"$j".txt"
                    ;;
                4)
                    echo "instance: i0"$j".txt"               
                    echo "alpha equals 0.8"
                    rodar 0.8 "i0"$j".txt"
                    ;;
                5)
                    echo "instance: i0"$j".txt"               
                    echo "alpha equals 1"
                    rodar 1 "i0"$j".txt"
                    ;;
                *) echo default
                ;;
            esac
        done
    done
}

executar
