#include <iostream>
#include <thread>
#include <vector>
#include <filesystem>
#include <atomic>

namespace fs = std::filesystem;

// ����������� ������� ���������� ������
const int MAX_THREADS = 8;

// ������� ������ �����
void searchFile(const fs::path& directory, const std::string& targetFile, std::atomic<bool>& found)
{
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (fs::is_directory(entry))
        {
            // ��������� ����� ���� ��� ������ � ���������
            std::thread([&]() {
                searchFile(entry.path(), targetFile, found);
                }).detach();
        }
        else if (entry.path().filename() == targetFile)
        {
            // ����������� �����
            std::cout << "Found file: " << entry.path() << std::endl;

            // �������� ���� ������ ��� ����������� �����
            found = true;
        }

        if (found)
        {
            // ���� ���� ��������, ��������� ����� � ����� �������
            break;
        }
    }
}

int main()
{
    std::string targetFile; // ������� ����, ���� ������� ������
    std::cout << "Enter the file name to search for: ";
    std::cin >> targetFile;

    std::atomic<bool> found = false;

    // �������� ����� � ���������� ��������
    fs::path rootPath = "/"; // ��� "C:\\" �� Windows

    // ��������� ����� ��� ������ � ���������� ��������
    std::thread([&]() {
        searchFile(rootPath, targetFile, found);
        }).join();

        return 0;
}
