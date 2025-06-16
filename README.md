# TallerFunda# **Compilador para un Lenguaje de Programación Propio(Frankenstein)**

Este proyecto es un compilador completo desarrollado como parte del taller de Compiladores. 
El programa traduce un lenguaje de programación propio y de alto nivel a código C++ ejecutable. 
El proceso de compilación abarca el análisis léxico, sintáctico, semántico y la generación de código.

## ** Características del Lenguaje**

Nuestro lenguaje está diseñado para ser simple pero potente, incluyendo las siguientes características requeridas y adicionales:

* **Declaración de Variables**: Soporte para tipos `int`, `float`, `string` y `bool`.
* **Operadores**:
    * **Aritméticos**: `+`, `-`, `*`, `/`.
    * **De Asignación**: `=`, `+=`, `-=`, `*=`, `/=`.
    * **De Comparación y Lógicos**: `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`.
* **Estructuras de Control**: 
    * Condicionales `if`, `else if`, `else` y `unless`.
    * Bucles `while` y `for`.
    * Control de bucles `break` y `continue`.
* **Funciones**: Soporte para la definición de funciones con parámetros, valores de retorno (`return`) y ámbito (scope) local.
                 El lenguaje también soporta **recursividad**, permitiendo que una función se llame a sí misma.
* **Entrada/Salida**: Funciones `print()` para mostrar datos en consola y `read()` para leer datos desde el teclado.
* **Característica Avanzada - Operador Pipe (`|>`):** Permite encadenar el resultado de una expresión como el primer argumento de una llamada a función.

---

## **🛠️ Tecnologías Utilizadas**

* **Análisis Léxico**: `Flex`
* **Análisis Sintáctico**: `Bison`
* **Lenguaje de Implementación**: `C++` (Estándar C++17)
* **Entorno de Ejecución**: `Linux`

---

## **📁 Estructura del Proyecto**

El compilador está organizado en módulos, cada uno responsable de una fase específica del proceso:

* `lexer.l`: Contiene las reglas de **Flex** para el análisis léxico y la generación de tokens.
* `parser.y`: Contiene las reglas gramaticales de **Bison**, las acciones para construir el AST y el manejo de errores sintácticos.
* `Ast.h / Ast.cpp`: Definen la estructura de datos para cada nodo del **Árbol de Sintaxis Abstracta (AST)** y las funciones para crearlos.
* `symbol_table.h / symbol_table.cpp`: Implementan la **Tabla de Símbolos**, que gestiona identificadores (variables y funciones), sus tipos y sus ámbitos (scopes).
* `type_checker.h / type_checker.cpp`: Realiza el **análisis semántico**, recorriendo el AST para verificar la consistencia de tipos y el uso correcto de los identificadores.
* `code_generator.h / code_generator.cpp`: Es responsable de la **generación de código**, traduciendo el AST validado a código C++.
* `common_types.h`: Define tipos de datos comunes (`NodeType`, `SymbolBasicType`) para evitar dependencias circulares entre los módulos.

---

## **🚀 Cómo Compilar y Ejecutar**

1.  **Requisitos**: Asegúrate de tener `flex`, `bison` y un compilador de C++ como `g++` instalados.

2.  **Compilar el Compilador**: Abre una terminal en el directorio del proyecto y ejecuta los siguientes comandos en orden:

    ```bash
    # 1. Generar el parser con Bison (crea parser.tab.c y parser.tab.h)
    bison -d parser.y
    
    # 2. Generar el lexer con Flex (crea lex.yy.c)
    flex lexer.l
    
    # 3. Compilar todos los archivos .cpp y .c usando el estándar C++17
    g++ -std=c++17 parser.tab.c lex.yy.c Ast.cpp symbol_table.cpp type_checker.cpp code_generator.cpp -o Compilador -lfl
    ```

3.  **Ejecutar el Compilador**: Para compilar un archivo de tu lenguaje (p. ej., `entrada.txt`), usa el siguiente comando:

    ```bash
    ./Compilador < entrada.txt
    ```
    Esto generará un archivo `salida.cpp` con el código C++ traducido.

