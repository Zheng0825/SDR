$(document).ready(function(){
  namespace ='/test';

  var socket = io.connect('http://' + document.domain + ':' + location.port + namespace);

  $('#txfrequp').click(function(){
    socket.emit('inc freq');
  });

  socket.on('confirm inc freq', function(msg){
    $('#txfreq').text(msg.txfreq);
  });

});