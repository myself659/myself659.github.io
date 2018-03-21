{% if page.comments != false%}

  <div id="disqus_thread"></div>
  <script>
    var disqus_config = function () {
      this.page.url = 'http://myself659.github.io{{ page.url }}';
      this.page.identifier = 'http://myself659.github.io{{ page.url }}';
    };
    (function() {
      var d = document, s = d.createElement('script');
      s.src = 'https://{{ site.disqus.shortname }}.disqus.com/embed.js';
      s.setAttribute('data-timestamp', +new Date());
      (d.head || d.body).appendChild(s);
    })();
  </script>
  <noscript>Please enable JavaScript to view the <a href="https://disqus.com/?ref_noscript" rel="nofollow">comments powered by Disqus.</a></noscript>
{% endif %}