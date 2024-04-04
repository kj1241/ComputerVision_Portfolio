//#include <iostream>
//#include <fstream>
//#include <vector>
//
//#pragma pack(push, 1) // ����ü �е� ��Ȱ��ȭ
//
//// BMP ���� ��� ����ü
//struct BMPFileHeader {
//    char signature[2];
//    uint32_t fileSize;
//    uint16_t reserved1;
//    uint16_t reserved2;
//    uint32_t dataOffset;
//};
//
//// BMP ���� ��� ����ü
//struct BMPInfoHeader {
//    uint32_t headerSize;
//    int32_t width;
//    int32_t height;
//    uint16_t planes;
//    uint16_t bitsPerPixel;
//    uint32_t compression;
//    uint32_t imageSize;
//    int32_t xPixelsPerMeter;
//    int32_t yPixelsPerMeter;
//    uint32_t totalColors;
//    uint32_t importantColors;
//};
//
//#pragma pack(pop) // ����ü �е� Ȱ��ȭ
//
//// BMP ���Ͽ��� �ؽ�ó �б� �Լ�
//std::vector<std::vector<std::vector<unsigned char>>> readBMP(const std::string& filePath) {
//    std::ifstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "������ �� �� �����ϴ�." << std::endl;
//        return {};
//    }
//
//    // BMP ���� ��� �б�
//    BMPFileHeader fileHeader;
//    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP ���� ��� �б�
//    BMPInfoHeader infoHeader;
//    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // ���Ͽ��� �̹��� ������ �б�
//    file.seekg(fileHeader.dataOffset, std::ios::beg);
//
//    std::vector<std::vector<std::vector<unsigned char>>> pixels(
//        infoHeader.height, std::vector<std::vector<unsigned char>>(infoHeader.width, std::vector<unsigned char>(3)));
//
//    for (int y = infoHeader.height - 1; y >= 0; --y) {
//        for (int x = 0; x < infoHeader.width; ++x) {
//            file.read(reinterpret_cast<char*>(&pixels[y][x][2]), 1); // Blue ä��
//            file.read(reinterpret_cast<char*>(&pixels[y][x][1]), 1); // Green ä��
//            file.read(reinterpret_cast<char*>(&pixels[y][x][0]), 1); // Red ä��
//        }
//
//        // BMP �̹����� �� ���� 4�� ��� ũ��� �����Ƿ� ������ �κ��� ����
//        file.seekg(infoHeader.width % 4, std::ios::cur);
//    }
//
//    file.close();
//
//    return pixels;
//}
//
//// �̹����� �׷��� �����Ϸ� ��ȯ�ϴ� �Լ�
//void convertToGrayScale(std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    for (auto& row : pixels) {
//        for (auto& pixel : row) {
//            // RGB ���� �׷��� �����Ϸ� ��ȯ
//            unsigned char gray = static_cast<unsigned char>(
//                0.299 * pixel[0] +
//                0.587 * pixel[1] +
//                0.114 * pixel[2]
//                );
//
//            // R, G, B ���� �׷��� ������ ������ ����
//            pixel[0] = gray;
//            pixel[1] = gray;
//            pixel[2] = gray;
//        }
//    }
//}
//
//// �׷��� ������ �̹����� BMP ���Ϸ� �����ϴ� �Լ�
//void saveBMP(const std::string& filePath, const std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    std::ofstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "������ �� �� �����ϴ�." << std::endl;
//        return;
//    }
//
//    // BMP ���� ��� �ۼ�
//    BMPFileHeader fileHeader;
//    fileHeader.signature[0] = 'B';
//    fileHeader.signature[1] = 'M';
//    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + pixels.size() * (pixels[0].size() * 3 + pixels[0].size() % 4);
//    fileHeader.reserved1 = 0;
//    fileHeader.reserved2 = 0;
//    fileHeader.dataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
//
//    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP ���� ��� �ۼ�
//    BMPInfoHeader infoHeader;
//    infoHeader.headerSize = sizeof(BMPInfoHeader);
//    infoHeader.width = pixels[0].size();
//    infoHeader.height = pixels.size();
//    infoHeader.planes = 1;
//    infoHeader.bitsPerPixel = 24; // R, G, B ���� 8��Ʈ
//    infoHeader.compression = 0;
//    infoHeader.imageSize = 0;
//    infoHeader.xPixelsPerMeter = 0;
//    infoHeader.yPixelsPerMeter = 0;
//    infoHeader.totalColors = 0;
//    infoHeader.importantColors = 0;
//
//    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // �̹��� ������ �ۼ�
//    for (int y = pixels.size() - 1; y >= 0; --y) {
//        for (int x = 0; x < pixels[0].size(); ++x) {
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][2]), 1); // Blue ä��
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][1]), 1); // Green ä��
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][0]), 1); // Red ä��
//        }
//
//        // BMP �̹����� �� ���� 4�� ��� ũ��� �����Ƿ� ������ �κ��� 0���� ä��
//        for (int i = 0; i < pixels[0].size() % 4; ++i) {
//            char padding = 0;
//            file.write(&padding, 1);
//        }
//    }
//
//    file.close();
//}
//
//int main() {
//    // ���� BMP �ؽ�ó ���� ��� ����
//    std::string originalTextureFilePath = "�����ؽ�ó.bmp";
//
//    // BMP ���Ͽ��� �ؽ�ó �б�
//    std::vector<std::vector<std::vector<unsigned char>>> pixels = readBMP(originalTextureFilePath);
//
//    // �̹����� �׷��� �����Ϸ� ��ȯ
//    convertToGrayScale(pixels);
//
//    // ������ BMP �ؽ�ó ���� ��� ����
//    std::string savedTextureFilePath = "�׷��̽������ؽ�ó.bmp";
//
//    // �׷��� ������ �̹����� BMP ���Ϸ� ����
//    saveBMP(savedTextureFilePath, pixels);
//
//    std::cout << "�׷��� ������ �̹����� �����߽��ϴ�." << std::endl;
//
//    return 0;
//}


