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
var n2_stat = false;

var n1_pow_arr = Array.from(new Array(20).keys()); //TODO: testing, should start as .fill(0);
var n2_pow_arr = new Array(20).fill(0);

var cmd_q = [];

////////////////////////////////////////////////////////////////
// Commands for Hub System
////////////////////////////////////////////////////////////////
const RSP_N1_ON  = 'NODE1 IS ON';
const RSP_N1_OFF = 'NODE1 IS OFF';


const RSP_N2_ON  = 'NODE2 IS ON';
const RSP_N2_OFF = 'NODE2 IS OFF';

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
  
  socket.on('update_req', () => 
  {
    console.log('[SIO]: update request received...');

    console.log("[SIO]: updating node status!");
    io.emit('update_res_stat', n1_stat, n2_stat);

    console.log('[SIO]: updating power consumption!');
    io.emit('update_res_pow', n1_pow_arr, n2_pow_arr);
  });

  socket.on('send_cmd', (msg) =>
  {
    console.log('w2n: ' + msg);
    cmd_q.push(msg);
  });

  setInterval(() => 
  {
    io.emit('update_res_stat', n1_stat, n2_stat);
    io.emit('update_res_pow', n1_pow_arr, n2_pow_arr);
  }, 1000);
    
});

////////////////////////////////////////////////////////////////
// Create Server for Python
////////////////////////////////////////////////////////////////
app.use(bdp.json());
app.post('/', function(req, res)
{
  var msg = req.body.msg;
  console.log("python: " + msg);

  switch(String(msg))
  {
    case RSP_N1_ON:
      n1_stat = true;
      break;
    case RSP_N1_OFF:
      n1_stat = false;
      break;
    case RSP_N2_ON:
      n2_stat = true;
      break;
    case RSP_N2_OFF:
      n2_stat = false;
      break;
  }

  if (cmd_q.length > 0)
  {
    res.setHeader("Content-Type", "text/plain");
    res.end(cmd_q.shift());
  }
  else
  {
    res.end();
  }
});

var pyserver = http.Server(app).listen(3000);
console.log('[SERVER]: python on *:3000');

// USEFUL:

// setInterval(() => 
// {
// var data_parsed = String.fromCharCode.apply(String, new Uint8Array(data));
// io.emit('threadMsg', data_parsed);
// }, 1000);

// function updateData1(powread)
// {
//   for (let i = 0; i < 10 - 1; i++)
//     powdata1[i] = powdata1[i + 1];
//   powdata1[10 - 1] = powread;
// }

// function updateData2(powread)
// {
//   for (let i = 0; i < 10 - 1; i++)
//     powdata2[i] = powdata2[i + 1];
//   powdata2[10 - 1] = powread;
// }