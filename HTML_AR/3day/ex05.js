var express = require('express');
var app = express();

app.use(function(req, res){
    res.send('hello world~');
}).listen(8080);