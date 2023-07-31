# svg2ico
```
    SVG批量转换ICO工具
    深度: 32
    最大像素: 256 * 256
```

## 界面
![界面](/doc/window.png)
![界面](/doc/window_2.png)

```
  执行结果,查看example目录
```
### interface info
|名字|信息|举例|
|---|---|----|
|选择路径|svg 文件夹根目录|D:\svgs |
|输出目录| 会在选择的路径下面新建一个out目录| D:\svgs\out\
|px|256之内的任意数字| 2,4,8,16,32,64,128,256|
|make 按钮 | 开始构建  | 会在out目录新建single目录和group目录  |
|build 按钮 | 生成.h和.rc文件, 用来构建资源DLL| 在make执行之后 |
---

## 执行结果
```
参考example目录,构建资源DLL,需要group文件夹.

示例:
    文件:us.svg
    像素: 2,4,8,16,32,64

```
|文件|信息|
|---|---|
|single| us_2.ico;us_4.ico;us_8.ico;....;us_64.ico|
|group | us.ico(包含2,4,6,8,16,32,64像素) |
|*.h| 生成资源dll,所需的头文件, * 是文件夹的名字,自动生成|
|*.rc| 生成资源dll, 所需的rc文件, * 是文件夹的名字,自动生成|




### ICO文件涉及到的C语言结构体
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
