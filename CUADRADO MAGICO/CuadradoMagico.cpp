#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Función para generar un cuadrado mágico de orden impar (método siamés)
vector<vector<int>> generateOddMagicSquare(int n, int base) {
    vector<vector<int>> magic(n, vector<int>(n, 0));
    int i = 0;
    int j = n / 2;
    int num = base;
    
    for (int k = 0; k < n * n; k++){
        magic[i][j] = num;
        num++;
        int new_i = (i - 1 + n) % n;  // sube una fila (con "envase")
        int new_j = (j + 1) % n;      // avanza una columna (con "envase")
        
        // Si la celda destino ya está ocupada, se coloca el número directamente debajo
        if (magic[new_i][new_j] != 0) {
            i = (i + 1) % n;
        } else {
            i = new_i;
            j = new_j;
        }
    }
    return magic;
}

// Función para generar cuadrados mágicos de orden doblemente par (n divisible por 4)
vector<vector<int>> generateDoublyEvenMagicSquare(int n, int base) {
    vector<vector<int>> magic(n, vector<int>(n, 0));
    int total = n * n;
    
    // Llenar la matriz con números consecutivos empezando en "base"
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            magic[i][j] = base + i * n + j;
        }
    }
    
    // Invertir ciertos números según el patrón para cuadrados doblemente pares
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (((i % 4 == 0) || (i % 4 == 3)) && ((j % 4 == 0) || (j % 4 == 3))) {
                magic[i][j] = base + total - 1 - (magic[i][j] - base);
            } else if (((i % 4 == 1) || (i % 4 == 2)) && ((j % 4 == 1) || (j % 4 == 2))) {
                magic[i][j] = base + total - 1 - (magic[i][j] - base);
            }
        }
    }
    return magic;
}

int main() {
    int n, base;
    cout << "Ingrese el orden del cuadrado magico (n x n): ";
    cin >> n;
    cout << "Ingrese el numero base: ";
    cin >> base;
    
    vector<vector<int>> magic;
    
    // Seleccionar el método según el orden
    if (n % 2 == 1) {
        magic = generateOddMagicSquare(n, base);
    } else if (n % 4 == 0) {
        magic = generateDoublyEvenMagicSquare(n, base);
    } else {
        cout << "El metodo para cuadrados magicos de orden par (n % 4 == 2) no esta implementado." << endl;
        return 1;
    }
    
    // Calcular las sumas de filas, columnas y diagonales
    vector<int> rowSums(n, 0), colSums(n, 0);
    int diag1 = 0, diag2 = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            rowSums[i] += magic[i][j];
            colSums[j] += magic[i][j];
            if (i == j)
                diag1 += magic[i][j];
            if (i + j == n - 1)
                diag2 += magic[i][j];
        }
    }
    
    // Configuración de la ventana gráfica con margen para mostrar los textos fuera del cuadrado
    const int cellSize = 80;          // Tamaño de cada celda en píxeles
    const int margin = 100;           // Margen para los textos externos
    const int windowWidth = n * cellSize + 2 * margin;
    const int windowHeight = n * cellSize + 2 * margin;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Cuadrado Magico");
    
    // Cargar la fuente (modifica la ruta según tu sistema)
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/Ebrima.ttf")) {
        cerr << "Error cargando fuente\n";
        return -1;
    }
    
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::White);
        
        // Dibujar la cuadrícula y los números del cuadrado mágico (con offset de margen)
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                // Dibujar celda (rectángulo)
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(margin + j * cellSize, margin + i * cellSize);
                cell.setFillColor(sf::Color::Transparent);
                cell.setOutlineThickness(2);
                cell.setOutlineColor(sf::Color::Black);
                window.draw(cell);
                
                // Crear texto con el número de la celda
                sf::Text text;
                text.setFont(font);
                text.setString(to_string(magic[i][j]));
                text.setCharacterSize(24);
                text.setFillColor(sf::Color::Blue);
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                text.setPosition(margin + j * cellSize + cellSize / 2.0f, margin + i * cellSize + cellSize / 2.0f);
                window.draw(text);
            }
        }
        
        // Dibujar las sumas de cada fila (a la derecha de la cuadrícula)
        for (int i = 0; i < n; i++){
            sf::Text text;
            text.setFont(font);
            text.setString("R:" + to_string(rowSums[i]));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Red);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            // Posición: a la derecha de la cuadrícula
            text.setPosition(margin + n * cellSize + 30, margin + i * cellSize + cellSize / 2.0f);
            window.draw(text);
        }
        
        // Dibujar las sumas de cada columna (debajo de la cuadrícula)
        for (int j = 0; j < n; j++){
            sf::Text text;
            text.setFont(font);
            text.setString("C:" + to_string(colSums[j]));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Red);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            // Posición: debajo de la cuadrícula
            text.setPosition(margin + j * cellSize + cellSize / 2.0f, margin + n * cellSize + 30);
            window.draw(text);
        }
        
        // Dibujar la suma de la diagonal principal (top-left fuera de la cuadrícula)
        sf::Text diag1Text;
        diag1Text.setFont(font);
        diag1Text.setString("Diag1: " + to_string(diag1));
        diag1Text.setCharacterSize(20);
        diag1Text.setFillColor(sf::Color::Magenta);
        sf::FloatRect d1Rect = diag1Text.getLocalBounds();
        diag1Text.setOrigin(d1Rect.left + d1Rect.width/2.0f, d1Rect.top + d1Rect.height/2.0f);
        // Posición: arriba y a la izquierda de la cuadrícula
        diag1Text.setPosition(margin - 50, margin - 30);
        window.draw(diag1Text);
        
        // Dibujar la suma de la diagonal secundaria (top-right fuera de la cuadrícula)
        sf::Text diag2Text;
        diag2Text.setFont(font);
        diag2Text.setString("Diag2: " + to_string(diag2));
        diag2Text.setCharacterSize(20);
        diag2Text.setFillColor(sf::Color::Magenta);
        sf::FloatRect d2Rect = diag2Text.getLocalBounds();
        diag2Text.setOrigin(d2Rect.left + d2Rect.width/2.0f, d2Rect.top + d2Rect.height/2.0f);
        // Posición: arriba y a la derecha de la cuadrícula
        diag2Text.setPosition(margin + n * cellSize + 50, margin - 30);
        window.draw(diag2Text);
        
        window.display();
    }
    
    return 0;
}
