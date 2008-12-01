import aspoker.com.bubsy.poker.models.PokerConfiguration;

package aspoker.com.bubsy.poker.models
{
	
	public class PokerSession
	{
		private var _sessionCount:int = 0;
		private var _sessionCookie:String;
		private var _sessionName:String;
		
		public function PokerSession()
		{

		}

		private function getSessionCountFromCookie():int {
		
		}
		
		public function incrementSessionCount():int {
			++_sessionCount;
			return _sessionCount;
		}
	}         
}