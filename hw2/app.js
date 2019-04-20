const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs')
var http = require('http')
var https = require('https')
var exec = require('child_process').exec;
var util = require('util');
const url = require('url');
spawn = require('child_process').spawn
ips = [];
root_ip = null
let app = express();
var volumes = []



app.use(bodyParser.urlencoded({ extended: true}));

app.get("/", (req, res) => {
	console.log(res.headersSent);
let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	const host = req.headers.host.replace(/:\d+/,"");
	console.log("host:",host)
	if(host != "10.0.2.15"){
		
		res.status(301).redirect("http://10.0.2.15:8888/");
		res.end();
	}
	else if(root_ip == ip){
		var child = exec(util.format('sudo iptables -L -v | awk \'NR > %d && NR < %d{print $1, $2}\'', 9  , 10 + (ips.length) * 2), (error, stdout, stderr) =>{
		console.info('stdout:');
		console.log(stdout);
		var test = stdout.split('\n')	
		test.pop();		
		console.log(test);
				
		let data = JSON.stringify(test);
		fs.writeFileSync('test.json', "["+data);
		let datas = JSON.stringify(ips)
		fs.appendFileSync('test.json', "," + datas+"]");
		volumes = test;
				//console.log(volumes);
		});
		
		res.sendFile(__dirname+'/data_vol.html');
	}
	else{
		let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
		console.log(`${ip} is asking for wifi!`);
		res.setHeader("Content-type", "text/html");
		if(ips.indexOf(ip) == -1){
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
		}
		else{
			res.sendFile(__dirname+'/logged.html');
		}
	}
});

app.post("/login", (req,res) => {
	console.log(req.body)
	let name = req.body.name;
	let password = req.body.password;
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(ip)
	
	if(name == "cnlab" && password == "mycnlab"){
		//modify firewall and record the IP
		if(ips.indexOf(ip) == -1){
			spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "-s", ip, "-j", "ACCEPT"]);
			spawn("iptables", ["-t", "nat", "-I", "PREROUTING", "-d", ip, "-j", "ACCEPT"]);
			spawn("iptables", ["-I", "FORWARD", "-s", ip, "-j", "ACCEPT"]);
			spawn("iptables", ["-I", "FORWARD", "-d", ip, "-j", "ACCEPT"]);
			ips.unshift(ip);
			
		} 
		console.log(ips);
		res.sendFile(__dirname+'/logged.html');
	}
	else if(name == "root" && password == "root" && (root_ip == null || root_ip == ip)){
		root_ip = ip;
		console.log("root ip:" + ip);
		var child = exec(util.format('sudo iptables -L -v | awk \'NR > %d && NR < %d{print $1, $2}\'', 9  , 10 + (ips.length) * 2), (error, stdout, stderr) =>{
		console.info('stdout:');
		console.log(stdout);
		var test = stdout.split('\n')	
		test.pop();		
		console.log(test);
				
		let data = JSON.stringify(test);
		fs.writeFileSync('test.json', "["+data);
		let datas = JSON.stringify(ips)
		fs.appendFileSync('test.json', "," + datas+"]");
		volumes = test;
				//console.log(volumes);
		});
		
		res.sendFile(__dirname+'/data_vol.html');
					
	}
	else{
		res.send("<h1>gg</h1>")
	}
});
app.get("/test.json", (req,res)=> {
	res.sendFile(__dirname+"/test.json");
});
app.get("/login", (req,res) => {
	let name = req.body.name;
	let password = req.body.password;
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(ip)
	if(ip == root_ip){
		var params = url.parse(req.url, true).query;
		console.log("num:" + params.name)
		if(params.name != null){
			spawn("iptables", ["-t", "nat", "-D", "PREROUTING", (params.name) * 2]);
			spawn("iptables", ["-t", "nat", "-D", "PREROUTING", (params.name) * 2 - 1]);
			spawn("iptables", ["-D", "FORWARD", (params.name) * 2]);
			spawn("iptables", ["-D", "FORWARD", (params.name) * 2 - 1]);
			console.log(ips[params.name-1] + "kicked out")
			ips.splice(params.name - 1, 1);
			console.log(ips);
		}
		var child = exec(util.format('sudo iptables -L -v | awk \'NR > %d && NR < %d{print $1, $2}\'', 9  , 10 + (ips.length) * 2), (error, stdout, stderr) =>{
		console.info('stdout:');
		console.log(stdout);
		var test = stdout.split('\n')	
		test.pop();		
		console.log(test);
				
		let data = JSON.stringify(test);
		fs.writeFileSync('test.json', "["+data);
		let datas = JSON.stringify(ips)
		fs.appendFileSync('test.json', "," + datas+"]");
		volumes = test;
				//console.log(volumes);
		});
		res.sendFile(__dirname+'/data_vol.html');
	}
	else if(ips.indexOf(ip) != -1){
		res.sendFile(__dirname+'/logged.html');
	}
	else{
		res.sendFile(__dirname+'/not logged.html')
	}
});
app.get("/logout", (req,res) => {
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	let pos = ips.indexOf(ip)
	console.log(pos);
	if(root_ip != null){
		root_ip = null;
	}
	if(pos != -1){
		spawn("iptables", ["-t", "nat", "-D", "PREROUTING", (pos + 1) * 2]);
		spawn("iptables", ["-t", "nat", "-D", "PREROUTING", (pos + 1) * 2 - 1]);
		spawn("iptables", ["-D", "FORWARD", (pos + 1) * 2]);
		spawn("iptables", ["-D", "FORWARD", (pos + 1) * 2 - 1]);
		ips.splice(pos, 1)
		res.send(`
		<html>
			<h1>Successfully logged out!</h1>
		</html>
		`);
	}
	else{
		res.sendFile(__dirname+'/not logged.html')
	}
});
/*app.use("*", (req,res) =>{
	const host = req.headers.host.replace(/:\d+/,"");
	console.log("host:",host)
	if(host != "10.0.2.15"){
		res.redirect("10.0.2.15:8888/")
	}
});*/
/*https.createServer().listen(8887, "10.0.2.15");
https.get("/", (req,res) =>{
	res.status(301).redirect("http://10.0.2.15:8888/");
		res.end();
	host = 0
	console.log(res.headersSent);
	const host = req.headers.host.replace(/:\d+/,"");
	console.log("host:",host)
	if(host != "10.0.2.15"){
		//res.writeHead(301, { "Location": "10.0.2.15:8888"}); 
		
	}
	else{
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(`${ip} is asking for wifi!`);
	res.setHeader("Content-type", "text/html");
	if(ips.indexOf(ip) == -1){
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
	}
	else{
		res.sendFile(__dirname+'/logged.html');
	}
	}
});*/
app.listen(8888,"10.0.2.15");
console.log("start listening");
