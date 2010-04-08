py2exe将Python程序转化成exe可执行程序的小工具

py2exe是一种python发布工具，可以把python脚本转换成windows下的可执行程序，不需要安装python便可运行。
py2exe现在可以用来创建使用了wxPython, Tkinter, Pmw, PyGTK, pygame, win32com client and server 等模块的程序。

详细介绍可以看它的官方网站
http://starship.python.net/crew/theller/py2exe/ 或者www.py2exe.org

1. 下载安装py2exe

    py2exe目前的版本是0.6.9，根据你安装的python的版本选择下载的文件
    比如我用是py2exe-0.6.9.win32-py2.6.exe

    安装后的文件应该在你的python安装目录下的Lib\site-packages\py2exe

2.  使用py2exe
我们先准备一个简单的python程序hello.py

    # hello.py
    def main():
      print "Hello, World!"

    if __name__ == '__main__':
      main()  

然后为使用py2exe写一个脚本setup.py

    # setup.py
    from distutils.core import setup
    import py2exe

    setup(console=["hello.py"])

 

运行setup.py，记得要传一个参数给它

    python setup.py py2exe

应该看到一些输出信息
引用：running py2exe
creating E:\Projects\WorkSpace\Python\build
creating E:\Projects\WorkSpace\Python\build\bdist.win32
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\collect
creating E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\temp
creating E:\Projects\WorkSpace\Python\dist
*** searching for required modules ***
*** parsing results ***
creating python loader for extension '_sre'
*** finding dlls needed ***
*** create binaries ***
*** byte compile python files ***
byte-compiling C:\Python23\lib\copy_reg.py to copy_reg.pyc
byte-compiling C:\Python23\lib\sre_compile.py to sre_compile.pyc
byte-compiling E:\Projects\WorkSpace\Python\build\bdist.win32\winexe\temp\_sre.py to _sre.pyc
byte-compiling C:\Python23\lib\macpath.py to macpath.pyc
byte-compiling C:\Python23\lib\popen2.py to popen2.pyc
byte-compiling C:\Python23\lib\atexit.py to atexit.pyc
byte-compiling C:\Python23\lib\os2emxpath.py to os2emxpath.pyc
byte-compiling C:\Python23\lib\sre_constants.py to sre_constants.pyc
byte-compiling C:\Python23\lib\re.py to re.pyc
byte-compiling C:\Python23\lib\ntpath.py to ntpath.pyc
byte-compiling C:\Python23\lib\stat.py to stat.pyc
byte-compiling C:\Python23\lib\string.py to string.pyc
byte-compiling C:\Python23\lib\warnings.py to warnings.pyc
byte-compiling C:\Python23\lib\UserDict.py to UserDict.pyc
byte-compiling C:\Python23\lib\repr.py to repr.pyc
byte-compiling C:\Python23\lib\copy.py to copy.pyc
byte-compiling C:\Python23\lib\types.py to types.pyc
byte-compiling C:\Python23\lib\posixpath.py to posixpath.pyc
byte-compiling C:\Python23\lib\sre.py to sre.pyc
byte-compiling C:\Python23\lib\linecache.py to linecache.pyc
byte-compiling C:\Python23\lib\sre_parse.py to sre_parse.pyc
byte-compiling C:\Python23\lib\os.py to os.pyc
*** copy extensions ***
copying C:\Python23\DLLs\_sre.pyd ->; E:\Projects\WorkSpace\Python\dist
*** copy dlls ***
py2exe会在当前目录下生成两个目录 build和dist
build里是一些py2exe运行时产生的中间文件，dist里有最终的可执行文件

    library.zip
     w9xpopen.exe
     python23.dll
     hello.exe

现在可以运行hello.exe了

    E:\Projects\WorkSpace\Python\dist>hello
    Hello, World!

不过记得如果要发布到别的机器上时，library.zip、 w9xpopen.exe、python23.dll这几个文件是必须要和hello.exe在一起的。

好了，这次先到这里，下次我们做一个wxPython的例子

最后，大家试试运行

    python setup.py py2exe --help

看看py2exe都有哪些参数