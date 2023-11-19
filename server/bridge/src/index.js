'use strict';

var express = require('express');
const { exec } = require("child_process");
var app = express();

app.set("port", process.env.PORT || 3000);

exec("./src/CraftRank", (error, stdout, stderr) => {
   console.log(stdout);
});

app.get('/craftsmen/', function (req, res) {
   const { query } = req;
   const { postalcode } = query;
   console.log("Request: ", query);

   // Call CraftRank with postalcode
   exec(`./src/CraftRank ${postalcode}`, (error, stdout, stderr) => {
      console.log(stdout);

      res.writeHead(200, {'Content-Type': 'application/json'});
      var response = { "craftsmen" : stdout }
   
      console.log(response);
      res.end(JSON.stringify(response));
   });

})

app.patch('/craftman/:id', function (req, res) {
   res.writeHead(200, {'Content-Type': 'application/json'});

   const unique_id = req.params.id;
   const { maxDrivingDistance, profilePictureScore, profileDescriptionScore } = req.query;

   var response = { "response" : "This is PATCH method with id=" + {
      unique_id, maxDrivingDistance, profilePictureScore, profileDescriptionScore
   } + "." }

   // TODO: Call CraftRank with parameters

   console.log(response);
   res.end(JSON.stringify(response));
});

var server = app.listen(app.get("port"), function () {

  var host = server.address().address
  var port = server.address().port

  console.log("Node.js API app listening at http://%s:%s", host, port)
});