////////////////////////////////////////////////////////////////////////////////
//
//     Copyright (C) 2008 Bruno Garnier <bruno.garnier@gmail.com>
//
//     This program is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

package aspoker.com.bubsy.poker.client.test
{

import aspoker.com.bubsy.poker.client.PokerClient;
import aspoker.com.bubsy.poker.client.event.LoginEvent;
import aspoker.com.bubsy.poker.client.model.User;

import flash.events.Event;

import flexunit.framework.TestCase;
import flexunit.framework.TestSuite;

public class TestAspoker extends TestCase {

    public function TestAspoker(methodName:String)
    {
        super(methodName);
    }

    public static function suite():TestSuite
    {
        var ts:TestSuite = new TestSuite();

        ts.addTest(new TestAspoker("auth"));
        ts.addTest(new TestAspoker("test2"));
        return ts;
    }

    public function auth():void
    {
       var user:User = PokerClient.user;

        user.addEventListener(
            LoginEvent.onPacketAuthOk,
            addAsync(authSucessfull,100)
               );

        user.loggin("bruno","onurb");
    }

    public function authSucessfull(e:Event):void
    {
        assertTrue(" Should work ", true);
    }

    public function test2():void {
        assertTrue("error", false);
    }

}
}