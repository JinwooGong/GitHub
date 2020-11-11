var http = require('http');

http.createServer(function (req, res){
    res.writeHead(200, {'Context-Type': 'text/html'});
    res.end('Hello World!');
}).listen(8080);