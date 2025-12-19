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

## 🚀 Getting Started

### Prerequisites

Make sure the following are installed:

* Qt **6.0 or higher**
* FFmpeg development libraries
* C++ compiler (GCC / Clang / MSVC)
* **CMake** (recommended ≥ 3.16)

#### Ubuntu / Debian

```bash
sudo apt install qt6-base-dev ffmpeg libavformat-dev libavcodec-dev libavutil-dev libswscale-dev cmake
```

---

### Build Instructions

```bash
git clone https://github.com/Vivx701/FrameFlow.git
cd FrameFlow
mkdir build && cd build
cmake ..
cmake --build .
```
---

## 📦 Building FrameFlow using Flatpak

FrameFlow can be built and packaged using **Flatpak**, which provides a sandboxed
and reproducible build environment across Linux distributions.

FrameFlow uses the KDE Flatpak runtime (Qt 6 LTS).

Install the runtime and SDK (one-time setup):

```bash
flatpak install flathub \
  org.kde.Platform//6.8 \
  org.kde.Sdk//6.8

```

Build and Install (Local Development Build)

```bash
flatpak-builder --force-clean build-dir \
  flatpak/io.github.vivx701.FrameFlow.yml \
  --install --user \
  --install-deps-from=flathub
```

Run the application

```bash
flatpak run io.github.vivx701.FrameFlow
```

---

## 🧪 Usage Overview

1. **Launch FrameFlow**
2. **Import an image sequence** into the workspace
3. **Arrange images in the desired order**
4. **Select the export type** based on your requirement (Video / GIF / Image Sprite)
5. **Fill in the output attributes** (resolution, frame rate, format, etc.)
6. **Export and track progress** using the built-in progress indicators

---

## 📜 License

This project is licensed under the **MIT License**.
See the [LICENSE](LICENSE) file for details.

---

## 🤝 Contributing

Contributions are welcome!
---

## 👤 Author

**Vivek P**
Software Engineer

GitHub: [https://github.com/Vivx701](https://github.com/Vivx701)

---

⭐ If you like this project, consider giving it a star!
