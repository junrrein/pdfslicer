# About

PDF Slicer is a simple app for extracting pages out of a PDF document
as a single file.

![](docs/readme-screenshot-1.png)

![](docs/readme-screenshot-2.png)

# Building

## Fedora

1. Install dependencies

```bash
sudo dnf install gtkmm30-devel poppler-glib-devel cairomm-devel intltool gettext
```

2. Check out a copy of the source code

```bash
git clone https://github.com/junrrein/pdfslicer.git
```

3. Build the application

```bash
cd ..
mkdir build-pdfslicer
cd build-pdfslicer
cmake ../pdfslicer/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
make
```

4. Install

```bash
sudo make install
```

5. Uninstall

```bash
sudo xargs rm < install_manifest.txt
```

# License

PDF Slicer can be used under the terms of the GNU GPL 3.0 license,
or, at your option, any later version.
A copy can be found at the LICENSE file.

A number of third-party libraries are distributed as part of this source
code repository. They are enumerated at the LICENSE-THIRD-PARTY file,
along with details about the license they were relased under.
