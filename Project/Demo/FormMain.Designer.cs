﻿namespace MiniDisplayDemo
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
            this.iButtonOpen = new System.Windows.Forms.Button();
            this.iButtonClose = new System.Windows.Forms.Button();
            this.iButtonFill = new System.Windows.Forms.Button();
            this.iButtonClear = new System.Windows.Forms.Button();
            this.iButtonSetPixel = new System.Windows.Forms.Button();
            this.iNumericX = new System.Windows.Forms.NumericUpDown();
            this.iNumericY = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.iNumericY)).BeginInit();
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
            this.iButtonSetPixel.Location = new System.Drawing.Point(197, 70);
            this.iButtonSetPixel.Name = "iButtonSetPixel";
            this.iButtonSetPixel.Size = new System.Drawing.Size(75, 23);
            this.iButtonSetPixel.TabIndex = 4;
            this.iButtonSetPixel.Text = "Set pixel";
            this.iButtonSetPixel.UseVisualStyleBackColor = true;
            this.iButtonSetPixel.Click += new System.EventHandler(this.iButonSetPixel_Click);
            // 
            // iNumericX
            // 
            this.iNumericX.Location = new System.Drawing.Point(183, 12);
            this.iNumericX.Name = "iNumericX";
            this.iNumericX.Size = new System.Drawing.Size(89, 20);
            this.iNumericX.TabIndex = 5;
            // 
            // iNumericY
            // 
            this.iNumericY.Location = new System.Drawing.Point(183, 44);
            this.iNumericY.Name = "iNumericY";
            this.iNumericY.Size = new System.Drawing.Size(89, 20);
            this.iNumericY.TabIndex = 6;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 261);
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
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button iButtonOpen;
        private System.Windows.Forms.Button iButtonClose;
        private System.Windows.Forms.Button iButtonFill;
        private System.Windows.Forms.Button iButtonClear;
        private System.Windows.Forms.Button iButtonSetPixel;
        private System.Windows.Forms.NumericUpDown iNumericX;
        private System.Windows.Forms.NumericUpDown iNumericY;
    }
}

