// File: webserver.js
// Authors: Malcolm McKellips, William Abrams
// Description: Backend server for servicing Astechs Web UI

// Server Setup
var http = require('http');
var server = http.createServer(server_handler);
var io = require('socket.io')(server)

// File Setup
var fs = require('fs'); //require filesystem module

// // Serial Port Setup
// const SerialPort = require('serialport');
// const Readline = require('@serialport/parser-readline');
// const port = new SerialPort('/dev/ttyACM0', { baudRate: 9600 });
// const parser = port.pipe(new Readline({ delimiter: '\n' }));

server.listen(8080); 
console.log('[SETUP]: listening on *:8080')

//Create Server
function server_handler (req, res)
{
  console.log("[REQ]: " + req.url);

  if (req.url == '/index.html' || req.url == "/" || req.url == "/:")
  {
    fs.readFile(__dirname + '/html/index.html', function(err, text)
    {
      res.setHeader("Content-Type", "text/html");
      res.end(text);
    });
  }
  else if (req.url == '/css/basic.css')
  {
    fs.readFile("css/basic.css", function (err, text)
    {
      res.setHeader("Content-Type", "text/css");
      res.end(text);
    })
  }
  else if (req.url == '/favicon.ico')
  {
    fs.readFile("img/favicon-32x32.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == '/img/node_on.png')
  {
    fs.readFile("img/node_on.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == '/img/node_off.png')
  {
    fs.readFile("img/node_off.png", function (err, text)
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
  else if (req.url == "/js/threadParse.js")
  {
    fs.readFile("js/threadParse.js", function (err, text)
    {
      res.setHeader("Content-Type", "text/js");
      res.end(text);
    })
  }
  else if (req.url == "/js/powerPlot.js")
  {
    fs.readFile("js/powerPlot.js", function (err, text)
    {
      res.setHeader("Content-Type", "text/js");
      res.end(text);
    })
  }
};

var count = 0;
// WebSocket Connection
io.on('connection', (socket) => 
{
  console.log('[SIO]: a user connected');
  socket.on('disconnect', () => 
  {
    console.log('[SIO] a user disconnected');
  });
  socket.on('updateReq', () => 
  {
    count += 1;
    console.log('[SIO]: update request received');
    io.emit('updateRes', count);
  });
});

// Serial Reading