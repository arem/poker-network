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
////////////////////////////////////////////////////////////////////////////////

package aspoker.com.bubsy.poker.client.model
{

public final class PlayerState
{
    public static const FOLD:PlayerState = new PlayerState("Fold");
    public static const CHECK:PlayerState = new PlayerState("Check");
    public static const CALL:PlayerState = new PlayerState("Call");
    public static const RAISE:PlayerState = new PlayerState("Raise");
    public static const PAID_BIG_BLIND:PlayerState = new PlayerState("Big");
    public static const PAID_SMALL_BLIND:PlayerState = new PlayerState("Small");
    public static const SITOUT:PlayerState = new PlayerState("Sit-Out");
    public static const SITIN:PlayerState = new PlayerState("Sit-In");
    public static const OBSERVER:PlayerState = new PlayerState("Observer");

    private var _name:String;
    public var amount:int=0;

    public function compare(playerState:PlayerState):Boolean
    {
        if (playerState.name == this.name)
            return true;

        return false;
    }

    public function isPlaying():Boolean
    {
        if (name != PlayerState.OBSERVER.name &&
            name != PlayerState.SITOUT.name)
        {
            return true;
        }

        return false;
    }

    public function PlayerState(name:String):void
    {
        _name = name;
    }

    public function get name():String
    {
        return _name;
    }
}
}