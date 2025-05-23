// Horacio Villela Hernandez A01712206 23/05/2025
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

/**
 * Aplica un filtro de escala de grises a una parte de la imagen.
 * 
 * - Complejidad: O(n) donde n = (end_row - start_row) * width
 *   ya que se recorre cada píxel una sola vez en esa región.
 */
void grayscale_filter(unsigned char* img, int width, int height, int channels, int start_row, int end_row) {
    int stride = width * channels; // Bytes por fila

    // Recorremos fila por fila dentro del rango asignado
    for (int y = start_row; y < end_row; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = y * stride + x * channels;
            unsigned char r = img[i];
            unsigned char g = img[i + 1];
            unsigned char b = img[i + 2];

            // Operación constante por píxel → O(1)
            unsigned char gray = static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);

            img[i] = img[i + 1] = img[i + 2] = gray;
        }
    }
}

int main() {
    const char* input_file = "input.jpg";     
    const char* output_file = "output.jpg";

    int width, height, channels;

    // Cargar imagen → O(1) para este contexto (depende del tamaño pero lo manejamos como entrada constante)
    unsigned char* img = stbi_load(input_file, &width, &height, &channels, 0);
    if (!img) {
        std::cerr << "Error al cargar la imagen.\n";
        return 1;
    }

    std::cout << "Imagen cargada: " << width << "x" << height << " (" << channels << " canales)\n";

    // Obtener núcleos disponibles → O(1)
    int thread_count = std::thread::hardware_concurrency();
    if (thread_count == 0) thread_count = 4;

    std::vector<std::thread> threads;
    int rows_per_thread = height / thread_count;

    auto start_time = std::chrono::high_resolution_clock::now();

    /**
     * Lanzamos hilos → O(t), donde t = thread_count
     * Cada hilo trabaja de forma independiente sobre O(n/t) píxeles,
     * por lo tanto, el procesamiento global sigue siendo O(n).
     */
    for (int t = 0; t < thread_count; ++t) {
        int start_row = t * rows_per_thread;
        int end_row = (t == thread_count - 1) ? height : start_row + rows_per_thread;

        threads.emplace_back(grayscale_filter, img, width, height, channels, start_row, end_row);
    }

    // Esperamos que todos los hilos terminen → O(t)
    for (auto& th : threads) th.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Procesamiento completado en " << elapsed.count() << " segundos.\n";

    // Guardar imagen procesada → O(n) ya que se recorre toda la imagen
    if (stbi_write_jpg(output_file, width, height, channels, img, 100)) {
        std::cout << "Imagen guardada como: " << output_file << "\n";
    } else {
        std::cerr << "Error al guardar la imagen.\n";
    }

    
    stbi_image_free(img);
    return 0;
}


