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
using System.Drawing.Imaging;

namespace MiniDisplayDemo
{
    public partial class FormMain : Form
    {
        Display iDisplay;
        DateTime iLastTickTime;
        Bitmap iBitmap;
        Rectangle iBitmapRect;

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
            ResetBitmap(Color.White);
            //
            iDisplay.Clear();
            iDisplay.SetBrightness(iDisplay.MaxBrightness());
            TimerStart();
            //TimerStop();
            //UpdateFrame();
        }

        public void OnDisplayClosed(Display aDisplay)
        {
            UpdateControls();
            TimerStop();
        }

        private void ResetBitmap(Color aColor)
        {
            iBitmap = new System.Drawing.Bitmap(iDisplay.WidthInPixels(), iDisplay.HeightInPixels(), PixelFormat.Format32bppArgb);
            for (int i = 0; i < iBitmap.Width; i++)
            {
                for (int j = 0; j < iBitmap.Height; j++)
                {
                    iBitmap.SetPixel(i, j, aColor);
                }
            }
            iPictureBoxDisplay.Image = iBitmap;

            Point pt = new Point(0, 0);
            iBitmapRect = new Rectangle(pt, iBitmap.Size);
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
            iPictureBoxDisplay.Enabled = iDisplay.IsOpen();
            iTrackBarBrightness.Enabled = iDisplay.IsOpen();

            if (iDisplay.IsOpen())
            {
                //
                // Set maximum pixels coordinates
                iNumericX.Maximum = iDisplay.WidthInPixels() - 1;
                iNumericY.Maximum = iDisplay.HeightInPixels() - 1;
                //
                iTrackBarBrightness.Minimum = iDisplay.MinBrightness();
                iTrackBarBrightness.Maximum = iDisplay.MaxBrightness();
                iTrackBarBrightness.Value = iDisplay.MaxBrightness();
            }
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
            ResetBitmap(Color.Black);
            //UpdateFrame();
            //iDisplay.Fill();
        }

        private void iButtonClear_Click(object sender, EventArgs e)
        {
            ResetBitmap(Color.White);
            //UpdateFrame();
            //iDisplay.Clear();
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

            UpdateFrame();

            DateTime afterRender = DateTime.Now;
            // Compute FPS
            iLabelFps.Text = (1.0 / newTickTime.Subtract(iLastTickTime).TotalSeconds).ToString("F0") + " / " +
                                           (1000 / iTimer.Interval).ToString() + " FPS - " +
                                           afterRender.Subtract(newTickTime).TotalMilliseconds + " ms";


            iLastTickTime = newTickTime;
        }

        /// <summary>
        /// 
        /// </summary>
        private void UpdateFrame()
        {
            //
            // Send it to our display
            // Watch performance of that loop
            // The empty loop itself takes 4ms in debug
            unsafe
            {
                // Use LockBits for best performance
                BitmapData bitmap = iBitmap.LockBits(iBitmapRect, ImageLockMode.ReadOnly, iBitmap.PixelFormat);
                uint* pixels = (uint*)bitmap.Scan0.ToPointer(); // Assuming 4 bytes per pixel since we specified the format ourselves
                for (int i = 0; i < bitmap.Width; i++)
                {
                    for (int j = 0; j < bitmap.Height; j++)
                    {
                        //Get pixel color
                        // We need to reverse our color so as to light up the black pixels
                        uint color = ~(pixels[j * bitmap.Width + i]);

                        //Now set our pixel
                        // We spend almost 10ms in there in debug maybe only 2ms in release
                        iDisplay.SetPixel(i, j, color);
                    }
                }
                iBitmap.UnlockBits(bitmap);
            }

            iDisplay.SwapBuffers();

        }

        private void iNumericTimerInterval_ValueChanged(object sender, EventArgs e)
        {
            iTimer.Interval = (int)iNumericTimerInterval.Value;
        }

        private void iPictureBoxDisplay_MouseMove(object sender, MouseEventArgs e)
        {
            if (iDisplay.IsOpen() && e.Button==MouseButtons.Left 
                && e.X >= 0 && e.X < iBitmap.Width
                && e.Y >= 0 && e.Y < iBitmap.Height)
            {
                // Set pixels in both bitmap and screen
                iBitmap.SetPixel(e.X, e.Y, Color.Black);
                iPictureBoxDisplay.Image = iBitmap; // Most ineficient I guess
                //iDisplay.SetPixel(e.X, e.Y, 1);
            }                            
        }

        private void iTrackBarBrightness_Scroll(object sender, EventArgs e)
        {
            iDisplay.SetBrightness(iTrackBarBrightness.Value);
        }
    }
}
