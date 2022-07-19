// receive HTTP request (MySQL command) and send back response(MySQL result)
// request format:
//   POST /mysql
//   Content-Type: application/json
//   {
//     "command": "SELECT * FROM table"
//   }
//   response format:
//   {
//     "result": [
//       {
//         "id": 1,
//         "name": "John"
//       },
//       {
//         "id": 2,
//         "name": "Jane"
//       }
//     ]
//   }
// my address: http://localhost:4000/mysql

// Access-Control-Allow-Origin: *


var mysql = require('mysql');
var connection = mysql.createConnection({
    host: '127.0.0.1',
    user: 'root',
    password: 'x',
    database: 'bank',
    timezone: 'Z',
    dateStrings: true
});



var express = require('express');
var app = express();

// var bodyParser = require('body-parser');
// app.use(express.json())


// application/x-www-form-urlencoded
app.use(express.urlencoded({
    extended: true
}));


var port = 4000;
var router = express.Router();


router.post('/mysql', function (req, res) {
    var command = req.body.command;
    if (command !== undefined) {
        console.log(command);
        connection.query(command, function (err, rows, fields) {
            if (err) {
                console.log(err);
                res.send(err);
            } else {
                console.log(rows);
                res.send(rows);
            }
        });
    } else if (req.body.username !== undefined) {
        username = req.body.username;
        password = req.body.password;
        databaseName = req.body.databaseName;

        console.log(username, password, databaseName);

        // stop connection and create new connection
        connection.end();
        connection = mysql.createConnection({
            host: '127.0.0.1',
            user: username,
            password: password,
            database: databaseName
        });

        // send back response
        res.send({
            "result": "success"
        });
    }
});

app.all('*', function (req, res, next) {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Access-Control-Allow-Methods", "PUT,POST,GET,DELETE,OPTIONS");
    res.header("X-Powered-By", ' 3.2.1')
    res.header("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
    next();
})

app.use('/', router);


app.listen(port);
console.log('Server running at http://127.0.0.1:' + port);
