#include <iostream>
#include <vector>
#include <conio.h>
#include <SFML/Graphics.hpp>

using namespace std;

class CuadradoLatino {
private:
    int n;
    int inicio;
    char operacion;
    int valor;
    vector<vector<int>> matriz;
    vector<int> sumaFilas;
    vector<int> sumaColumnas;

public:
    CuadradoLatino(int tam, int ini, char op, int val) : n(tam), inicio(ini), operacion(op), valor(val) {
        matriz.resize(n, vector<int>(n));
        sumaFilas.resize(n, 0);
        sumaColumnas.resize(n, 0);
        generarCuadrado();
    }

    void generarCuadrado() {
        vector<int> filaBase(n);
        filaBase[0] = inicio;
        for (int i = 1; i < n; i++) {
            switch (operacion) {
                case '+': filaBase[i] = filaBase[i - 1] + valor; break;
                case '-': filaBase[i] = filaBase[i - 1] - valor; break;
                case '*': filaBase[i] = filaBase[i - 1] * valor; break;
            }
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                matriz[i][j] = filaBase[(j + i) % n];
                sumaFilas[i] += matriz[i][j];
                sumaColumnas[j] += matriz[i][j];
            }
        }
    }

    void mostrarEnConsola() {
        for (const auto& fila : matriz) {
            for (int num : fila) {
                cout << num << " \t";
            }
            cout << endl;
        }
    }

    void dibujarEnSFML() {
        const int cellSize = 50;
        const int offset = 50;
        sf::RenderWindow window(sf::VideoMode((n + 2) * cellSize, (n + 2) * cellSize), "Cuadrado Latino");
        sf::Font font;
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::White);

            // Dibujar la matriz
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                    cell.setPosition(j * cellSize + offset, i * cellSize + offset);
                    cell.setOutlineColor(sf::Color::Black);
                    cell.setOutlineThickness(2);
                    window.draw(cell);

                    sf::Text text;
                    text.setFont(font);
                    text.setString(to_string(matriz[i][j]));
                    text.setCharacterSize(20);
                    text.setFillColor(sf::Color::Black);
                    text.setPosition(j * cellSize + 15 + offset, i * cellSize + 10 + offset);
                    window.draw(text);
                }
            }

            // Dibujar las sumas de las filas (a la derecha de la matriz)
            for (int i = 0; i < n; i++) {
                sf::Text textFila;
                textFila.setFont(font);
                textFila.setString(to_string(sumaFilas[i]));
                textFila.setCharacterSize(20);
                textFila.setFillColor(sf::Color::Red);
                textFila.setPosition(n * cellSize + 15 + offset, i * cellSize + 10 + offset);
                window.draw(textFila);
            }

            // Dibujar las sumas de las columnas (debajo de la matriz)
            for (int j = 0; j < n; j++) {
                sf::Text textColumna;
                textColumna.setFont(font);
                textColumna.setString(to_string(sumaColumnas[j]));
                textColumna.setCharacterSize(20);
                textColumna.setFillColor(sf::Color::Red);
                textColumna.setPosition(j * cellSize + 15 + offset, n * cellSize + 10 + offset);
                window.draw(textColumna);
            }

            window.display();
        }
    }
};

void menu() {
    int opcion = 0;
    bool ejecutando = true;
    while (ejecutando) {
        system("cls");
        cout << "\n\t\tCuadrado Latino\n\n";
        cout << (opcion == 0 ? "--> " : "    ") << "Resolver cuadrado latino\n";
        cout << (opcion == 1 ? "--> " : "    ") << "Salir\n";

        char tecla = _getch();
        if (tecla == 72 && opcion > 0) opcion--; // Flecha arriba
        if (tecla == 80 && opcion < 1) opcion++; // Flecha abajo
        if (tecla == 13) { // Enter
            if (opcion == 1) ejecutando = false;
            else {
                int tam, ini, val;
                char op;
                cout << "Ingrese el tamano del cuadrado latino: ";
                cin >> tam;
                cout << "Ingrese el numero inicial: ";
                cin >> ini;
                do {
                    cout << "Ingrese la sucesion (+ num, - num, * num): ";
                    cin >> op >> val;
                } while (op != '+' && op != '-' && op != '*');
                
                CuadradoLatino cl(tam, ini, op, val);
                cl.mostrarEnConsola();
                cl.dibujarEnSFML();
            }
        }
    }
}

int main() {
    menu();
    return 0;
}