// packages
var http = require('http');
var fs = require('fs');
// server
var port = 8080;
var server = http.createServer(function (req, res) 
{
  console.log("Request: " + req.url);

  if (req.url == '/index.html' || req.url == "/")
  {
    fs.readFile("web/index.html", function(err, text)
    {
      res.setHeader("Content-Type", "text/html");
      res.end(text);
    });
  }
  else if (req.url == '/img/favicon-32x32.png')
  {
    fs.readFile("/img/favicon-32x32.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == '/img/favicon-16x16.png')
  {
    fs.readFile("/img/favicon-16x16.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == "/img/astechs.png")
  {
    fs.readFile("img/astechs.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }

});

console.log("starting server")
server.listen(port);