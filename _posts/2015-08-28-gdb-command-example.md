---
layout: post
title: gdb自定义断点操作
categories: tool
tags: GDB

---

gdb是c/c++上调试利器，有很多技巧能让调试程序与解决问题更加方便与高效，下面关于command 命令的使用一个实例，具体如下：

### 1. 设置断点
```
(gdb) b GenVedioSeekPoint
Breakpoint 1 at 0x402e58: file GenIndex.cpp, line 140.
(gdb) 
```

### 2. 利用commad自定义断点操作
```
(gdb) command  1
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>p *pstPktHead
>continue 
>end
(gdb) 
```

### 3. 设置gdb log信息输出到指定文件
```
(gdb) set logging file genindex.txt 
(gdb) set logging on
Copying output to genindex.txt.
(gdb) set pagination  off
(gdb)
```

### 4. 开始或继续执行程序
```
(gdb) run
```

有输出信息如下：

```
...
Breakpoint 1, GenVedioSeekPoint (pstPktHead=0x7fffffdfdcff, offset=71678) at GenIndex.cpp:140
140             uiTimeStamp = pstPktHead->stBlockHead.iTimeStamp;
$253 = {cFlag = 170 '\252', uiPktLen = 236, uiPktSeq = 3418810467, sCheckNum = 1, cBlockCount = 27 '\033', stBlockHead = {cBlogTag = 90 'Z', iTimeStamp = 14811476, iDatalen = 0}}

Breakpoint 1, GenVedioSeekPoint (pstPktHead=0x7fffffdfddeb, offset=71914) at GenIndex.cpp:140
140             uiTimeStamp = pstPktHead->stBlockHead.iTimeStamp;
$254 = {cFlag = 170 '\252', uiPktLen = 155, uiPktSeq = 3962300516, sCheckNum = 1, cBlockCount = 59 ';', stBlockHead = {cBlogTag = 90 'Z', iTimeStamp = 9503060, iDatalen = 0}}

Breakpoint 1, GenVedioSeekPoint (pstPktHead=0x7fffffdfde86, offset=72069) at GenIndex.cpp:140
140             uiTimeStamp = pstPktHead->stBlockHead.iTimeStamp;
$255 = {cFlag = 170 '\252', uiPktLen = 1959, uiPktSeq = 3774212197, sCheckNum = 257, cBlockCount = 125 '}', stBlockHead = {cBlogTag = 90 'Z', iTimeStamp = 127730004, iDatalen = 0}}
...
```

同时在gdb的运行程序的目录下生成genindex.txt文件，这样可以通过分析genindex.txt找问题的原因；同时整个执行的过程中不需要个人操作，在断点不断命中的情况下极大提高效率，如果是生产的环境，避免长时间占用进程，进而影响业务，

### 5. 分析log文件 
这一步你可以用shell，awk， sed，python等来分析genindex.txt文件内容 





