// Código generado por mi compilador
#include <iostream>
#include <string>
#include <vector>

int suma(int x, int y) {
    return (x + y);
}

int resta(int x, int y) {
    return (x - y);
}

int multiplicacion(int x, int y) {
    return (x * y);
}

float division(float x, float y) {
    return (x / y);
}

void exponente(int y, int x) {
    if (x == 0) {
        std::cout << 1 << std::endl;
    }
    int res = y;
    while (x > 1) {
        res = (res * y);
        x = (x - 1);
    }
    std::cout << "Resultado: " << std::endl;
    std::cout << res << std::endl;
}


int main() {
    std::string respuesta = "";
    std::string operacion = "";
    int valor_1 = 0;
    int valor_2 = 0;
    while (respuesta != "-1") {
        std::cout << "---CALCULADORA SIMPLE---" << std::endl;
        std::cout << "Ingrese la operacion deseada: " << std::endl;
        std::cout << "-Suma (Escriba: +)" << std::endl;
        std::cout << "-Resta (Escriba: -)" << std::endl;
        std::cout << "-Multiplicacion (Escriba: *)" << std::endl;
        std::cout << "-Division(Escriba: /)" << std::endl;
        std::cout << "-Potencia (Escriba: **)" << std::endl;
        std::cin >> operacion;
        if (operacion == "+") {
            std::cout << "Ingresa el primer numero: " << std::endl;
            std::cin >> valor_1;
            std::cout << "Ingresa el segundo numero: " << std::endl;
            std::cin >> valor_2;
            std::cout << "Resultado: " << std::endl;
            std::cout << suma(valor_1, valor_2) << std::endl;
        } else if (operacion == "-") {
            std::cout << "Ingresa el primer numero: " << std::endl;
            std::cin >> valor_1;
            std::cout << "Ingresa el segundo numero: " << std::endl;
            std::cin >> valor_2;
            std::cout << "Resultado: " << std::endl;
            std::cout << resta(valor_1, valor_2) << std::endl;
        } else if (operacion == "*") {
            std::cout << "Ingresa el primer numero: " << std::endl;
            std::cin >> valor_1;
            std::cout << "Ingresa el segundo numero: " << std::endl;
            std::cin >> valor_2;
            std::cout << "Resultado: " << std::endl;
            std::cout << multiplicacion(valor_1, valor_2) << std::endl;
        } else if (operacion == "/") {
            std::cout << "Ingresa el dividendo: " << std::endl;
            std::cin >> valor_1;
            std::cout << "Ingresa el divisor: " << std::endl;
            std::cin >> valor_2;
            std::cout << "Resultado: " << std::endl;
            std::cout << division(valor_1, valor_2) << std::endl;
        } else if (operacion == "**") {
            std::cout << "Ingresa la base: " << std::endl;
            std::cin >> valor_1;
            std::cout << "Ingresa el exponente: " << std::endl;
            std::cin >> valor_2;
            exponente(valor_1, valor_2);
        } else {
            std::cout << ("Operador incorrecto: " + operacion) << std::endl;
            continue;
        }
        std::cout << "¿Desea hacer otra operacion?(Escriba -1 si quiere salir)" << std::endl;
        std::cin >> respuesta;
    }
    return 0;
}
