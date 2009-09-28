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
    import aspoker.com.bubsy.poker.client.component.CBuyinPopup;

    import flash.display.DisplayObject;
    import flash.events.MouseEvent;

    import mx.events.FlexEvent;
    import mx.managers.PopUpManager;

    public class BuyInPopup
    {
        private var _table:Table;
        private var popup:CBuyinPopup;

        public function BuyInPopup(parent:DisplayObject,table:Table)
        {
            popup = new CBuyinPopup();

            _table = table;

            PopUpManager.addPopUp
            (
                popup,
                parent,
                true
            );
          popup.btonBuy.addEventListener(MouseEvent.CLICK,buyin);
          popup.addEventListener(FlexEvent.CREATION_COMPLETE,setBuyInLimit);
          PopUpManager.centerPopUp(popup);
        }

        private function setBuyInLimit(e:FlexEvent):void
        {
            popup.amoutSlide.minimum = _table.BuyInLimitMin/100;
            popup.amoutSlide.maximum = _table.BuyInLimitmax/100;
            popup.amoutSlide.snapInterval = (_table.BuyInLimitmax/100) / (_table.BuyInLimitMin/100) ;
            popup.amoutSlide.tickInterval = (_table.BuyInLimitmax/100) / (_table.BuyInLimitMin/100) ;
            popup.amoutSlide.labels = [_table.BuyInLimitMin/100,_table.BuyInLimitmax/100]
        }

        private function buyin(e:MouseEvent):void
        {
            _table.buyIn(popup.amoutSlide.value*100);
            PopUpManager.removePopUp(popup);
        }
    }
}
