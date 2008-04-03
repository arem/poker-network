/*
 * Copyright (C) 2005, 2006, 2007, 2008 Thomas Cort <tom@tomcort.com>
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
	ipp_message *msg;
	ipp_card *Kh;
	ipp_card *Kc;
	ipp_card *Ad, *Kd;
	ipp_card *As, *Ks, *Qs, *Js, *Ts, *_7s, *_6s;
	ipp_card *hand[5];

	ipp_init();

	Kh = ipp_new_card();
	assertNotNull("ipp_new_card() failed", Kh);
	Kh->rank = KING;
	Kh->suit = HEARTS;

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

	_7s = ipp_new_card();
	assertNotNull("ipp_new_card() failed", _7s);
	_7s->rank = SEVEN;
	_7s->suit = SPADES;

	_6s = ipp_new_card();
	assertNotNull("ipp_new_card() failed", _6s);
	_6s->rank = SIX;
	_6s->suit = SPADES;

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = Ts;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHTFLUSH A", msg->payload);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_STRAIGHTFLUSH);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_STRAIGHTFLUSH);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = Ts;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "STRAIGHT A", msg->payload);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_STRAIGHT);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_STRAIGHT);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Qs;
	hand[3] = Js;
	hand[4] = _7s;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "FLUSH A K Q J 7", msg->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_FLUSH);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_FLUSH);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Ks;
	hand[3] = Js;
	hand[4] = _7s;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "ONEPAIR K A J 7", msg->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_ONEPAIR);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_ONEPAIR);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Ad;
	hand[3] = Kd;
	hand[4] = _7s;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "TWOPAIR A K 7", msg->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_TWOPAIR);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_TWOPAIR);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Ad;
	hand[3] = Kd;
	hand[4] = Kc;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "FULLHOUSE K A", msg->payload);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_FULLHOUSE);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_FULLHOUSE);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Ks;
	hand[2] = Js;
	hand[3] = Kd;
	hand[4] = Kc;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "THREEOFAKIND K A J", msg->payload);
	assertEqual("Didn't detect proper hand.", msg->type, MSG_THREEOFAKIND);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_THREEOFAKIND);
	ipp_free_message(msg);

	hand[0] = Kh;
	hand[1] = Ks;
	hand[2] = Js;
	hand[3] = Kd;
	hand[4] = Kc;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "FOUROFAKIND K J", msg->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_FOUROFAKIND);
	ipp_free_message(msg);

	hand[0] = As;
	hand[1] = Kd;
	hand[2] = Js;
	hand[3] = _7s;
	hand[4] = _6s;

	msg = ipp_eval(hand);
	assertNotNull("ipp_eval() returned null", msg);
	assertStringEqual("Didn't detect proper hand.", "HIGHCARD A K J 7 6", msg->payload);
	assertEqual("Didn't detect proper hand.", ipp_validate_unknown_msg(msg->payload), MSG_HIGHCARD);
	ipp_free_message(msg);

	ipp_free_card(_6s);
	ipp_free_card(_7s);
	ipp_free_card(Ts);
	ipp_free_card(Js);
	ipp_free_card(Qs);
	ipp_free_card(Ks);
	ipp_free_card(As);
	ipp_free_card(Kd);
	ipp_free_card(Ad);
	ipp_free_card(Kc);
	ipp_free_card(Kh);

	ipp_exit();

	return PASS;
}
