# 桌面图标把玩器 (*＾▽＾)／
欢迎使用桌面图标把玩器 v0.6！

在这里，你可以让你的桌面图标组成旋转动画，也可以体验图标贪吃蛇 ...

# 使用效果
## 圆形模式
![Alt text](screenshots/circle_mode.jpg?raw=true)
## 椭圆模式
![Alt text](screenshots/ellipse_mode.jpg?raw=true)
## 贪吃蛇模式
![Alt text](screenshots/snake_game_mode.jpg?raw=true)

# 最新版本
到 [Release](https://github.com/okcy1016/icon-player/releases) 页面查看下载，解压后执行 icon_player.exe。

你也可以到网盘下载。

1、百度网盘：链接: https://pan.baidu.com/s/1n6GD4MiqJL1TVSOXUrgHBw 提取码: bjia

2、天翼云盘：https://cloud.189.cn/t/zQJRbeiUNFVn

# 我想自己编译该怎么办？
本项目的开发环境是 Windows x64 + [msys2](https://www.msys2.org/)。
安装好 msys2、换源后，使用下列命令更新并安装软件包：
```shell
pacman -Syu
pacman -S mingw-w64-x86_64-fltk mingw-w64-x86_64-gcc mingw-w64-x86_64-make git
```

使用 make 来编译整个项目
```shell
git clone https://gitlab.com/okcy1016/icon-player
cd icon-player
mingw32-make all
```

运行
```shell
./icon_player.exe
```

你也可以使用 Microsoft VS 来构建，你需要自行安装 fltk 库的相关文件，自行编译。

(注：本项目使用了 [ReIcon](https://www.sordum.org/8366/reicon-v1-9-restore-desktop-icon-layouts/) 来实现桌面布局的备份与恢复。）
