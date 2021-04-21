////////////////////////////////////////////////////////////////
// File: webserver.js
// Authors: Malcolm McKellips, William Abrams
// Description: Backend server for servicing Astechs Web UI
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Essentials Setup
////////////////////////////////////////////////////////////////
var fs   = require('fs');
var http = require('http');
var sio  = require('socket.io');
var app  = require('express')();
var bdp  = require('body-parser');

////////////////////////////////////////////////////////////////
// Create Server for Web
////////////////////////////////////////////////////////////////
var server = http.createServer(server_handler).listen(8080);
console.log('[SERVER]: webui on *:8080');
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
  else if (req.url == '/about.html')
  {
	fs.readFile(__dirname + '/html/about.html', function(err, text)
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

////////////////////////////////////////////////////////////////
// Socket to Communicate with WebUI
////////////////////////////////////////////////////////////////
var io = sio(server)
io.on('connection', (socket) => 
{
  console.log('[SIO]: a user connected');
  socket.on('disconnect', () => 
  {
    console.log('[SIO] a user disconnected');
  });
  // socket.on('updateReq', () => 
  // {
    //   count += 1;
    //   console.log('[SIO]: update request received');
    //   io.emit('updateRes', count);
    // });
    
  });
  
// setInterval(() => 
// {
// var data_parsed = String.fromCharCode.apply(String, new Uint8Array(data));
// io.emit('threadMsg', data_parsed);
// }, 1000);

////////////////////////////////////////////////////////////////
// Create Server for Python
////////////////////////////////////////////////////////////////
app.use(bdp.json());
app.post('/', function(req, res)
{
  var msg = req.body.msg;
  console.log("python: " + msg);
  res.setHeader("Content-Type", "text/plain");
  res.end('test');
});

var pyserver = http.Server(app).listen(3000);
console.log('[SERVER]: python on *:3000');


////////////////////////////////////////////////////////////////
// Socket to Communicate with Python
////////////////////////////////////////////////////////////////
// var pyio = sio(pyserver);

// pyio.on( "connection", function(pysock) 
// {
//   console.log('[SIO]: a user connected');
  
//   pysock.on('disconnect', () => 
//   {
//     console.log('[SIO] a user disconnected');
//   });

//   pysock.on( 'python-message', function( msg ) 
//   {
//     console.log('message Receiving');
//       // httpsocket.broadcast.emit( 'message', msg );
//   });
// });