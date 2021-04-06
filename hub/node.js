// File: node.js
// Author: Malcolm McKellips
// Description: Backend server for servicing Astechs Web UI

// Server Setup
var http = require('http').createServer(handler); //require http server, and create server with function handler()
var io = require('socket.io')(http) //require socket.io module and pass the http object (server)

//File Setup
var fs = require('fs'); //require filesystem module

//GPIO Setup
var Gpio = require('onoff').Gpio; //include onoff to interact with the GPIO
var LED = new Gpio(4, 'out'); //use GPIO pin 4 as output
var pushButton = new Gpio(17, 'in', 'rising',{debounceTimeout: 10}); //use GPIO pin 17 as input button

//Serial Port Setup
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const port = new SerialPort('/dev/ttyACM0', { baudRate: 9600 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));// Read the port data -> delimited by '\n'

//listen to port 8080
http.listen(8080); 

//Create Server
//Handle service of front end "index.html"
//Handle service of images in /img folder
function handler (req,res)
{
  //~ console.log("Request: " + req.url);

  if (req.url == '/index.html' || req.url == "/" || req.url == "/:")
  {
    fs.readFile(__dirname + '/html/index.html', function(err, text)
    {
      res.setHeader("Content-Type", "text/html");
      res.end(text);
    });
  }
  else if (req.url == '/img/favicon-32x32.png')
  {
    fs.readFile("img/favicon-32x32.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == '/img/favicon-16x16.png')
  {
    fs.readFile("img/favicon-16x16.png", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == '/img/node.png')
  {
    fs.readFile("img/node.png", function (err, text)
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
  else if (req.url == "/js/ext/socketio.js")
  {
    fs.readFile("js/ext/socketio.js", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  else if (req.url == "/js/handleio.js")
  {
    fs.readFile("js/handleio.js", function (err, text)
    {
      res.setHeader("Content-Type", "image/png");
      res.end(text);
    })
  }
  
};

io.sockets.on('connection', function (socket) {// WebSocket Connection
  console.log('client connected...');
  var powerdata = 0;
  //Open serial port
  port.on("open", () => {
  console.log('serial port open');
  });
  
  //Watch for hardware interrupts on pushButton
  pushButton.watch(function (err, value) { 
    if (err) {
      console.error('There was an error', err); 
      return;
    }
    socket.emit('button',Math.random()*100); //send dummy power data
  });
  
  //Service 'light' event
  socket.on('light', function(data) { //get light switch status from client
    LED.writeSync(data); //turn LED on or off
  });
  
  //Service Serial Data reception
  parser.on('data', data =>{
    newdata = data.replace(/\D/g,''); //filter out non-numerical chars. -> fixes weird huge buffer error
    //~ console.log('serial input:' ,data);
    //~ console.log('serial input length: ',data.length);
    console.log('Serial input:' ,newdata);
    //~ console.log('newdata length: ',newdata.length);
    socket.emit('serialrec',Number(newdata)); //cast serial receive data event
  });
  
});

process.on('SIGINT', function () { //on ctrl+c
  LED.writeSync(0); // Turn LED off
  LED.unexport(); // Unexport LED GPIO to free resources
  pushButton.unexport(); // Unexport Button GPIO to free resources
  process.exit(); //exit completely
});
