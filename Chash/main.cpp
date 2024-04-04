//#include <iostream>
//#include <fstream>
//#include <vector>
//
//#pragma pack(push, 1) // 구조체 패딩 비활성화
//
//// BMP 파일 헤더 구조체
//struct BMPFileHeader {
//    char signature[2];
//    uint32_t fileSize;
//    uint16_t reserved1;
//    uint16_t reserved2;
//    uint32_t dataOffset;
//};
//
//// BMP 정보 헤더 구조체
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
//#pragma pack(pop) // 구조체 패딩 활성화
//
//// BMP 파일에서 텍스처 읽기 함수
//std::vector<std::vector<std::vector<unsigned char>>> readBMP(const std::string& filePath) {
//    std::ifstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "파일을 열 수 없습니다." << std::endl;
//        return {};
//    }
//
//    // BMP 파일 헤더 읽기
//    BMPFileHeader fileHeader;
//    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP 정보 헤더 읽기
//    BMPInfoHeader infoHeader;
//    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // 파일에서 이미지 데이터 읽기
//    file.seekg(fileHeader.dataOffset, std::ios::beg);
//
//    std::vector<std::vector<std::vector<unsigned char>>> pixels(
//        infoHeader.height, std::vector<std::vector<unsigned char>>(infoHeader.width, std::vector<unsigned char>(3)));
//
//    for (int y = infoHeader.height - 1; y >= 0; --y) {
//        for (int x = 0; x < infoHeader.width; ++x) {
//            file.read(reinterpret_cast<char*>(&pixels[y][x][2]), 1); // Blue 채널
//            file.read(reinterpret_cast<char*>(&pixels[y][x][1]), 1); // Green 채널
//            file.read(reinterpret_cast<char*>(&pixels[y][x][0]), 1); // Red 채널
//        }
//
//        // BMP 이미지의 각 행은 4의 배수 크기로 끝나므로 나머지 부분은 무시
//        file.seekg(infoHeader.width % 4, std::ios::cur);
//    }
//
//    file.close();
//
//    return pixels;
//}
//
//// 이미지를 그레이 스케일로 변환하는 함수
//void convertToGrayScale(std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    for (auto& row : pixels) {
//        for (auto& pixel : row) {
//            // RGB 값을 그레이 스케일로 변환
//            unsigned char gray = static_cast<unsigned char>(
//                0.299 * pixel[0] +
//                0.587 * pixel[1] +
//                0.114 * pixel[2]
//                );
//
//            // R, G, B 값을 그레이 스케일 값으로 설정
//            pixel[0] = gray;
//            pixel[1] = gray;
//            pixel[2] = gray;
//        }
//    }
//}
//
//// 그레이 스케일 이미지를 BMP 파일로 저장하는 함수
//void saveBMP(const std::string& filePath, const std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    std::ofstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "파일을 열 수 없습니다." << std::endl;
//        return;
//    }
//
//    // BMP 파일 헤더 작성
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
//    // BMP 정보 헤더 작성
//    BMPInfoHeader infoHeader;
//    infoHeader.headerSize = sizeof(BMPInfoHeader);
//    infoHeader.width = pixels[0].size();
//    infoHeader.height = pixels.size();
//    infoHeader.planes = 1;
//    infoHeader.bitsPerPixel = 24; // R, G, B 각각 8비트
//    infoHeader.compression = 0;
//    infoHeader.imageSize = 0;
//    infoHeader.xPixelsPerMeter = 0;
//    infoHeader.yPixelsPerMeter = 0;
//    infoHeader.totalColors = 0;
//    infoHeader.importantColors = 0;
//
//    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // 이미지 데이터 작성
//    for (int y = pixels.size() - 1; y >= 0; --y) {
//        for (int x = 0; x < pixels[0].size(); ++x) {
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][2]), 1); // Blue 채널
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][1]), 1); // Green 채널
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][0]), 1); // Red 채널
//        }
//
//        // BMP 이미지의 각 행은 4의 배수 크기로 끝나므로 나머지 부분은 0으로 채움
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
//    // 원본 BMP 텍스처 파일 경로 설정
//    std::string originalTextureFilePath = "원본텍스처.bmp";
//
//    // BMP 파일에서 텍스처 읽기
//    std::vector<std::vector<std::vector<unsigned char>>> pixels = readBMP(originalTextureFilePath);
//
//    // 이미지를 그레이 스케일로 변환
//    convertToGrayScale(pixels);
//
//    // 저장할 BMP 텍스처 파일 경로 설정
//    std::string savedTextureFilePath = "그레이스케일텍스처.bmp";
//
//    // 그레이 스케일 이미지를 BMP 파일로 저장
//    saveBMP(savedTextureFilePath, pixels);
//
//    std::cout << "그레이 스케일 이미지를 저장했습니다." << std::endl;
//
//    return 0;
//}


