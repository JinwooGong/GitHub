const express = require('express');
const fs = require('fs');
var app = express();

app.use('/public', express.static(__dirname + '/public'));
app.listen(8080);

app.get('/ex10', function (req,res){
    fs.readFile(__dirname + '/ex10.html','utf8',function(err, data){
        res.end(data);
    });
});

app.use(function( req, res){
    if(req.url == '/favicon.ico'){
        return res.status(404);
    }
    res.end('hello world');
});