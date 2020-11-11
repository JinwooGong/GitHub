const express = require('express');
var app = express();

app.listen(8080);

app.use(function (req,res){
    var q = req.query;
    console.log(q);
    res.json(q);
});