//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <immintrin.h>
//#include <cstring> 
//
//#pragma pack(push, 1) // 구조체 패딩 비활성화
//
//// BMP 파일 헤더 구조체
//struct BMPFileHeader {
//    char signature[2];
//    uint32_t fileSize;
//    uint16_t reserved1;
//    uint16_t reserved2;
//    uint32_t dataOffset;
//};
//
//// BMP 정보 헤더 구조체
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
//#pragma pack(pop) // 구조체 패딩 활성화
//
//// BMP 파일에서 텍스처 읽기 함수
//std::vector<std::vector<std::vector<unsigned char>>> readBMP(const std::string& filePath) {
//    std::ifstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "파일을 열 수 없습니다." << std::endl;
//        return {};
//    }
//
//    // BMP 파일 헤더 읽기
//    BMPFileHeader fileHeader;
//    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));
//
//    // BMP 정보 헤더 읽기
//    BMPInfoHeader infoHeader;
//    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // 파일에서 이미지 데이터 읽기
//    file.seekg(fileHeader.dataOffset, std::ios::beg);
//
//    std::vector<std::vector<std::vector<unsigned char>>> pixels(
//        infoHeader.height, std::vector<std::vector<unsigned char>>(infoHeader.width, std::vector<unsigned char>(3)));
//
//    for (int y = infoHeader.height - 1; y >= 0; --y) {
//        for (int x = 0; x < infoHeader.width; ++x) {
//            file.read(reinterpret_cast<char*>(&pixels[y][x][2]), 1); // Blue 채널
//            file.read(reinterpret_cast<char*>(&pixels[y][x][1]), 1); // Green 채널
//            file.read(reinterpret_cast<char*>(&pixels[y][x][0]), 1); // Red 채널
//        }
//
//        // BMP 이미지의 각 행은 4의 배수 크기로 끝나므로 나머지 부분은 무시
//        file.seekg(infoHeader.width % 4, std::ios::cur);
//    }
//
//    file.close();
//
//    return pixels;
//}
//
//// 이미지를 그레이 스케일로 변환하는 함수 (SIMD 사용)
//void convertToGrayScaleSIMD(std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    for (auto& row : pixels) {
//        for (auto& pixel : row) {
//            // RGB 값을 SIMD 레지스터에 로드
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
//            // RGB를 그레이 스케일로 변환하기 위한 가중치 생성
//            __m256i weights = _mm256_set_epi16(0, 0, 0, 299, 0, 587, 0, 114, 0, 0, 0, 0, 0, 0, 0, 0);
//
//            // 가중치를 사용하여 RGB 값을 곱하고 더함
//            __m256i result = _mm256_maddubs_epi16(rgb, weights);
//
//            // 결과를 8비트 오른쪽으로 시프트
//            result = _mm256_srli_epi16(result, 8);
//
//            // 그레이 스케일 값 추출
//            alignas(32) unsigned char gray[32];
//            _mm256_storeu_si256((__m256i*)gray, result);
//
//            // R, G, B 값을 그레이 스케일 값으로 설정
//            pixel[0] = gray[0] + gray[8] + gray[16]; // R
//            pixel[1] = gray[0] + gray[8] + gray[16]; // G
//            pixel[2] = gray[0] + gray[8] + gray[16]; // B
//        }
//    }
//}
//// 그레이 스케일 이미지를 BMP 파일로 저장하는 함수
//void saveBMP(const std::string& filePath, const std::vector<std::vector<std::vector<unsigned char>>>& pixels) {
//    std::ofstream file(filePath, std::ios::binary);
//    if (!file.is_open()) {
//        std::cerr << "파일을 열 수 없습니다." << std::endl;
//        return;
//    }
//
//    // BMP 파일 헤더 작성
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
//    // BMP 정보 헤더 작성
//    BMPInfoHeader infoHeader;
//    infoHeader.headerSize = sizeof(BMPInfoHeader);
//    infoHeader.width = pixels[0].size();
//    infoHeader.height = pixels.size();
//    infoHeader.planes = 1;
//    infoHeader.bitsPerPixel = 24; // R, G, B 각각 8비트
//    infoHeader.compression = 0;
//    infoHeader.imageSize = 0;
//    infoHeader.xPixelsPerMeter = 0;
//    infoHeader.yPixelsPerMeter = 0;
//    infoHeader.totalColors = 0;
//    infoHeader.importantColors = 0;
//
//    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(BMPInfoHeader));
//
//    // 이미지 데이터 작성
//    for (int y = pixels.size() - 1; y >= 0; --y) {
//        for (int x = 0; x < pixels[0].size(); ++x) {
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][2]), 1); // Blue 채널
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][1]), 1); // Green 채널
//            file.write(reinterpret_cast<const char*>(&pixels[y][x][0]), 1); // Red 채널
//        }
//
//        // BMP 이미지의 각 행은 4의 배수 크기로 끝나므로 나머지 부분은 0으로 채움
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
//    // 원본 BMP 텍스처 파일 경로 설정
//    std::string originalTextureFilePath = "원본텍스처.bmp";
//
//    // BMP 파일에서 텍스처 읽기
//    std::vector<std::vector<std::vector<unsigned char>>> pixels = readBMP(originalTextureFilePath);
//
//    // 이미지를 그레이 스케일로 변환 (SIMD 사용)
//    convertToGrayScaleSIMD(pixels);
//
//    // 저장할 BMP 텍스처 파일 경로 설정
//    std::string savedTextureFilePath = "그레이스케일텍스처_SIMD.bmp";
//
//    // 그레이 스케일 이미지를 BMP 파일로 저장
//    saveBMP(savedTextureFilePath, pixels);
//
//    std::cout << "그레이 스케일 이미지를 SIMD를 사용하여 저장했습니다." << std::endl;
//
//    return 0;
//}
//

