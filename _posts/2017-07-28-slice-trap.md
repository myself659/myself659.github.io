---
layout: post
title:   slice复用的陷阱
categories:  golang
tags:  golang 
--- 

### 前言  

先下结论：slice复用得当心，引用不当深埋雷。如若复用请分叉，分叉之后再使用。


### 问题 

先看一下代码吧

```
package main

import (
	"fmt"
)

func a() {
	x := []int{}
	x = append(x, 0)
	x = append(x, 1)  // commonTags := labelsToTags(app.Labels)
	y := append(x, 2) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	z := append(x, 3) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	fmt.Println(y, z)
}

func b() {
	x := []int{}
	x = append(x, 0)
	x = append(x, 1)
	x = append(x, 2)  // commonTags := labelsToTags(app.Labels)
	y := append(x, 3) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	z := append(x, 4) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	fmt.Println(y, z)
}

func main() {
	a()
	b()
}
```
上面的如此简单的代码，分析代码希望得到预期结果如下：

```
[0 1 2] [0 1 3]
[0 1 2 3] [0 1 2 4]
```

但是执行后，得到结果如下：

```
Michaels-iMac:golab eric$ go run slice.go  
[0 1 2] [0 1 3]
[0 1 2 4] [0 1 2 4]
```

### 原因分析  

我们先不急着分析具体原因，我们对比一下下面这段代码，看看执行结果是怎么样

```
package main

import (
	"fmt"
)

func a() {
	x := []int{}
	x = append(x, 0)
	x = append(x, 1)  // commonTags := labelsToTags(app.Labels)
	y := append(x, 2) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	z := append(x, 3) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	fmt.Println(y, z)
}

func deepcopy(src []int) []int {
	dst := make([]int, len(src))
	copy(dst, src)
	return dst
}

func b() {
	x := []int{}
	x = append(x, 0)
	x = append(x, 1)
	x = append(x, 2) // commonTags := labelsToTags(app.Labels)
	y := deepcopy(x)
	y = append(y, 3)  // Tags: append(commonTags, labelsToTags(d.Labels)...)
	z := append(x, 4) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	fmt.Println(y, z)
}

func main() {
	a()
	b()
}

```

具体执行结果如下：
```
Michaels-iMac:golab eric$ go run slice-2.go    
[0 1 2] [0 1 3]
[0 1 2 3] [0 1 2 4]
```
对比错误的代码，会发现问题原因出现下面的代码：

```
	y := append(x, 3) // Tags: append(commonTags, labelsToTags(d.Labels)...)
	z := append(x, 4) // Tags: append(commonTags, labelsToTags(d.Labels)...)
```

从内存角度来思考，y与z对应是同一段内存，z的操作override的y的操作。（备注：y与z为什么会更对应同一段内存，请了解一下slice的实现及slice的巧妙的使用，也请阅读下面参考文章[Golang slices gotcha](http://allegro.tech/2017/07/golang-slices-gotcha.html)）

解决方法就是如果slice要进行复用的时候，进行深度copy再进行使用。


### 总结 

slice在golang编程属于超高频使用，如果出现上面的错误，前期没有发现，如果上线，并且系统复杂，出现了问题，定位成本是很大。

虽然上面没有slice的内存分析，但是对于程序员来说学习一些内存知识还很帮助，理解上面的问题就是小case了。


### 参考  

1. [Golang slices gotcha](http://allegro.tech/2017/07/golang-slices-gotcha.html)

