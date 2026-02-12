# FrameFlow

> ⚠️ **Version Status:** `v0.1.0-alpha`
> This is the **first public release** of FrameFlow and is **not yet stable**. APIs, features, and UI may change in future versions.

FrameFlow is an **open-source Qt C++ application** for creating multimedia outputs (videos, GIFs, image sprites) from image sequences. It is designed for developers, creators, and hobbyists who want a **simple but powerful desktop tool** built on top of **Qt 6** and **FFmpeg**.

---

## 🔖 Versioning

FrameFlow follows **Semantic Versioning**:

* **v0.x.x** → Early development (breaking changes allowed)
* **v1.0.0** → First stable release
* **v1.x.x** → Backward-compatible improvements

Current release:

```
v0.1.0-alpha
```

---

## ✨ Features

* 🎞️ Create **videos** from image sequences
* 🖼️ Generate **image sprites**
* 🎬 Export **GIF animations**
* ⚙️ Attribute-based configuration for each output type
* 📊 Progress tracking and status callbacks
* 🧩 Modular output writer architecture
* 🖥️ Cross‑platform (Linux & Windows)

---

## 🛠️ Technology Stack

* **Language:** C++17
* **UI Framework:** Qt 6 (Qt Widgets)
* **Build System:** CMake
* **Media Backend:** FFmpeg
  * libavformat
  * libavcodec
  * libavutil
  * libswscale

---

# 🏗️ Building FrameFlow

FrameFlow provides platform-specific build scripts to simplify development:

- 🐧 **Linux:** `build.sh`
- 🪟 **Windows:** `build.ps1`

---

# 🐧 Linux Build Guide (build.sh)

## 🔹 Prerequisites

Ubuntu / Debian:

```bash
sudo apt install qt6-base-dev ffmpeg \
libavformat-dev libavcodec-dev libavutil-dev libswscale-dev \
cmake build-essential
```

---

## 🔹 Script Usage

```bash
./build.sh --help
```

### Available Options

| Option | Description |
|--------|------------|
| `--configure` | Configure project using CMake |
| `--build` | Compile the project |
| `--clean` | Remove build directory |
| `--deploy` | Build Flatpak and generate `.flatpak` bundle |
| `--tests` | Enable test build (if supported) |
| `-h`, `--help` | Show help message |

---

## 🔹 Typical Development Build

```bash
chmod +x build.sh

./build.sh --configure
./build.sh --build
```

---

## 🔹 Clean Rebuild

```bash
./build.sh --clean
./build.sh --configure
./build.sh --build
```

---

## 📦 Flatpak Deploy (Linux Only)

```bash
./build.sh --deploy
```

This will:

1. Build Flatpak using the manifest
2. Create a local Flatpak repository
3. Generate distributable `frameflow.flatpak`

---

# 🪟 Windows Build Guide (build.ps1)

## 🔹 Prerequisites

- Qt 6 (MSVC or MinGW version)
- FFmpeg development libraries
- CMake ≥ 3.16
- Visual Studio (for MSVC builds)

---

## 🔹 Enable Script Execution (One-Time Per Session)

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

---

## 🔹 Script Usage

```powershell
.\build.ps1 -Help
```

### Available Options

| Option | Description |
|--------|------------|
| `-Configure` | Configure project using CMake |
| `-Build` | Compile the project |
| `-Clean` | Remove build directory |
| `-Tests` | Enable test build (if supported) |
| `-Help` | Show help message |

---

## 🔹 Typical Development Build

```powershell
.\build.ps1 -Configure
.\build.ps1 -Build
```

---

## 🔹 Clean Rebuild

```powershell
.\build.ps1 -Clean
.\build.ps1 -Configure
.\build.ps1 -Build
```

---

# 📂 Build Output

After successful build:

```
build/
```

The executable will be located inside the generated build directory.

---

# 🧪 Usage Overview

1. **Launch FrameFlow**
2. **Import an image sequence** into the workspace
3. **Arrange images in the desired order**
4. **Select export type** (Video / GIF / Image Sprite)
5. **Fill output attributes** (resolution, frame rate, format, etc.)
6. **Export and track progress**

---

## 📜 License

This project is licensed under the **MIT License**.
See the `LICENSE` file for details.

---

## 🤝 Contributing

Contributions are welcome!

---

## 👤 Author

**Vivek P**  
Software Engineer

GitHub: https://github.com/Vivx701

---

⭐ If you like this project, consider giving it a star!

