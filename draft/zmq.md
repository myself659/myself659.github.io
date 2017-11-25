

# 应用场景  

bitcoind 实现消息的通知  

通知钱包服务  


# nsq 

https://segment.com/blog/scaling-nsq/ 

原理需要在好好分析一下。


A Topic is a specific stream of info or type of info (like visits or impressions).
A Channel is basically an individual consumer of that stream that gets it's own copy. So you could have a backup channel on the visits topic for archival along with a "api" channel for further processing. The "backup" and "api" consumers would receive independent copies of the data.

If you want multiple consumers to get the same message, use a different channel name. If you want to "load balance" among multiple consumers, use the same channel name. 


