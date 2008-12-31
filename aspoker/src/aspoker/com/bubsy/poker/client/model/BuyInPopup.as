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
            trace("ok complete" + _table.BuyInLimitbest + "-" + _table.BuyInLimitmax );
            popup.amoutSlide.minimum = _table.BuyInLimitMin;
            popup.amoutSlide.maximum = _table.BuyInLimitmax;
            popup.amoutSlide.snapInterval = _table.BuyInLimitmax / _table.BuyInLimitMin ;
            popup.amoutSlide.tickInterval = _table.BuyInLimitmax / _table.BuyInLimitMin ;
          //  popup.amoutSlide.value = 5000000;

            popup.amoutSlide.labels = [_table.BuyInLimitMin,_table.BuyInLimitmax]
        }

        private function buyin(e:MouseEvent):void
        {
            trace(popup.amoutSlide.value);
            _table.buyIn(popup.amoutSlide.value);
            PopUpManager.removePopUp(popup);
        }
    }
}