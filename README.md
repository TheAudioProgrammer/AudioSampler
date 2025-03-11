# Tap Audio Sampler

This is a simple audio sampler created with JUCE.

## Building the Project with CMake

### Prerequisites
Ensure you have the following installed:
- **CMake** (minimum version required: 3.x)
- **C++ Compiler** (e.g., GCC, Clang, MSVC)
- **(Optional) Ninja or Make** (for build system generation)

### Build Instructions

1. **Clone the repository:**
2. **Navigate into the repository folder**
3. **Create a folder called build**
    ```sh 
    $ cmake -B build
    ```

    **Optional:** You can also invoke the `-G` flag to build project files for the IDE of your choice.  
    
    **XCode**
    ```sh
    $ cmake -B build -G "Xcode"
    ```

    **Visual Studio Code 2019**
    ```sh
    $ cmake -S . -B build -G "Visual Studio 16 2019"
    ```
    
    **Visual Studio Code 2022**
    ```sh
    $ cmake -S . -B build -G "Visual Studio 17 2022"
    ```

5. **Navigate into the `build` folder and open the project file**


