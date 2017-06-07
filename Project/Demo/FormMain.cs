using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SharpLib.MiniDisplay;

namespace MiniDisplayDemo
{
    public partial class FormMain : Form
    {
        Display iDisplay;
        DateTime iLastTickTime;

        public FormMain()
        {
            InitializeComponent();

            iDisplay = new Display();
            iDisplay.OnOpened += OnDisplayOpened;
            iDisplay.OnClosed += OnDisplayClosed;
            UpdateControls();
            iNumericTimerInterval.Value = iTimer.Interval;
        }

        public void OnDisplayOpened(Display aDisplay)
        {
            UpdateControls();
            // Set maximum pixels coordinates
            iNumericX.Maximum = iDisplay.WidthInPixels()-1;
            iNumericY.Maximum = iDisplay.HeightInPixels()-1;
            //
            iDisplay.Clear();
            TimerStart();
            //iDisplay.SwapBuffers();
        }

        public void OnDisplayClosed(Display aDisplay)
        {
            UpdateControls();
            TimerStop();
        }

        private void TimerStart()
        {
            iLastTickTime = DateTime.Now; //Reset timer to prevent jump
            iTimer.Enabled = true;
            
        }

        private void TimerStop()
        {
            iLastTickTime = DateTime.Now; //Reset timer to prevent jump
            iTimer.Enabled = false;
            
        }

        private void UpdateControls()
        {
            iButtonSetPixel.Enabled = iDisplay.IsOpen();
            iButtonOpen.Enabled = !iDisplay.IsOpen();
            iButtonClose.Enabled = iDisplay.IsOpen();
            iButtonClear.Enabled = iDisplay.IsOpen();
            iButtonFill.Enabled = iDisplay.IsOpen();
        }

        private void iButtonOpen_Click(object sender, EventArgs e)
        {
            iDisplay.Open(SharpLib.MiniDisplay.Type.AutoDetect);
        }

        private void iButtonClose_Click(object sender, EventArgs e)
        {
            iDisplay.Close();
        }

        private void iButtonFill_Click(object sender, EventArgs e)
        {
            iDisplay.Fill();
        }

        private void iButtonClear_Click(object sender, EventArgs e)
        {
            iDisplay.Clear();
        }

        private void iButonSetPixel_Click(object sender, EventArgs e)
        {
            iDisplay.SetPixel((int)iNumericX.Value, (int)iNumericY.Value, 1);
        }

        private void FormMain_Load(object sender, EventArgs e)
        {

        }

        private void iTimer_Tick(object sender, EventArgs e)
        {
            DateTime newTickTime = DateTime.Now;
            iDisplay.SwapBuffers();
            DateTime afterRender = DateTime.Now;
            // Compute FPS
            iLabelFps.Text = (1.0 / newTickTime.Subtract(iLastTickTime).TotalSeconds).ToString("F0") + " / " +
                                           (1000 / iTimer.Interval).ToString() + " FPS - " +
                                           afterRender.Subtract(newTickTime).TotalMilliseconds + " ms";


            iLastTickTime = newTickTime;
        }

        private void iNumericTimerInterval_ValueChanged(object sender, EventArgs e)
        {
            iTimer.Interval = (int)iNumericTimerInterval.Value;
        }
    }
}
