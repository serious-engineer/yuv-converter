A Simple C++ Exec which can convert any yuv files from NV12 Color format to NV21,NV24 or any other colorformat. 

YUV Converter
This project is a C++ application that converts YUV video files between various formats, such as NV12, NV21, and NV24. The application reads raw YUV video data and provides options for conversion between these formats, supporting customizable resolution and frame count for efficient processing.

Table of Contents
Overview
Supported Formats
Features
Installation
Usage
Examples
Contributing
License
Overview
YUV Converter is designed for developers and media engineers who need to work with raw YUV video data. This tool reads NV12, NV21, and converts these to other YUV formats, allowing for greater flexibility in processing and analysis.

Supported Formats
The current version supports:

NV12 to NV21
NV12 to NV24
Other formats and conversions may be added in future updates.

Features
Frame Limiting: Specify the number of frames to convert.
Automatic Frame Capping: The app will automatically cap the frame count to match the number of available frames in the file, if requested frames exceed the total frames.
Error Handling: Robust error handling for file I/O, format mismatches, and other conversion-related issues.

Installation
Clone the Repository:

git clone https://github.com/yourusername/yuv-converter.git
Build the Project in VS

Dependencies: Ensure C++14 is enabled and installed with the necessary standard library.

Usage
To use the YUV converter, run the compiled executable with the following syntax:

yuvconverter <input_file> <output_file> <width> <height> <num_frames> <output_format>
<input_file>: Path to the raw YUV input file.
<output_file>: Path for the output YUV file.
<width>: Width of the input video.
<height>: Height of the input video.
<num_frames>: Number of frames to convert.
<output_format>: Specify output format: NV21 or NV24.
Example
Convert sample_nv12.yuv to output_nv21.yuv in NV21 format, with a resolution of 1920x1080 and 30 frames:

yuvconverter sample_nv12.yuv output_nv21.yuv 1920 1080 30 NV21
Examples
Convert NV12 to NV24

yuvconverter sample_nv12.yuv output_nv24.yuv 1280 720 15 NV24
Run with FFplay
To test your output YUV file, you can use FFplay:


ffplay -f rawvideo -pixel_format nv24 -video_size 1280x720 output_nv24.yuv
Contributing
Feel free to open issues and submit pull requests. Please make sure your code adheres to the coding style of the project and includes comments for clarity.

License
This project is licensed under the MIT License - see the LICENSE file for details.

