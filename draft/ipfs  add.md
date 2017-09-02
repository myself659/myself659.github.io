
# ipfs add 


add.go  核心代码  


数据结构  Adder 


# 加入到root命令 


var rootSubcommands = map[string]*cmds.Command{   



# add 添加确认选项

因为文件一旦添加就永远存在 

# 对于同一个目录下文件 需要支持动态overlap 

