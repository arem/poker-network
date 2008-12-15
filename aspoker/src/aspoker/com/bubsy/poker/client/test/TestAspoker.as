package aspoker.com.bubsy.poker.client.test
{

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

        ts.addTest(new TestAspoker("test1"));
        ts.addTest(new TestAspoker("test2"));
        return ts;
    }

    public function test1():void
    {
        assertTrue("no error", true == true);
    }

    public function test2():void {
        assertTrue("error", false);
    }

}
}