#include <iostream>
#include <fstream>

int main() {
    // JPG 파일 읽기
    std::ifstream inFile("원본텍스처.jpg", std::ios::binary);

    if (!inFile) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return 1;
    }

    // SOI 마커 확인
    uint8_t marker1, marker2;
    inFile.read(reinterpret_cast<char*>(&marker1), 1);
    inFile.read(reinterpret_cast<char*>(&marker2), 1);

    if (marker1 == 0xFF && marker2 == 0xD8) {
        std::cout << "SOI (Start of Image) 마커가 발견되었습니다." << std::endl;
    }
    else {
        std::cerr << "JPG 파일이 아닌 것 같습니다." << std::endl;
        return 1;
    }

    // APPn 마커 확인 (예시로 APP0 마커만 확인)
    while (true) {
        inFile.read(reinterpret_cast<char*>(&marker1), 1);
        inFile.read(reinterpret_cast<char*>(&marker2), 1);

        if (marker1 == 0xFF && marker2 >= 0xE0 && marker2 <= 0xEF) {
            std::cout << "APPn (Application-specific) 마커가 발견되었습니다." << std::endl;
            break;
        }
        else if (marker1 != 0xFF) {
            std::cerr << "JPG 파일의 구조가 올바르지 않습니다." << std::endl;
            return 1;
        }
    }

    // 그레이 스케일 변환
    std::ofstream outFile("output_gray.jpg", std::ios::binary);

    uint8_t R, G, B;
    while (true) {
        inFile.read(reinterpret_cast<char*>(&R), 1);
        inFile.read(reinterpret_cast<char*>(&G), 1);
        inFile.read(reinterpret_cast<char*>(&B), 1);

        if (inFile.eof()) // 파일의 끝에 도달하면 종료
            break;

        // 그레이 스케일로 변환
        uint8_t gray = static_cast<uint8_t>(0.2126 * R + 0.7152 * G + 0.0722 * B);

        // 그레이 스케일 값으로 변환된 RGB 값을 쓰기
        outFile.write(reinterpret_cast<char*>(&gray), 1);
        outFile.write(reinterpret_cast<char*>(&gray), 1);
        outFile.write(reinterpret_cast<char*>(&gray), 1);
    }

    inFile.close();
    outFile.close();

    std::cout << "그레이 스케일 변환 완료. output_gray.jpg 파일에 저장되었습니다." << std::endl;

    return 0;
}
