// Código generado por mi compilador
#include <iostream>
#include <string>
#include <vector>

int factorial(int n) {
    if (n < 0) {
        return (-1);
    } else if (n == 0) {
        return 1;
    } else {
        return (n * factorial((n - 1)));
    }
}


int main() {
    std::string SEPARADOR;
    std::string MENSAJE_INICIAL;
    SEPARADOR = "--------------------";
    MENSAJE_INICIAL = "Iniciando demostracion del lenguaje...";
    std::cout << MENSAJE_INICIAL << std::endl;
    std::cout << SEPARADOR << std::endl;
    std::cout << "Bucle FOR del 0 al 9, pero se salta el 3 y se detiene en 7:" << std::endl;
    for (int i = 0; (i < 10); (i = (i + 1))) {
        if (i == 3) {
            continue;
        }
        if (i > 6) {
            break;
        }
        std::cout << i << std::endl;
    }
    std::cout << SEPARADOR << std::endl;
    int acumulador = 100;
    acumulador = (acumulador + 50);
    std::cout << "Prueba de '+=', valor esperado 150:" << std::endl;
    std::cout << acumulador << std::endl;
    std::cout << SEPARADOR << std::endl;
    std::cout << "Prueba de 'unless':" << std::endl;
    if (!(acumulador == 100)) {
        std::cout << "Correcto, 'unless' se ejecuto porque el acumulador NO es 100." << std::endl;
    }
    std::cout << SEPARADOR << std::endl;
    std::cout << "Escribe 'salir' para terminar el bucle while." << std::endl;
    std::string entrada = "";
    while (entrada != "salir") {
        std::cout << "Ingresa un comando:" << std::endl;
        std::cin >> entrada;
        std::cout << ("Comando recibido: " + entrada) << std::endl;
    }
    std::cout << SEPARADOR << std::endl;
    std::cout << "Prueba del Operador de Tuberia con factorial de 6:" << std::endl;
    std::cout << factorial(6) << std::endl;
    std::cout << SEPARADOR << std::endl;
    std::cout << "¡Demostracion completada!" << std::endl;
    return 0;
}
