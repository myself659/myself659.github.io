
# 深入EVM


Solidity提供很多高级语言的特性，但是这些特性让我很难理解程序在运行的原理。阅读
Solidity说明书仍然让我觉得困惑。 


string, bytes32, byte[], bytes之间有什么不同？

- 应该使用哪个？
- 当string是怎么转化bytes？是可以转化byte[]?
- 他们的开销是多少？  

在evm中mappings是怎么实现的？

- 为什么不能删除一个mapping？
- 是否使用mappings mappings？（可以的，是如何实现呢？）
- 为什么有存储mappings？为什么没有内存 mappings？ 

在EVM中一个编译过的智能合约是怎么样？

- 一个智能合约是如何创建的？
- 什么是智能合约的构造函数？
- 什么是回调函数？


我认为学习solidity在EVM运行的原理是值得的。 主要原因如下：

1. Solidity并不是EVM上最终运行的语言，未来一定会有更好EVM语言？
2. EVM是一个数据库引擎。搞懂智能合约在EVM运行原理，必须理解在EVM中数据中如何管理，存储，组织的 
3. 理解实现原理才能成为一个开源贡献者。Ethereum工具还处于早期阶段，理解EVM原理可以让你写出NB的工具
4. 学习EVM原理可以让你深入密码学，数据结构，编程语言设计等领域


在下面一系列的文章中，通过分析一个简单的智能合约以理解在EVM 字节码层次上是如何工作的

主要的内容如下：

- EVM 字节码基础知识  
- mappings, arrays 是如何实现
- 当新合约创建的时候将会发生什么
- 方法是如何调用的
- ABI如何连接不同EVM编程语言


我的最终目的是能够理解智能合约的字节码形式。

## 一个简单的例子  


## 不足 

http://api.btckan.com/news/topic/35732  

## 为什么需要EVM 

通过虚拟机保证安全吗？