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

public class Card
{
    public static var card2string:Array = [
    '2h', '3h', '4h', '5h', '6h', '7h','8h', '9h', 'Th', 'Jh', 'Qh', 'Kh', 'Ah',
    '2d', '3d', '4d', '5d', '6d','7d', '8d', '9d', 'Td', 'Jd', 'Qd', 'Kd', 'Ax',
    '2c', '3c', '4c', '5c', '6c', '7c', '8c', '9c', 'Tc', 'Jc', 'Qc', 'Kc', 'Ac',
    '2s', '3s', '4s', '5s', '6s', '7s', '8s', '9s', 'Ts', 'Js', 'Qs', 'Ks', 'As',
    'back'
    ];

    private var _value:Number;
    private var _name:String;

    public static function getCard(cardNumber:int):int
    {
        return card2string[cardNumber];
    }

    public function Card(value:Number)
    {
        _value = value;
        _name = card2string[value];
    }

    public function get value():Number
    {
        return _value;
    }

    public function get name():String
    {
        return _name;
    }
}
}