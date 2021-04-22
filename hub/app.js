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
// Variables for Hub System
////////////////////////////////////////////////////////////////

var n1_stat = false;
var n2_stat = true; //TODO: testing, should start as false

var n1_pow_arr = new Array(20).fill(0);
var n2_pow_arr = new Array(20).fill(0);

////////////////////////////////////////////////////////////////
// Commands for Hub System
////////////////////////////////////////////////////////////////

const CMD_N1_ON  = 'NODE1 ON\r\n';
const CMD_N1_OFF = 'NODE1 OFF\r\n';
const CMD_N1_TOG = 'NODE1 TOG\r\n';

const CMD_N2_ON  = 'NODE2 ON\r\n';
const CMD_N2_OFF = 'NODE2 OFF\r\n';
const CMD_N2_TOG = 'NODE2 TOG\r\n';

////////////////////////////////////////////////////////////////
// Socket to Communicate with WebUI
////////////////////////////////////////////////////////////////
var io = sio(server)
// var count = 0
io.on('connection', (socket) => 
{
  console.log('[SIO]: a user connected');
  socket.on('disconnect', () => 
  {
    console.log('[SIO] a user disconnected');
  });
  
  socket.on('update_req', () => 
  {
    console.log("updating node state!");
    io.emit('update_res_stat', n1_stat, n2_stat);
      // count += 1;
      // console.log('[SIO]: update request received');
      // io.emit('update_res_stat', count);
  });
    
});

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



// setInterval(() => 
// {
// var data_parsed = String.fromCharCode.apply(String, new Uint8Array(data));
// io.emit('threadMsg', data_parsed);
// }, 1000);