4.  **Compilar y Ejecutar el Código Generado**:

    ```bash
    g++ -o mi_programa salida.cpp
    ./mi_programa
    ```

---

## **⚙️ Flujo de Compilación: ¿Cómo Funciona?**

El proceso de compilación se divide en las siguientes etapas secuenciales:

1.  **Fase 1: Análisis Léxico**
    * El código fuente es procesado por el analizador léxico generado por **Flex** (`lexer.l`).
    * El analizador lee el texto y lo descompone en una secuencia de *tokens* (ej: `IF`, `IDENTIFIER`, `NUMBER`, `PLUS`).

2.  **Fase 2: Análisis Sintáctico y Construcción del AST**
    * La secuencia de tokens se pasa al analizador sintáctico generado por **Bison** (`parser.y`).
    * Este analizador verifica que los tokens sigan el orden y la estructura definidos por la gramática del lenguaje. Si la sintaxis es correcta, se construye en memoria un **Árbol de Sintaxis Abstracta (AST)**.

3.  **Fase 3: Análisis Semántico**
    * Con el AST ya construido, el **verificador de tipos** (`type_checker.cpp`) lo recorre de forma recursiva.
    * Durante el recorrido, utiliza la **tabla de símbolos** para verificar reglas semánticas como la correcta declaración de variables, la compatibilidad de tipos y el uso adecuado de funciones.

4.  **Fase 4: Generación de Código**
    * Si el análisis semántico finaliza sin errores, el **generador de código** (`code_generator.cpp`) recorre el AST validado y traduce cada nodo a su equivalente en código C++, escribiéndolo en un archivo de salida (`salida.cpp`).

---

## **Prueba completa del las Funcionalidades que tiene el codigo**

// Este programa calcula el factorial de una serie de números
// y demuestra varias características del lenguaje.

// 1. Declaración Múlti-variables
string separador, mensaje_inicial;
separador = "--------------------";
mensaje_inicial = "Iniciando demostracion del lenguaje...";

// 2. Definición de una Función Recursiva (Factorial)
int factorial(int n) {
    // 3. Sentencia IF / ELSE IF / ELSE
    if (n < 0) {
        return -1; // Factorial no definido para negativos
    } else if (n == 0) {
        return 1;
    } else {
        return n * factorial(n - 1); // Llamada recursiva
    }
}

// -- El código de aquí en adelante se generara dentro de main() --

print(mensaje_inicial);
print(separador);

// 4. Bucle FOR con declaración de variable interna, BREAK y CONTINUE
print("Bucle FOR del 0 al 9, pero se salta el 3 y se detiene en 7:");
for (int i = 0; i < 10; i = i + 1) {
    if (i == 3) {
        continue; // 5. Salta la iteración actual
    }
    if (i > 6) {
        break;    // 6. Sale del bucle prematuramente
    }
    print(i);
}

print(separador);

// 7. Operadores de Asignacion Compuesta (+=)
int acumulador = 100;
acumulador += 50; // Ahora acumulador es 150
print("Prueba de '+=', valor esperado 150:");
print(acumulador);

print(separador);

// 8. Sentencia UNLESS
print("Prueba de 'unless':");
unless (acumulador == 100) {
    print("Correcto, 'unless' se ejecuto porque el acumulador NO es 100.");
}

print(separador);

// 9. Bucle WHILE y Lectura desde Teclado (READ)
print("Escribe 'salir' para terminar el bucle while.");
string entrada = "";
while (entrada != "salir") {
    print("Ingresa un comando:");
    read(entrada);
    print("Comando recibido: " + entrada);
}

print(separador);

// 10. Operador de Tuberia (|>)
print("Prueba del Operador de Tuberia con factorial de 6:");
6 |> factorial |> print; // => print(factorial(6))

print(separador);
print("¡Demostracion completada!");

## **✒️ Autores**

Este proyecto fue desarrollado por:

* Benjamín Guillermo Andres Garces Zarate                                                   
* Pablo Andres Jorquera Herrera