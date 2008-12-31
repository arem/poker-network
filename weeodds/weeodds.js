//
//    Copyright (C) 2008 Anthony Catel <a.catel@weelya.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

var odds_calculator = new Class({
	initialize: function(){
		this.used = new Hash();
		this.board = new Hash();
		this.current_sel = false;
		this.current_players = 10;
		this.config = {
			'webservice':'/poker-eval-webservice/'
		}

	},

	init: function(){
		this.create_table();
		this.set_player_name(this.hc_el.players[0], 'Me');
		
	},
	
	draw_cards: function()
	{
		var typec = '';
		var typen = '';
		

		for (var x = 14; x >= 2; x--) {
			if (x < 10) {
				typen = x;
			} else if (x == 10) {
				typen = 'T';
			} else if (x == 11) {
				typen = 'J';
			} else if (x == 12) {
				typen = 'Q';
			} else if (x == 13) {
				typen = 'K';
			} else if (x == 14) {
				typen = 'A';
			}
			for (var i = 0; i < 4; i++) {
				switch(i) {
					case 0:
						typec = 'D';
						break;
					case 1:
						typec = 'C';
						break;
					case 2:
						typec = 'H';
						break;
					case 3:
						typec = 'S';
						break;
				}

				new Element('div').addClass('card_small card_' + typen + typec).inject(this.hc_el.lc).setProperties({'rel': typen + typec, 'id': 'cardslst_' + typen + typec}).addEvent('click', function(e) {
					// On clique sur une carte a droite
					
					var thiscard_el = new Event(e).target;
					var thiscard = thiscard_el.getProperty('rel'); // On recupere la carte (e.g. AK)
					if (this.current_sel && !this.used.has(thiscard)) { // Une carte est selectionee et pas encore utilise
						
						if (this.is_board(this.current_sel)) { // Setting board card
							var board_item = this.current_sel.getProperty("rel").toInt();
							if (board_item != 0 && !this.board.hasValue(board_item - 1)) { // Not good order
								return;
							}
						}
						
						this.current_sel.setStyle('background-color', 'transparent'); // On enleve le border rouge						
						this.give_card(this.current_sel, thiscard);
	
						var next_item = this.current_sel.getNext();
						this.current_sel = false;
						if (next_item) {
							this.selcard(next_item);
						}
					}
				}.bind(this));
			}
		}	
	},
	
	set_player_name: function(player,name) {
		player.hc_player_name.set('text', name);
	},
	
	is_board: function(el) {
		if (el.getParent().getProperty('id') == null) {
			return true;
		}
		return false;
	},

	set_card: function(el, card) {
		el.addClass('card_'+card);
	},
	
	clearplayer: function(player_index) {
		var player = this.hc_el.players[player_index];
		var used = this.used;
		var callback = this.reset_card;
		
		player.cards.each(function (val, key) {
			player.cards.erase(key);
			used.erase(key);
			callback($("cardslst_" + key), key);
			
			
		});
		this.reset_card(player.hc_card_1, "back");
		this.reset_card(player.hc_card_2, "back");
		player.hc_player.addClass('hc_player_empty');
		player.active = false;
		if (this.current_sel) {
			this.current_sel.setStyle('background-color', 'transparent');
			this.current_sel = false;
		}
	},
	
	summon: function(total) {
		var players = this.hc_el.players;
		
		for (var i = 0; i < this.current_players; i++) {
			if (!players[i].active) {
				players[i].active = true;
				players[i].hc_player.removeClass('hc_player_empty');
			}
		}
	},
	
	give_card: function(el, card_value) {
		var currentcard = el.getProperty("title");
		var isboard = this.is_board(el);
		
		this.reset_card(el, card_value);
		if (currentcard) { // Il y a deja une carte à cette place
			this.used.erase(currentcard); // On la remove
			if (isboard) {
				this.board.erase(currentcard); // remove from board cards
			} else {
				this.hc_el.players[el.getParent().getProperty('id').toInt()].cards.erase(currentcard); // Remove player card
				
			}
			this.reset_card($("cardslst_" + currentcard), currentcard); // Give card back
		}
		this.used.set(card_value, true);
		if (isboard) {
			this.board.set(card_value, el.getProperty("rel").toInt()); // Setting board card
		} else {
			this.hc_el.players[el.getParent().getProperty('id').toInt()].cards.set(card_value, true); // setting player card
			
		}
		this.reset_card($("cardslst_" + card_value), 'fold');
		this.calc();

	},
	
	selcard: function(that) {
		
		$$('.updatable').setStyle('background-color', 'transparent'); // On enleve les border rouge
		that.setStyle('background-color', '#C66D6D'); // on met du rouge sur la carte en question
		this.current_sel = that;	// On defini la carte selectionne	
	},
	
	calc: function() {
		var opponents_rate = 0.0;
		var rate = 0.0;
		
		new Request.JSON({
			url: this.config.webservice,
			method:'get',
			onComplete:function(resJSON, resText) {
				var len = resJSON.eval.length;
				for (var i = 0; i < len; i++) {
					rate = parseFloat(resJSON.eval[i].ev / 10);
					if (i != 0) {
						opponents_rate += rate;
					}
					this.hc_el.players[i].hc_pot.set('text', rate.toFixed(2) + '%');
				}
				$('opponents').set('text', 'Opponents : '+ opponents_rate.toFixed(2) +'%');
			}.bind(this)
		}).send('pockets='+this.getJsonPlayers()+'&board='+this.getJsonBoard()+'&iterations=150000');
		
	},
	
	getCardsPlayer: function(player) {
		var items = new Array();
		var cards = player.cards;
		
		cards.each(function (val, key) {
			items.push(key);
		});
		for (i = cards.getLength(); i < 2; i++) {
			items.push('__');
		}
		return items;
	},
	
	getJsonPlayers: function() {
		var all = new Array();
		this.hc_el.players.each(function (el) {
			if (el.active) {
				all.push(this.getCardsPlayer(el));
			}
		}.bind(this));
		return JSON.encode(all);
	},
	
	getJsonBoard: function() {
		var items = new Array();
		this.board.each(function(val, key) {
			items.push(key);
		});
		for (i = this.board.getLength(); i < 5; i++) {
			items.push('__');
		}
		return JSON.encode(items);
	},
	
	reset_card:function(el, card_value){
		var card_size;
		if($type(el)!='array'){
			el = new Array(el);
		}
		el.each(function(card){
			card_size = 'card_small';
			if(card.hasClass('card_big')){
				card_size = 'card_big';
			}
			card.removeProperty('class');
			if(card_value){
				card.addClass('card_'+card_value);
			}
			card.addClass(card_size + ' updatable');
			card.setProperty("title", card_value);
		});
	},
	
	set_player_cards: function(player, cards) {
		var card_size;
		player.hc_card_1.removeClass('card_back');
		player.hc_card_2.removeClass('card_back');
		if (cards[0]) {
			player.hc_card_1.addClass('card_'+cards[0]);
			player.cards.set(cards[0], true);
		}
		if (cards[1]) {
			player.hc_card_2.addClass('card_'+cards[1]);
			player.cards.set(cards[1], true);
		}
	},
	
	create_table: function(){
		//Init some vars
		var hc_top = '';
		var hc_player_empty = 'hc_player_empty';
		var hc_player;
		
		this.hc_el = {};
		this.hc_el.players = new Array();

		var hc_player_container;
		var hc_player_name;
		var hc_cards;
		var hc_last_action;
		var hc_pot;
		var hc_foot;
		var chooser;
		var card_size = 'card_small';

		// Basic elements
		this.hc_el.hc = new Element('div').addClass('hc');
		this.hc_el.lc = new Element('div').addClass('lc');


		// Head
		new Element('div').addClass('hc_head').inject(this.hc_el.hc, 'inside');
		this.hc_el.hc_content = new Element('div').addClass('hc_content').inject(this.hc_el.hc, 'inside');
		hc_foot = new Element('div').addClass('hc_foot').inject(this.hc_el.hc, 'inside');

		// Create player counter
		
		chooser = new Element('select', {
			'styles': {
				'width': '120px'
			}	
		}).inject(hc_foot, 'inside').addEvent('change', function(e) {
			var nplayers = new Event(e).target.value.toInt();
			if (nplayers < this.current_players) {
				for (var i = 10; i > nplayers; i--) {
					
					this.clearplayer(i-1);
				}
			}
			this.current_players = nplayers;
			this.summon(nplayers);
			this.calc();
		}.bind(this));
		
		for (var i = 10; i >= 2; i--) {
			new Element('option', {
				'value': i,
				'html': i + ' players'
			}).inject(chooser, 'inside');
		}
		
		new Element('div', {'html': 'Opponents : 0.00%', 'id': 'opponents'}).inject(hc_foot, 'inside');
		
		// Create the table
		var hc_table = new Element('div').addClass('hc_table').inject(this.hc_el.hc_content, 'inside');
		var hc_cards = new Element('div').addClass('hc_table_cards').inject(hc_table, 'inside');
		
		this.hc_el.hc_table_cards = new Array();
		
		// Create board's cards
		for (var i = 0; i < 5; i++) {
			var current = new Element('div').addClass('card_big card_back updatable').setProperty("rel", i + "_board").inject(hc_cards, 'inside');
			this.hc_el.hc_table_cards.push(current);
		}

		// Create ten players
		for (var i = 0; i < 10; i++){
			if (i == 0) {
				card_size = 'card_big';
				hc_player_empty = '';
			} else {
				card_size = 'card_small';
				//hc_player_empty = 'hc_player_empty';
				hc_player_empty = '';
			}
			if (i > 0 && i < 4 || i > 7){
				hc_top = 'hc_top';
			} else{
				hc_top = '';
			}
			hc_player = new Element('div').addClass('hc_player hc_player_'+i+' '+hc_top+' '+hc_player_empty);
			hc_player_container = new Element('div').addClass('hc_player_container').inject(hc_player, 'inside');
			hc_player_name = new Element('div').addClass('hc_player_name').inject(hc_player_container, 'inside');
			
			hc_cards = new Element('div').addClass('hc_cards').inject(hc_player_container, 'inside').setProperty('id', i + '_cards');
			
			hc_card_1 = new Element('div').addClass(card_size+' card_back updatable').inject(hc_cards);
			hc_card_2 = new Element('div').addClass(card_size+' card_back updatable').inject(hc_cards);
			
			hc_pot = new Element('div').addClass('hc_pot').inject(hc_player, 'inside');
			

			this.hc_el.players.push({
						'hc_player':hc_player,
						'hc_player_container':hc_player_container,
						'hc_player_name':hc_player_name,
						'hc_cards':hc_cards,
						'hc_last_action':hc_last_action,
						'hc_pot':hc_pot,
						'hc_card_1':hc_card_1,
						'hc_card_2':hc_card_2,
						'cards': new Hash(),
						'active': true
					});
			hc_player.inject(this.hc_el.hc_content, 'inside');
			

		}
		
		//Draw ;-)
		this.hc_el.hc.inject('container', 'inside');
		this.hc_el.lc.inject('container', 'inside');
		
		this.draw_cards();// Draw right cards
		
		$$('.updatable').addEvent('click', function(e) { // On clique sur la carte d'un joueur ou de la board
			var that = new Event(e).target;

			this.selcard(that);

		}.bind(this));
	}

})
window.addEvent('domready',function(){
	var odds = new odds_calculator();

	odds.init();
	odds.calc();
});
