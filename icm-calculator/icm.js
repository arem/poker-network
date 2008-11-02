var ICM = {
	url:"http://icm.localhost/icm-webservice/",
	stacks:[],
	payouts:[],
	equities:[],
	getStacks:function(){
		s = $("input[name^='stack'][value!='']");
		for(var i=0; i<s.length;i++){
			if(!isNaN(s[i].value)){
				this.stacks[this.stacks.length] = parseFloat(s[i].value);
			} else {
				this.hasError = true;
				this.errorString += s[i].value + " is not a number <br />";
			}
		}
	},
	getPayouts:function(){
		p = $("input[name^='payout'][value!='']");
		for(var i=0; i<p.length;i++){
			if(!isNaN(p[i].value)){
				this.payouts[this.payouts.length] = parseFloat(p[i].value);
			}else{
				this.hasError = true;
				this.errorString += p[i].value + " is not a number <br />";
			} 
		}
	},
	prepare:function(){
		this.getStacks();
		this.getPayouts();
		stacks = '[' + this.stacks.toString() + ']';
		payouts = '[' + this.payouts.toString() + ']';
		try{
			$.get(this.url, {'stacks':stacks, 'payouts':payouts}, function(data){
				ICM.displayResults(data);
			});
		} catch(e){
			alert('exception caught');
			this.debug(e.toString());
		}
		
	},
	displayResults:function(result){
		this.equities = eval(result)
		for(var i=0;i<this.equities.length;i++){
			$("span#eq" + (i+1)).text(this.equities[i]);
		}
		this.stacks = [];
		this.payouts = [];
		this.equities = [];
	},
	debug:function(str){
		obj = document.getElementById('log');
		obj.innerHTML = obj.innerHTML + str + '<br />';
	}
	
	
}