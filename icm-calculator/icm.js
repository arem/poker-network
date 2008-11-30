/*
#
#Copyright (c) 2008 Thomas Johnson <tomfmason@gmail.com>
#
#Permission is hereby granted, free of charge, to any person
#obtaining a copy of this software and associated documentation
#files (the "Software"), to deal in the Software without
#restriction, including without limitation the rights to use,
#copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the
#Software is furnished to do so, subject to the following
#conditions:
#
#The above copyright notice and this permission notice shall be
#included in all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#OTHER DEALINGS IN THE SOFTWARE.
#
*/
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