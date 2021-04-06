var socket = io(); //load socket.io-client and connect to the host that serves the page
window.addEventListener("load", function(){ //when page loads
  var lightbox = document.getElementById("light");
  lightbox.addEventListener("change", function() { //add event listener for when checkbox changes
    socket.emit("light", Number(this.checked)); //send button status to server (as 1 or 0)
  });
});

socket.on('button', function (data) { //get button event from client
  document.getElementsByName('powerdisplay')[0].value = data; //display the dummy power on front end
});

socket.on('serialrec', function (data) { //get button event from client
  document.getElementsByName('powerdisplay')[0].value = data; //display any serially received data
});