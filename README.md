# KX Map Converter

<p align="center">
  <img src="data/images/kxconverter_logo.png" alt="KX Converter Logo">
</p>

**From the creators of KX Tools, KX Map Converter is a high-performance command-line utility for converting legacy Guild Wars 2 KX-Maps `.txt` files to a clean, modern `.json` format. Explore all our projects at [kxtools.xyz](https://kxtools.xyz).**

<p align="center">
  <a href="https://github.com/kxtools/kx-converter/releases"><img src="https://img.shields.io/github/v/release/kxtools/kx-converter?style=for-the-badge&logo=github" alt="Latest Release"></a>
  <a href="https://github.com/kxtools/kx-converter/blob/main/LICENSE"><img src="https://img.shields.io/github/license/kxtools/kx-converter?style=for-the-badge" alt="License"></a>
  <img src="https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=cplusplus" alt="C++17">
</p>

This tool is designed for developers, data managers, and route creators who need to modernize old map data into a structured, easily parsable format for use in applications like **[KX Map Studio](https://github.com/kxtools/kx-map-studio)** or other custom tools.

---

## What is KX Map Converter?

For years, Guild Wars 2 map routes were stored in an inconsistent `.txt` format, with swapped Y/Z coordinate values.

**KX Map Converter** is a high-performance C++ utility that solves this problem. It reads legacy `.txt` files, intelligently parses each line to extract checkpoint data, and converts it into a clean, well-structured `.json` file. It's built to handle the messy, real-world data found in user-generated files.

---

## Key Features

*   ⚡️ **High-Performance C++:** Built for speed and efficiency, capable of processing thousands of files in seconds.
*   🧠 **Robust & Intelligent Parsing:** Handles a wide variety of formatting inconsistencies, including:
    *   Variable whitespace (tabs, multiple spaces).
    *   Comma-separated or space-separated coordinates.
    *   Names with or without quotes, containing spaces, apostrophes, and other special characters.
    *   Coordinates in scientific e-notation.
*   📂 **Batch Processing:** Convert an entire folder of `.txt` files with a single command.
*   🔄 **Recursive Search:** Optionally scan through all subdirectories for `.txt` files to convert.
*   📝 **Author Metadata:** Embed an author's name directly into the output JSON files.
*   ✨ **Clean JSON Output:** Generates human-readable and machine-parsable JSON, perfect for use in web apps, overlays, or other developer tools.
*   🛡️ **Graceful Error Handling:** Identifies and reports parsing errors on a per-line basis, skipping problematic files to ensure the batch process can complete.
*   **User-Friendly Progress Bar:** A clean progress bar shows the status of the conversion process for large directories.

---

## Getting Started

### Using the Executable

1.  **Download:** Grab the latest `kx-converter.exe` from the [GitHub Releases page](https://github.com/kxtools/kx-converter/releases).
2.  **Run from Command Line:** Open a terminal (Command Prompt, PowerShell, or Windows Terminal) and navigate to the folder where you extracted the files.
    ```powershell
    cd C:\Tools\KXConverter
    ```
3.  **Start Converting!** Run `kx-converter.exe` with the required input and output paths.

### Usage

**Basic Conversion**

This will convert all `.txt` files in the `C:\my-old-maps` folder and place the new `.json` files in `C:\my-new-maps`.

```bash
.\kx-converter.exe -i "C:\my-old-maps" -o "C:\my-new-maps" --author "MyName"
```

**Recursive Conversion**

This will search `C:\my-old-maps` and all of its subfolders for `.txt` files, preserving the directory structure in the output folder.

```bash
.\kx-converter.exe -i "C:\my-old-maps" -o "C:\my-new-maps" --author "MyName" --recursive
```

---

### Command-Line Options

| Flag              | Alias | Description                                                  | Required |
| :---------------- | :---- | :----------------------------------------------------------- | :------- |
| `--input <path>`  | `-i`  | Path to the input directory containing `.txt` files.         | **Yes**  |
| `--output <path>` | `-o`  | Path to the output directory for converted `.json` files.    | **Yes**  |
| `--author <name>` |       | Name of the author to add to the JSON file.                  | No       |
| `--recursive`     |       | Process the input directory recursively.                     | No       |
| `--overwrite`     |       | Allow overwriting existing files in the output directory.    | No       |
| `--verbose`       | `-v`  | Enable detailed logging output for debugging.                | No       |

---

## Example Conversion

The converter transforms messy `.txt` lines into clean JSON objects.

**Input (`some_map.txt`)**

```
1	'Start Point' 100.5 200.0 -350.75
2   Checkpoint_2  110.0, 225.3, -340.0
3 'A longer name with spaces' 120.8 -250.1 -330.9
```

**Output (`some_map.json`)**

```json
{
  "Author": "MyName",
  "Coordinates": [
    {
      "Name": "Start Point",
      "X": 100.5,
      "Y": -350.75,
      "Z": 200.0
    },
    {
      "Name": "Checkpoint_2",
      "X": 110.0,
      "Y": -340.0,
      "Z": 225.3
    },
    {
      "Name": "A longer name with spaces",
      "X": 120.8,
      "Y": -330.9,
      "Z": -250.1
    }
  ],
  "Name": "some_map"
}
```

> **Note:** The `Y` and `Z` coordinates are intentionally swapped in the output JSON.

---

## Community & Support

Join our community to ask questions, give feedback, or just hang out!

*   🌐 **Our Website:** Explore our other projects at [kxtools.xyz](https://kxtools.xyz)
*   💬 **Discord:** Connect with us and other users: [https://discord.gg/z92rnB4kHm](https://discord.gg/z92rnB4kHm)
*   🐙 **GitHub:** Report issues, suggest features, or contribute code on the [Issues Page](https://github.com/kxtools/kx-converter/issues).

---

## For Developers (Building from Source)

1.  **Requirements:**
    *   A C++17 compliant compiler (e.g., MSVC, GCC, Clang).
    *   [CMake](https://cmake.org/download/) (version 3.14 or higher).
2.  **Clone the Repository:**
    ```bash
    git clone https://github.com/kxtools/kx-converter.git
    cd kx-converter
    ```
3.  **Configure and Build with CMake:**
    ```bash
    # Create a build directory
    cmake -S . -B build

    # Build the project
    cmake --build build --config Release
    ```
4.  **Run:** The executable will be located in the `build/Release` (or `build/`) directory.

---

## Contributing

We welcome contributions from the community!

*   **Report Bugs:** Found a file that doesn't parse correctly? Please open an issue on our [GitHub Issues page](https://github.com/kxtools/kx-converter/issues) and provide the problematic `.txt` file or line if possible.
*   **Suggest Features:** Have an idea for a new feature or an improvement? Open an issue to discuss it!
*   **Code Contributions:** If you'd like to contribute code, please open an issue first to discuss your planned changes. This helps avoid duplicate work and ensures alignment with the project's goals.

---

## Acknowledgements

This project wouldn't be possible without these incredible open-source libraries:

*   **[CLI11](https://github.com/CLIUtils/CLI11):** For powerful and easy-to-use command-line parsing.
*   **[spdlog](https://github.com/gabime/spdlog):** For fast and flexible logging.
*   **[nlohmann/json](https://github.com/nlohmann/json):** For making JSON in C++ a breeze.
*   **[Indicators](https://github.com/p-ranav/indicators):** For the clean and modern progress bars.
*   **[GoogleTest](https://github.com/google/googletest):** For robust unit testing.

## Star History

<p align="center">
  <a href="https://star-history.com/#kxtools/kx-converter&Date">
    <img src="https://api.star-history.com/svg?repos=kxtools/kx-converter&type=Date" alt="Star History Chart">
  </a>
</p>