//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <immintrin.h>
//#include <cstring> 
//
//#pragma pack(push, 1) // ����ü �е� ��Ȱ��ȭ
//
//// BMP ���� ��� ����ü
//struct BMPFileHeader {
//    char signature[2];
//    uint32_t fileSize;
//    uint16_t reserved1;
//    uint16_t reserved2;
//    uint32_t dataOffset;
//};
//
//// BMP ���� ��� ����ü
//struct BMPInfoHeader {
//    uint32_t headerSize;
//    int32_t width;
//    int32_t height;
//    uint16_t planes;
//    uint16_t bitsPerPixel;
//    uint32_t compression;
//    uint32_t imageSize;
//    int32_t xPixelsPerMeter;
//    int32_t yPixelsPerMeter;
//    uint32_t totalColors;
//    uint32_t importantColors;
//};
//
//#pragma pack(pop) // ����ü �е� Ȱ��ȭ
//
//// BMP ���Ͽ��� �ؽ�ó �б� �Լ�
//std::vector<std::vector<std::vector<unsigned char>>> readBMP(const std::string& filePath) {
//    std::ifstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "������ �� �� �����ϴ�." << std::endl;
//        return {};
//    }
//
//    // BMP ���� ��� �б�
//    BMPFileHeader fileHeader;
//    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP ���� ��� �б�
//    BMPInfoHeader infoHeader;
//    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // ���Ͽ��� �̹��� ������ �б�
//    file.seekg(fileHeader.dataOffset, std::ios::beg);
//
//    std::vector<std::vector<std::vector<unsigned char>>> pixels(
//        infoHeader.height, std::vector<std::vector<unsigned char>>(infoHeader.width, std::vector<unsigned char>(3)));
//
//    for (int y = infoHeader.height - 1; y >= 0; --y) {
//        for (int x = 0; x < infoHeader.width; ++x) {
//            file.read(reinterpret_cast<char*>(&pixels[y][x][2]), 1); // Blue ä��
//            file.read(reinterpret_cast<char*>(&pixels[y][x][1]), 1); // Green ä��
//            file.read(reinterpret_cast<char*>(&pixels[y][x][0]), 1); // Red ä��
//        }
//
//        // BMP �̹����� �� ���� 4�� ��� ũ��� �����Ƿ� ������ �κ��� ����
//        file.seekg(infoHeader.width % 4, std::ios::cur);
//    }
//
//    file.close();
//
//    return pixels;
//}
//
//// �̹����� �׷��� �����Ϸ� ��ȯ�ϴ� �Լ� (SIMD ���)
//void convertToGrayScaleSIMD(std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    for (auto& row : pixels) {
//        for (auto& pixel : row) {
//            // RGB ���� SIMD �������Ϳ� �ε�
//            __m256i rgb = _mm256_set_epi8(
//                0, 0, 0, 0,
//                0, 0, 0, pixel[2],
//                0, 0, 0, 0,
//                0, 0, 0, pixel[1],
//                0, 0, 0, 0,
//                0, 0, 0, pixel[0],
//                0, 0, 0, 0,
//                0, 0, 0, 0
//            );
//
//            // RGB�� �׷��� �����Ϸ� ��ȯ�ϱ� ���� ����ġ ����
//            __m256i weights = _mm256_set_epi16(0, 0, 0, 299, 0, 587, 0, 114, 0, 0, 0, 0, 0, 0, 0, 0);
//
//            // ����ġ�� ����Ͽ� RGB ���� ���ϰ� ����
//            __m256i result = _mm256_maddubs_epi16(rgb, weights);
//
//            // ����� 8��Ʈ ���������� ����Ʈ
//            result = _mm256_srli_epi16(result, 8);
//
//            // �׷��� ������ �� ����
//            alignas(32) unsigned char gray[32];
//            _mm256_storeu_si256((__m256i*)gray, result);
//
//            // R, G, B ���� �׷��� ������ ������ ����
//            pixel[0] = gray[0] + gray[8] + gray[16]; // R
//            pixel[1] = gray[0] + gray[8] + gray[16]; // G
//            pixel[2] = gray[0] + gray[8] + gray[16]; // B
//        }
//    }
//}
//// �׷��� ������ �̹����� BMP ���Ϸ� �����ϴ� �Լ�
//void saveBMP(const std::string& filePath, const std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    std::ofstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "������ �� �� �����ϴ�." << std::endl;
//        return;
//    }
//
//    // BMP ���� ��� �ۼ�
//    BMPFileHeader fileHeader;
//    fileHeader.signature[0] = 'B';
//    fileHeader.signature[1] = 'M';
//    fileHeader.fileSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + pixels.size() * (pixels[0].size() * 3 + pixels[0].size() % 4);
//    fileHeader.reserved1 = 0;
//    fileHeader.reserved2 = 0;
//    fileHeader.dataOffset = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
//
//    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP ���� ��� �ۼ�
//    BMPInfoHeader infoHeader;
//    infoHeader.headerSize = sizeof(BMPInfoHeader);
//    infoHeader.width = pixels[0].size();
//    infoHeader.height = pixels.size();
//    infoHeader.planes = 1;
//    infoHeader.bitsPerPixel = 24; // R, G, B ���� 8��Ʈ
//    infoHeader.compression = 0;
//    infoHeader.imageSize = 0;
//    infoHeader.xPixelsPerMeter = 0;
//    infoHeader.yPixelsPerMeter = 0;
//    infoHeader.totalColors = 0;
//    infoHeader.importantColors = 0;
//
//    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // �̹��� ������ �ۼ�
//    for (int y = pixels.size() - 1; y >= 0; --y) {
//        for (int x = 0; x < pixels[0].size(); ++x) {
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][2]), 1); // Blue ä��
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][1]), 1); // Green ä��
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][0]), 1); // Red ä��
//        }
//
//        // BMP �̹����� �� ���� 4�� ��� ũ��� �����Ƿ� ������ �κ��� 0���� ä��
//        for (int i = 0; i < pixels[0].size() % 4; ++i) {
//            char padding = 0;
//            file.write(&padding, 1);
//        }
//    }
//
//    file.close();
//}
//
//int main() {
//    // ���� BMP �ؽ�ó ���� ��� ����
//    std::string originalTextureFilePath = "�����ؽ�ó.bmp";
//
//    // BMP ���Ͽ��� �ؽ�ó �б�
//    std::vector<std::vector<std::vector<unsigned char>>> pixels = readBMP(originalTextureFilePath);
//
//    // �̹����� �׷��� �����Ϸ� ��ȯ (SIMD ���)
//    convertToGrayScaleSIMD(pixels);
//
//    // ������ BMP �ؽ�ó ���� ��� ����
//    std::string savedTextureFilePath = "�׷��̽������ؽ�ó_SIMD.bmp";
//
//    // �׷��� ������ �̹����� BMP ���Ϸ� ����
//    saveBMP(savedTextureFilePath, pixels);
//
//    std::cout << "�׷��� ������ �̹����� SIMD�� ����Ͽ� �����߽��ϴ�." << std::endl;
//
//    return 0;
//}
//

