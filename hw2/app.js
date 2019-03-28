

















const express = require("express");
const bodyParser = require("body-parser");

let app = express();
app.use(bodyParser.urlencoded({ extended: true }));

app.gt(/\/*/, (req, res) => {
	let ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	console.log(`${ip} is asking for wifi!`);
	res.setHeader("Content-type", "text/html")
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
app.post("/login", (req, ret) => {
	console.log(req.body)
	let name = req.body.name;
	let password = req.body.password;
	let ip = req.headers[`x-forwarded-for`] || req.connection.remoteAddress;
	console.log(ip)
	if(name == "cnlab" && password == "mycnlab"){
		res.send("<h1>登入成功</hl>")
		//修改防火牆, 並且把此人的IP記下來
		
		
		
		
	}else{
		res.send("<h1>帳號或密碼有誤 QQ</h1>")
	}
});

app.listen(8888);
console.log("start listening!")