# scrollwheelfix-NG

A small plugin which allows zooming via scroll wheel during object animations

Using CommonlibSSE-NG and therefor
supports SSE and SAE

## Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/) (_the free Community
  edition is fine!_)
- [CMake](https://cmake.org/download/) 3.25.1+ (_please install the Latest
  Release_)
- [`vcpkg`](https://github.com/microsoft/vcpkg)
  - 1. Clone the repository using git OR [download it as a
    .zip](https://github.com/microsoft/vcpkg/archive/refs/heads/master.zip)
  - 2. Go into the `vcpkg` folder and double-click on `bootstrap-vcpkg.bat`
  - 3. Edit your system or user Environment Variables and add a new one:
    - Name: `VCPKG_ROOT`  
      Value: `C:\path\to\wherever\your\vcpkg\folder\is`
  - 4. Alternatively to modifiyng your system path:
    open a VS CMD prompt and enter: `set VCPKG_ROOT=C:\path\to\vcpkg` followed by `devenv`

Once you have Visual Studio 2022 installed, you can open this folder in
basically any C++ editor, e.g. [VS Code](https://code.visualstudio.com/) or
[CLion](https://www.jetbrains.com/clion/) or [Visual
Studio](https://visualstudio.microsoft.com/)
- > _for VS Code, if you are not automatically prompted to install the
  [C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
and [CMake
Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
extensions, please install those and then close VS Code and then open this
project as a folder in VS Code_

You may need to click `OK` on a few windows, but the project should
automatically run CMake!

