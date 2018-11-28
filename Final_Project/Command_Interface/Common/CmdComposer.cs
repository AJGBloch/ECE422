namespace CmdInterface.common
{
    public class CmdComposer : AComposer
    {
        public CmdComposer(IView_FormMain i_view) : base(i_view) { }

        public void GenericCmd(string cmd)
        {
            CmdString(cmd);
        }
        public void LedOff()
        {
            CmdString("led_off");
        }

        public void LedOn()
        {
            CmdString("led_on");
        }

        public void TimedOutput(int duration)
        {
            CmdString("a"+duration.ToString());
        }
    }
}
