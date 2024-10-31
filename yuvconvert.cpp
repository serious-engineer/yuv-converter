#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>  // For std::stoi and std::exit

// Function to convert NV12 to NV21
void convertNV12toNV21(const std::string& inputFile, const std::string& outputFile, int width, int height, int numFrames) {
    std::ifstream input(inputFile, std::ios::binary | std::ios::ate);
    if (!input) {
        std::cerr << "Error: Could not open input file." << std::endl;
        std::exit(1);
    }

    int frameSize = width * height;
    int chromaSize = frameSize / 2;
    int totalFrameSize = frameSize + chromaSize;
    std::streamsize fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    int maxFramesInFile = static_cast<int>(fileSize / totalFrameSize);

    if (numFrames > maxFramesInFile) {
        std::cerr << "Warning: Requested number of frames (" << numFrames << ") exceeds available frames in the file (" << maxFramesInFile << ")." << std::endl;
        std::cerr << "Capping the number of frames to " << maxFramesInFile << "." << std::endl;
        numFrames = maxFramesInFile;
    }

    char* yPlane = new char[frameSize];
    char* uvPlane = new char[chromaSize];
    char* vuPlane = new char[chromaSize];

    for (int frame = 0; frame < numFrames; ++frame) {
        if (!input.read(yPlane, frameSize)) {
            std::cerr << "Error: Could not read Y plane for frame " << frame + 1 << "." << std::endl;
            break;
        }
        if (!input.read(uvPlane, chromaSize)) {
            std::cerr << "Error: Could not read UV plane for frame " << frame + 1 << "." << std::endl;
            break;
        }

        for (int i = 0; i < chromaSize / 2; i++) {
            vuPlane[2 * i] = uvPlane[2 * i + 1];
            vuPlane[2 * i + 1] = uvPlane[2 * i];
        }

        std::ofstream output(outputFile, std::ios::binary | std::ios::app);
        if (!output) {
            std::cerr << "Error: Could not open output file." << std::endl;
            std::exit(1);
        }
        output.write(yPlane, frameSize);
        output.write(vuPlane, chromaSize);
        output.close();
    }

    delete[] yPlane;
    delete[] uvPlane;
    delete[] vuPlane;
    input.close();
}

// Function to convert NV12 to NV24
void convertNV12toNV24(const std::string& inputFile, const std::string& outputFile, int width, int height, int numFrames) {
    std::ifstream input(inputFile, std::ios::binary | std::ios::ate);
    if (!input) {
        std::cerr << "Error: Could not open input file." << std::endl;
        std::exit(1);
    }

    int frameSize = width * height;
    int chromaSizeNV12 = frameSize / 2;
    int chromaSizeNV24 = frameSize * 2; // NV24 has full chroma data for both U and V planes
    int totalFrameSizeNV12 = frameSize + chromaSizeNV12;

    std::streamsize fileSize = input.tellg();
    input.seekg(0, std::ios::beg);

    int maxFramesInFile = static_cast<int>(fileSize / totalFrameSizeNV12);

    if (numFrames > maxFramesInFile) {
        std::cerr << "Warning: Requested number of frames (" << numFrames << ") exceeds available frames in the file (" << maxFramesInFile << ")." << std::endl;
        std::cerr << "Capping the number of frames to " << maxFramesInFile << "." << std::endl;
        numFrames = maxFramesInFile;
    }

    char* yPlane = new char[frameSize];
    char* uvPlaneNV12 = new char[chromaSizeNV12];
    char* uvPlaneNV24 = new char[chromaSizeNV24];

    for (int frame = 0; frame < numFrames; ++frame) {
        if (!input.read(yPlane, frameSize)) {
            std::cerr << "Error: Could not read Y plane for frame " << frame + 1 << "." << std::endl;
            break;
        }
        if (!input.read(uvPlaneNV12, chromaSizeNV12)) {
            std::cerr << "Error: Could not read UV plane for frame " << frame + 1 << "." << std::endl;
            break;
        }

        // Expand NV12 chroma plane to NV24 format
        for (int j = 0; j < height; j += 2) {
            for (int i = 0; i < width; i += 2) {
                int srcIndex = (j / 2) * width + (i / 2) * 2;
                char u = uvPlaneNV12[srcIndex];
                char v = uvPlaneNV12[srcIndex + 1];

                int dstIndex = j * width + i;
                uvPlaneNV24[dstIndex] = u;                     // U component for (i, j)
                uvPlaneNV24[frameSize + dstIndex] = v;         // V component for (i, j)

                uvPlaneNV24[dstIndex + 1] = u;                 // U component for (i+1, j)
                uvPlaneNV24[frameSize + dstIndex + 1] = v;     // V component for (i+1, j)

                uvPlaneNV24[dstIndex + width] = u;             // U component for (i, j+1)
                uvPlaneNV24[frameSize + dstIndex + width] = v; // V component for (i, j+1)

                uvPlaneNV24[dstIndex + width + 1] = u;         // U component for (i+1, j+1)
                uvPlaneNV24[frameSize + dstIndex + width + 1] = v; // V component for (i+1, j+1)
            }
        }

        // Write Y and expanded UV planes to the output file
        std::ofstream output(outputFile, std::ios::binary | std::ios::app);
        if (!output) {
            std::cerr << "Error: Could not open output file." << std::endl;
            std::exit(1);
        }
        output.write(yPlane, frameSize);
        output.write(uvPlaneNV24, chromaSizeNV24);
        output.close();
    }

    delete[] yPlane;
    delete[] uvPlaneNV12;
    delete[] uvPlaneNV24;

    input.close();
}




int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file> <width> <height> <num_frames> <output format (nv21 or nv24)>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int width = std::stoi(argv[3]);
    int height = std::stoi(argv[4]);
    int numFrames = std::stoi(argv[5]);
    std::string format = argv[6];

    if (format == "nv21") {
        convertNV12toNV21(inputFile, outputFile, width, height, numFrames);
    }
    else if (format == "nv24") {
        convertNV12toNV24(inputFile, outputFile, width, height, numFrames);
    }
    else {
        std::cerr << "Error: Unsupported output format. Use 'nv21' or 'nv24'." << std::endl;
        return 1;
    }

    return 0;
}
