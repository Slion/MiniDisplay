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

        public FormMain()
        {
            InitializeComponent();

            iDisplay = new Display();
            iDisplay.OnOpened += OnDisplayOpened;            
        }

        public void OnDisplayOpened(Display aDisplay)
        {
            // Set maximum pixels coordinates
            iNumericX.Maximum = iDisplay.WidthInPixels()-1;
            iNumericY.Maximum = iDisplay.HeightInPixels()-1;
            //
            iDisplay.Clear();
            iDisplay.SwapBuffers();
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
            iDisplay.SwapBuffers();
        }

        private void iButtonClear_Click(object sender, EventArgs e)
        {
            iDisplay.Clear();
            iDisplay.SwapBuffers();
        }

        private void iButonSetPixel_Click(object sender, EventArgs e)
        {
            iDisplay.SetPixel((int)iNumericX.Value, (int)iNumericY.Value, 1);
            iDisplay.SwapBuffers();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {

        }
    }
}
