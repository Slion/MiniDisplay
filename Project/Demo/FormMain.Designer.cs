namespace MiniDisplayDemo
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.iButtonOpen = new System.Windows.Forms.Button();
            this.iButtonClose = new System.Windows.Forms.Button();
            this.iButtonFill = new System.Windows.Forms.Button();
            this.iButtonClear = new System.Windows.Forms.Button();
            this.iButtonSetPixel = new System.Windows.Forms.Button();
            this.iNumericX = new System.Windows.Forms.NumericUpDown();
            this.iNumericY = new System.Windows.Forms.NumericUpDown();
            this.iLabelX = new System.Windows.Forms.Label();
            this.iLabelY = new System.Windows.Forms.Label();
            this.iTimer = new System.Windows.Forms.Timer(this.components);
            this.iNumericTimerInterval = new System.Windows.Forms.NumericUpDown();
            this.iLabelTimerInterval = new System.Windows.Forms.Label();
            this.iLabelFps = new System.Windows.Forms.Label();
            this.iPictureBoxDisplay = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericTimerInterval)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.iPictureBoxDisplay)).BeginInit();
            this.SuspendLayout();
            // 
            // iButtonOpen
            // 
            this.iButtonOpen.Location = new System.Drawing.Point(12, 12);
            this.iButtonOpen.Name = "iButtonOpen";
            this.iButtonOpen.Size = new System.Drawing.Size(75, 23);
            this.iButtonOpen.TabIndex = 0;
            this.iButtonOpen.Text = "Open";
            this.iButtonOpen.UseVisualStyleBackColor = true;
            this.iButtonOpen.Click += new System.EventHandler(this.iButtonOpen_Click);
            // 
            // iButtonClose
            // 
            this.iButtonClose.Location = new System.Drawing.Point(12, 41);
            this.iButtonClose.Name = "iButtonClose";
            this.iButtonClose.Size = new System.Drawing.Size(75, 23);
            this.iButtonClose.TabIndex = 1;
            this.iButtonClose.Text = "Close";
            this.iButtonClose.UseVisualStyleBackColor = true;
            this.iButtonClose.Click += new System.EventHandler(this.iButtonClose_Click);
            // 
            // iButtonFill
            // 
            this.iButtonFill.Location = new System.Drawing.Point(12, 70);
            this.iButtonFill.Name = "iButtonFill";
            this.iButtonFill.Size = new System.Drawing.Size(75, 23);
            this.iButtonFill.TabIndex = 2;
            this.iButtonFill.Text = "Fill";
            this.iButtonFill.UseVisualStyleBackColor = true;
            this.iButtonFill.Click += new System.EventHandler(this.iButtonFill_Click);
            // 
            // iButtonClear
            // 
            this.iButtonClear.Location = new System.Drawing.Point(12, 99);
            this.iButtonClear.Name = "iButtonClear";
            this.iButtonClear.Size = new System.Drawing.Size(75, 23);
            this.iButtonClear.TabIndex = 3;
            this.iButtonClear.Text = "Clear";
            this.iButtonClear.UseVisualStyleBackColor = true;
            this.iButtonClear.Click += new System.EventHandler(this.iButtonClear_Click);
            // 
            // iButtonSetPixel
            // 
            this.iButtonSetPixel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iButtonSetPixel.Location = new System.Drawing.Point(307, 67);
            this.iButtonSetPixel.Name = "iButtonSetPixel";
            this.iButtonSetPixel.Size = new System.Drawing.Size(75, 23);
            this.iButtonSetPixel.TabIndex = 4;
            this.iButtonSetPixel.Text = "Set pixel";
            this.iButtonSetPixel.UseVisualStyleBackColor = true;
            this.iButtonSetPixel.Click += new System.EventHandler(this.iButonSetPixel_Click);
            // 
            // iNumericX
            // 
            this.iNumericX.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iNumericX.Location = new System.Drawing.Point(307, 9);
            this.iNumericX.Name = "iNumericX";
            this.iNumericX.Size = new System.Drawing.Size(75, 20);
            this.iNumericX.TabIndex = 5;
            // 
            // iNumericY
            // 
            this.iNumericY.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iNumericY.Location = new System.Drawing.Point(307, 41);
            this.iNumericY.Name = "iNumericY";
            this.iNumericY.Size = new System.Drawing.Size(75, 20);
            this.iNumericY.TabIndex = 6;
            // 
            // iLabelX
            // 
            this.iLabelX.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iLabelX.AutoSize = true;
            this.iLabelX.Location = new System.Drawing.Point(284, 11);
            this.iLabelX.Name = "iLabelX";
            this.iLabelX.Size = new System.Drawing.Size(17, 13);
            this.iLabelX.TabIndex = 7;
            this.iLabelX.Text = "X:";
            // 
            // iLabelY
            // 
            this.iLabelY.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iLabelY.AutoSize = true;
            this.iLabelY.Location = new System.Drawing.Point(284, 43);
            this.iLabelY.Name = "iLabelY";
            this.iLabelY.Size = new System.Drawing.Size(17, 13);
            this.iLabelY.TabIndex = 8;
            this.iLabelY.Text = "Y:";
            // 
            // iTimer
            // 
            this.iTimer.Interval = 15;
            this.iTimer.Tick += new System.EventHandler(this.iTimer_Tick);
            // 
            // iNumericTimerInterval
            // 
            this.iNumericTimerInterval.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iNumericTimerInterval.Location = new System.Drawing.Point(307, 99);
            this.iNumericTimerInterval.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.iNumericTimerInterval.Name = "iNumericTimerInterval";
            this.iNumericTimerInterval.Size = new System.Drawing.Size(75, 20);
            this.iNumericTimerInterval.TabIndex = 9;
            this.iNumericTimerInterval.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.iNumericTimerInterval.ValueChanged += new System.EventHandler(this.iNumericTimerInterval_ValueChanged);
            // 
            // iLabelTimerInterval
            // 
            this.iLabelTimerInterval.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.iLabelTimerInterval.AutoSize = true;
            this.iLabelTimerInterval.Location = new System.Drawing.Point(203, 101);
            this.iLabelTimerInterval.Name = "iLabelTimerInterval";
            this.iLabelTimerInterval.Size = new System.Drawing.Size(98, 13);
            this.iLabelTimerInterval.TabIndex = 10;
            this.iLabelTimerInterval.Text = "Timer interval (ms) :";
            // 
            // iLabelFps
            // 
            this.iLabelFps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.iLabelFps.AutoSize = true;
            this.iLabelFps.Location = new System.Drawing.Point(12, 288);
            this.iLabelFps.Name = "iLabelFps";
            this.iLabelFps.Size = new System.Drawing.Size(27, 13);
            this.iLabelFps.TabIndex = 11;
            this.iLabelFps.Text = "FPS";
            // 
            // iPictureBoxDisplay
            // 
            this.iPictureBoxDisplay.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.iPictureBoxDisplay.Cursor = System.Windows.Forms.Cursors.Cross;
            this.iPictureBoxDisplay.Location = new System.Drawing.Point(40, 156);
            this.iPictureBoxDisplay.Name = "iPictureBoxDisplay";
            this.iPictureBoxDisplay.Size = new System.Drawing.Size(100, 50);
            this.iPictureBoxDisplay.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.iPictureBoxDisplay.TabIndex = 12;
            this.iPictureBoxDisplay.TabStop = false;
            this.iPictureBoxDisplay.MouseMove += new System.Windows.Forms.MouseEventHandler(this.iPictureBoxDisplay_MouseMove);
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(394, 310);
            this.Controls.Add(this.iPictureBoxDisplay);
            this.Controls.Add(this.iLabelFps);
            this.Controls.Add(this.iLabelTimerInterval);
            this.Controls.Add(this.iNumericTimerInterval);
            this.Controls.Add(this.iLabelY);
            this.Controls.Add(this.iLabelX);
            this.Controls.Add(this.iNumericY);
            this.Controls.Add(this.iNumericX);
            this.Controls.Add(this.iButtonSetPixel);
            this.Controls.Add(this.iButtonClear);
            this.Controls.Add(this.iButtonFill);
            this.Controls.Add(this.iButtonClose);
            this.Controls.Add(this.iButtonOpen);
            this.Name = "FormMain";
            this.Text = "Mini Display Demo";
            this.Load += new System.EventHandler(this.FormMain_Load);
            ((System.ComponentModel.ISupportInitialize)(this.iNumericX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericTimerInterval)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.iPictureBoxDisplay)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button iButtonOpen;
        private System.Windows.Forms.Button iButtonClose;
        private System.Windows.Forms.Button iButtonFill;
        private System.Windows.Forms.Button iButtonClear;
        private System.Windows.Forms.Button iButtonSetPixel;
        private System.Windows.Forms.NumericUpDown iNumericX;
        private System.Windows.Forms.NumericUpDown iNumericY;
        private System.Windows.Forms.Label iLabelX;
        private System.Windows.Forms.Label iLabelY;
        private System.Windows.Forms.Timer iTimer;
        private System.Windows.Forms.NumericUpDown iNumericTimerInterval;
        private System.Windows.Forms.Label iLabelTimerInterval;
        private System.Windows.Forms.Label iLabelFps;
        private System.Windows.Forms.PictureBox iPictureBoxDisplay;
    }
}

