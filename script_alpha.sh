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
                    for((k=1;k<=5;k++))
                    do
                        echo "instance: i0"$j".txt"               
                        echo "alpha equals 0"
                        rodar 0 "i0"$j".txt"
                        sleep 1
                    done
                    ;;
                2)
                    for((k=1;k<=5;k++))
                    do
                        echo "instance: i0"$j".txt"               
                        echo "alpha equals 0.2"
                        rodar 0.2 "i0"$j".txt"
                        sleep 1
                    done
                    ;;
                3)
                    for((k=1;k<=5;k++))
                    do
                        echo "instance: i0"$j".txt"               
                        echo "alpha equals 0.5"
                        rodar 0.5 "i0"$j".txt"
                        sleep 1
                    done
                    ;;
                4)
                    for((k=1;k<=5;k++))
                    do
                        echo "instance: i0"$j".txt"               
                        echo "alpha equals 0.8"
                        rodar 0.8 "i0"$j".txt"
                        sleep 1
                    done
                    ;;
                5)
                    for((k=1;k<=5;k++))
                    do                
                        echo "instance: i0"$j".txt"               
                        echo "alpha equals 1"
                        rodar 1 "i0"$j".txt"
                        sleep 1
                    done
                    ;;
                *) echo default
                ;;
            esac
        done
    done
}

executar