#include <iostream>
#include <fstream>

int main() {
    // JPG ���� �б�
    std::ifstream inFile("�����ؽ�ó.jpg", std::ios::binary);

    if (!inFile) {
        std::cerr << "������ �� �� �����ϴ�." << std::endl;
        return 1;
    }

    // SOI ��Ŀ Ȯ��
    uint8_t marker1, marker2;
    inFile.read(reinterpret_cast<char*>(&marker1), 1);
    inFile.read(reinterpret_cast<char*>(&marker2), 1);

    if (marker1 == 0xFF && marker2 == 0xD8) {
        std::cout << "SOI (Start of Image) ��Ŀ�� �߰ߵǾ����ϴ�." << std::endl;
    }
    else {
        std::cerr << "JPG ������ �ƴ� �� �����ϴ�." << std::endl;
        return 1;
    }

    // APPn ��Ŀ Ȯ�� (���÷� APP0 ��Ŀ�� Ȯ��)
    while (true) {
        inFile.read(reinterpret_cast<char*>(&marker1), 1);
        inFile.read(reinterpret_cast<char*>(&marker2), 1);

        if (marker1 == 0xFF && marker2 >= 0xE0 && marker2 <= 0xEF) {
            std::cout << "APPn (Application-specific) ��Ŀ�� �߰ߵǾ����ϴ�." << std::endl;
            break;
        }
        else if (marker1 != 0xFF) {
            std::cerr << "JPG ������ ������ �ùٸ��� �ʽ��ϴ�." << std::endl;
            return 1;
        }
    }

    // �׷��� ������ ��ȯ
    std::ofstream outFile("output_gray.jpg", std::ios::binary);

    uint8_t R, G, B;
    while (true) {
        inFile.read(reinterpret_cast<char*>(&R), 1);
        inFile.read(reinterpret_cast<char*>(&G), 1);
        inFile.read(reinterpret_cast<char*>(&B), 1);

        if (inFile.eof()) // ������ ���� �����ϸ� ����
            break;

        // �׷��� �����Ϸ� ��ȯ
        uint8_t gray = static_cast<uint8_t>(0.2126 * R + 0.7152 * G + 0.0722 * B);

        // �׷��� ������ ������ ��ȯ�� RGB ���� ����
        outFile.write(reinterpret_cast<char*>(&gray), 1);
        outFile.write(reinterpret_cast<char*>(&gray), 1);
        outFile.write(reinterpret_cast<char*>(&gray), 1);
    }

    inFile.close();
    outFile.close();

    std::cout << "�׷��� ������ ��ȯ �Ϸ�. output_gray.jpg ���Ͽ� ����Ǿ����ϴ�." << std::endl;

    return 0;
}
