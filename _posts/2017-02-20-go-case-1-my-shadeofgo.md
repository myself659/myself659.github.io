---
layout: post
title:  Go的50度灰补充：http response只能读一次
categories:  Go
tags:  Go  
--- 


### 问题 

还是从代码开始吧 

```
func fetch(url string) {
    tlsConfig := &tls.Config{
        InsecureSkipVerify: true,
    }

    transport := &http.Transport{
        TLSClientConfig: tlsConfig,
    }
    client := http.Client{Transport: transport}
    resp, err := client.Get(url)
    if err != nil {
        fmt.Println(err)
        <-time.After(300 * time.Second)
        go fetch(url)
        return
    }

    buf, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println("fetchyh:", err)
        return
    }

    save(buf) //保存html到文件
    
    defer resp.Body.Close()

    // http to doc
    doc, err := goquery.NewDocumentFromResponse(resp)
    if err != nil {
        fmt.Println(err, "http resp to doc failed")
        return
    }

    /*
        <div class="datanowin" id="myCont2">
    */
    datanowin := doc.Find("div.datanowin")
    fmt.Println("datanowin:", datanowin.Length())
    tables := datanowin.Find("table")

    tablelen := tables.Length()
    fmt.Println("tablelen:", tablelen)
    for i := 0; i < tablelen; i++ {
        item := tables.Eq(i)
        tableDo(item)
    }
    fmt.Println(doc.Find(".table").Length())

}
```


调用上面的代码后，html网页上没有找到自己所需要的内容，上面的代码问题在哪里？
假想一下，一般有以下原因选项：
1. http请求失败
2. html网页没有对应内容 
3. goquery使用错误导致未找到对应内容

自己排查一下，发现原因不是上面三个选项上，那又是什么原因呢？
原因是对于http response实例只能读一次，只有第一次才是从0开始读，下一次是上一次读到位置开始的

### 代码分析  

层层调用与跳转就此略过，只看读取buf内容最终实现代码：

```
// A Reader implements the io.Reader, io.ReaderAt, io.WriterTo, io.Seeker,
// io.ByteScanner, and io.RuneScanner interfaces by reading from
// a byte slice.
// Unlike a Buffer, a Reader is read-only and supports seeking.
type Reader struct {
    s        []byte
    i        int64 // current reading index
    prevRune int   // index of previous rune; or < 0
}

```

```
func (r *Reader) Read(b []byte) (n int, err error) {
    if r.i >= int64(len(r.s)) {
        return 0, io.EOF
    }
    r.prevRune = -1
    n = copy(b, r.s[r.i:])
    r.i += int64(n) // 读index向后移动
    return
}
```

看到这些代码问题就很清楚了，第一次读取resp.Body时ioutil.ReadAll将所有内容都完了，第二次读调用goquery.NewDocumentFromResponse没有读到任何内容。 

其实问题不止上面这个：看了ioutil.ReadAll的流程后，又看了goquery.NewDocumentFromResponse的实现，发现还有一个问题:代码还存在一个问题，代码存在两次调用defer resp.Body.Close()，这是goquery.NewDocumentFromResponse接管了resp.Body,代码如下：

```
func NewDocumentFromResponse(res *http.Response) (*Document, error) {
    if res == nil {
        return nil, errors.New("Response is nil")
    }
    defer res.Body.Close() // 接管了resp.Body
    if res.Request == nil {
        return nil, errors.New("Response.Request is nil")
    }

    // Parse the HTML into nodes
    root, e := html.Parse(res.Body)
    if e != nil {
        return nil, e
    }

    // Create and fill the document
    return newDocument(root, res.Request.URL), nil
}
```


一种简单解决上述问题代码如下：

```
func fetch(url string) {
    tlsConfig := &tls.Config{
        InsecureSkipVerify: true,
    }

    transport := &http.Transport{
        TLSClientConfig: tlsConfig,
    }
    client := http.Client{Transport: transport}
    resp, err := client.Get(url)
    if err != nil {
        fmt.Println(err)
        <-time.After(300 * time.Second)
        go fetch(url)
        return
    }
/*
    buf, err := ioutil.ReadAll(resp.Body)
    if err != nil {
        fmt.Println("fetchyh:", err)
        return
    }

    save(buf) //保存html到文件
*/    
    // defer resp.Body.Close()

    // http to doc
    doc, err := goquery.NewDocumentFromResponse(resp)
    if err != nil {
        fmt.Println(err, "http resp to doc failed")
        return
    }

    /*
        <div class="datanowin" id="myCont2">
    */
    datanowin := doc.Find("div.datanowin")
    fmt.Println("datanowin:", datanowin.Length())
    tables := datanowin.Find("table")

    tablelen := tables.Length()
    fmt.Println("tablelen:", tablelen)
    for i := 0; i < tablelen; i++ {
        item := tables.Eq(i)
        tableDo(item)
    }
    fmt.Println(doc.Find(".table").Length())

}
```


### 总结 

第一次遇到这个问题，我就想到是先调用ioutil.ReadAll读取resp.Body的内容，后面调用goquery.NewDocumentFromResponse就没有读到东西。于是我把ioutil.ReadAll相关代码去掉，问题解决了。后面我就没有深入追究原因(还是给自己找个理由：忙)。

本来我是抗拒写这篇文章的，想想自己在这个问题老是不长记性，还是读一下相关源码，简单记录下来，加深印象。

**读源码是最有收益的解决问题方式**。

最后推荐一下这篇文章：[50 Shades of Go: Traps, Gotchas, and Common Mistakes for New Golang Devs](http://devs.cloudimmunity.com/gotchas-and-common-mistakes-in-go-golang/)

(end)






