package aspoker.com.bubsy.poker.client.model
{
    public class Player
    {
        private var _name:String;
        private var _url:String;
        private var _auto:Boolean;
        private var _outfit:String;
        private var _wait_for:Boolean;
        private var _buy_in_payed:Boolean;
        private var _serial:int;
        private var _sit_out_next_turn:Boolean;
        private var _blind:String;
        private var _seat:int;
        private var _remove_next_turn:Boolean;
        private var _game_id:Boolean;

        public function Player(PacketPokerPlayerArrive:Object)
        {
            setPlayer(PacketPokerPlayerArrive);
        }

        public function get name():String
        {
            return _name;
        }

        public function setPlayer(PacketPokerPlayerArrive:Object):void
        {
            _name = PacketPokerPlayerArrive.name;
            _url = PacketPokerPlayerArrive.url;
            _auto = PacketPokerPlayerArrive.auto;
            _outfit = PacketPokerPlayerArrive.outfit;
            _wait_for = PacketPokerPlayerArrive.wait_for;
            _buy_in_payed = PacketPokerPlayerArrive.buy_in_payed;
            _serial = PacketPokerPlayerArrive.serial;
            _sit_out_next_turn= PacketPokerPlayerArrive.sit_out_next_turn;
            _blind = PacketPokerPlayerArrive.blind;
            _seat = PacketPokerPlayerArrive.seat;
            _remove_next_turn = PacketPokerPlayerArrive.remove_next_turn;
            _game_id = PacketPokerPlayerArrive.game_id;
        }
    }
}