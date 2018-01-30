
Each user of the system is represented by an Upspin user name, which looks like an email address; a public/private key pair; and the network address of a directory server:

在upspin系统中以用户名表示一个用户，它的格式看起来像电子邮件，实际上是一对公私钥，一个目录服务器的网络地址。

That directory server holds a hierarchical tree of names pointing to the user’s data, which is held in a store server, possibly encrypted. Each item in the tree is represented by a directory entry containing a list of references that point to data in the store server:

目录服务器保存一个分层树指向用户的数据，这些用户的数据保存在存储服务器，其中数据可能会加密。 分层树每一项表示一个目录项，其中的每一项指向数据 

All the users are connected through a central key server at key.upspin.io, which holds the public key and directory server address for each user. 

所有用户需要连接一个中央服务器key.upspin.io，在这里保存每个用户的公钥和目录服务器。

From top to bottom, these represent:

Shared directory and store servers used by multiple users.
A single-user system with a combined directory and store server.
A camera served by a special-purpose combined directory and store server.

自顶向下，他们各自表示如下：
多用户之间共享目录和存储服务器
单个用户系统结合目录和存储服务器
一个摄像连接指定的目录和存储服务器 

To illustrate the relationship between these components, here is the sequence of requests a client exchanges with the servers to read the file augie@upspin.io/Images/Augie/large.jpg:

为了说明这些模块之间的关系，读取augie@upspin.io/Images/Augie/large.jpg过程中，客户端与服务端之间的交互流程

私人数据  获取数据 
The client asks the key server for the record describing the owner of the file, which is the user name at the beginning of the file name (augie@upspin.io). The key server’s response contains the name of the directory server holding that user’s tree (dir.upspin.io) and Augie’s public key.  【返回公钥和用户目录树】


The client asks the directory server for the directory entry describing the file. The response contains a list of block references, which include the name of the store server (store.upspin.io).
获取存储服务器地址和数据块的hash 


The client can then ask the store server for each of the blocks, pipelining the requests for efficiency.
客户端以流水线方式高效从存储服务器获取数据块。

The client decrypts the blocks (using Augie’s public key) and concatenates them to assemble the file.

客户端利用公钥解密数据块并重新拼成文件。
（文件描述方式）


indriect  这种灵活的设计方式 无处不在  



# 原文  

https://upspin.io/doc/arch.md 

