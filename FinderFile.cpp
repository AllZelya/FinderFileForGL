#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>
#include <atomic>

namespace fs = std::filesystem;

// Максимальна кількість одночасних потоків
const int MAX_THREADS = 8;

// Функція пошуку файлу
void searchFile(const fs::path& directory, const std::string& targetFile, std::atomic<bool>& found)
{
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (fs::is_directory(entry))
        {
            // Створюємо новий потік для пошуку в підкаталозі
            std::thread([&]() {
                searchFile(entry.path(), targetFile, found);
                }).detach();
        }
        else if (entry.path().filename() == targetFile)
        {
            // Знаходження файлу
            std::cout << "Found file: " << entry.path() << std::endl;

            // Сповіщаємо інші потоки про знаходження файлу
            found = true;
        }

        if (found)
        {
            // Якщо файл знайдено, зупиняємо пошук в інших потоках
            break;
        }
    }
}

int main()
{
    std::string targetFile; // Заданий файл, який потрібно знайти
    std::cout << "Enter the file name to search for: ";
    std::cin >> targetFile;

    std::atomic<bool> found = false;

    // Починаємо пошук з кореневого каталогу
    fs::path rootPath = "/"; // або "C:\\" на Windows

    // Створюємо поток для пошуку з кореневого каталогу
    std::thread([&]() {
        searchFile(rootPath, targetFile, found);
        }).join();

        return 0;
}
