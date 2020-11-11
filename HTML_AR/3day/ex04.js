var http = require('http');
var url = require('url');

http.createServer(function (req, res){
    res.writeHead(200, {'Content-Type':'text/html'});
    console.log(req.url);
    var q = url.parse(req.url, true).query;
    console.log(q);
    res.end(q.t1 + ', '+q.t2);
}).listen(8080);