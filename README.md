## 特性

  * 支持拼音首字母匹配和完全匹配
  * 支持多音字匹配
  * 仅支持UTF-8编码环境

## 安装
### mac
* 首先需要安装 bash_complete

``` sh
brew install bash_complete
```
* 然后进入工程，编译，安装

``` sh
make
make install
```

## 规则

```
 实验目录如下：
 ls
 SVN培训  全球眼  浙江建行  浙江农信

 使用: (输完后按 TAB 键自动补全)
    cd S     <tab>             进入[SVN培训]
    cd q     <tab>             进入[全球眼]
    cd z     <tab>             自动补全[浙江]
    cd zj    <tab><tab>        提示[浙江建行 浙江农信]备选
    cd zjj   <tab>             进入[浙江建行]
    cd zj1   <tab>             进入[浙江建行]
    cd zj2   <tab>             进入[浙江农信]
```
