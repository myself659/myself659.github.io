# 目录 

```
Michaels-iMac:bitcore-wallet-service eric$ tree 
.
├── LICENCE
├── Makefile
├── README.md
├── bcmonitor
│   └── bcmonitor.js
├── bitcorenode
│   └── index.js
├── bws.js
├── config.js
├── db
├── emailservice
│   └── emailservice.js
├── fiatrateservice
│   └── fiatrateservice.js
├── index.js
├── installation.md
├── lib
│   ├── blockchainexplorer.js
│   ├── blockchainexplorers
│   │   ├── insight.js
│   │   └── request-list.js
│   ├── blockchainmonitor.js
│   ├── common
│   │   ├── constants.js
│   │   ├── defaults.js
│   │   ├── index.js
│   │   └── utils.js
│   ├── emailservice.js
│   ├── errors
│   │   ├── clienterror.js
│   │   └── errordefinitions.js
│   ├── expressapp.js
│   ├── fiatrateproviders
│   │   ├── bitpay.js
│   │   ├── bitstamp.js
│   │   └── index.js
│   ├── fiatrateservice.js
│   ├── index.js
│   ├── locallock.js
│   ├── lock.js
│   ├── messagebroker.js
│   ├── model
│   │   ├── address.js
│   │   ├── addressmanager.js
│   │   ├── copayer.js
│   │   ├── email.js
│   │   ├── index.js
│   │   ├── notification.js
│   │   ├── preferences.js
│   │   ├── pushnotificationsub.js
│   │   ├── session.js
│   │   ├── txconfirmationsub.js
│   │   ├── txnote.js
│   │   ├── txproposal.js
│   │   ├── txproposal_legacy.js
│   │   ├── txproposalaction.js
│   │   └── wallet.js
│   ├── notificationbroadcaster.js
│   ├── pushnotificationsservice.js
│   ├── server.js
│   ├── stats.js
│   ├── storage.js
│   └── templates
│       ├── en
│       │   ├── new_copayer.plain
│       │   ├── new_incoming_tx.plain
│       │   ├── new_outgoing_tx.plain
│       │   ├── new_tx_proposal.plain
│       │   ├── tx_confirmation.plain
│       │   ├── txp_finally_rejected.plain
│       │   └── wallet_complete.plain
│       ├── es
│       │   ├── new_copayer.plain
│       │   ├── new_incoming_tx.plain
│       │   ├── new_outgoing_tx.plain
│       │   ├── new_tx_proposal.plain
│       │   ├── tx_confirmation.plain
│       │   ├── txp_finally_rejected.plain
│       │   └── wallet_complete.plain
│       ├── fr
│       │   ├── new_copayer.plain
│       │   ├── new_incoming_tx.plain
│       │   ├── new_outgoing_tx.plain
│       │   ├── new_tx_proposal.plain
│       │   ├── txp_finally_rejected.plain
│       │   └── wallet_complete.plain
│       └── ja
│           ├── new_copayer.plain
│           ├── new_incoming_tx.plain
│           ├── new_outgoing_tx.plain
│           ├── new_tx_proposal.plain
│           ├── txp_finally_rejected.plain
│           └── wallet_complete.plain
├── locker
│   └── locker.js
├── messagebroker
│   └── messagebroker.js
├── package.json
├── pushnotificationsservice
│   └── pushnotificationsservice.js
├── scripts
│   ├── clean_db.mongodb
│   └── level2mongo.js
├── start.sh
├── stop.sh
└── test
  

23 directories, 105 files
Michaels-iMac:bitcore-wallet-service eric$ 
```

主要代码都在这里
