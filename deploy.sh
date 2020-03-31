#!/bin/bash
SSH="ssh -t $BCUSER@$BCHOST"
DOTH="cd /root/gocode/src/github.com/myself659/ipds-blog-website/ipds-public && pwd && git pull  "
$SSH  $DOTH