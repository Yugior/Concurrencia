// Horacio Villela Hernandez A01712206 23/05/2025
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <chrono>  

using namespace std;

// Función que convierte un píxel RGB a escala de grises
// Fórmula estándar: Gray = 0.3*R + 0.59*G + 0.11*B
unsigned char toGray(unsigned char r, unsigned char g, unsigned char b) {
    return static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);
}

int main() {
    // Medimos el tiempo de inicio
    auto start = chrono::high_resolution_clock::now();

    int width, height, channels;

    // Cargar la imagen original (3 canales: RGB)
    unsigned char* img = stbi_load("input.jpg", &width, &height, &channels, 3);

    if (img == nullptr) {
        cerr << "Error al cargar la imagen.\n";
        return 1;
    }

    // Calculamos el tamaño total en bytes de la imagen
    size_t imgSize = width * height * 3;

    // Creamos una nueva imagen para el resultado
    unsigned char* grayImg = new unsigned char[imgSize];

    // Recorremos cada píxel (secuencialmente)
    for (int i = 0; i < width * height; i++) {
        int index = i * 3; // Cada píxel tiene 3 bytes: R, G, B

        unsigned char r = img[index];
        unsigned char g = img[index + 1];
        unsigned char b = img[index + 2];

        // Convertimos a escala de grises
        unsigned char gray = toGray(r, g, b);

        // Asignamos el mismo valor a R, G, B (escala de grises)
        grayImg[index]     = gray;
        grayImg[index + 1] = gray;
        grayImg[index + 2] = gray;
    }

    // Guardamos la imagen resultante como JPG
    if (!stbi_write_jpg("output_seq.jpg", width, height, 3, grayImg, 100)) {
        cerr << "Error al guardar la imagen procesada.\n";
        return 1;
    }

    // Liberamos memoria
    stbi_image_free(img);
    delete[] grayImg;

    // Medimos el tiempo de finalización
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;

    cout << "Procesamiento secuencial completado en " << duration.count() << " ms\n";

    return 0;
}
