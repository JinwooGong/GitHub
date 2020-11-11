const express = require('express');
const fs = require('fs');
var app = express();

app.listen(8080);

app.get('/ex09', function (req,res){
    fs.readFile(__dirname + '/ex06.html','utf8',function(err, data){
        res.end(data);
    });
});

app.use(function( req, res){
    if(req.url == '/favicon.ico'){
        return res.status(404);
    }
    res.end('hello world');
});