const express = require('express');
const bodyParser = require('body-parser');
spawn = require('child_process').spawn
let app = express();
var mysql = require('mysql');
var connect = mysql.createConnection({
	host:'localhost',
	user:'root',
	password:'root',
	database:'my_db'
});
	

app.use(bodyParser.urlencoded({ extended: true}));

app.get(/\/*/, (req, res) => {
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(`${ip} is asking for wifi!`);
	res.setHeader("Content-type", "text/html");
	res.send(`
	<html>
		<form action="login" method="post">
			name: <input type="text" name="name" />
			</br>
			password: <input type="password" name="password" />
			</br>
			<button>GO!</button>
		</form>
	</html>
	`);
});

app.post("/login", (req,res) => {
	console.log(req.body)
	let name = req.body.name;
	let password = req.body.password;
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(ip)
	if(name == "cnlab" && password == "mycnlab"){
		res.send("<h1>login success!</h1>")
		//modify firewall and record the IP
		spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "-s", ip, "-j", "ACCEPT"]);
		spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "-d", ip, "-j", "ACCEPT"]);
		spawn("iptables", ["-I", "FORWARD", "-s", ip, "-j", "ACCEPT"]);
		spawn("iptables", ["-I", "FORWARD", "-d", ip, "-j", "ACCEPT"]); 
	}else{
		res.send("<h1>gg</h1>")
	}
});

app.listen(8888,"10.0.2.15");
console.log("start listening");
