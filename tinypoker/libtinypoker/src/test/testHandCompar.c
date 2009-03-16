/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009 Thomas Cort <linuxgeek@gmail.com>
 * 
 * This file is part of libtinypoker.
 * 
 * libtinypoker is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * libtinypoker is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * libtinypoker.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

int main()
{
	ipp_message *msga, *msgb;
	ipp_card *Kh;
	ipp_card *Ac, *Kc;
	ipp_card *Ad, *Kd;
	ipp_card *As, *Ks, *Qs, *Js, *Ts, *_9s, *_7s, *_6s;
	ipp_card *hand[5];

	ipp_init();

	Kh = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kh);
	Kh->rank = KING;
	Kh->suit = HEARTS;

	Ac = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ac);
	Ac->rank = ACE;
	Ac->suit = CLUBS;

	Kc = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kc);
	Kc->rank = KING;
	Kc->suit = CLUBS;

	Ad = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ad);
	Ad->rank = ACE;
	Ad->suit = DIAMONDS;

	Kd = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kd);
	Kd->rank = KING;
	Kd->suit = DIAMONDS;

	As = ipp_new_card();
	assertNotNull("ipp_new_card() failed", As);
	As->rank = ACE;
	As->suit = SPADES;

	Ks = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Ks);
	Ks->rank = KING;
	Ks->suit = SPADES;

	Qs = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Qs);
	Qs->rank = QUEEN;
	Qs->suit = SPADES;

	Js = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Js);
	Js->rank = JACK;
	Js->suit = SPADES;

	Ts = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Js);
	Ts->rank = TEN;
	Ts->suit = SPADES;

	_9s = ipp_new_card();
	assertNotNull("ipp_new_card() failed", _9s);
	_9s->rank = NINE;
	_9s->suit = SPADES;

	_7s = ipp_new_card();
	assertNotNull("ipp_new_card() failed", _7s);
	_7s->rank = SEVEN;
	_7s->suit = SPADES;

	_6s = ipp_new_card();
	assertNotNull("ipp_new_card() failed", _6s);
	_6s->rank = SIX;
	_6s->suit = SPADES;

/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = Ts;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHTFLUSH A", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_STRAIGHTFLUSH);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_STRAIGHTFLUSH);

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Js;
	hand[3] = _7s;
	hand[4] = _6s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "HIGHCARD A K J 7 6", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_HIGHCARD);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = Ts;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHTFLUSH A", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_STRAIGHTFLUSH);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_STRAIGHTFLUSH);

	hand[0] = Ks;
	hand[1] = Qs;
	hand[2] = Js;
	hand[3] = Ts;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHTFLUSH K", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_STRAIGHTFLUSH);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Kd;
	hand[3] = Kh;
	hand[4] = Kc;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "FOUROFAKIND K A", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_FOUROFAKIND);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_FOUROFAKIND);

	hand[0] = Ks;
	hand[1] = Kd;
	hand[2] = Kh;
	hand[3] = Kc;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "FOUROFAKIND K 9", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_FOUROFAKIND);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);


/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = Ts;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHT A", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_STRAIGHT);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_STRAIGHT);

	hand[0] = Qs;
	hand[1] = Kd;
	hand[2] = Js;
	hand[3] = Ts;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHT K", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_STRAIGHT);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Ac;
	hand[3] = Ad;
	hand[4] = Ks;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "FULLHOUSE A K", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_FULLHOUSE);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_FULLHOUSE);

	hand[0] = Ks;
	hand[1] = Kd;
	hand[2] = As;
	hand[3] = Ac;
	hand[4] = Kh;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "FULLHOUSE K A", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_FULLHOUSE);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Ac;
	hand[3] = Ts;
	hand[4] = Ks;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "TWOPAIR A K T", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_TWOPAIR);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_TWOPAIR);

	hand[0] = Ks;
	hand[1] = Kd;
	hand[2] = As;
	hand[3] = Ac;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "TWOPAIR A K 9", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_TWOPAIR);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = _9s;
	hand[1] = Ad;
	hand[2] = Ac;
	hand[3] = Ts;
	hand[4] = As;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "THREEOFAKIND A T 9", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_THREEOFAKIND);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_THREEOFAKIND);

	hand[0] = Ks;
	hand[1] = Kd;
	hand[2] = As;
	hand[3] = Kh;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "THREEOFAKIND K A 9", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_THREEOFAKIND);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = _9s;
	hand[1] = Ad;
	hand[2] = _7s;
	hand[3] = Ts;
	hand[4] = As;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "ONEPAIR A T 9 7", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_ONEPAIR);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_ONEPAIR);

	hand[0] = Ks;
	hand[1] = Kd;
	hand[2] = As;
	hand[3] = _7s;
	hand[4] = _9s;

	msgb = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msgb);
	assertStringEqual("Didn't detect proper hand.", "ONEPAIR K A 9 7", msgb->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msgb->payload), MSG_ONEPAIR);

	assertEqual("msga should beat msgb", 1, ipp_hand_compar(msga, msgb));
	assertEqual("msga should beat msgb", -1, ipp_hand_compar(msgb, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));

	ipp_free_message(msga);
	ipp_free_message(msgb);

/* =-=-=-=-=-= */

	hand[0] = _9s;
	hand[1] = Ad;
	hand[2] = _7s;
	hand[3] = Ts;
	hand[4] = As;

	msga = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msga);
	assertStringEqual("Didn't detect proper hand.", "ONEPAIR A T 9 7", msga->payload);
	assertEqual("Didn't detect proper hand.", msga->type, MSG_ONEPAIR);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msga->payload), MSG_ONEPAIR);

	assertEqual("msga should beat NULL", 1, ipp_hand_compar(msga, NULL));
	assertEqual("msga should beat NULL", -1, ipp_hand_compar(NULL, msga));
	assertEqual("msga should equal msga", 0, ipp_hand_compar(msga, msga));
	assertEqual("NULL should equal NULL", 0, ipp_hand_compar(NULL, NULL));

	ipp_free_message(msga);

/* =-=-=-=-=-= */

	ipp_free_card(_6s);
	ipp_free_card(_7s);
	ipp_free_card(_9s);
	ipp_free_card(Ts);
	ipp_free_card(Js);
	ipp_free_card(Qs);
	ipp_free_card(Ks);
	ipp_free_card(As);
	ipp_free_card(Kd);
	ipp_free_card(Ad);
	ipp_free_card(Kc);
	ipp_free_card(Ac);
	ipp_free_card(Kh);

	ipp_exit();
	return PASS;
}
