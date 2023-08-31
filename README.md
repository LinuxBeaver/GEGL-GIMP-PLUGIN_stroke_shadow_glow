

### Location to put Binaries

Windows
C:\Users\USERNAME\AppData\Local\gegl-0.4\plug-ins

Linux
/home/(USERNAME)/.local/share/gegl-0.4/plug-ins

Linux (Flatpak)
/home/(USERNAME)/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

![image](https://github.com/LinuxBeaver/GEGL-SSG-Stroke-Shadow-Glow-/assets/78667207/504a464c-5aca-4f0e-9ba7-7dd6ed7a2292)


![image](https://github.com/LinuxBeaver/GEGL-SSG-Stroke-Shadow-Glow-/assets/78667207/ea6533f0-8716-4d5b-99fc-10daa152201a)

![image](https://github.com/LinuxBeaver/GEGL-SSG-Stroke-Shadow-Glow-/assets/78667207/5482ec26-b420-402b-ad56-cfa5f5065a7c)

## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```
## Preview of this plugin in GEGL Graph mode. 
![image preview](preview3.png  )

## Example of SSG inside a complex GEGL Graph being blended by Linear Light. Also the theme is "meaty text" 
The point of this image is to show that SSG is capable of being fused by blend modes inside GEGL Graphs. Normal Drop Shadow is NOT capable of doing this.
The meat text is just for fun.
![image](https://github.com/LinuxBeaver/GEGL-SSG-Stroke-Shadow-Glow-/assets/78667207/0c147e54-3919-4c88-b253-cb678f8e43d8)
