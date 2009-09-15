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
import aspoker.com.bubsy.poker.client.communication.JsonStream;
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

        ts.addTest(new TestAspoker("test_auth"));
        ts.addTest(new TestAspoker("test_queue"));
        return ts;
    }

    public function test_auth():void
    {
        PokerClient.user.addEventListener(
            LoginEvent.onPacketAuthOk,
            addAsync(authSucessfull,1000)
        );

        PokerClient.user.loggin("bruno","onurb");
    }

    public function authSucessfull(e:Event):void
    {
        assertTrue("Authentification sucessfull", true);
    }

    public function test_queue():void {
        var o:JsonStream = new JsonStream();
        try{
                o.ping();
                o.ping();
                o.ping();
                assertTrue("no error", false);
            } catch (e:Error) {
                trace("something bad happened");
                assertTrue("flood detected",true,"flooood" );
         }        
    }

}
}
