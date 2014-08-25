var nodeo3s  = require('../build/Release/nodeo3s'),
	o3s;

module.exports = function() {
	if (!o3s) o3s = new O3S();
	return o3s;
}

function O3S() { 
	this.dico = null;
	this.nodes = {};

	this.init = function(cPort, ip, mPort, mGroup) {
		console.log("O3S connector initialization...");
		this.dico = new nodeo3s.Dico("SynchronizedVariables", cPort, ip, mPort, mGroup);

		if (this.dico) console.log('[Dico created]');
		else console.error('[Dico creation failed]');
	}

	return this;
}



