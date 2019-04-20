<a href="https://hosted.weblate.org/engage/pdf-slicer/?utm_source=widget">
<img src="https://hosted.weblate.org/widgets/pdf-slicer/-/svg-badge.svg" alt="Translation state" />
</a>

# PDF Slicer

This is a simple application for extracting, rotating and reordering pages of a PDF document,
with undo/redo support.

![](docs/readme-screenshot-1.png)

![](docs/readme-screenshot-2.png)

![](docs/readme-screenshot-3.png)

# Installing

# Linux

You can install PDF Slicer using Flatpak. [Get it from Flathub](https://flathub.org/apps/details/com.github.junrrein.PDFSlicer).

# Building

## Fedora

1. Install dependencies

```bash
sudo dnf install gtkmm30-devel poppler-glib-devel cairomm-devel qpdf-devel uuid-devel intltool gettext
```

2. Check out a copy of the source code

```bash
git clone --recursive https://github.com/junrrein/pdfslicer.git
```

3. Build the application

```bash
cd ..
mkdir build-pdfslicer
cd build-pdfslicer
cmake ../pdfslicer/ -DCMAKE_BUILD_TYPE=Release
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

## Flatpak

```bash
flatpak remote-add --from gnome https://sdk.gnome.org/gnome.flatpakrepo
flatpak install gnome org.gnome.Platform//3.30
flatpak install gnome org.gnome.Sdk//3.30
wget https://raw.githubusercontent.com/junrrein/pdfslicer/master/com.github.junrrein.PDFSlicer.json
flatpak-builder --repo=pdfslicer-repo pdfslicer com.github.junrrein.PDFSlicer.json --force-clean
flatpak --user remote-add --no-gpg-verify --if-not-exists pdfslicer-repo pdfslicer-repo
flatpak --user install pdfslicer-repo com.github.junrrein.PDFSlicer
```

### Uninstall

```bash
flatpak --user uninstall com.github.junrrein.PDFSlicer
flatpak --user remote-delete pdfslicer-repo
rm -rf pdfslicer/ pdfslicer-repo/
```

# License

PDF Slicer can be used under the terms of the GNU GPL 3.0 license, or, at your option, any later version. A copy can be found at the LICENSE file.

The application icons were made by [Lapo Calamandrei](calamandrei@gmail.com) and [Tobias Bernard](tbernard@gnome.org). They can be used under the terms of the GNU GPL 3.0 license, or, at your option, any later version. A copy can be found at the LICENSE file.

A number of third-party libraries are distributed as part of this source
code repository. They are enumerated at the LICENSE-THIRD-PARTY file,
along with details about the license they were relased under.
