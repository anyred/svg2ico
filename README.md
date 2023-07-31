# svg2ico [简体中文](README_zh.md)
```
    SVG batch conversion to ICO
    Depth: 32
    Maximum resolution: 256 * 256
```

## interface
![interface](/doc/window.png)
![interface2](/doc/window_2.png)
```
    see example folder
```
### interface info
|name|info|example|
|---|---|----|
|select path|svg files root directory|D:\svgs |
|out path| root/out| D:\svgs\out\
|px|Any number within 256, separated by commas| 2,4,8,16,32,64,128,256|
|make button | Start the build  | Generate single and group folders under the out directory  |
|build button |The .h file and rc file required to generate the resource DLL| after make |
---

## Execution results
```
reference example directory,The `group` folder is required to build the resource DLL.

eg:
    file :us.svg
    pixel: 2,4,8,16,32,64

```
|file|info|
|---|---|
|single| us_2.ico;us_4.ico;us_8.ico;....;us_64.ico|
|group | us.ico(include pixel:2,4,6,8,16,32,64) |
|*.h| The header files required to build the resource DLL, nedd group folder, * is folder name,Automatically generated|
|*.rc| RC files required to build resource DLLs,nedd group folder, * is folder name, Automatically generated|




### ICO file C Struct
```c
struct ico_header
{
    short reserved;  // 0
    short type;      // 1:ico 2:cursor
    short count;     // images count;
};

struct ico_entry
{
    uint8_t     width;    // max :255
    uint8_t     height;   // max :255
    uint8_t     color_count;
    uint8_t     reserved;
    uint16_t    planes;
    uint16_t    bit_count;
    uint32_t    bytes_in_res;
    uint32_t    image_offset;
};
```
