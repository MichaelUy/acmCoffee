var spawn = require('child_process').spawn;
var lockfile = require('lockfile');
var sendgrid = require('sendgrid')('usr', 'pwd');
var fs = require('fs');

/*
 * GET home page.
 */

exports.index = function(req, res){
  res.render('index', { title: 'Express' });
};

exports.make_coffee = function(req, res) {
	var email = req.body.email
	,	cups = req.body.cups;
	console.log(cups, email);
	lockfile.lock('execution.lock', function(er) {
		if (er) {
			console.log('coffee locked.');
			res.render('locked', { title: 'Express' });
		} else {
			sendgrid.send({
				to:email,
				from:'acm.ucr.coffeepp@gmail.com',
				subject:'Your coffee',
				html:['<p>Hello!</p><p>Your coffee will be ready in '+ cups +'.5 minutes</p>']
			}, function(){console.log(arguments)});
			var coffee = spawn('./coffee',['' + cups]);
			coffee.stdout.on('data',function(data){
				if (/^0/.test(data)) {
					var count = + data;
					console.log(count);
					fs.writeFile('public/readyornot.txt', count, function(er){
						if (er) {
							console.log('ERROR, COULD NOT SAVE.');
						}
					});
					if(!count) {
						console.log('coffee\'s ready');
						sendgrid.send({
							to:email,
							from:'acm.ucr.coffeepp@gmail.com',
							subject:'Your coffee',
							html:'<p>Hello!</p><p>Congratulations, your coffee is ready!</p>'
						}, function(){console.log(arguments)});
						lockfile.unlock('execution.lock', function(er){
							if (er) {
								console.log('failed to unlock!!!!!');
							}
						});
					}
				}
			});
			coffee.stderr.on('data',function(data){
				console.log(data.toString());
			});
			coffee.on('end',function(end){
				console.log('process ended');
			});
		  	res.render('brewing', { title: 'Express' });
		}
	});
}
